import serial
import time


class SerialManager:
    def __init__(self, port="COM7", baudrate=115200):
        self.ser = serial.Serial(port, baudrate)
        time.sleep(2)     
        

    def send_expression(self, expression):
        self.ser.write(f"{expression.value}\n".encode())
        self.ser.flush()
        print(f"Sent: {expression.value}")

    def close(self):
        self.ser.flush()
        self.ser.close()

    def send_text(self, text):
        self.ser.write(f"TEXT:{text}\n".encode())
        self.ser.flush()

    def send_body(self, body):
        self.ser.write(f"BODY:{body}\n".encode())
        self.ser.flush()

    def send_text(self, text):
        self.ser.write(f"TEXT:{text}\n".encode())
        self.ser.flush()
        print(f"Sent: TEXT:{text}")

    def send_body(self, body):
        self.ser.write(f"BODY:{body}\n".encode())
        self.ser.flush()
        print(f"Sent: BODY:{body}")