#include <Arduino.h>
#include <BluetoothSerial.h>

#include "serial_handler.h"
#include "expressions.h"

BluetoothSerial SerialBT;

static String notificationTitle = "";
static String notificationBody = "";

static unsigned long lastReconnectAttempt = 0;

static TaskHandle_t bluetoothReconnectTaskHandle = NULL;
static volatile bool reconnectRequested = false;

// =======================================================
// Bluetooth reconnect task
// =======================================================

void bluetoothReconnectTask(void *parameter)
{
    while (true)
    {
        if (reconnectRequested)
        {
            reconnectRequested = false;

            Serial.println("[BT] Background reconnect attempt...");

            BTAddress pc("YOUR_PC_BLUETOOTH_MAC");

            if (SerialBT.connect(
                pc,
                0,
                ESP_SPP_SEC_ENCRYPT | ESP_SPP_SEC_AUTHENTICATE,
                ESP_SPP_ROLE_MASTER
            ))
            {
                Serial.println("[BT] Connected to PC!");

                setExpression(NORMAL);
            }
            else
            {
                Serial.println("[BT] Reconnect failed.");
            }
        }

        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

// =======================================================
// Init
// =======================================================

void initSerialHandler()
{
    Serial.begin(115200);

    SerialBT.enableSSP(false, false);
    SerialBT.begin("ForgeBridge-ESP32", true);

    Serial.println("Bluetooth started");

    Serial.println("Bonded devices:");
    Serial.println(SerialBT.getNumberOfBondedDevices());

    Serial.println("Connecting to ADITYAS-PC...");

    BTAddress pc("YOUR_PC_BLUETOOTH_MAC");

    if (SerialBT.connect(
        pc,
        0,
        ESP_SPP_SEC_ENCRYPT | ESP_SPP_SEC_AUTHENTICATE,
        ESP_SPP_ROLE_MASTER
    ))
    {
        Serial.println("Connected to PC!");
    }
    else
    {
        Serial.println("Failed to connect to PC.");
    }

    // Start Bluetooth reconnect task
    xTaskCreatePinnedToCore(
        bluetoothReconnectTask,
        "BTReconnect",
        8192,
        NULL,
        1,
        &bluetoothReconnectTaskHandle,
        0
    );

    Serial.println("[BT] Reconnect task started.");
}

// =======================================================
// Update
// =======================================================

void updateSerialHandler()
{
    // ===================================================
    // Bluetooth connection manager
    // ===================================================

    if (!SerialBT.connected())
    {
        if (millis() - lastReconnectAttempt >= 5000)
        {
            lastReconnectAttempt = millis();

            Serial.println("Bluetooth disconnected.");
            Serial.println("Trying to reconnect...");

            setNotificationState(NOTIFICATION_IDLE);
            setExpression(SLEEP);

            // Ask background task to reconnect
            reconnectRequested = true;
        }

        return;
    }

    // ===================================================
    // Receive commands
    // ===================================================

    bool hasData = false;
    String command = "";

    if (SerialBT.available())
    {
        command = SerialBT.readStringUntil('\n');
        hasData = true;
    }
    else if (Serial.available())
    {
        command = Serial.readStringUntil('\n');
        hasData = true;
    }

    if (!hasData)
        return;

    command.trim();

    Serial.print("Received: ");
    Serial.println(command);

    if (command == "NORMAL")
    {
        setNotificationState(NOTIFICATION_IDLE);
        setExpression(NORMAL);
    }
    else if (command == "HAPPY")
    {
        setNotificationState(NOTIFICATION_IDLE);
        setExpression(HAPPY);
    }
    else if (command == "SURPRISED")
    {
        setNotificationState(NOTIFICATION_IDLE);
        setExpression(SURPRISED);
    }
    else if (command == "SAD")
    {
        setNotificationState(NOTIFICATION_IDLE);
        setExpression(SAD);
    }
    else if (command == "THINKING")
    {
        setNotificationState(NOTIFICATION_IDLE);
        setExpression(THINKING);
    }
    else if (command == "ALERT")
    {
        setNotificationState(NOTIFICATION_IDLE);
        setExpression(ALERT);
    }
    else if (command == "MUSIC")
    {
        setExpression(MUSIC);
    }
    else if (command == "MESSAGE")
    {
        setExpression(MESSAGE);
        setNotificationState(NOTIFICATION_ENTER);
    }
    else if (command == "SLEEP")
    {
        setExpression(SLEEP);
    }
    else if (command.startsWith("TEXT:"))
    {
        notificationTitle = command.substring(5);

        Serial.print("Title: ");
        Serial.println(notificationTitle);
    }
    else if (command.startsWith("BODY:"))
    {
        notificationBody = command.substring(5);

        Serial.print("Body: ");
        Serial.println(notificationBody);
    }
}

// =======================================================
// Notification getters
// =======================================================

String getNotificationTitle()
{
    return notificationTitle;
}

String getNotificationBody()
{
    return notificationBody;
}