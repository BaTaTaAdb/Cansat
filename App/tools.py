import pywinusb.hid as hid


def get_usb_devices():
    # Find all USB HID devices
    all_devices = hid.HidDeviceFilter().get_devices()
    usb_devices = []

    for device in all_devices:
        usb_device = {}

        # Get device information
        usb_device['vendor_id'] = device.vendor_id
        usb_device['product_id'] = device.product_id
        usb_device['serial_number'] = device.serial_number
        usb_device['manufacturer'] = device.vendor_name
        usb_device['product'] = device.product_name

        usb_devices.append(usb_device)

    return usb_devices


if __name__ == "__main__":
    # Get the list of USB devices
    devices = get_usb_devices()

    # Print the information of each USB device
    for device in devices:
        print(f"Vendor ID: {device['vendor_id']}")
        print(f"Product ID: {device['product_id']}")
        print(f"Serial Number: {device['serial_number']}")
        print(f"Manufacturer: {device['manufacturer']}")
        print(f"Product: {device['product']}")
        print()

    # Make sure to install the pywinusb library before running the code
    # You can install it using pip: pip install pywinusb
