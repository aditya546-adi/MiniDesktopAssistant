Mini desk assistant :
- Made using an ESP32 and an OLED display with an animated face 
- It can detect yt music and change the animation when music is playing
- It can read Windows notifications and display them on the screen
- It interacts when you pet it 
- It goes to sleep when the Bluetooth is disconnected

TECH STACK

Hardware :
- Esp 32
- OLED display (SH1106)
- Capacitive Touch Switch

Software (esp 32) :
- c++
- Arduino framework
- Adafruit GFX
- Adafruit SH110X
- BluetoothSerial

Pc :
- python
- Runtime API's
  • Notifications
  • Media Control
  • Bluetooth RFCOMM

Comunication :
- Bluetooth Classic SPP
- Custom text-based serial protocol between Python and Esp 32

Development & Version Control :
- Arduino IDE
- PowerShell
- Git



Connections :
- OLED TO ESP32 :
   • VCC - 3V3
   • GND - GND
   • SCL - GPIO22
   • SDA - GPIO21
- Capacitive Touch Switch TO ESP32 :
   • VCC - 3V3
   • GND - GND
   • SIG - GPIO2
  
UPLOADING INO CODE

Once the OLED and touch sensor have been connected to the esp32, make a folder on your computer and download all the included files. Download the Arduino IDE, launch the Mini Assistant ino file, find the serial_handler.cpp tab, paste your Bluetooth MAC address where needed, and upload the code. 

CODE OF RECEIVER

To launch Forge Bridge, launch PowerShell, go to the Forge Bridge folder, confirm that the ESP32 is linked and Bluetooth is enabled, then launch Python receiver.py and leave the receiver running in the background. Make a shortcut for receiver.py and put it in the Windows Startup folder to have Windows launch the receiver automatically.



