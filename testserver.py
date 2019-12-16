from flask import Flask, request, abort

from linebot import (
    LineBotApi, WebhookHandler
)
from linebot.exceptions import (
    InvalidSignatureError
)
from linebot.models import (
    MessageEvent, TextMessage, TextSendMessage,
)

request_status = "idle"
 
app = Flask(__name__)

line_bot_api = LineBotApi('dLblb4VDb8m8UqCZ72OK6iZBvaI+qYod18SSqX8H/OGvPVqkM9J/1kA0ON387QBPZs249em7lobV/oI+VSlzVf9Di7IzHS2Dftdi+TMLF8FxIPZ8e7ZdwBiayIT/kx1a0PQYOJ7S8pR3X9O6np9SewdB04t89/1O/w1cDnyilFU=')
handler = WebhookHandler('060622f7370b033cba6950d2b56c524c')

@app.route("/callback", methods=['GET', 'POST'])
def callback():
    # get X-Line-Signature header value
    signature = request.headers['X-Line-Signature']

    # get request body as text
    body = request.get_data(as_text=True)
    app.logger.info("Request body: " + body)

    # handle webhook body
    try:
        handler.handle(body, signature)
    except InvalidSignatureError:
        print("Invalid signature. Please check your channel access token/channel secret.")
        abort(400)

    return 'OK'


@handler.add(MessageEvent, message=TextMessage)
def handle_message(event):
    global request_status
    input_str = event.message.text
    if input_str == 'Auto':
        line_bot_api.reply_message(
            event.reply_token,
            TextSendMessage(text='Auto mode enabled')
        )
        request_status = "auto"
    elif input_str == 'Manual':
        line_bot_api.reply_message(
            event.reply_token,
            TextSendMessage(text='Manual mode enabled')
        )
        request_status = "manual"
    elif input_str == 'Dust level':
        line_bot_api.reply_message(
            event.reply_token,
            TextSendMessage(text='Dust level = some random number')
        )
    elif input_str == 'on':
        line_bot_api.reply_message(
            event.reply_token,
            TextSendMessage(text='Turn the system on!')
        )
        request_status = "on"
    elif input_str == 'off':
        line_bot_api.reply_message(
            event.reply_token,
            TextSendMessage(text='Turn the system off!')
        )
        request_status = "off"
    else:
        line_bot_api.reply_message(
            event.reply_token,
            TextSendMessage(text='Bot does not know what are you talking about.')
        )

@app.route('/helloesp', methods=['GET'])
def helloesp():
    return 'Hello ESP8266, from Flask'

@app.route('/status_request', methods=['GET'])
def status_request():
    # print(request_status)
    return request_status


if __name__ == "__main__":
    app.run(host='localhost', port=80)