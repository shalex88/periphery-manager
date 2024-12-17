# Install 'pip install pyserial' in advance
import serial
import argparse
import time

def serial_client(port, baudrate):
    with serial.Serial(port, baudrate, timeout=1) as ser:
        print(f"[Serial Client] Connected on port {port} at {baudrate} baud")

        try:
            while True:
                message = input("[Serial Client] Enter message: ")
                ser.write(message.encode())
                ser.flush()
                ascii_values = [ord(char) for char in message]
                print(f'[Serial Client] Sent ({len(message)} bytes): '
                      f'"{message}" [{str(" ".join(map(str, ascii_values)))}]')

                time.sleep(0.5)

                response = ser.read(ser.in_waiting)
                data_list = list(response)
                print(f'[Serial Client] Received ({len(response)} bytes): '
                      f'"{response.decode()}" [{" ".join(map(str, data_list))}]')
        except KeyboardInterrupt:
            print("[Serial Client] Connection closed.")


def parse_arguments():
    parser = argparse.ArgumentParser(description='Connect to a serial port and send messages.')
    parser.add_argument('port', type=str, help='Serial port to connect to, e.g., COM3 or /dev/ttyUSB0')
    parser.add_argument('--baudrate', type=int, default=115200, help='Baudrate for the serial connection [default: 115200]')
    return parser.parse_args()


if __name__ == "__main__":
    args = parse_arguments()
    serial_client(args.port, args.baudrate)