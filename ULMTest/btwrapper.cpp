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
    //TODO
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
    char buffer[1024];
    sprintf(buffer, "(plan ((tcb-approach-location %s)))\n",
                       getParam<std::string>("goal").c_str() );

    std::cout << "Sending: " << std::string(buffer) << std::endl;
    if( connection_.send(buffer))
    {
        setStatus( BT::NodeStatus::RUNNING);

        std::string reply;
        if( connection_.recv(&reply))
        {
            std::cout << "Received: " << reply << std::endl;
            if( reply.find("SUCCESS") != std::string::npos )
            {
                return BT::NodeStatus::SUCCESS;
            }
        }
    }
    // failure if either send, recv of string::find failed
    return BT::NodeStatus::FAILURE;
}

void MoveBaseToGoal::halt()
{
    //TODO
}

void MoveBaseToGoal::Register(BT::BehaviorTreeFactory &factory, AsioSession &connection)
{
    BT::NodeBuilder builder = [&connection](const std::string& name, const BT::NodeParameters& params)
    {
        return std::unique_ptr<MoveBaseToGoal>(new MoveBaseToGoal(name, params, connection));
    };

    factory.registerBuilder("MoveBaseToGoal", builder);
}
