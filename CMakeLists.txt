cmake_minimum_required(VERSION 3.20)

project(tt
    VERSION 1.0.0
    LANGUAGES CXX
)

add_library(tt
    src/Client.cpp
    src/ClientFactory.cpp
    src/Role.cpp
    
    src/io/Choice.cpp
    src/io/Connect.cpp
    src/io/End.cpp
    src/io/Entry.cpp
    src/io/Error.cpp
    src/io/Join.cpp
    src/io/Message.cpp
    src/io/MessageFactory.cpp
    src/io/Report.cpp
    src/io/Start.cpp
    src/io/Stop.cpp
    src/io/Update.cpp
    
    src/world/Action.cpp
    src/world/Assignment.cpp
    src/world/Constant.cpp
    src/world/Ending.cpp
    src/world/Entity.cpp
    src/world/Signature.cpp
    src/world/State.cpp
    src/world/Status.cpp
    src/world/Turn.cpp
    src/world/Variable.cpp
    src/world/World.cpp
    src/world/Value.cpp

    include/tt/util/BlockingQueue.h
    include/tt/util/CompletionEvent.h
    
    include/tt/util/ThreadPool.h
    include/tt/util/Util.h
    src/Settings.cpp
    src/ExternalAPI.cpp
    
)

add_library(tt::tt ALIAS tt)

target_include_directories(tt
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_compile_features(tt
    PUBLIC
        cxx_std_17
)

include(FetchContent)

FetchContent_Declare(
    nlohmann_json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG v3.12.0
)
set(JSON_BuildTests OFF CACHE INTERNAL "")
FetchContent_MakeAvailable(nlohmann_json)

set(CPR_USE_SYSTEM_CURL ON CACHE BOOL "" FORCE)

FetchContent_Declare(
    cpr 
    GIT_REPOSITORY 
    https://github.com/libcpr/cpr.git
    GIT_TAG 1.10.5
)
set(CPR_BUILD_TESTS OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(cpr)

find_package(OpenSSL REQUIRED)
find_package(Threads REQUIRED)

target_link_libraries(tt
    PUBLIC
        nlohmann_json::nlohmann_json
        OpenSSL::SSL
        OpenSSL::Crypto
        Threads::Threads
    PRIVATE
        cpr::cpr
)