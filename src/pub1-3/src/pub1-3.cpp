#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/twist.hpp"
#include <termios.h>
#include <unistd.h>
#include <stdio.h>

class TurtleController : public rclcpp::Node
{
public:
    TurtleController()
    : Node("turtle_controller")
    {
        publisher_ = this->create_publisher<geometry_msgs::msg::Twist>("turtle1/cmd_vel", 10);
        timer_ = this->create_wall_timer(std::chrono::milliseconds(100),
            std::bind(&TurtleController::control_loop, this));
        set_conio_terminal_mode();
        RCLCPP_INFO(this->get_logger(), "Press [f/b/l/r/s] to move the turtle. Ctrl+C to exit.");
    }

    ~TurtleController()
    {
        reset_terminal_mode();
    }

private:
    geometry_msgs::msg::Twist msg;

    void control_loop()
    {
        if (kbhit())
        {
            char c = getchar();
            switch (c)
            {
                case 'f':
                    msg.linear.x = 2.0;
                    msg.angular.z = 0.0;
                    break;
                case 'b':
                    msg.linear.x = -2.0;
                    msg.angular.z = 0.0;
                    break;
                case 'l':
                    msg.linear.x = 0.0;
                    msg.angular.z = 2.0;
                    break;
                case 'r':
                    msg.linear.x = 0.0;
                    msg.angular.z = -2.0;
                    break;
                case 's':
                    msg.linear.x = 0.0;
                    msg.angular.z = 0.0;
                    break;
                default:
                    return;
            }
            publisher_->publish(msg);
        }
    }

    // 비동기 키보드 입력을 위한 터미널 설정
    struct termios orig_termios;

    void reset_terminal_mode()
    {
        tcsetattr(0, TCSANOW, &orig_termios);
    }

    void set_conio_terminal_mode()
    {
        struct termios new_termios;

        tcgetattr(0, &orig_termios);
        new_termios = orig_termios;
        new_termios.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(0, TCSANOW, &new_termios);
    }

    int kbhit()
    {
        struct timeval tv = { 0L, 0L };
        fd_set fds;
        FD_ZERO(&fds);
        FD_SET(0, &fds);
        return select(1, &fds, NULL, NULL, &tv);
    }

    rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr publisher_;
    rclcpp::TimerBase::SharedPtr timer_;
};

int main(int argc, char * argv[])
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<TurtleController>());
    rclcpp::shutdown();
    return 0;
}