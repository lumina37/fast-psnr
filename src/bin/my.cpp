#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <tlct.hpp>

#include "psnr/impl/my.native.hpp"

namespace tn = tlct::tspc;

int main()
{
    auto src = cv::imread("src_square.png");
    const auto calib_cfg = tn::CalibConfig::fromXMLPath("Boys.xml");
    const auto layout = tn::Layout::fromCfgAndImgsize(calib_cfg, src.size());

    cv::Mat dst;
    psnr::dbg::dbgDrawMicroImageEdges(layout, src, dst);
    cv::imwrite("dbg_circles_square.png", dst);
    return 0;
}
