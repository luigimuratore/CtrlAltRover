<div align="center">
  
# **Ctrl+Alt+Rover**  

YouTube video: https://www.youtube.com/watch?v=NtzdcXTT0lE



<img src="https://github.com/user-attachments/assets/bb571947-9537-4be8-b1b9-f51236d3c7e8" width="258" >

<img src="https://github.com/user-attachments/assets/8aec6359-d8e9-464a-b26d-df6d11f6b411" width="250" >

### Master's Degree in Mechatronic Engineer  

<img src="https://github.com/luigimuratore/Fluid_Automation-CONVEYORS/assets/126814136/c104c1e7-fe39-4fee-b0c7-95fbba004564" width="80" >

# **Robot Learning**

### Muratore Luigi		
### Gennero Giorgia

--------------------
</div>


# Abstract
The extension of the analysis involved the physical construction of a mobile robot to test the Sim-to-Real transfer.
----------------------------------

# Mobile Robot: Sim-to-Real Transfer

## Description

The extension of the analysis involved the physical construction of a mobile robot to test the Sim-to-Real transfer. The robot was built entirely at home and features the following key components:

- **Chassis and Wheels:**  
  The robot’s structure is a rectangular wooden frame, laser-cut with a Cartesian robot. The wheels are Mecanum wheels, chosen for their omnidirectional capabilities, which allow the vehicle to move in any direction. Both the body and the rollers of the wheels were designed in Autodesk Inventor (see the CAD model image below) and 3D-printed in PLA before being assembled. The fully assembled rover is shown in the image following the description.

- **Manipulator Arm:**  
  A 5-degree-of-freedom manipulator arm is mounted on top of the chassis, further enhancing its functionality.

- **Actuation:**  
  The robot is powered by four DC motors controlled using two L298N drivers. A 12V power bank serves as the power source.

- **Sensors:**  
  The robot is equipped with:
  - Four ultrasonic sensors for measuring distances, mounted on the four sides of the robot.
  - Four infrared sensors mounted under the central body.
  - A camera for potential vision-based tasks.
  - A LiDAR for mapping and obstacle detection.

- **Controller:**  
  A Raspberry Pi 4b is used to control all functionalities—from running Python scripts to test each feature independently to using ROS2 Humble for handling sensors, motion control, and even loading trained models for real-world tests.

<img src="https://github.com/user-attachments/assets/bb571947-9537-4be8-b1b9-f51236d3c7e8" width="200" >
*Figure: CAD model in Inventor*

The goal of this physical robot is to implement a reinforcement learning algorithm to enable autonomous navigation and tasks such as parking. The robot must learn to navigate close to a wall on its right side and park inside a designated area. The initial requirement is that the robot must be positioned close to the wall and not too far from the parking slot.

---

## Mecanum Wheels Insight

Mecanum wheels allow omnidirectional motion, making them ideal for applications requiring high maneuverability. Each wheel has external rollers positioned at a 45° angle relative to the wheel’s axis. This configuration enables movement in any direction by varying the speed and rotation of each wheel. For example:

- **Forward/Backward:**  
  All wheels rotate at the same speed and in the same direction, so the longitudinal force vectors add up while the transverse vectors cancel out.

- **Rotation:**  
  Wheels on one side rotate in one direction while those on the opposite side rotate in reverse, generating a torque about the vertical axis.

- **Sideways:**  
  Diagonal wheels rotate in the same direction and the other diagonal wheels rotate oppositely, causing the transverse vectors to sum up while the longitudinal ones cancel.

This unique capability makes Mecanum wheels ideal for robotics and transfer vehicles where space and omnidirectional motion are critical.


<img src="https://github.com/user-attachments/assets/7db5314b-44ac-4ead-b859-ce8ff8164f63" width="250" >
*Figure: Mecanum wheels navigation*

---

## Simulation Environment – MuJoCo

The robot was simulated in the MuJoCo environment to test its behavior. The simulation involved creating a virtual model of the rover and its surroundings:

- **Robot Model:**  
  The rover is modeled with a central body and spherical wheels. To approximate the behavior of the Mecanum wheels (which are complex to model), spheres with two actuators (representing two rotational axes) were used. Gravity is incorporated into the model.

- **Sensors:**  
  Ultrasonic sensors are placed at the center of the lateral faces to measure distances.

- **Environment:**  
  The simulation environment consists of a flat floor and walls that define the parking area. Note that all dimensions in the simulation are scaled three times larger than the physical robot because of issues encountered in the MuJoCo environment.

The simulation is controlled via an XML file (`rover.xml`), which defines the robot's features and environment.



<img src="https://github.com/user-attachments/assets/50dda0cc-047a-402e-9561-840d371337ee" width="250" >
*Figure: Rover in MuJoCo environment*

Below are images showing the full simulation environment:

<img src="https://github.com/user-attachments/assets/5014884e-936f-4dfb-93d4-2a9b04bb644f" width="250" >
*Figure: Initial position in MuJoCo*

<img src="https://github.com/user-attachments/assets/4f76b070-d651-4efa-8ed5-23cb9df78e8a" width="250" >
*Figure: Target position in MuJoCo*

The training and testing processes are managed by the scripts `train_rover.py` and `test_rover.py`, respectively.

