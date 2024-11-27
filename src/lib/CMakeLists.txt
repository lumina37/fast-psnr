set(PSNR_LIB_NAME psnr-lib)

add_library(${PSNR_LIB_NAME} INTERFACE)
target_compile_features(${PSNR_LIB_NAME} INTERFACE cxx_std_20)
if (MSVC)
    target_compile_options(${PSNR_LIB_NAME} INTERFACE /arch:AVX2 $<$<CONFIG:Release>:/O2>)
else ()
    target_compile_options(${PSNR_LIB_NAME} INTERFACE -mavx2 $<$<CONFIG:Release>:-O3>)
endif ()
target_include_directories(${PSNR_LIB_NAME} INTERFACE
        "${PROJECT_SOURCE_DIR}/src/include"
)
