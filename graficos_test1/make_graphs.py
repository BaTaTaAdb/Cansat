import matplotlib.pyplot as plt
import matplotlib.ticker as ticker
import pandas as pd
from datetime import datetime

# Best graphs: Data6


for i in range(10):
    # Read the CSV file
    # Replace with your CSV file path
    file_path = f'./Testes/DATA_2023_11_16/DATA{i+1}.CSV'
    data = pd.read_csv(file_path)
    data['Z-Position'] = data['Z-Position'] * 100
    data['Timestamp'] = data['Timestamp'] / 1000

    if i+1 == 4:
        data = data[(data['Timestamp'] >= 80) & (data['Timestamp'] <= 85)]

    # Create the first figure for Descent Velocity
    """fig1 = plt.figure(figsize=(10, 5))
    plt.plot(data['Timestamp'], data['Descent Velocity'],
             label='Descent Velocity')
    plt.title('Descent Velocity over Time')
    plt.xlabel('Timestamp')
    plt.ylabel('Descent Velocity')
    plt.legend()"""

    # Create the second figure for Vel_Z
    fig1 = plt.figure(figsize=(10, 5))
    plt.plot(data['Timestamp'], data['Z-Position'],
             label='Z-Position', color='red')
    plt.title('Z-Position over Time')
    plt.xlabel('Timestamp (s)')
    plt.ylabel('Z-Position (m)')
    plt.legend()

    fig2 = plt.figure(figsize=(10, 5))
    plt.plot(data['Timestamp'], data['Pressure'],
             label='Pressure', color='orange')
    plt.title('Pressure over Time')
    plt.xlabel('Timestamp (s)')
    plt.ylabel('Pressure (hPa)')
    plt.legend()

    # Fig3
    fig3, ax1 = plt.subplots(figsize=(10, 5))
    # Plot Pressure on the primary y-axis
    color = 'orange'
    ax1.plot(data['Timestamp'], data['Pressure'],
             label='Pressure', color=color)
    ax1.set_xlabel('Timestamp (s)')
    ax1.set_ylabel('Pressure (hPa)', color=color)
    ax1.tick_params(axis='y', labelcolor=color)
    # ax1.set_ylim(bottom=None, top=1020)  # Set maximum value for primary y-axis
    ax1.grid(True, linestyle='-', color='gray',
             which='both')  # Primary axis grid
    # Use Formatter for precise labels
    ax1.yaxis.set_major_formatter(ticker.FormatStrFormatter('%.2f'))

    # Setting x-axis limit
    # ax1.set_xlim(left=0)  # Start x-axis at 0

    # Set number of ticks
    ax1.yaxis.set_major_locator(ticker.MaxNLocator(20))

    # Setting grid and ticks for x-axis
    ax1.xaxis.set_major_locator(ticker.MaxNLocator(10))  # Major ticks
    ax1.xaxis.set_minor_locator(ticker.AutoMinorLocator(2))  # Minor ticks

    ax1.xaxis.set

    # Create a second y-axis for Z-Position
    ax2 = ax1.twinx()
    color = 'red'
    ax2.plot(data['Timestamp'], data['Z-Position'],
             label='Z-Position', color=color)
    ax2.set_ylabel('Z-Position (m)', color=color)
    ax2.tick_params(axis='y', labelcolor=color)
    # Set maximum value for secondary y-axis
    # ax2.set_ylim(bottom=None, top=-0.1)
    # Minor ticks for secondary y-axis
    ax2.yaxis.set_minor_locator(ticker.AutoMinorLocator(2))

    # Formatting grid for secondary y-axis
    ax2.grid(True, which='both', axis='y', linestyle='--', color='gray')

    # Rotating x-axis labels for better readability
    plt.xticks(rotation=45)

    # Adding title and legend
    plt.title('Pressure and Z-Position over Time')
    fig3.tight_layout()

    # To handle the legend for both lines
    lines_1, labels_1 = ax1.get_legend_handles_labels()
    lines_2, labels_2 = ax2.get_legend_handles_labels()
    ax2.legend(lines_1 + lines_2, labels_1 + labels_2, loc='upper left')

    fig4 = plt.figure(figsize=(10, 5))
    plt.plot(data['Timestamp'], data['Heading Velocity'],
             label='Heading Velocity', color='orange')
    plt.title('Heading Velocity over Time')
    plt.xlabel('Timestamp (s)')
    plt.ylabel('Heading Velocity (m/s)')
    plt.legend()

    # Display both figures
    # plt.show(block=False)  # 'block=False' allows the program to continue running

    # Ask the user if they want to save the graphs
    # if input("Save graphs? ").lower() in ("y", "s"):
    if True:
        # formatted_date = datetime.now().strftime("%Y-%m-%d_%H-%M-%S")
        formatted_date = datetime.now().strftime("%Y-%m-%d")
        # fig1.savefig(f"./Graphs/DATA{i+1}_Descent_Velocity_{formatted_date}.png")
        fig1.savefig(
            f"./Graphs/Z-Position/DATA{i+1}_Z-Position_{formatted_date}.png")
        fig2.savefig(
            f"./Graphs/Pressure/DATA{i+1}_Pressure_{formatted_date}.png")
        fig3.savefig(
            f"./Graphs/Combined/DATA{i+1}_Combined_{formatted_date}.png")
        fig4.savefig(
            f"./Graphs/HeadingVelocity/DATA{i+1}_HeadingVelocity_{formatted_date}.png")

        plt.close()
