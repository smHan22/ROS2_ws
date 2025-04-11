#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/vector3.hpp"
#include <memory>
#include <iostream>
#include <limits>

int main(int argc, char* argv[])
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<rclcpp::Node>("mynode");
    auto qos_profile = rclcpp::QoS(rclcpp::KeepLast(10));
    auto pub = node->create_publisher<geometry_msgs::msg::Vector3>("mytopic", qos_profile);

    geometry_msgs::msg::Vector3 message;

    while (rclcpp::ok())
    {
        std::cout << "실수값 3개를 입력하세요 (x y z): ";
        std::cin >> message.x >> message.y >> message.z;

        // 입력 오류 처리
        if (std::cin.fail()) {
            std::cin.clear(); // 오류 상태 초기화
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // 잘못된 입력 무시
            std::cout << "잘못된 입력입니다. 다시 시도하세요.\n";
            continue;
        }

        RCLCPP_INFO(node->get_logger(), "Publish: x=%.2f, y=%.2f, z=%.2f",
                    message.x, message.y, message.z);

        pub->publish(message);
    }

    rclcpp::shutdown();
    return 0;
}
