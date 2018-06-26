#include "btwrapper.h"

BTWrapper::BTWrapper(AsioSession &connection):
    connection_(connection)
{


}

BT::NodeStatus BTWrapper::activateJoystick()
{
    //TODO
    return BT::NodeStatus::SUCCESS;
}

BT::NodeStatus BTWrapper::deactivateJoystick()
{
    //TODO
    return BT::NodeStatus::SUCCESS;
}

BT::NodeStatus BTWrapper::isBatteryOK()
{
    std::cout << "checking batteries... " << std::endl;
    return BT::NodeStatus::SUCCESS;
}

void BTWrapper::Register(BT::BehaviorTreeFactory &factory, AsioSession &connection)
{
    static BTWrapper container(connection);
    auto container_ptr = &container;

    factory.registerSimpleAction("ActivateJoystickNavigation",
                                 [container_ptr](){ return container_ptr->activateJoystick(); });

    factory.registerSimpleAction("DisableJoystickNavigation",
                                 [container_ptr](){ return container_ptr->deactivateJoystick(); });

    factory.registerSimpleCondition("IsBatteryLevelOK",
                                    [container_ptr](){ return container_ptr->isBatteryOK(); });
}


//------------------------------------------------

MoveBaseToGoal::MoveBaseToGoal(const std::string &name, const BT::NodeParameters &params,
                               AsioSession &connection):
    ActionNodeBase(name, params),
    connection_(connection)
{}

BT::NodeStatus MoveBaseToGoal::tick()
{
    // called when the action starts
    if( isHalted() )
    {
        char buffer[1024];
        sprintf(buffer, "(plan ((tcb-approach-location %s)))\n",
                           getParam<std::string>("goal").c_str() );

        std::cout << "Sending: " << std::string(buffer) << std::endl;
        if( connection_.send(buffer))
        {
            return BT::NodeStatus::RUNNING;
        }
        else {
            return BT::NodeStatus::FAILURE;
        }
    }
    else {
        std::string reply;
        try{
            if( connection_.recv(&reply))
            {
                std::cout << "Received: " << reply << std::endl;
                if( reply.find("SUCCESS") != std::string::npos )
                {
                    return BT::NodeStatus::SUCCESS;
                }
                else{
                    return BT::NodeStatus::FAILURE;
                }
            }
            return BT::NodeStatus::RUNNING;
        }
        catch(std::runtime_error& err)
        {
            std::cout << "Failed communication> " << err.what() << std::endl;
            return BT::NodeStatus::FAILURE;
        }
    }
}

void MoveBaseToGoal::halt()
{
    connection_.send("(abort)");
    std::string reply;
    connection_.recv(&reply);
}

void MoveBaseToGoal::Register(BT::BehaviorTreeFactory &factory, AsioSession &connection)
{
    BT::NodeBuilder builder = [&connection](const std::string& name, const BT::NodeParameters& params)
    {
        return std::unique_ptr<MoveBaseToGoal>(new MoveBaseToGoal(name, params, connection));
    };

    factory.registerBuilder("MoveBaseToGoal", builder);
}
