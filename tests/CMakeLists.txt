enable_testing()

function(psnr_add_test name srcs)
    add_executable(${name} ${srcs})
    target_link_libraries(${name} PRIVATE doctest psnr-lib)
    doctest_discover_tests(${name})
endfunction()

psnr_add_test(minicase-v1 "minicase-v1.cpp")
psnr_add_test(minicase-v2 "minicase-v2.cpp")
psnr_add_test(minicase-v3 "minicase-v3.cpp")
psnr_add_test(minicase-v4 "minicase-v4.cpp")
psnr_add_test(minicase-v5 "minicase-v5.cpp")
