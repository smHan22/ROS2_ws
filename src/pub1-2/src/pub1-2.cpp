#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/vector3.hpp"
#include <memory>
#include <chrono>
#include <iostream>
using namespace rclcpp;
using namespace std;
int main(int argc, char * argv[])
{
    init(argc, argv);
    auto node = std::make_shared<Node>("node_pub1");
    auto qos_profile = QoS(KeepLast(10));
    auto mypub = node->create_publisher<geometry_msgs::msg::Vector3>("topic_pub1",
    qos_profile );

    geometry_msgs::msg::Vector3 message;

    WallRate loop_rate(1.0); //반복주파수를 저장하는 객체(단위 Hz)
    while(ok())
    {
        cout << "실수값 3개를 입력하시오: " << endl;
        cin >> message.x >> message.y >> message.z;
        cout << "\n" << endl;


        RCLCPP_INFO(node->get_logger(), "Publish: x=%.2f, y=%.2f, z=%.2f", message.x, message.y, message.z);
        mypub->publish(message);

        loop_rate.sleep();
    }
    shutdown();
    return 0;
}