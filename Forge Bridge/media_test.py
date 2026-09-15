import asyncio

from winrt.windows.media.control import (
    GlobalSystemMediaTransportControlsSessionManager as MediaManager
)


async def main():

    sessions = await MediaManager.request_async()

    print("========== MEDIA SESSIONS ==========")

    for session in sessions.get_sessions():

        print()
        print("APP ID:", session.source_app_user_model_id)

        try:
            info = await session.try_get_media_properties_async()

            print("TITLE :", info.title)
            print("ARTIST:", info.artist)
            print("ALBUM :", info.album_title)

            playback = session.get_playback_info().playback_status
            print("PLAYBACK:", playback)

        except Exception as e:
            print("ERROR:", e)

    print()
    print("====================================")


asyncio.run(main())