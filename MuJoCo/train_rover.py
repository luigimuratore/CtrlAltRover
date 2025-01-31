import gym
import numpy as np
import mujoco_py
from stable_baselines3 import PPO
from stable_baselines3.common.env_checker import check_env
from stable_baselines3.common.vec_env import DummyVecEnv
from stable_baselines3.common.monitor import Monitor
from stable_baselines3.common.callbacks import EvalCallback

# Define the MuJoCo environment
class MecanumWallFollowEnv(gym.Env):
    def __init__(self):
        super(MecanumWallFollowEnv, self).__init__()
        
        # Load MuJoCo XML model
        self.model = mujoco_py.load_model_from_path("rover.xml")
        self.sim = mujoco_py.MjSim(self.model)
        self.viewer = None

        # Observation space: sensor readings
        self.observation_space = gym.spaces.Box(
            low=0, high=np.inf, shape=(self.model.nsensor,), dtype=np.float32
        )
        # Action space: Forward, Right, Stop
        self.action_space = gym.spaces.Discrete(3)

        # Get the body ID of the robot
        self.robot_body_id = self.model.body_name2id("robot_body") 

        self.max_steps = 8000      # Maximum steps per episode
        self.current_step = 0
        self.safe_distance = 0.6   # Safe distance from the wall
        self.park = False          # Parking slot detection

        self.epsilon = 0.1         # Initial probability to explore 
        self.epsilon_decay = 0.995 # Decay per episode
        self.epsilon_min = 0.1     # Minimal exploration


    def reset(self):
        self.sim.reset()
        self.current_step = 0
        self.park = False
        return self._get_obs()
    
    def _get_obs(self):
        sensors = ["sensor_front", "sensor_rear", "sensor_left", "sensor_right"]
        distances = []
        for sensor in sensors:
            try:
                sensor_id = self.model.sensor_name2id(sensor)
                distances.append(self.sim.data.sensordata[sensor_id])
            except KeyError:
                distances.append(0.0)
        return np.array(distances, dtype=np.float32)


    # Useful function to monitor the training process 
    def get_robot_position(self):
        # Get the (x, y, z) position of the robot body.
        position = self.sim.data.body_xpos[self.robot_body_id]
        return position
    

    def step(self, action):
        self.current_step += 1

        # Epsilon-greedy logic
        if np.random.rand() < self.epsilon:
            action = self.action_space.sample()  # Explore: choose random action

        # Decay epsilon
        self.epsilon = max(self.epsilon * self.epsilon_decay, self.epsilon_min)


        # Set motor velocities
        velocity = 12.0
        if action == 0:  # Forward
            wheel_velocities = [velocity, velocity, velocity, velocity, 0, 0, 0, 0]
        elif action == 1:  # Right
            wheel_velocities = [0, 0, 0, 0, -velocity, -velocity, -velocity, -velocity]
        elif action == 2:  # Stop
            wheel_velocities = [0, 0, 0, 0, 0, 0, 0, 0]
        else:
            raise ValueError(f"Invalid action: {action}")

        # Apply velocities
        actuators = [
            "FL_wheel", "FR_wheel", "RL_wheel", "RR_wheel",
            "SIM_FL_wheel", "SIM_FR_wheel", "SIM_RL_wheel", "SIM_RR_wheel"
        ]
        for i, actuator in enumerate(actuators):
            actuator_id = self.model.actuator_name2id(actuator)
            self.sim.data.ctrl[actuator_id] = wheel_velocities[i]


        # Step simulation
        self.sim.step()

        # Get observations
        obs = self._get_obs()
        distance_front = obs[0]
        distance_rear = obs[1]  
        distance_left = obs[2]  
        distance_right = obs[3]

        position = self.get_robot_position()

        crash = False # Variable to activate/disactivate the crash penalty 
        if abs(distance_right)<self.safe_distance:
            crash = True

        # Base reward
        reward = 1

        if not self.park: 
            reward -= 0.2*action
            if distance_right > 2:
                self.park = True

        if self.park:
            crash = False
            if action == 0:
                reward -= 1
            else:
                reward += 1000 - pow(action,6)

        if crash and abs(distance_right) < self.safe_distance:  # Collision 
            reward -= 3  
    

        # Check termination condition
        done = bool(self.current_step >= self.max_steps)

        return obs, reward, done, {}

    
    def render(self, mode='human'):
        if self.viewer is None:
            self.viewer = mujoco_py.MjViewer(self.sim)
        self.viewer.render()


    def close(self):
        if self.viewer is not None:
            self.viewer = None



# TRAINING
env = MecanumWallFollowEnv()
env = Monitor(env, "./logs")
check_env(env) 

env = DummyVecEnv([lambda: env])

# Define PPO with optimized hyperparameters
model = PPO(
    "MlpPolicy",
    env,
    verbose=1,
    learning_rate=3e-4,
    gamma=0.98,
    n_steps=1024,
    batch_size=32,
    clip_range=0.2,
    ent_coef=0.05  
)

# Train the model
eval_callback = EvalCallback(
    env, best_model_save_path="./best_model", log_path="./logs", eval_freq=1000)

model.learn(total_timesteps=1000000, callback=eval_callback)

# Save the trained model
model.save("parking")
env.close()


