import pywinusb.hid as hid

def get_gamepad_devices():
    # Find all USB HID devices
    all_devices = hid.HidDeviceFilter().get_devices()
    gamepad_devices = []

    for device in all_devices:
        try:
            device.open()

            # Iterate through capabilities of the device
            for report in device.find_input_reports():
                for item in report:
                    if item.usage_page == 0x01 and item.usage_id == 0x05:  # Generic Desktop Page and Gamepad Usage ID
                        gamepad_device = {
                            'vendor_id': device.vendor_id,
                            'product_id': device.product_id,
                            'serial_number': device.serial_number,
                            'manufacturer': device.vendor_name,
                            'product': device.product_name
                        }
                        gamepad_devices.append(gamepad_device)
                        break  # Break if a gamepad usage is found

        finally:
            device.close()

    return gamepad_devices

# Get the list of gamepad devices
gamepads = get_gamepad_devices()

# Print the information of each gamepad device
for gamepad in gamepads:
    print(f"Vendor ID: {gamepad['vendor_id']}")
    print(f"Product ID: {gamepad['product_id']}")
    print(f"Serial Number: {gamepad['serial_number']}")
    print(f"Manufacturer: {gamepad['manufacturer']}")
    print(f"Product: {gamepad['product']}")
    print()
