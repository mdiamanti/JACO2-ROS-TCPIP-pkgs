# Jaco2-ROS-TCPIP-pkgs
Current repository includes the appropriate ROS packages that allow remote communication and control of Kinova Jaco<sup>2</sup> robotic arm via the use of TCP/IP protocol.

## Supported Versions
The configuration used was 64 bit Ubuntu 14.04 and ROS indigo. These packages may work with other configurations as well, but it has only been tested for the one recommended.

## Installation
The installation as follows was tested on Ubuntu 14.04 and ROS indigo.

### Step 1. Install kinova-ros
Install [kinova-ros](https://github.com/Kinovarobotics/kinova-ros#installation) package, following the installation instructions that are analytically reported in the link.

### Step 2. Install JACO2-ROS-TCPIP-pkgs
Add the git repository in your catkin workspace:
```
cd ~/catkin_ws/src
git clone https://github.com/mdiamanti/JACO2-ROS-TCPIP-pkgs.git
```
