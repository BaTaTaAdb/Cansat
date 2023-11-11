import matplotlib.pyplot as plt
import pandas as pd

# Read the CSV file
file_path = 'test.csv'  # Replace with your CSV file path
data = pd.read_csv(file_path)

# Plotting Accel_Z
plt.figure(figsize=(10, 5))
plt.plot(data['Timestamp'], data['Accel_Z'], label='Z-Acceleration')
plt.title('Z-Acceleration over Time')
plt.xlabel('Timestamp')
plt.ylabel('Acceleration in Z')
plt.legend()
plt.show()

# Plotting Vel_Z
plt.figure(figsize=(10, 5))
plt.plot(data['Timestamp'], data['Vel_Z'], label='Z-Velocity', color='red')
plt.title('Z-Velocity over Time')
plt.xlabel('Timestamp')
plt.ylabel('Velocity in Z')
plt.legend()
plt.show()
