cmake_minimum_required(VERSION 3.5)

set(PROJECT_NAME server)
set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREAD ON)

project(${PROJECT_NAME})
find_package(Threads)
find_package(Boost REQUIRED system thread timer chrono)


set(HEADERS_SOURCES main.cpp
        server.cpp
        server.h
        ../communication_protocol.cpp
        ../communication_protocol.h ../shared_memory.h ../shared_memory.cpp)

add_executable(${PROJECT_NAME} ${HEADERS_SOURCES})
set(CMAKE_BUILD_TYPE Debug)

set_target_properties(PROPERTIES
        CXX_STANDART 17
        CXX_STANDART_REQUIRED ON
        COMPILE_OPTIONS BOOST_ASIO_ENABLE_HANDLER_TRACKING
        COMPILE_OPTIONS BOOST_THREADS_ENABLE_HANDLER_TRACKING
        #COMPILE_OPTIONS BOOST_ATOMIC_ENABLE_HANDLER_TRACKING
        COMPILE_OPTIONS "-Wpedantic;-Wall;-Wextra; "
        COMPILE_OPTIONS "-fsanitize=thread"
        LINK_OPTIONS "-fsanitize=thread"
        )

target_link_libraries(${PROJECT_NAME}
        ${CMAKE_THREAD_LIBS_INIT}
        ${Boost_LIBRARIES}
        )


