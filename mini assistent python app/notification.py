from dataclasses import dataclass

@dataclass
class Notification:

    app: str
    sender: str
    message: str