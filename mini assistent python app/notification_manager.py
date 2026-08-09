from notification import Notification

class NotificationManager:

    def get_test_notification(self):

        return Notification(

            app="WhatsApp",

            sender="Rohit Sharma",

            message="Hi bro..."
        )