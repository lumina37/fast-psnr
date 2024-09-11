#include <filesystem>
#include <iostream>
#include <ranges>

#include "psnr.hpp"

namespace fs = std::filesystem;
namespace rgs = std::ranges;

using YuvFrame = psnr::io::YuvFrame<uint8_t, 1, 1>;
using YuvIO = psnr::io::YuvIO<YuvFrame>;

int main(int argc, char* argv[])
{
    fs::path lhs_fpath{argv[1]};
    fs::path rhs_fpath{argv[2]};

    constexpr int ysize = 4080 * 3068;
    auto lhs_yuvio = YuvIO::fromPath(lhs_fpath, ysize);
    auto rhs_yuvio = YuvIO::fromPath(rhs_fpath, ysize);
    YuvFrame lhs_frame{ysize};
    YuvFrame rhs_frame{ysize};

    for (const int i : rgs::views::iota(0, 1)) {
        lhs_yuvio.poll_into(lhs_frame);
        rhs_yuvio.poll_into(rhs_frame);
        double res = psnr::native::compute_psnr(lhs_frame.getY(), rhs_frame.getY(), ysize);
        std::cout << res << std::endl;
    }
}