import time
import pygetwindow as gw
from serial_manager import SerialManager
from expressions import Expression

assistant = SerialManager("COM7")

last_state = None

while True:

    music_found = False

    windows = gw.getAllTitles()

    for title in windows:
        if "YouTube Music" in title:
            music_found = True
            break

    if music_found != last_state:

        if music_found:
            print("🎵 MUSIC PLAYING")
            assistant.send_expression(Expression.MUSIC)

        else:
            print("⏸️ MUSIC STOPPED")
            assistant.send_expression(Expression.NORMAL)

        last_state = music_found

    time.sleep(1)