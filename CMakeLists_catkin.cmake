find_package(catkin REQUIRED COMPONENTS )

message(STATUS "------------------------------------------")
message(STATUS "BehaviourTree is being built using CATKIN.")
message(STATUS "------------------------------------------")

catkin_package(
    INCLUDE_DIRS include
    LIBRARIES behavior_tree_core
)

if(catkin_FOUND AND CATKIN_ENABLE_TESTING)
    include_directories(gtest/include)

    catkin_add_gtest(behavior_tree_core_test ${BT_Tests} )
    target_link_libraries(behavior_tree_core_test
        ${BEHAVIOR_TREE_LIBRARIES} crossdoor_nodes
        ${catkin_LIBRARIES} )	
endif()




