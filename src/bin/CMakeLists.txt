function(psnr_add_executable name srcs)
    add_executable(${name} ${srcs})
    target_link_libraries(${name} PRIVATE psnr-lib)
    target_include_directories(${name} PRIVATE
            ${argparse_SOURCE_DIR}/include
    )
endfunction()

psnr_add_executable(main-v1 "main-v1.cpp")
psnr_add_executable(main-v2 "main-v2.cpp")
psnr_add_executable(main-v3 "main-v3.cpp")
psnr_add_executable(main-v4 "main-v4.cpp")

psnr_add_executable(minicase-v1 "minicase-v1.cpp")
psnr_add_executable(minicase-v2 "minicase-v2.cpp")
psnr_add_executable(minicase-v3 "minicase-v3.cpp")
psnr_add_executable(minicase-v4 "minicase-v4.cpp")
