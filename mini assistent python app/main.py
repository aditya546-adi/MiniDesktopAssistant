"""
from serial_manager import SerialManager
from expressions import Expression
import time

assistant = SerialManager("COM7")

print("Connected")

assistant.send_expression(Expression.HAPPY)

print("Sent")

while True:
    time.sleep(1)
"""
from serial_manager import SerialManager
from expressions import Expression
import time

assistant = SerialManager("COM7")

while True:
    assistant.send_expression(Expression.HAPPY)
    print("HAPPY")
    time.sleep(3)

    assistant.send_expression(Expression.NORMAL)
    print("NORMAL")
    time.sleep(3)