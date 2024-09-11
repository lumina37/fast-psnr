function(psnr_add_executable name srcs)
    add_executable(${name} ${srcs})
    target_link_libraries(${name} PRIVATE psnr)

    if (TLCT_ENABLE_LTO)
        set_target_properties(${name} PROPERTIES
                INTERPROCEDURAL_OPTIMIZATION ON
        )
    endif ()

    copy_dlls_if_needed(${name})
endfunction()

psnr_add_executable(bench-native "native.cpp")
