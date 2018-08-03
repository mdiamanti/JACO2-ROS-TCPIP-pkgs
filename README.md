# Jaco2-ROS-TCPIP-pkgs
Current repository includes the appropriate ROS packages that allow remote communication and control of Kinova Jaco<sup>2</sup> 6 DOF robotic arm via the use of TCP/IP protocol. 

The ROS packages of current repository were developed in order to be used along with the `raspberry_tcp_ip.project` of [Jaco2-CodeSys-projects](https://github.com/mdiamanti/Jaco2-CodeSys-projects) repository.

## File System
- `tcp_ip_comm` ROS package provides all the functionality required for remote communication of Kinova Jaco<sup>2</sup> Gazebo simulation with any system that supports TCP/IP socket programming. 
- `gazebo_plugin` ROS package offers a ROS action server so as to control the robotic arm by executing trajectories of type [control_msgs/FollowJointTrajectoryAction](http://docs.ros.org/hydro/api/control_msgs/html/action/FollowJointTrajectory.html). More specifically, the robotic arm that is controlled through this package is Kinova Jaco<sup>2</sup> 6 DOF with three robot fingers, or else the model j2n6s300 as it is named in the required [kinova-ros](https://github.com/Kinovarobotics/kinova-ros#installation) package.

## Supported Versions
The configuration used was 64 bit Ubuntu 14.04 and ROS indigo. These packages may work with other configurations as well, but they have only been tested for the one recommended.

## Installation
The installation as follows was tested on Ubuntu 14.04 and ROS indigo.

#### Step 1. Install kinova-ros
Install [kinova-ros](https://github.com/Kinovarobotics/kinova-ros#installation) package, following the installation instructions that are analytically reported in the link.

#### Step 2. Install Jaco2-ROS-TCPIP-pkgs
Add the git repository in your catkin workspace:
```
cd ~/catkin_ws/src
git clone https://github.com/mdiamanti/JACO2-ROS-TCPIP-pkgs.git
```
Afterwards, compile your catkin workspace as usual:
```
cd ..
catkin_make
```
## How to use this repository
The `tcp_ip_comm` and `gazebo_plugin` packages were developed in order to be used in combination and to control remotely the movement of the robotic arm. Otherwise, each package can be used separately after some small modifications to meet a different purpose. 

The `tcp_ip_comm` node receives through socket programming the desired joint angular position of the arm, which is then sent to `gazebo_plugin` node that is responsible for executing the corresponding trajectory. 

An overview of the above described process is given by the following ROS graph, generated with the use of the ROS tool rqt_graph.

![tcpip_rosgraph](https://user-images.githubusercontent.com/39567867/43488813-90d3e84c-9523-11e8-9cbb-3b84a207cae4.png)

The form of the joint angular position message that is received from `tcp_ip_comm` node constitutes a string with the angular position of each joint in rads (variable of type double). Each angular position is separated from the others by spaces. Some examples are as follows:
```
"0.0 2.9 1.3 4.2 1.4 0.0"         -> home position of the arm
"3.14 3.14 3.14 3.14 3.14 3.14"   -> candle position of the arm
```
## Execution

#### Step 1. Launch j2n6s300 robotic arm in Gazebo
First of all, in order to use these packages you should launch j2n6s300 robotic arm in Gazebo using the launch file included in [kinova_gazebo](https://github.com/Kinovarobotics/kinova-ros/tree/master/kinova_gazebo) package:
```
roslaunch kinova_gazebo robot_launch.launch 
or
roslaunch kinova_gazebo robot_launch.launch kinova_robotType:=j2n6s300
```

#### Step 2. Launch tcp_ip_comm and gazebo_plugin packages
If you wish to use `tcp_ip_comm` and `gazebo_plugin` packages in combination, as described in the previous section, you can launch the .launch file included in `tcp_ip_comm` package. In this way, both `tcp_ip_comm` and `gazebo_plugin` nodes are launched:
```
roslaunch tcp_ip_comm tcp_ip_comm_launch.launch
```
Otherwise, you can launch each node separately in the following way:
```
rosrun gazebo_plugin joint_trajectory_client
and/or
rosrun tcp_ip_comm server_node
```