### Observation and Action Space
The observation is a vector including distances measured by the four ultrasonic sensors. For example:  

obs = [distance_front, distance_rear, distance_left, distance_right]


- **Action Space:**  
The action space is simplified for the parking task. The available actions are described in the table below.

| Action | Description  |
| ------ | ------------ |
| 0      | Move forward |
| 1      | Move right   |
| 2      | Stay still   |

---

## Training

The **Proximal Policy Optimization (PPO)** algorithm is used for training. PPO improves the expected reward while ensuring stable policy updates by using a clipped objective function that prevents excessively large updates. To enhance exploration, an **epsilon-greedy policy** strategy is integrated. The agent selects a random action with probability (1 – ε) and follows the best-known action with probability ε. This trade-off between exploration and exploitation allows the agent to experience a variety of situations, leading to a more robust policy.

The following hyperparameters were used during training:
- **Learning rate (α):** 3 × 10⁻⁴
- **Clipping parameter (ε):** 0.2
- **Discount factor (γ):** 0.98
- **Batch size:** 64
- **Entropy coefficient:** 0.05
- **Number of steps:** 1024

---

## Reward Function

In training, a form of **curriculum learning** was implemented to gradually introduce more complex tasks to the agent. Initially, the reward function encouraged simple behaviors (e.g., moving forward) by providing high rewards for advancing. Once this skill was learned, more challenging actions (such as translating right or stopping) were gradually introduced.

Key aspects of the reward function:
- A positive reward is given when the robot moves right into the parking slot (i.e., first moving forward, then translating right).
- A negative reward is given if the robot touches the walls, remains stationary when it should move forward, or continues moving forward when it should translate right.
- A penalty is applied for any incorrect action.

### Mathematical Description

The reward function is defined as follows:

r = { 1 - 0.2 * a + malus, if not park { 0 + 𝟙(a ≠ 0) * (1001 - a^6) + malus, if park

where the malus is defined by:

malus = { -3, if crash and |d_right| < d_safe { 0, otherwise


with:
- `park = True` if |d_right| > 2
- `crash = True` if |d_right| < d_safe and not park

The termination condition is:

done = True if current_step ≥ max_steps


Here, 𝟙(·) is the indicator function that returns 1 when the condition is true and 0 otherwise, and **a** represents the action index taken by the agent.

### Reward Algorithm Pseudocode

```plaintext
reward ← 1
crash ← False

if distance_right < safe_distance then
    crash ← True

if not park then
    reward ← reward - 0.2 × action
    if distance_right > 2 then
         park ← True
    end if
end if

if park then
    crash ← False
    if action = 0 then
         reward ← reward - 1
    else
         reward ← reward + 1000 - (action^6)
    end if
end if

if crash and |distance_right| < safe_distance then
    reward ← reward - 3
end if

done ← (current_step ≥ max_steps)
```

### TESTING

The simplified action space eases policy learning by reducing the number of decisions the agent must consider. However, it also introduces limitations. For instance, if the rover becomes stuck (e.g., in a corner), it cannot recover by translating sideways due to the restricted set of actions.

During testing, it was observed that while the rover often reached the parking area, it sometimes approached the wall too closely or got stuck at the corners. Despite these issues, when not hindered by these failure cases, the rover successfully reached its target position.

The trained model used for testing is stored as `parking.zip`.


### Real Rover Performance
The best-trained model was transferred to the Raspberry Pi environment for testing on the real rover. Although deploying an unstable policy in a real-world setting is generally not recommended, the simplicity of the actions and the non-hazardous nature of the tests allowed for real-world trials.

Key points during real-world testing:

Hardware and Software Challenges:
Some issues arose due to hardware compatibility and software integration.
Environmental Discrepancies:
The simulated environment (scaled larger due to MuJoCo issues) did not perfectly match the real-world setup. For instance, while the rover’s mass was accurately derived from the CAD model, friction was not modeled in simulation because the 3D-printed wheels were coated with a rubber-like material—making it hard to compute an exact friction coefficient.
Performance:
Multiple tests were performed with varied initial positions and parking area sizes. Although the rover generally recognized the target parking area, it often failed to enter the slot correctly, sometimes getting stuck near the walls.

### Conclusion and Future Works
This project explored the challenges and solutions of sim-to-real transfer in reinforcement learning using both a simulated Hopper environment and a custom-built rover. By applying Proximal Policy Optimization (PPO) and Uniform Domain Randomization, the study demonstrated the importance of robust training methods to bridge the gap between simulation and reality.

Key takeaways include:

The physical robot showcases the potential of RL in real-world tasks such as autonomous navigation and parking.
Curriculum learning and a carefully designed reward function enabled incremental learning of complex behaviors.
The current policy, although promising, suffers from instability and limited recovery options when encountering unfavorable states.
Future work will focus on:

Expanding the action space to enable more nuanced decision-making and recovery strategies.
Integrating motor encoder data to improve positional accuracy.
Utilizing the onboard infrared sensors for smoother navigation.
Incorporating the front-mounted camera with computer vision algorithms (e.g., object recognition) to assist in various tasks.
This project underscores the potential of reinforcement learning and sim-to-real transfer in robotics while highlighting areas for further improvement and research.
