#include <filesystem>
#include <ranges>

#include <argparse/argparse.hpp>
#include <fmt/core.h>
#include <ittnotify.h>

#include "psnr.hpp"

namespace fs = std::filesystem;
namespace rgs = std::ranges;

using YuvFrame = psnr::io::YuvFrame<uint8_t, 1, 1>;
using YuvIO = psnr::io::YuvIO<YuvFrame>;

int main(int argc, char* argv[])
{
    argparse::ArgumentParser program("v1", PSNR_VERSION, argparse::default_arguments::all);
    program.add_argument("width").help("frame width").scan<'i', size_t>();
    program.add_argument("height").help("frame height").scan<'i', size_t>();
    program.add_argument("frames").help("frames").scan<'i', size_t>();
    program.add_argument("lpath").help("file path of the lhs yuv").required();
    program.add_argument("rpath").help("file path of the rhs yuv").required();

    try {
        program.parse_args(argc, argv);
    } catch (const std::exception& err) {
        fmt::print("{}\n", err.what());
        fmt::print("{}\n", program.help().str());
        std::exit(1);
    }

    const auto width = program.get<size_t>("width");
    const auto height = program.get<size_t>("height");
    const auto frames = program.get<size_t>("frames");
    const fs::path lhs_fpath{program.get("lpath")};
    const fs::path rhs_fpath{program.get("rpath")};

    const size_t ysize = width * height;
    auto lhs_yuvio = YuvIO::fromPath(lhs_fpath, ysize);
    auto rhs_yuvio = YuvIO::fromPath(rhs_fpath, ysize);
    YuvFrame lhs_frame{ysize};
    YuvFrame rhs_frame{ysize};

    double psnr_acc = 0;

    __itt_domain* domain = __itt_domain_create("Domain.Global");
    __itt_string_handle* handle_main = __itt_string_handle_create("psnr");
    __itt_task_begin(domain, __itt_null, __itt_null, handle_main);

    for (const auto i : rgs::views::iota(0, (int)frames)) {
        lhs_yuvio.poll_into(lhs_frame);
        rhs_yuvio.poll_into(rhs_frame);
        double psnr = psnr::v1::compute_psnr(lhs_frame.getY(), rhs_frame.getY(), ysize);
        psnr_acc += psnr;
    }

    __itt_task_end(domain);

    const double psnr_avg = psnr_acc / (double)frames;
    fmt::print("{}", psnr_avg);
}
