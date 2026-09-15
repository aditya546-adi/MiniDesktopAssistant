import asyncio
import json

from winrt.windows.devices.bluetooth.rfcomm import (
    RfcommServiceProvider,
    RfcommServiceId,
)
from winrt.windows.networking.sockets import StreamSocketListener
from winrt.windows.storage.streams import DataReader, DataWriter

from winrt.windows.ui.notifications.management import UserNotificationListener
from winrt.windows.media.control import (
    GlobalSystemMediaTransportControlsSessionManager as MediaManager
)


# ============================================================
# GLOBALS
# ============================================================

connected_socket = None
connected_writer = None

notification_active = False


# ============================================================
# YOUTUBE MUSIC MONITOR
# ============================================================

async def music_monitor():

    last_state = None

    while True:

        try:

            if notification_active:
                last_state = None
                await asyncio.sleep(0.5)
                continue

            sessions = await MediaManager.request_async()

            is_playing = False

            for session in sessions.get_sessions():

                # Only Chrome
                if session.source_app_user_model_id != "Chrome":
                    continue

                info = await session.try_get_media_properties_async()

                # Ignore sessions without a title
                if not info.title:
                    continue

                # ------------------------------------------------
                # YouTube Music detection
                # ------------------------------------------------
                # Normal YouTube usually has empty album metadata.
                # YouTube Music provides album information.
                # ------------------------------------------------

                if not info.album_title:
                    continue

                playback = (
                    session.get_playback_info().playback_status
                )

                # 4 = Playing
                if playback == 4:
                    is_playing = True
                    break

            # ------------------------------------------------
            # State changed
            # ------------------------------------------------

            if is_playing != last_state:

                if connected_writer is not None:

                    if is_playing:

                        connected_writer.write_string(
                            "MUSIC\n"
                        )

                        await connected_writer.store_async()

                        print(
                            "[MUSIC] YouTube Music PLAYING 🕺"
                        )

                    else:

                        connected_writer.write_string(
                            "NORMAL\n"
                        )

                        await connected_writer.store_async()

                        print(
                            "[MUSIC] YouTube Music PAUSED/STOPPED"
                        )

                last_state = is_playing

        except Exception as e:

            print(
                "[!] Music monitor error:",
                e
            )

        await asyncio.sleep(0.5)


# ============================================================
# WINDOWS NOTIFICATION
# ============================================================

async def notification_listener():

    listener = UserNotificationListener.current

    status = await listener.request_access_async()

    print(
        "[NOTIFICATIONS] Access status:",
        status
    )

    if int(status) != 1:

        print(
            "[!] Windows notification access not granted."
        )

        return

    print(
        "[OK] Windows notification listener is running!"
    )

    print(
        "[OK] Waiting for phone notifications..."
    )

    loop = asyncio.get_running_loop()

    def notification_changed(sender, args):

        loop.call_soon_threadsafe(
            lambda: asyncio.create_task(
                process_notification(
                    listener,
                    args
                )
            )
        )

    listener.add_notification_changed(
        notification_changed
    )

    while True:

        await asyncio.sleep(3600)


# ============================================================
# PROCESS NOTIFICATION
# ============================================================

async def process_notification(listener, args):

    global connected_writer
    global notification_active

    notification_active = True

    try:

        notification = listener.get_notification(
            args.user_notification_id
        )

        app_name = (
            notification
            .app_info
            .display_info
            .display_name
        )

        visual = notification.notification.visual

        texts = []

        for binding in visual.bindings:

            for text_element in binding.get_text_elements():

                text = text_element.text.strip()

                if text:
                    texts.append(text)

        if not texts:

            return

        data = {

            "app":
                texts[0]
                if len(texts) > 0
                else "",

            "title":
                texts[1]
                if len(texts) > 1
                else "",

            "message":
                "\n".join(texts[2:])
                if len(texts) > 2
                else "",

            "id":
                notification.id,
        }

        # Use actual Windows app name
        data["app"] = app_name

        print()

        print(
            "========== NOTIFICATION =========="
        )

        print(
            json.dumps(
                data,
                ensure_ascii=False,
                indent=2
            )
        )

        print(
            "==================================="
        )

        # ------------------------------------------------
        # Send notification to ESP32
        # ------------------------------------------------

        if connected_writer is not None:

            title = (
                data
                .get("title", "")
                .replace("\n", " ")
                .strip()
            )

            message = (
                data
                .get("message", "")
                .replace("\n", " ")
                .strip()
            )

            payload = (

                "MESSAGE\n"

                "TEXT:" +
                title +
                "\n"

                "BODY:" +
                message +
                "\n"
            )

            connected_writer.write_string(
                payload
            )

            await connected_writer.store_async()

            print(
                "[BT] Notification sent to ESP32!"
            )

            # Keep notification priority
            await asyncio.sleep(6)

            notification_active = False

        else:

            print(
                "[BT] ESP32 not connected."
            )

    except Exception as e:

        print(
            "[!] Notification error:",
            e
        )


