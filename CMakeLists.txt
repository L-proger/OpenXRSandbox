cmake_minimum_required(VERSION 3.16)

project(OpenXRSandbox C CXX ASM)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 11)

add_executable(OpenXRSandbox)

include(openxr_loader_windows/OpenXRConfig.cmake)

target_sources(OpenXRSandbox
PRIVATE
    Src/Main.cpp
    Src/OpenXRContext.h
)

target_include_directories(OpenXRSandbox
PRIVATE
    #openxr_loader_windows/include
)


target_compile_definitions(OpenXRSandbox
PRIVATE
    XR_OS_WINDOWS
    XR_USE_PLATFORM_WIN32
    XR_USE_GRAPHICS_API_D3D11
)

target_link_libraries(OpenXRSandbox
PRIVATE
    OpenXR::headers
    OpenXR::openxr_loader
)


function(copyLoaderDllToBuildDir exeTargetName)

    SET(fileList "")
    list(APPEND fileList "${CMAKE_CURRENT_LIST_DIR}/openxr_loader_windows/x64/bin/openxr_loader.dll")
    list(APPEND fileList "${CMAKE_CURRENT_LIST_DIR}/openxr_loader_windows/x64/bin/openxr_loader.pdb")

    add_custom_command(TARGET ${exeTargetName} POST_BUILD
        COMMAND_EXPAND_LISTS
        COMMAND ${CMAKE_COMMAND} -E copy
                ${fileList}
                ${CMAKE_CURRENT_BINARY_DIR})

endfunction(copyLoaderDllToBuildDir)


copyLoaderDllToBuildDir(OpenXRSandbox)