from launch import LaunchDescription
from launch.actions import ExecuteProcess
from launch_ros.actions import Node

def generate_launch_description():
    return LaunchDescription([
        Node(
            package='my_lab3_pkg',
            executable='node_name_publisher',
            name='adam_node',
            parameters=[{
                'battery_min_voltage': 36.0,
                'battery_max_voltage': 42.0
            }]
        ),
        ExecuteProcess(
            cmd=['ros2', 'bag', 'record', '/battery_voltage', '/battery_percentage'],
            output='screen'
        )
    ])
