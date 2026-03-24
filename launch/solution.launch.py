import os
from launch_ros.actions import Node
from launch import LaunchDescription
from ament_index_python.packages import get_package_share_directory


def generate_launch_description():
    package_dir = get_package_share_directory('mpc_rbt_student')
    rviz_config_path = os.path.join(package_dir, 'rviz', 'config.rviz')

    # Nový lokalizační uzel
    localization_node = Node(
        package='mpc_rbt_student',
        executable='localization',    # Název z CMakeLists.txt
        name='localization_node',
        output='screen',               # Abychom viděli výpisy v terminálu
        parameters=[{'use_sim_time': True}]
    )

    # RViz pro vizualizaci (nepovinné, ale velmi užitečné)
    rviz_node = Node(
        package='rviz2',
        executable='rviz2',
        name='rviz2',
        arguments=['-d', rviz_config_path],
        output='screen'
    )
    

    
    # Tady ROSu říkáme, co všechno má reálně spustit
    return LaunchDescription([
        localization_node,
        rviz_node,
        Node(
            package='tf2_ros',
            executable='static_transform_publisher',
            name='static_transform_publisher',
            # argumenty: x, y, z, yaw, pitch, roll, parent_frame, child_frame
            arguments=['-0.5', '0.0', '0', '0', '0', '0', 'map', 'odom']
        )
    ])
