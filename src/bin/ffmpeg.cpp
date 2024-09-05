#include <filesystem>
#include <fstream>

namespace fs = std::filesystem;

int main(int argc, char* argv[])
{
    argparse::ArgumentParser program("PSNR", PSNR_VERSION, argparse::default_arguments::all);
    program.add_argument("param_file_path").help("the PSNR parameter file path").required();

    try {
        program.parse_args(argc, argv);
    } catch (const std::exception& err) {
        std::cerr << err.what() << std::endl;
        std::cerr << program;
        std::exit(1);
    }

    const auto& param_file_path = program.get<std::string>("param_file_path");
    const auto cfg_map = tlct::ConfigMap::fromPath(param_file_path);

    if (cfg_map.getPipelineType() == tlct::PipelineType::RLC) {
        namespace tn = tlct::raytrix;
        mainProc<tn::ParamConfig, tn::Layout>(cfg_map);
    } else {
        namespace tn = tlct::tspc;
        mainProc<tn::ParamConfig, tn::Layout>(cfg_map);
    }
}
