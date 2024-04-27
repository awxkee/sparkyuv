cmake_minimum_required(VERSION 3.22)

project(yuv)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 20)

add_definitions(-DHWY_DISABLED_TARGETS=\(HWY_SCALAR\))
#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -march=armv8-a+simd+crypto+sve")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=armv8-a+simd+crypto+sve")

include(CheckCXXCompilerFlag)
check_cxx_compiler_flag("-pthread" COMPILER_SUPPORTS_THREADS)

set(HIGHWAY_DIRECTORY "highway")
file(GLOB DIRECTORY_EXIST "${HIGHWAY_DIRECTORY}")

if(NOT HIGHWAY_DIRECTORY)
    message(FATAL_ERROR "Error: The directory ${HIGHWAY_DIRECTORY} does not exist. Did you run `deps.sh`?")
endif()

set(SPARKYUV_SOURCES
        src/sparkyuv.cpp
        src/ChannelLength.cpp
        src/YCbCrP16.cpp
        src/ChannelsReformat.cpp
        src/Scale.cpp
        src/NV12Flyer.cpp
        src/NV16Flyer.cpp
        src/NV24Flyer.cpp
        src/RGB565Reformat.cpp
        src/YcCbcCrc.cpp
        src/Eotf.cpp
        src/YCgCo.cpp
        src/YCgCoR.cpp
        src/YDzDx.cpp
        src/YIQ.cpp
        src/YDbDr.cpp
        src/YCbCr400.cpp
        src/YCbCr411.cpp
        src/Flip.cpp
        src/Transpose.cpp
        src/CopyImage.cpp
        src/Rotate.cpp
        src/FastGaussian.cpp
        src/FastGaussian.h
        src/GaussianBlur.cpp
        src/FastGaussianNeon.cpp
        src/FastGaussianNeon.h)

set(HWY_SOURCES
        highway/hwy/aligned_allocator.cc highway/hwy/targets.cc highway/hwy/targets.cc
        highway/hwy/nanobenchmark.cc highway/hwy/per_target.cc highway/hwy/timer.cc
        highway/hwy/abort.cc
        src/Eotf.cpp
        src/Eotf-inl.h
        src/GaussianBlur-inl.h)

if (BUILD_SHARED)
    add_library(sparkyuv SHARED ${SPARKYUV_SOURCES} ${HWY_SOURCES})
else ()
    add_library(sparkyuv STATIC ${SPARKYUV_SOURCES} ${HWY_SOURCES})
endif ()

if (APPLE)
    add_definitions(-DHWY_COMPILE_ONLY_STATIC)
endif ()

if (CMAKE_BUILD_TYPE STREQUAL "Release" AND NOT MSVC)
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")
endif ()

target_include_directories(sparkyuv PUBLIC ${CMAKE_SOURCE_DIR} ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/hwy ${CMAKE_SOURCE_DIR}/highway ${CMAKE_SOURCE_DIR}/highway/hwy)

if (COMPILER_SUPPORTS_THREADS)
    message(STATUS "Threads are supported. Adding multithreading when needed.")
    add_definitions(-DTHREADS_SUPPORTED=1)
else ()
    message(STATUS "Threads are not supported")
endif ()

if (BUILD_TOOLS)
    add_executable(yuvtools tools/main.cpp tools/JPEGEncoder.cpp)
    add_executable(yuvbench
            tools/YuvBenchmarkMain.cpp
            tools/bench/YuvBenchmark10.cpp
            tools/bench/YuvBenchmark10.h
            tools/bench/YuvBenchmarkYCgCo.cpp
            tools/bench/YuvBenchmarkYCgCoR.cpp
            tools/bench/YuvBenchmarkYCgCoR.h
            tools/bench/YuvBenchmarkYcCbcCrc.cpp
            tools/bench/YuvBenchmarkYcCbcCrc.h
            tools/bench/YuvSupport.cpp
            tools/bench/YuvSupport.h
            tools/bench/YuvBenchmarkYDzDx.cpp
            tools/bench/YuvBenchmarkYDzDx.h
            tools/bench/YuvBenchmarkYIQ.cpp
            tools/bench/YuvBenchmarkYIQ.cpp
            tools/bench/YuvBenchmarkYIQ.h
            tools/bench/YuvBenchmarkYDbDr.cpp
            tools/bench/YuvBenchmarkYDbDr.h
            tools/bench/YuvBenchmarkBase.cpp
            tools/bench/YuvBenchmarkNV.cpp
            tools/bench/YuvBenchmarkNV.h)

    add_library(libyuv STATIC IMPORTED)
    set_target_properties(yuvtools libyuv PROPERTIES IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/libyuv.a)

    target_include_directories(yuvtools PRIVATE ${CMAKE_SOURCE_DIR} ${JPEG_INCLUDE_DIR})

    find_package(JPEG REQUIRED)

    find_package(benchmark REQUIRED)

    add_subdirectory(highway)

    target_link_libraries(yuvtools PRIVATE JPEG::JPEG libyuv sparkyuv)

    target_link_libraries(yuvbench PRIVATE JPEG::JPEG libyuv sparkyuv benchmark::benchmark)

endif ()

#add_definitions(-DSPARKYUV_FULL_CHANNELS=1)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_definitions(-DHWY_COMPILE_ONLY_STATIC)
    add_definitions(-DNOACCELERATED_SAMPLER)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")
endif ()

if (MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Gv")
endif()

if (NOT APPLE)
    install(TARGETS sparkyuv
            LIBRARY DESTINATION libsparkyuv
    )

    install(DIRECTORY include/
            DESTINATION include/sparkyuv
            FILES_MATCHING PATTERN "*.h"
    )
endif ()