import serial
import time


ser = serial.Serial('/dev/ttyUSB0', 9600, timeout=1)

def send_command(command):
    ser.write(command.encode())

def main():
    try:
        while True:
            user_input = input("Enter 'C' to rotate CW, 'D' to rotate CCW, 'S' to stop: ")
            if user_input == 'C' or user_input == 'D' or user_input == 'S':
                send_command(user_input)
                response = ser.readline().decode().strip()
                print("Arduino:", response)
                if response.startswith("Angle"):
                    sensor_status = ser.readline().decode().strip()
                    print("Arduino:", sensor_status)
            else:
                print("Invalid input. Enter 'C', 'D', or 'S'.")
    except KeyboardInterrupt:
        print("Exiting...")
    finally:
        ser.close()

if __name__ == "__main__":
    main()
