<div align="center">

# **Ctrl+Alt+Rover**

YouTube video: https://www.youtube.com/watch?v=NtzdcXTT0lE


  <img src="https://github.com/luigimuratore/Fluid_Automation-CONVEYORS/assets/126814136/c104c1e7-fe39-4fee-b0c7-95fbba004564" width="150" >


### Master's Degree in Mechatronic Engineer

# **Robot Learning**

### Muratore Luigi		
### Gennero Giorgia

--------------------
</div>


# Abstract
We will show you the results of our robotics project we did during this semester.

----------------------------------

## IDEA
Our idea is based on a **5 degrees of freedom anthropomorphic robot manipulator**. We drew inspiration from different online projects.

Since we did the design part before the main concepts of the robotics course, we mistakenly focused on the aesthetics part more than the technical one.
So, for example, we chose 5 DOF even if, during the course, we figured out that the **best solution would have been 6 DOF**.

This is because a robotic arm with 5 degrees of freedom has some limitations, such as:
#### • Limited dexterity
#### • Less versatility

A secondary problem was that the **wrist we built was not spherical**.

Due to these problems, we could use neither some procedures and some approximations treated during the lectures nor the analytic solution of Inverse Kinematics but the numerical one. 

----------------------------------

## CAD
Using modelling and CAD programs we moved the project to a 3Dspace, we chose** SolidWorks and Fusion360** to design the robot and to set the requirements.
We chose SolidWorks because we have the licence thanks to Politecnico and Fusion360 instead, because there is the possibility to create some animations directly inside the program, so we did some loop movements in a very easy way to better visualize our idea.

<div align="center">
  <img src="https://github.com/luigimuratore/Anthropomorphic_robot_manipulator_5DOF-Im_not_a_Robotic_Arm/assets/126814136/4a408277-ff97-4677-998e-b5391bf33465" width="500" />
</div>




------------------------------

## GOAL
Our base goal was to use the robot as a **pick and place** to take an object from one position and move it to another one.

https://github.com/luigimuratore/Anthropomorphic_robot_manipulator_5DOF-Im_not_a_Robotic_Arm/assets/126814136/2e70519c-70a1-426c-b131-4d7889ed0b72

------------------------------------------

## Bill of materials
The robot has been totally **3D printed and built at home**.
In order to complete the 3d printing part we used just over **4 kg of plastic**, and it took about **250 hours overall**. (including problems and faulty print that needed to be printed again)


<div align="center">
  <img width="800" alt="print" src="https://github.com/luigimuratore/Anthropomorphic_robot_manipulator_5DOF-Im_not_a_Robotic_Arm/assets/126814136/5b0815d6-134c-44f5-bfb3-7041eb927fa1">

  <img width="500" src="https://github.com/luigimuratore/Anthropomorphic_robot_manipulator_5DOF-Im_not_a_Robotic_Arm/assets/126814136/d4e30a13-b516-45d8-8a91-e5177a7d934c">
</div>



### **Motors**
We mainly used **stepper motors** for all the movements except for the end effector where we used a servo motor. 
In particular, we assembled:
#### • 1 Nema 14 for the first joint
#### • 2 Nema 23 for the second joint
#### • 1 Nema 17 with an internal reduction of 1:5 for the third joint
#### • 1 Nema 17 for the fourth joint
#### • 1 Nema 17 for the fifth joint
#### • 1 Servo of 20 kg for the gripper

### **Drivers**
Each motor is controlled in power by a (tb6560) driver.
(The driver is necessary because the stepper motor operates by accurately synchronizing with the pulse signal output from the controller to the driver itself.
Using switches mounted on the driver board, we could set all the parameters specific to each motor, such as the nominal current, excitation current, and step ratio.)


### **Power supply**
The whole system is supplied by a 24V 13A power supply.
All the drivers require 24 volts, so they are all connected in parallel to the output of the power supply. 
The servo, instead, needs about 6 volts, and it is connected to a smaller power supply.
The boards have a separate circuit, just to be sure to not supercharge them.

