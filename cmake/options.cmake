include(CMakeDependentOption)

option(PSNR_HEADER_ONLY "Enable the header-only mode" OFF)
cmake_dependent_option(PSNR_BUILD_SHARED_LIBS "Specifies the type of PSNR to build" ON
        "NOT PSNR_HEADER_ONLY AND BUILD_SHARED_LIBS" OFF)
