cmake_minimum_required(VERSION 3.20)
project(SimpleRPC)
set(CMAKE_CXX_STANDARD 17)
set(LIB_NAME SimpleRPC)
include_directories(
        ${PROJECT_SOURCE_DIR}/3rd
        ${PROJECT_SOURCE_DIR}/3rd/uWebSockets
        ${PROJECT_SOURCE_DIR}/src/gen
        ${PROJECT_SOURCE_DIR}/include
        ${PROJECT_SOURCE_DIR}/src/sample/services
)

add_library(
        ${LIB_NAME}
        src/server/RpcRequest.cpp
        src/server/RpcResponse.cpp
        src/ServiceDefine.cpp
        src/server/RpcServer.cpp
)

target_link_libraries(${LIB_NAME}
        ${PROJECT_SOURCE_DIR}/3rd/uWebSockets/uSockets.a
        pthread
        z
        )

add_executable(
        RpcGenerator
        src/gen/main.cpp
        src/ServiceDefine.cpp
        src/gen/RpcServiceGenerator.cpp
        src/utils.cpp
)
target_link_libraries(
        RpcGenerator
        ${LIB_NAME}
)
add_executable(
        TestServer
        src/sample/main.cpp
        src/sample/TestRPCServer.cpp
        src/sample/services/SampleService.cpp
        src/sample/services/StringService.cpp
        src/sample/services/ArcService.cpp
)


target_link_libraries(
        TestServer
        ${LIB_NAME}
)


