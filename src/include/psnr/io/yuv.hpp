#pragma once

#include <filesystem>
#include <fstream>

#include "psnr/common/defines.h"
#include "psnr/helper/static_math.hpp"

namespace psnr::_io {

namespace fs = std::filesystem;

constexpr size_t SIMD_FETCH_SIZE = 128 / 8;

template <typename TFrame_>
class YuvIO
{
public:
    using TFrame = TFrame_;

    PSNR_API inline YuvIO(std::ifstream&& ifs, size_t ysize)
        : ifs_(std::move(ifs)), ysize_(ysize), usize_(ysize >> TFrame::Ushift), vsize_(ysize >> TFrame::Vshift),
          total_size_(ysize_ + usize_ + vsize_){};
    PSNR_API static inline YuvIO fromPath(const fs::path& fpath, size_t ysize)
    {
        std::ifstream ifs{fpath, std::ios::binary};
        return {std::move(ifs), ysize};
    }

    PSNR_API inline YuvIO& skip(int n);
    PSNR_API inline TFrame poll();
    PSNR_API inline void poll_into(TFrame& frame);

private:
    std::ifstream ifs_;
    size_t ysize_;
    size_t usize_;
    size_t vsize_;
    size_t total_size_;
};

template <typename TFrame>
YuvIO<TFrame>& YuvIO<TFrame>::skip(int n)
{
    ifs_.seekg(total_size_);
    return *this;
}

template <typename TFrame>
TFrame YuvIO<TFrame>::poll()
{
    TFrame frame{ysize_, usize_, vsize_};
    ifs_.read((char*)frame.y_, ysize_);
    ifs_.read((char*)frame.u_, usize_);
    ifs_.read((char*)frame.v_, vsize_);
    return frame;
}

template <typename TFrame>
void YuvIO<TFrame>::poll_into(TFrame& frame)
{
    ifs_.read((char*)frame.y_, ysize_);
    ifs_.read((char*)frame.u_, usize_);
    ifs_.read((char*)frame.v_, vsize_);
}

template <typename TElem_, size_t Ushift_, size_t Vshift_>
class YuvFrame
{
public:
    friend class YuvIO<YuvFrame>;

    using TElem = TElem_;
    static constexpr size_t Ushift = Ushift_;
    static constexpr size_t Vshift = Vshift_;

    PSNR_API inline YuvFrame(size_t ysize, size_t usize, size_t vsize) : ysize_(ysize), usize_(usize), vsize_(vsize)
    {
        this->alloc();
    }
    PSNR_API explicit inline YuvFrame(size_t ysize) : ysize_(ysize), usize_(ysize >> Ushift), vsize_(ysize >> Vshift)
    {
        this->alloc();
    }

    YuvFrame(const YuvFrame& rhs) = delete;
    YuvFrame operator=(const YuvFrame& rhs) = delete;
    PSNR_API inline YuvFrame(YuvFrame&& rhs) noexcept
        : ysize_(rhs.ysize_), usize_(rhs.usize_), vsize_(rhs.vsize_), buffer_(std::exchange(rhs.buffer_, nullptr)),
          y_(std::exchange(rhs.y_, nullptr)), u_(std::exchange(rhs.u_, nullptr)), v_(std::exchange(rhs.v_, nullptr)){};
    PSNR_API inline YuvFrame& operator=(YuvFrame&& rhs) noexcept
    {
        ysize_ = rhs.ysize_;
        usize_ = rhs.usize_;
        vsize_ = rhs.vsize_;
        buffer_ = std::exchange(rhs.buffer_, nullptr);
        y_ = std::exchange(rhs.y_, nullptr);
        u_ = std::exchange(rhs.u_, nullptr);
        v_ = std::exchange(rhs.v_, nullptr);
        return *this;
    };

    PSNR_API inline ~YuvFrame() { std::free(buffer_); }

    [[nodiscard]] PSNR_API inline size_t getYSize() const noexcept { return ysize_; }
    [[nodiscard]] PSNR_API inline size_t getUSize() const noexcept { return usize_; }
    [[nodiscard]] PSNR_API inline size_t getVSize() const noexcept { return vsize_; }
    [[nodiscard]] PSNR_API inline const TElem* getY() const noexcept { return y_; }
    [[nodiscard]] PSNR_API inline const TElem* getU() const noexcept { return u_; }
    [[nodiscard]] PSNR_API inline const TElem* getV() const noexcept { return v_; }

private:
    inline void alloc();

    size_t ysize_;
    size_t usize_;
    size_t vsize_;
    void* buffer_;
    TElem* y_;
    TElem* u_;
    TElem* v_;
};

template <typename TElem_, size_t Ushift_, size_t Vshift_>
void YuvFrame<TElem_, Ushift_, Vshift_>::alloc()
{
    {
        constexpr size_t ubase = 1 << (Ushift * 2);

        if (!_hp::is_mul_of<ubase>(ysize_)) {
            return;
        }

        if constexpr (Ushift != Vshift) {
            constexpr size_t vbase = 1 << (Vshift * 2);
            if (!_hp::is_mul_of<vbase>(ysize_)) {
                return;
            }
        }

        size_t aligned_ysize = _hp::align_to<SIMD_FETCH_SIZE>(ysize_);
        size_t aligned_usize = _hp::align_to<SIMD_FETCH_SIZE>(usize_);
        size_t aligned_vsize;
        if constexpr (Ushift == Vshift) {
            aligned_vsize = aligned_usize;
        } else {
            aligned_vsize = _hp::align_to<SIMD_FETCH_SIZE>(vsize_);
        }

        const size_t total_size = aligned_ysize + aligned_usize + aligned_vsize + SIMD_FETCH_SIZE;
        buffer_ = std::malloc(total_size);

        y_ = (TElem*)_hp::round_to<SIMD_FETCH_SIZE>((size_t)buffer_);
        u_ = (TElem*)((size_t)y_ + aligned_ysize);
        v_ = (TElem*)((size_t)u_ + aligned_usize);
    }
}

template class YuvFrame<uint8_t, 1, 1>;
using Yuv420Frame = YuvFrame<uint8_t, 1, 1>;

} // namespace psnr::_io