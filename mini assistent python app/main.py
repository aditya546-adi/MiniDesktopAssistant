from serial_manager import SerialManager
from expressions import Expression
from notification_manager import NotificationManager
import time

assistant = SerialManager("COM7")
manager = NotificationManager()

notification = manager.get_test_notification()

print("=" * 35)
print("Notification Received")
print()
print(f"App    : {notification.app}")
print(f"Sender : {notification.sender}")
print(f"Message: {notification.message}")
print("=" * 35)

assistant.send_expression(Expression.MESSAGE)

time.sleep(2)

assistant.send_expression(Expression.NORMAL)

assistant.close()