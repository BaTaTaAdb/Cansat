import matplotlib.pyplot as plt
import pandas as pd
from datetime import datetime

# Read the CSV file
file_path = 'test.csv'  # Replace with your CSV file path
data = pd.read_csv(file_path)

# Create the first figure for Accel_Z
fig1 = plt.figure(figsize=(10, 5))
plt.plot(data['Timestamp'], data['Accel_Z'], label='Z-Acceleration')
plt.title('Z-Acceleration over Time')
plt.xlabel('Timestamp')
plt.ylabel('Acceleration in Z')
plt.legend()

# Create the second figure for Vel_Z
fig2 = plt.figure(figsize=(10, 5))
plt.plot(data['Timestamp'], data['Vel_Z'], label='Z-Velocity', color='red')
plt.title('Z-Velocity over Time')
plt.xlabel('Timestamp')
plt.ylabel('Velocity in Z')
plt.legend()

# Display both figures
plt.show(block=False)  # 'block=False' allows the program to continue running

# Ask the user if they want to save the graphs
if input("Save graphs? ").lower() in ("y", "s"):
    formatted_date = datetime.now().strftime("%Y-%m-%d_%H-%M-%S")
    fig1.savefig(f"Accel_Z_{formatted_date}.png")
    fig2.savefig(f"Vel_Z_{formatted_date}.png")