<div align="center">
  <img width="700" src="https://github.com/luigimuratore/Anthropomorphic_robot_manipulator_5DOF-Im_not_a_Robotic_Arm/assets/126814136/b7ff3642-92a2-4dbf-b5d3-34c699867e2e">
</div>


-------------------------------------

## **Control**
We designed different control algorithms.

We first based the control system on an** Arduino MEGA 2560**, such that allows us to control all the motors manually using 3 joysticks with 2 axes each.


https://github.com/luigimuratore/Anthropomorphic_robot_manipulator_5DOF-Im_not_a_Robotic_Arm/assets/126814136/d903b7e4-c2bc-40c0-92e4-ec4dd431185a


The Arduino Mega, however, has some limitations.
It is a single-task microcontroller, and it has one core so, it can only execute one instruction at a time.

So, to obtain faster computation and since in the future we would like to implement different sensors in the control system, we moved to a **Raspberry Pi 4b** that has enough power to run a Linux system with ROS2 where we ran all the python scripts and MoveIt.

As far as the Robot Control is concerned, we used a decentralized control system, particularly an independent joint control architecture.

This approach was chosen due to its simplicity in implementation and scalability for potential future modifications to the arm’s design.
In this architecture, each of the 5DOF stepper motors is controlled by a dedicated driver and controller. The control system transmits individual commands to each joint controller specifying the desired displacement, by means of step value.

Additionally, to keep the project cost-effective, we opted for a simpler control system without encoders.
The implementation of encoders in the future could enhance the control system’s precision by providing real-time feedback on the actual joint positions, potentially enabling closed-loop control for improved accuracy.

<div align="center">
  <img width="700" src="https://github.com/luigimuratore/Anthropomorphic_robot_manipulator_5DOF-Im_not_a_Robotic_Arm/assets/126814136/685748a0-2555-464a-b329-d6948ca6eea8">
</div>


## **Kinematics**
During the course, we studied direct and inverse kinematics to analyze all the main motion aspects of a robot.
In robotics, kinematics is the branch of study concerned with the relationship between the geometry of a robot’s structure (links and joints) and the motion of its end effector (the gripper or tool at the tip) doesn’t considering the forces or torques that cause the motion.

The performance of the robot can be analyzed looking at:
#### • **Workspace volume:**
the reachable volume where the robot’s end-effector can operate.
In our case, having a workspace close to a truncated cone, we have a volume of about 0.19 m3.
#### • **Accuracy**:
that is how closely a robot’s actions match the intended outcome. 
We estimated an error between 1 mm and 3 mm.
#### • **Repeatability**:
is how consistently a robot performs the same action over multiple attempts. In our case, is between 0.5 and 1 mm.
 

## •	**Forward Kinematics**
This problem involves calculating the position and orientation of the end effector given the joint angles (inputs) of the robot.

Where is and how is our end effector oriented?
Firstly, we would to have a kinematics model of our manipulator, and we started studying the direct kinematics problem with the Denavit–Hartenberg convention.
Thanks to this convention, we could describe each frame wrt the previous one using 4 parameters.
We schematized the manipulator in an open chain with 5 revolute joints, 6 links, and the gripper as an end effector, as it is shown in the picture with DH parameters.
Then we performed the Denavit-Hardenberg analysis to place the frames and have a kinematics model of the robot.

We studied Forward Kinematics on MATLAB, using:
### •	RVC-Toolbox with functions:
#### o	ETS3 to compute the homogeneous transformation matrix
#### o	fkine to compute the forward kinematics
#### o	others 
to schematise the robot in a simulated environment.


https://github.com/luigimuratore/Anthropomorphic_robot_manipulator_5DOF-Im_not_a_Robotic_Arm/assets/126814136/d33fc6ba-cdf6-4e45-b6f7-b525e0e66aaa


### •	**Inverse Kinematics**
here we need to solve another problem: how could achieve a desired position and orientation for the end effector function of the joint variables??
For this reason, we solved the inverse kinematics problem, we used the numerical solution in MATLAB instead of the analytical one due to the fact our wrist was not spherical.
In these videos, we obtained with the inverseKinematic function the joint variables results to reach a desired pose.
Then we checked the results inserting the obtained values in the q-variables of the simulated environment and we observed that we did not attain the same result but the best approximation solution.