# ============================================================
# BLUETOOTH CONNECTION
# ============================================================

async def handle_connection(socket):

    global connected_socket
    global connected_writer

    print()

    print(
        "[+] ESP32 CONNECTED!"
    )

    # Close previous connection
    if connected_writer is not None:

        try:
            connected_writer.close()

        except:
            pass

    connected_socket = socket

    connected_writer = DataWriter(
        socket.output_stream
    )

    reader = DataReader(
        socket.input_stream
    )

    try:

        while True:

            count = await reader.load_async(
                4096
            )

            if count == 0:

                print(
                    "[-] ESP32 disconnected."
                )

                break

            data = bytearray(count)

            reader.read_bytes(
                data
            )

            text = data.decode(
                "utf-8",
                errors="replace"
            )

            print(
                "[ESP32 DATA]",
                text
            )

    except Exception as e:

        print(
            "[!] Bluetooth connection error:",
            e
        )

    finally:

        try:
            reader.close()

        except:
            pass

        try:
            socket.close()

        except:
            pass

        if connected_socket == socket:

            connected_socket = None
            connected_writer = None

        print(
            "[-] Bluetooth connection closed."
        )


# ============================================================
# MAIN
# ============================================================

async def main():

    print(
        "========================================"
    )

    print(
        "       FORGE BRIDGE RECEIVER"
    )

    print(
        "========================================"
    )

    print()

    # --------------------------------------------------------
    # Bluetooth RFCOMM
    # --------------------------------------------------------

    print(
        "[BT] Creating RFCOMM service..."
    )

    service_id = (
        RfcommServiceId.serial_port
    )

    provider = (
        await RfcommServiceProvider.create_async(
            service_id
        )
    )

    listener = StreamSocketListener()

    loop = asyncio.get_running_loop()

    def connection_received(
        sender,
        args
    ):

        loop.call_soon_threadsafe(

            lambda: asyncio.create_task(
                handle_connection(
                    args.socket
                )
            )
        )

    listener.add_connection_received(
        connection_received
    )

    await listener.bind_service_name_async(
        provider.service_id.as_string()
    )

    provider.start_advertising(
        listener
    )

    print()

    print(
        "[OK] RFCOMM service is running!"
    )

    print(
        "[OK] UUID:",
        service_id.as_string()
    )

    print(
        "[OK] Waiting for ESP32..."
    )

    print()

    # --------------------------------------------------------
    # Windows Notifications
    # --------------------------------------------------------

    notification_task = asyncio.create_task(
        notification_listener()
    )

    # --------------------------------------------------------
    # YouTube Music Monitor
    # --------------------------------------------------------

    music_task = asyncio.create_task(
        music_monitor()
    )

    print(
        "[OK] Windows notification capture enabled!"
    )

    print(
        "[OK] YouTube Music detection enabled!"
    )

    print()

    print(
        "========================================"
    )

    print(
        "        FORGE BRIDGE READY 🔥"
    )

    print(
        "========================================"
    )

    print()

    try:

        while True:

            await asyncio.sleep(
                3600
            )

    except KeyboardInterrupt:

        print()

        print(
            "Stopping Forge Bridge receiver..."
        )

    finally:

        notification_task.cancel()

        music_task.cancel()

        provider.stop_advertising()

        listener.close()


# ============================================================
# START
# ============================================================

if __name__ == "__main__":

    asyncio.run(
        main()
    )