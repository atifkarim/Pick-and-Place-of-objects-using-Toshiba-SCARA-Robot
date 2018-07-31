# Description

Toshiba TH650 SCARA robot used for pick and place object from an conveyor belt. Detection of the object was done by Kinect V2 depth sensor.
After performing the detection of the coordinates of the object the position of the object's regarding conveyor belt's axis was transferred
to robot using ROS node. Calculating time and speed of the conveyor belt robot performed the action of pick and place. TO differentiate
between desired and unwanted object on the conveyor belt HOG(Histogram of Oriented Gradient) feature based cascade classifier was used. Position
of the object was converted to robot's world coordinate system. A socket communication was there to perform the communication between operating 
computer and the robot.

# Dependencies

• ROS indigo on a computer with Ubuntu 14.04

http://wiki.ros.org/indigo/Installation/Ubuntu

• Kinect V2 sensor ROS package

https://github.com/code-iai/iai_kinect2

# Procedure to run the package

Procedure for initializing robot communication
node
1. At the beginning in the ROS terminal execute the command roslaunch
kinect2 bridge kinect2 bridge.launch. It will initiate the Kinect V2
camera and start the rqt Gui which stands for changing the parameter
for filtering PCL data.
2. Initiate rosrun robot communication object sensing node by which
the presence of the object will be done and it sends a boolean True or
False value based on object presence to the robot communication node.
3. Run rosrun pick and place pcl filter for getting the published XYZ
coodinate of the object. It publishes these position information to the
robot communication node.
4. Execute the robot communication node by rosrun robot communication
stationary belt receive command or rosrun robot communication
movebelt receive command. Accordingly, the first node is for stationary condition of conveyor belt and the later for moving conveyor belt.


Linking the robot communication node with
controller
1. First of all, the LAB computer needs to be connected to the Server Network to send data from the computer to the controller of the robot. This
can be done by pressing the network icon at the right bottom of the
desktop screen.
2. Then turn on the Power button of the controller and wait untill it shows
"Connected" on the ROS terminal of the node "stationary belt receive
command" or "movebelt receive command". At this time red light
of the SERVO OFF button will light up.
3. When the terminal will show "Connected" then push the SERVO ON
button. After clicking this button a sound will be heard of unlocking the
servo motor of the robot. So it is recommended to push the RUN button
atleast 5 seconds later of pushing SERVO ON button.
