#ifndef BTWRAPPER_H
#define BTWRAPPER_H

#include <cstdlib>
#include <iostream>
#include "server.h"
#include <behavior_tree_core/xml_parsing.h>

class MoveBaseToGoal: public BT::ActionNodeBase
{
    // this is private: only the factory can create an instance
    MoveBaseToGoal(const std::string& name,
                   const BT::NodeParameters& params,
                   AsioSession &connection );
public:
    
    static void Register(BT::BehaviorTreeFactory& factory, AsioSession& connection);

    virtual BT::NodeStatus tick() override;

    virtual void halt() override;


    static const BT::NodeParameters& requiredNodeParameters()
    {
        static BT::NodeParameters params = {{"goal2D", "0,0"}};
        return params;
    }

private:

    AsioSession&   connection_;
};

//-------------------------------------------------------------------------

class BTWrapper
{
    // this is private: only the factory can create an instance
    BTWrapper(AsioSession &connection);
    
public:

    BT::NodeStatus activateJoystick();

    BT::NodeStatus deactivateJoystick();

    BT::NodeStatus isBatteryOK();

    static void Register(BT::BehaviorTreeFactory& factory,
                         AsioSession &connection);

private:

    AsioSession& connection_;
};

#endif // BTWRAPPER_H