https://github.com/luigimuratore/Anthropomorphic_robot_manipulator_5DOF-Im_not_a_Robotic_Arm/assets/126814136/a908d335-726f-4b8c-948e-b110806daab9


### **URDF**
To study motion planning there was a need for an external file that described the main characteristics of the robot’s structure.
This is a specific file that describes all the properties of the robot regarding links and joints.
In particular, it contains information such as mass, origin, geometry, material, inertia, limits, and collisions.
It is essential to handle the robot in the virtual environments to compute planning trajectory.

<div align="center">
  <img width="700" src="https://github.com/luigimuratore/Anthropomorphic_robot_manipulator_5DOF-Im_not_a_Robotic_Arm/assets/126814136/a5b70806-c542-4ff2-ba2a-f22638d0c6ce">
</div>


### **Simscape Multibody Link**
Thanks to the Simscape Multibody Link tool on SolidWorks, we could export the XML file.
This is another useful file that allowed us to create the model on MATLAB/Simulink as well.

Setting the parameters as limits, velocities, and accelerations we could simulate our model checking that everything worked also in simulink.

<div align="center">
  <img width="900" src="https://github.com/luigimuratore/Anthropomorphic_robot_manipulator_5DOF-Im_not_a_Robotic_Arm/assets/126814136/b65df7a1-e9b6-471a-94cc-4ac128d7f9c6">
</div>


### **ROS**
Working with ROS, we had to create a sort of network to manage to communicate and exchange data. We created a package with a subscriber that takes our input data, such as angle positions or points in the workspace, and a publisher that sends them to another node that runs the script for the robot.

<div align="center">
  <img src="https://github.com/luigimuratore/Anthropomorphic_robot_manipulator_5DOF-Im_not_a_Robotic_Arm/assets/126814136/b86069da-b7ac-4341-9d24-2145e0a2b2d6" />
</div>



### **Scripts**
We used **Python** for all the coding parts.
For example, once we found the correct angle of each joint, we needed to convert them into steps for the stepper motors.
It took into account the transmission ratio and excitation ratio.

In our case, the transmission ratio goes from 1 to 6.5, while the excitation ratio is either 4 or 8 or 32. 
The constant 1.8 is characteristic of the stepper motor, and it is essentially the angular distance the motor moves in a single step.

<div align="center">
  <img src="https://github.com/luigimuratore/Anthropomorphic_robot_manipulator_5DOF-Im_not_a_Robotic_Arm/assets/126814136/1c9b92eb-944d-4fa4-807d-f464ba73dcc9"/>
</div>

### **MoveIt**
Then with the URDF file, we could finally implement the model on MoveIt, a special tool that allows to plan trajectories and Inverse Kinematics with different types of solvers.
We set the environment by putting the robot in its “zero position”, then we added a virtual cube in the workspace.

For all the computations, we used an already existing library of MoveIt called **OMPL - RRT planning library.**

This is one of the planning algorithms in MoveIt, in particular:
### • **OMPL** (Open Motion Planning Library):
this is a free and open-source software library that provides a collection of algorithms for solving motion planning problems.

It doesn’t deal with specific robot kinematics or dynamics but focuses on the core task of finding a collision-free path for a robot to move from a starting configuration (pose) to a goal configuration while avoiding obstacles in the environment.
### • **RRT** (Rapidly-exploring Random Tree):
RRT is a sampling-based motion planning algorithm.
It iteratively builds a tree-like structure in the robot’s configuration space (all possible poses the robot can take).

After choosing the solver we started working on the interface between the robot and the cube, so we started planning the trajectory:

Once we did that, we executed the planning computed by the solver, and we obtained the Inverse Kinematics solution with all the angles related to each joint.


https://github.com/luigimuratore/Anthropomorphic_robot_manipulator_5DOF-Im_not_a_Robotic_Arm/assets/126814136/9106dfd2-7dc9-4f0e-8a77-2a585b057a1e

-----------------------------------------------------

