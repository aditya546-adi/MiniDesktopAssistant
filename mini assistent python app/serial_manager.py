import serial
import time


class SerialManager:
    def __init__(self, port="COM7", baudrate=115200):
        self.ser = serial.Serial(port, baudrate)
        time.sleep(2)      # Wait for ESP32 reset

    def send_expression(self, expression):
        self.ser.write(f"{expression.value}\n".encode())

    def close(self):
        self.ser.flush()
        self.ser.close()