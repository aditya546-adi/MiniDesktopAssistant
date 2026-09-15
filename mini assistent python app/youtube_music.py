import asyncio

from winrt.windows.media.control import (
    GlobalSystemMediaTransportControlsSessionManager as MediaManager
)

from serial_manager import SerialManager
from expressions import Expression

assistant = SerialManager("COM7")

async def music_monitor():

    last_state = None

    while True:

        try:
            sessions = await MediaManager.request_async()

            is_playing = False

            for session in sessions.get_sessions():

                if session.source_app_user_model_id != "Chrome":
                    continue

                info = await session.try_get_media_properties_async()

                if not info.title:
                    continue

                playback = session.get_playback_info().playback_status

                if playback == 4:
                    is_playing = True
                    break

            if is_playing != last_state:

                if is_playing:
                    print("🎵 MUSIC PLAYING")
                    assistant.send_expression(Expression.MUSIC)

                else:
                    print("⏸️ MUSIC STOPPED")
                    assistant.send_expression(Expression.NORMAL)

                last_state = is_playing

        except Exception as e:
            print("[!] Music error:", e)

        await asyncio.sleep(0.5)


asyncio.run(music_monitor())




 