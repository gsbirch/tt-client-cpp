cmake_minimum_required(VERSION 3.20)

project(tt
    VERSION 0.1
    LANGUAGES CXX
)

add_library(tt
    src/Client.cpp
    src/Role.cpp
    src/io/Join.cpp
    src/io/Message.cpp
    src/ClientFactory.cpp
)

target_include_directories(tt
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_compile_features(tt
    PUBLIC
        cxx_std_17
)
