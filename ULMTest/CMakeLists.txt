cmake_minimum_required(VERSION 2.8)

project(ULMTest)

add_executable(ULMTest
    server.cpp
    btwrapper.cpp
    main.cpp
)

target_link_libraries(ULMTest behavior_tree_core boost_system zmq)