# **Tests**
## **Pick and Place**
The goal of this first test was to use the robot for Pick and Place operations.
Specifically, we put a cube in the workspace and we computed the trajectories to move it to a second position.

(In this first position we had the best accuracy error of 2 mm; we suspect mainly due to the pulley of the first motor that skidded a little bit.
All the other motors attained the position with an error of less than 1 mm.)

In the end the robot reached the correct position to move the cube and, playing with Python code, we were able to move the whole system very smoothly.

https://github.com/luigimuratore/Anthropomorphic_robot_manipulator_5DOF-Im_not_a_Robotic_Arm/assets/126814136/16b4ba27-fd71-4346-87d7-b9f26c1f5906

## **I’m not a Robotic arm**
I’m not a Robotic arm was more like an entertaining challenge than a learning experience because it needed technical movements that produced errors in the Inverse Kinematic solver since we had one degree of freedom less than the solver required.
After some attempts and some approximation, we reached the goal.

In this case we had the most problems.
As we can see in the video, we could not do a single transition movement from the first position to the second one, so it took three more, and it seems not very smooth, because the Python scripts produced a lot of errors, and we are still checking why.

Other than that, the Inverse Kinematic solution was correct, and all the positions were reached successfully.


https://github.com/luigimuratore/Anthropomorphic_robot_manipulator_5DOF-Im_not_a_Robotic_Arm/assets/126814136/357e5dae-3826-43bf-a5b1-750ac5aa274b




-------------------------------------------------

## **Future upgrades**
Looking at the future, we have a lot of upgrades or implementations in mind.
Actually, we are already working on some of them, while about the last we need more study.


### **Vacuum gripper**
One of the upgrades we are working on is a new end effector solution.
We would like to substitute the classic gripper, the “two fingers” one, with a new concept using a vacuum system and suction cups to hold objects.
On the right there is a design we are currently testing, and on the left there is the company from where we drew inspiration.
We are studying the effort needed to efficiently hold an object so the power necessary to be generated by a pump.


### **Stereo camera**
The second upgrade, almost ready to be implemented, is a stereo camera.
Our goal is to use the stereo cameras for making stereo-views and 3D pictures, so process them with algorithms of **object recognition** and **object classification**. In this optic, we could put objects wherever in the workspace and let the robot process the data and decide what to do according to their colors and their shapes.
So far, we put a simple camera in front of the robot, outside the workspace, that is able to recognize the color of the object placed in the workspace and some simple shapes. We are working with OpenCV on Python, in particular with algorithms of edge detection for the shape and HSV colorspace for color detection.

Since the camera is not a stereo-camera, it is not able to detect the exact position of the object and so gives the robot the information to compute the inverse kinematics.
However, it is possible to put an object in a position that the robot already knows and perform the recognition algorithms.
We are aiming to take a stereo camera or create it using two cameras and calibrating them, and configure it directly on the top of the robot.


### **Artificial Intelligence**
A very interesting implementation could be the Artificial Intelligence.
Nowadays, it is very easy to play with different types of AI so why don’t do something useful for our projects.

#### **Voice control**
A very interesting step could be to add a voice control to the system, so something that recognizes the voice and, using predefined words, could perform some tasks associated to the word itself.

#### **Code generator**
The AI may also be used to generate code, and maybe train the algorithms to improve the accuracy at every trajectory done.
The last two could be merged, so the last step would be to combine code generator with voice control, to ask for a specific task and let the AI work on it to generate the code for the robot.


### **EMG control**
The last upgrade we tough is to combine this project with another project we are working on, which is based on Electromyography (EMG).
Electromyography, in fact, is exactly the technique for evaluating and recording the electrical activity produced by skeletal muscles, we use it to read signals from the muscles and process them to perform predefined tasks.
It could be very interesting to try to combine these two projects to achieve a remote control of the robot arm using electrodes placed on a human body.

<div align="center">
  <img width="900" src="https://github.com/luigimuratore/Anthropomorphic_robot_manipulator_5DOF-Im_not_a_Robotic_Arm/assets/126814136/361bf8ad-98e1-4fd1-b121-80568bf2eba0">
</div>


-----------------------------------------
