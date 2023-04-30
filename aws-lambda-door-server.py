from urllib.request import urlopen, Request
import os
import re


def get_message(event):
    text = event["body"]
    message = re.search("Body=(.*)&To", text)
    return message.group(1)


def lambda_handler(event, context):

    # This is the url of the public webserver and the port (e.g. http://200.100.210.25:200/)
    webserver = os.getenv("webserver")
    url_turn_on_relay = webserver + "RELAY=ON"

    # This message sent through WhatsApp should be equal to this password for the webserver to work. This is one of many security measures you can implement.
    password = os.getenv("password")

    # Here we get the message from WhatsApp. You can also view all the details (using print(event)) that are sent like phone number from where the message was sent, etc.
    message = get_message(event)

    # If the message sent through WhatsApp is equal to the password, the request to switch the RELAY ON is made.
    if message == password:

        turn_on_relay = Request(url=url_turn_on_relay, method="GET")

        with urlopen(turn_on_relay) as response:
            print(response.status)
            print(response.read().decode())
