set(PSNR_LIB_NAME psnr-lib)

if (PSNR_HEADER_ONLY)
    set(PSNR_LIB_TYPE "INTERFACE")
    set(PSNR_LIB_DEP_SCOPE "INTERFACE")
else ()
    if (PSNR_BUILD_SHARED_LIBS)
        set(PSNR_LIB_TYPE "SHARED")
    else ()
        set(PSNR_LIB_TYPE "STATIC")
    endif ()
    set(PSNR_LIB_DEP_SCOPE "PUBLIC")

    set(PSNR_LIB_SRCS "lib.cpp")
endif ()

add_library(${PSNR_LIB_NAME} ${PSNR_LIB_TYPE} ${PSNR_LIB_SRCS})
set_target_properties(${PSNR_LIB_NAME} PROPERTIES
        OUTPUT_NAME ${PROJECT_NAME}
        VERSION ${PSNR_VERSION}
        SOVERSION ${PSNR_VERSION}
        DEBUG_POSTFIX "-${PSNR_VERSION}d"
        RELEASE_POSTFIX "-${PSNR_VERSION}")
target_compile_features(${PSNR_LIB_NAME} ${PSNR_LIB_DEP_SCOPE} cxx_std_20)
if (MSVC)
    target_compile_options(${PSNR_LIB_NAME} ${PSNR_LIB_DEP_SCOPE} /arch:AVX2)
else ()
    target_compile_options(${PSNR_LIB_NAME} ${PSNR_LIB_DEP_SCOPE} -mavx2)
endif ()
target_include_directories(${PSNR_LIB_NAME} ${PSNR_LIB_DEP_SCOPE}
        "${PROJECT_SOURCE_DIR}/src/include"
)

if (NOT PSNR_HEADER_ONLY AND PSNR_BUILD_SHARED_LIBS)
    target_compile_definitions(${PSNR_LIB_NAME} PRIVATE "_PSNR_BUILD_LIBS")
endif ()
