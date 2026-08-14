from serial_manager import SerialManager
from expressions import Expression

assistant = SerialManager("COM7")

print("Assistant Started!")

while True:
    cmd = input("Command: ").strip()

    if cmd.upper() == "EXIT":
        break

    elif cmd.upper().startswith("TEXT "):
        assistant.send_text(cmd[5:])

    elif cmd.upper().startswith("BODY "):
        assistant.send_body(cmd[5:])

    else:
        try:
            assistant.send_expression(Expression[cmd.upper()])
        except KeyError:
            print("Invalid command!")