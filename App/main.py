import serial
import time
import matplotlib.pyplot as plt
import matplotlib.animation as animation
from controller import Controller, PS4_VID, PS4_PID, RC_PID, RC_VID

ser = serial.Serial(
    port='COM3',
    baudrate=9600,
    timeout=2000
)
ser.setRTS(0)
ser.bytesize = serial.EIGHTBITS  # number of bits per bytes
ser.parity = serial.PARITY_NONE  # set parity check: no parity
ser.stopbits = serial.STOPBITS_ONE  # number of stop bits
ser.xonxoff = False  # disable software flow control
ser.rtscts = False  # disable hardware (RTS/CTS) flow control
ser.dsrdtr = False  # disable hardware (DSR/DTR) flow control


def read_serial_data():
    try:
        if ser.in_waiting > 0:
            line = ser.readline().decode('utf-8').rstrip()
            data = line.split(',')
            if len(data) == 5:
                return [float(d) for d in data]
    except Exception as e:
        print(f"Error reading data: {e}")
    return None


def send_serial_data(data: int):
    """
    Sends an integer data to the serial port.
    """
    try:
        # Convert the integer to a string and encode it
        ser.write(str(data).encode())

        # Optionally, you can add a newline character to signify the end of data
        ser.write(b'\n')
        print(f"Sent {data}")
        time.sleep(0.01)

    except Exception as e:
        print(f"Error sending data: {e}")


def get_plot_format(ax):
    # Set Y axis limit of plot
    ax.set_ylim([0, 360])
    ax.set_title("Arduino Data")                        # Set title of figure
    ax.set_ylabel("Value")                              # Set title of y axis


def animate(i, ax, data_list):
    data = read_serial_data()
    if data:
        # Process and plot the data
        data_list.append(data)
        data_list = data_list[-50:]  # Keep only the latest 50 data points

        ax.clear()
        get_plot_format(ax)

        # Plot each data point
        for j in range(5):
            ax.plot([d[j] for d in data_list])

    # Get controller command
    controller_command = controller.get_direction()
    ax.set_title(f"Arduino Data - Command: {controller_command}")

    # Send the command based on controller input
    match controller_command:
        case "Right":
            controller_command = 1
        case "Left":
            controller_command = -1
        case _:
            controller_command = 0
    send_serial_data(controller_command)


if __name__ == "__main__":
    try:
        data_list = []
        controller = Controller(PS4_VID, PS4_PID, "PS4")
        # controller = Controller(RC_VID, RC_PID, "RC")
        controller.initialize_device()
        controller.start_reading()

        fig = plt.figure()
        ax = fig.add_subplot(111)

        ani = animation.FuncAnimation(
            fig, animate, fargs=(ax, data_list), interval=50, frames=100)
        plt.show()
        controller.stop_reading()
    finally:
        ser.close()
