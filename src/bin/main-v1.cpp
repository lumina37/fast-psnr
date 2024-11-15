#include <cstddef>
#include <filesystem>
#include <iostream>
#include <ranges>

#include <argparse/argparse.hpp>

#include "psnr.hpp"

namespace fs = std::filesystem;
namespace rgs = std::ranges;

int main(int argc, char* argv[])
{
    argparse::ArgumentParser program("v1", psnr_VERSION, argparse::default_arguments::all);
    program.add_argument("width").help("frame width").scan<'i', size_t>();
    program.add_argument("height").help("frame height").scan<'i', size_t>();
    program.add_argument("frames").help("frames").scan<'i', size_t>();
    program.add_argument("lpath").help("file path of the lhs yuv").required();
    program.add_argument("rpath").help("file path of the rhs yuv").required();
    program.add_epilog(PSNR_COMPILE_INFO);

    try {
        program.parse_args(argc, argv);
    } catch (const std::exception& err) {
        std::cerr << err.what() << std::endl;
        std::cerr << program;
        std::exit(1);
    }

    const auto width = program.get<size_t>("width");
    const auto height = program.get<size_t>("height");
    const auto frames = program.get<size_t>("frames");
    const fs::path lhs_fpath{program.get("lpath")};
    const fs::path rhs_fpath{program.get("rpath")};

    const size_t ysize = width * height;
    auto lhs_yuvio = psnr::Yuv420IO::fromPath(lhs_fpath, ysize);
    auto rhs_yuvio = psnr::Yuv420IO::fromPath(rhs_fpath, ysize);
    psnr::Yuv420Frame lframe{ysize};
    psnr::Yuv420Frame rframe{ysize};

    double psnr_acc = 0;
    for (const auto i : rgs::views::iota(0, (int)frames)) {
        lhs_yuvio.poll_into(lframe);
        rhs_yuvio.poll_into(rframe);
        double psnr = psnr::PsnrOp<psnr::mse::v1::MseOpu8>(lframe.getY(), rframe.getY(), ysize);
        psnr_acc += psnr;
    }

    const double psnr_avg = psnr_acc / (double)frames;
    std::cout << psnr_avg << std::endl;
}
