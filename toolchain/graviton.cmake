set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR aarch64)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mcpu=neoverse-n1 -march=armv8.2-a")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mcpu=neoverse-n1 -march=armv8.2-a")
