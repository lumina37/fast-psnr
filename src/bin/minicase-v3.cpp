#include <cstddef>
#include <iostream>
#include <memory>

#include "psnr.hpp"

int main()
{
    constexpr auto width = 1920;
    constexpr auto height = 1080;
    constexpr size_t ysize = width * height;
    const uint8_t* lhs = (uint8_t*)std::malloc(ysize);
    const uint8_t* rhs = (uint8_t*)std::malloc(ysize);

    double psnr = psnr::PsnrOp<psnr::mse::v3::MseOpu8>(lhs, rhs, ysize);

    std::cout << psnr << std::endl;
}
