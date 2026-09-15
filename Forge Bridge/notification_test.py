import asyncio
import json

from winrt.windows.ui.notifications.management import UserNotificationListener


async def main():
    listener = UserNotificationListener.current

    status = await listener.request_access_async()
    print("Access status:", status)
    print("Waiting for Windows notifications...")

    def notification_changed(sender, args):
        try:
            notification = listener.get_notification(
                args.user_notification_id
            )

            visual = notification.notification.visual

            texts = []

            for binding in visual.bindings:
                for text_element in binding.get_text_elements():
                    text = text_element.text.strip()

                    if text:
                        texts.append(text)

            if len(texts) >= 3:
                data = {
                    "app": texts[0],
                    "title": texts[1],
                    "message": "\n".join(texts[2:])
                }
            else:
                data = {
                    "app": texts[0] if len(texts) > 0 else "",
                    "title": texts[1] if len(texts) > 1 else "",
                    "message": ""
                }

            print("\n========== JSON ==========")
            print(json.dumps(data, ensure_ascii=False, indent=2))
            print("===========================")

        except Exception as e:
            print("Error reading notification:", e)

    listener.add_notification_changed(notification_changed)

    while True:
        await asyncio.sleep(1)


asyncio.run(main())