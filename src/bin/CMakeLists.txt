function(psnr_add_executable name srcs)
    add_executable(${name} ${srcs})
    target_link_libraries(${name} PRIVATE psnr-lib)
    target_include_directories(${name} PRIVATE
            ${argparse_SOURCE_DIR}/include
    )
endfunction()

psnr_add_executable(bench-v1 "v1.cpp")
psnr_add_executable(bench-v2 "v2.cpp")
psnr_add_executable(bench-v3 "v3.cpp")
psnr_add_executable(bench-v4 "v4.cpp")
