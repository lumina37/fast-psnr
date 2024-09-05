#pragma once

#include "cmake.h"

#ifdef PSNR_HEADER_ONLY
#    define PSNR_API
#else
#    ifdef PSNR_BUILD_SHARED_LIBS
#        ifdef _MSC_VER
#            ifdef _PSNR_BUILD_LIBS
#                define PSNR_API __declspec(dllexport)
#            else
#                define PSNR_API __declspec(dllimport)
#            endif
#        else
#            ifdef _PSNR_BUILD_LIBS
#                define PSNR_API __attribute__((visibility("default")))
#            else
#                define PSNR_API
#            endif
#        endif
#    else
#        define PSNR_API
#    endif
#endif
