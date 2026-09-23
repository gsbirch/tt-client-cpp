cmake_minimum_required(VERSION 3.20)

project(tt
    VERSION 0.1
    LANGUAGES CXX
)

add_library(tt
    src/Client.cpp
    src/ClientFactory.cpp
    src/Role.cpp
    
    src/io/Choice.cpp
    src/io/Connect.cpp
    src/io/End.cpp
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
    src/world/Registry.cpp
    src/world/Signature.cpp
    src/world/State.cpp
    src/world/Status.cpp
    src/world/Turn.cpp
    src/world/Variable.cpp
    src/world/World.cpp

    include/tt/util/BlockingQueue.h
    include/tt/util/CompletionEvent.h
    include/tt/util/JsonUtil.h
    include/tt/util/ThreadPool.h
    include/tt/util/Util.h
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

include(FetchContent)

FetchContent_Declare(
    nlohmann_json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG v3.12.0
)

FetchContent_MakeAvailable(nlohmann_json)

target_link_libraries(tt
    PUBLIC nlohmann_json::nlohmann_json
)
