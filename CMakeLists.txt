cmake_minimum_required(VERSION 3.27)

project(yuv)

set(CMAKE_CXX_STANDARD 20)

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
        src/YDbDr.cpp)

set(HWY_SOURCES hwy/aligned_allocator.cc hwy/targets.cc hwy/targets.cc hwy/nanobenchmark.cc hwy/per_target.cc hwy/timer.cc
        src/Eotf.cpp
        src/Eotf-inl.h)

if (BUILD_SHARED)
    add_library(sparkyuv SHARED ${SPARKYUV_SOURCES} ${HWY_SOURCES})
else ()
    add_library(sparkyuv STATIC ${SPARKYUV_SOURCES})
endif ()

if (APPLE)
    add_definitions(-DHWY_COMPILE_ONLY_STATIC)
endif ()

if (CMAKE_BUILD_TYPE STREQUAL "Release")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")
endif ()

target_include_directories(sparkyuv PUBLIC ${CMAKE_SOURCE_DIR} ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/hwy ${CMAKE_SOURCE_DIR}/highway ${CMAKE_SOURCE_DIR}/highway/hwy)

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
            tools/bench/YuvBenchmarkBase.cpp)

    add_library(libyuv STATIC IMPORTED)
    set_target_properties(yuvtools libyuv PROPERTIES IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/libyuv.a)

    target_include_directories(yuvtools PRIVATE ${CMAKE_SOURCE_DIR} ${JPEG_INCLUDE_DIR})

    find_package(JPEG REQUIRED)

    find_package(benchmark REQUIRED)

    target_link_libraries(yuvtools PRIVATE JPEG::JPEG libyuv sparkyuv)

    target_link_libraries(yuvbench PRIVATE JPEG::JPEG libyuv sparkyuv benchmark::benchmark)

endif ()

#add_definitions(-DSPARKYUV_FULL_CHANNELS=1)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_definitions(-DHWY_COMPILE_ONLY_STATIC)
    add_definitions(-DNOACCELERATED_SAMPLER)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")
endif ()