#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/int32.hpp"
#include <memory>
#include <chrono>
using namespace rclcpp;
int main(int argc, char * argv[])
{
    init(argc, argv);
    auto node = std::make_shared<Node>("node_pub1");
    auto qos_profile = QoS(KeepLast(10));
    auto mypub = node->create_publisher<std_msgs::msg::Int32>("topic_pub1",
    qos_profile );

    std_msgs::msg::Int32 message;
    message.data = 0;

    WallRate loop_rate(1.0); //반복주파수를 저장하는 객체(단위 Hz)
    while(ok())
    {
        RCLCPP_INFO(node->get_logger(), "Publish: %d", message.data);
        mypub->publish(message);
        message.data += 1;
        loop_rate.sleep(); //반복주파수에서 남은 시간 만큼 sleep
    }
    shutdown();
    return 0;
}