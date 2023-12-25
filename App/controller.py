import time
from typing import Literal
import hid
from tools import get_usb_devices
import threading

""" Remote RC """
RC_VID = 0x1209
RC_PID = 0x4F54


""" PS4 Dualshock :) """
PS4_VID = 0x054C
PS4_PID = 0x09CC


class Controller:
    def __init__(self, vendor_id: int, product_id: int, device_name: Literal["PS4", "RC"]) -> None:
        self.vendor_id = vendor_id
        self.product_id = product_id
        self.h = None
        self.reading_thread = None
        self.running = False
        self.device_name = device_name

    def initialize_device(self):
        try:
            self.h = hid.device()
            self.h.open(self.vendor_id, self.product_id)  # Open the device

            print("Manufacturer: %s" % self.h.get_manufacturer_string())
            print("Product: %s" % self.h.get_product_string())

        except IOError as e:
            print(e)
            print("Error: Device not found")

    def start_reading(self):
        self.running = True
        self.reading_thread = threading.Thread(target=self.read_loop)
        self.reading_thread.start()

    def stop_reading(self):
        self.running = False
        if self.reading_thread:
            self.reading_thread.join()

    def read_loop(self):
        while self.running:
            try:
                data = self.h.read(64)  # Adjust the size as needed
                if data:
                    if self.device_name == "RC":
                        self.rc_parse_data(data)
                    else:
                        self.ps4_parse_data(data)
                    # print(self.x, self.y)
                    # print(self.direction)
            except IOError as e:
                print(e)
                break
            time.sleep(0.001)

    def ps4_parse_data(self, data):
        self.x = data[1]
        self.y = data[2]
        self.direction = "None"
        if data[1] >= 160:
            self.direction = "Right"
        elif data[1] <= 96:
            self.direction = "Left"
        else:
            self.direction = "None"

    def rc_parse_data(self, data):
        self.x = data[4]
        self.y = data[5]
        self.direction = "None"
        if data[4] >= 5:
            self.direction = "Right"
        elif data[4] <= 2:
            self.direction = "Left"
        else:
            self.direction = "None"

    def get_direction(self):
        return self.direction

    def close_device(self):
        if self.h:
            self.h.close()


if __name__ == "__main__":
    get_usb_devices()
    controller = Controller(PS4_VID, PS4_PID, "PS4")
    # controller = Controller(RC_VID, RC_PID, "RC")

    controller.initialize_device()
    controller.start_reading()

    while True:
        try:
            time.sleep(1)
        except KeyboardInterrupt:
            break
    controller.stop_reading()
