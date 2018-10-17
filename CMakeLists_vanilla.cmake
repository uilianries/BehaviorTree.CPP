message(STATUS "------------------------------------------")
message(STATUS "BehaviourTree: using vanilla configuration.")
message(STATUS "------------------------------------------")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

find_package(GTest)
include_directories(${GTEST_INCLUDE_DIRS})
if(NOT GTEST_FOUND)
    message(WARNING " GTest not found!")
endif(NOT GTEST_FOUND)

if(GTEST_FOUND)
    include_directories(gtest/include)

    add_executable(behavior_tree_core_test ${BT_Tests} )
    target_link_libraries(behavior_tree_core_test
        ${BEHAVIOR_TREE_LIBRARIES} crossdoor_nodes
        ${GTEST_LIBRARIES}
        ${GTEST_MAIN_LIBRARIES}
        )		
endif()




