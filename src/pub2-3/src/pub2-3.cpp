#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/twist.hpp"
#include <memory>
#include <chrono>
#include <functional>

using namespace std::chrono_literals;

// 전역 퍼블리셔 선언
std::shared_ptr<rclcpp::Publisher<geometry_msgs::msg::Twist>> pub;

// 콜백 함수 (인자 없음)
void callback()
{
    auto msg = geometry_msgs::msg::Twist();
    msg.linear.x = 2.0;      // 선속도
    msg.angular.z = 1.0;     // 각속도

    RCLCPP_INFO(rclcpp::get_logger("turtle_controller"),
                "Publishing: linear=%.2f angular=%.2f",
                msg.linear.x, msg.angular.z);

    pub->publish(msg);
}

int main(int argc, char* argv[])
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<rclcpp::Node>("turtle_controller");

    pub = node->create_publisher<geometry_msgs::msg::Twist>("/turtle1/cmd_vel", 10);

    std::function<void()> fn = std::bind(callback);
    auto timer = node->create_wall_timer(100ms, fn);

    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}
