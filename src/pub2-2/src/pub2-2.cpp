#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/vector3.hpp"
#include <memory>
#include <chrono>
#include <functional>
#include <iostream>

using namespace std::chrono_literals;

void callback(rclcpp::Node::SharedPtr node,
              rclcpp::Publisher<geometry_msgs::msg::Vector3>::SharedPtr pub)
{
    float x, y, z;
    std::cout << "실수(float) 3개를 입력하세요: ";
    std::cin >> x >> y >> z;

    auto message = geometry_msgs::msg::Vector3();
    message.x = x;
    message.y = y;
    message.z = z;

    RCLCPP_INFO(node->get_logger(), "Publish: [%.2f, %.2f, %.2f]", x, y, z);
    pub->publish(message);
}

int main(int argc, char* argv[])
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<rclcpp::Node>("mynode");
    auto qos_profile = rclcpp::QoS(rclcpp::KeepLast(10));

    // Vector3 메시지 타입 퍼블리셔 생성
    auto pub = node->create_publisher<geometry_msgs::msg::Vector3>("mytopic", qos_profile);

    // 콜백 함수 바인딩
    std::function<void()> fn = std::bind(callback, node, pub);

    // 타이머 콜백 설정 (1초 간격, 하지만 cin으로 블로킹됨)
    auto timer = node->create_wall_timer(1s, fn);

    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}
