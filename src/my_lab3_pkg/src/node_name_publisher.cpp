#include <chrono>
#include <memory>
#include <string>
#include <algorithm>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"
#include "std_msgs/msg/float32.hpp"

using namespace std::chrono_literals;

class NodeNamePublisher : public rclcpp::Node
{
public:
  NodeNamePublisher() : Node("adam_node")
  {
    this->declare_parameter("battery_min_voltage", 32.0);
    this->declare_parameter("battery_max_voltage", 42.0);

    name_publisher_ = this->create_publisher<std_msgs::msg::String>("node_name", 10);
    battery_publisher_ = this->create_publisher<std_msgs::msg::Float32>("battery_percentage", 10);

    timer_ = this->create_wall_timer(500ms, std::bind(&NodeNamePublisher::publish_name, this));

    battery_subscription_ = this->create_subscription<std_msgs::msg::Float32>(
      "battery_voltage", 10,
      std::bind(&NodeNamePublisher::battery_callback, this, std::placeholders::_1));
  }

private:
  void publish_name()
  {
    std_msgs::msg::String msg;
    msg.data = this->get_name();
    name_publisher_->publish(msg);
  }

  void battery_callback(const std_msgs::msg::Float32::SharedPtr msg)
{
  double min_v = this->get_parameter("battery_min_voltage").as_double();
  double max_v = this->get_parameter("battery_max_voltage").as_double();

  std_msgs::msg::Float32 out;

  float percent = static_cast<float>(((msg->data - min_v) / (max_v - min_v)) * 100.0);
  percent = std::clamp(percent, 0.0f, 100.0f);

  out.data = percent;
  battery_publisher_->publish(out);

  RCLCPP_INFO(this->get_logger(), "U=%.2f V, min=%.2f, max=%.2f, pct=%.2f",
              msg->data, min_v, max_v, out.data);
}

  rclcpp::Publisher<std_msgs::msg::String>::SharedPtr name_publisher_;
  rclcpp::Publisher<std_msgs::msg::Float32>::SharedPtr battery_publisher_;
  rclcpp::Subscription<std_msgs::msg::Float32>::SharedPtr battery_subscription_;
  rclcpp::TimerBase::SharedPtr timer_;
};

int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<NodeNamePublisher>());
  rclcpp::shutdown();
  return 0;
}
