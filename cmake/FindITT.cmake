if (WIN32)
    add_library(ITT INTERFACE)
    target_include_directories(ITT INTERFACE "C:/Program Files/IntelSWTools/GPA/sdk/include")
    target_link_libraries(ITT INTERFACE "C:/Program Files/IntelSWTools/GPA/sdk/lib/x64/libittnotify64.lib")
    set(ITT_FOUND ON)
endif ()
