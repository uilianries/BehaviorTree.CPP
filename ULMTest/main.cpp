#include "btwrapper.h"


#include <behavior_tree_core/xml_parsing.h>
#include <behavior_tree_logger/bt_cout_logger.h>
#include <behavior_tree_logger/bt_zmq_publisher.h>


int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        std::cerr << "Usage: client <bt-file.xml>\n";
        std::exit(1);
    }

    int port = 20021;

    BT::BehaviorTreeFactory factory;
    BT::XMLParser parser(factory);
    parser.loadFromFile(argv[1]);

    factory.registerSimpleAction("PlugToChargeStation",
                                 [](){ return BT::NodeStatus::SUCCESS; } );

    std::cout << "File loaded" << std::endl;

    //--------------------------------------
    boost::asio::io_service io_service;
    using boost::asio::ip::tcp;
    tcp::acceptor acceptor(io_service, tcp::endpoint(tcp::v4(), port));
    tcp::socket socket(io_service);

    std::cout << "Waiting for first and only connection" << std::endl;
    boost::asio::ip::tcp::endpoint endpoint;
    boost::system::error_code ec;
    acceptor.accept(socket, endpoint, ec);
    if (ec)
    {
       // An error occurred.
    }

    AsioSession session( std::move(socket) );

    //------------------------------------
    std::cout << "Client connected" << std::endl;

    BTWrapper::Register(factory, session );
    MoveBaseToGoal::Register(factory, session );

    std::cout << "Registration done" << std::endl;

    std::vector<BT::TreeNodePtr> nodes;
    BT::TreeNodePtr root_node = parser.instantiateTree(nodes);

    BT::StdCoutLogger logger_cout(root_node.get());
    BT::PublisherZMQ publisher_zmq(root_node.get());

    std::cout << "Start execution" << std::endl;

    BT::NodeStatus status = BT::NodeStatus::IDLE;

    while (status != BT::NodeStatus::SUCCESS)
    {
        std::cout << "------- Tick ------"<< std::endl;
        status = root_node->executeTick();
        std::this_thread::sleep_for( std::chrono::milliseconds(200) );
    }
}

