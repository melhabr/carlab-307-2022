import struct
from digi.xbee.devices import XBeeDevice

# TODO: Replace with the serial port where your local module is connected to.
PORT = "/dev/cu.usbserial-DN041RCA"
# TODO: Replace with the baud rate of your local module.
BAUD_RATE = 115200


device = XBeeDevice(PORT, BAUD_RATE)

def main():

    device = XBeeDevice(PORT, BAUD_RATE)   

    try:
        device.open()

        def data_receive_callback(xbee_message):
            print("From %s >> %s" % (xbee_message.remote_device.get_64bit_addr(),
                                     xbee_message.data.decode()))

        device.add_data_received_callback(data_receive_callback)

        while True:

            nums = input("Enter numbers: ").strip().split(' ')
            data = bytearray([int(x) for x in nums])
            device.send_data_broadcast(data)

    finally:
        if device is not None and device.is_open():
            device.close()


if __name__ == '__main__':
    main()