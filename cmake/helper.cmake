function(copy_dlls_if_needed name)
    if (WIN32)
        add_custom_command(TARGET ${name} POST_BUILD COMMAND ${CMAKE_COMMAND} -E
                copy_if_different $<TARGET_RUNTIME_DLLS:${name}> $<TARGET_FILE:${name}> $<TARGET_FILE_DIR:${name}> COMMAND_EXPAND_LISTS)
    endif ()
endfunction()

set(PSNR_COMPILE_INFO "<tag:${PROJECT_VERSION}> by [${CMAKE_CXX_COMPILER_ID}-${CMAKE_CXX_COMPILER_VERSION} (${CMAKE_SYSTEM_PROCESSOR})]")

set(PSNR_CONFIGURE_DIR "${PROJECT_SOURCE_DIR}/src/include/psnr/common")
configure_file("${PSNR_CONFIGURE_DIR}/cmake.h.in" "${PSNR_CONFIGURE_DIR}/cmake.h" @ONLY)
