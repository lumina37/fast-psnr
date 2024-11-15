#include <cstddef>
#include <iostream>

#include "psnr.hpp"

int main()
{
    const auto width = 1920;
    const auto height = 1080;

    const size_t ysize = width * height;
    psnr::Yuv420Frame lframe{ysize};
    psnr::Yuv420Frame rframe{ysize};

    double psnr = psnr::PsnrOp<psnr::mse::v3::MseOpu8>(lframe.getY(), rframe.getY(), ysize);

    std::cout << psnr << std::endl;
}
