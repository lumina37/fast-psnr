if (PSNR_HEADER_ONLY)
    set(PSNR_LIB_TYPE "INTERFACE")
    set(PSNR_LIB_DEP_SCOPE "INTERFACE")
else ()
    if (PSNR_BUILD_SHARED_LIBS)
        set(PSNR_LIB_TYPE "SHARED")
        set(PSNR_LIB_COMPILE_DEFS "_PSNR_BUILD_LIBS")
    else ()
        set(PSNR_LIB_TYPE "STATIC")
    endif ()
    set(PSNR_LIB_DEP_SCOPE "PUBLIC")

    set(PSNR_LIB_SRCS "lib.cpp")
endif ()

set(PSNR_LIB_NAME psnr)
add_library(${PSNR_LIB_NAME} ${PSNR_LIB_TYPE} ${PSNR_LIB_SRCS})
set_target_properties(${PSNR_LIB_NAME} PROPERTIES
        VERSION ${PSNR_VERSION}
        SOVERSION ${PSNR_VERSION}
        DEBUG_POSTFIX "-${PSNR_VERSION}d"
        RELEASE_POSTFIX "-${PSNR_VERSION}")
target_compile_features(${PSNR_LIB_NAME} ${PSNR_LIB_DEP_SCOPE} cxx_std_20)
target_compile_definitions(${PSNR_LIB_NAME} PRIVATE ${PSNR_LIB_COMPILE_DEFS})
target_include_directories(${PSNR_LIB_NAME} ${PSNR_LIB_DEP_SCOPE}
        "${PROJECT_SOURCE_DIR}/src/include"
)
target_link_libraries(${PSNR_LIB_NAME} ${PSNR_LIB_DEP_SCOPE} ITT)
