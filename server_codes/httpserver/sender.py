import signal
signal.signal(signal.SIGINT, signal.SIG_DFL)
import asyncio
import json
import random



class infoWechatProtocol(asyncio.Protocol):
    def __init__(self, obj, loop):
        self.message = json.dumps(obj)
        self.loop = loop

    def connection_made(self, transport):
        transport.write(self.message.encode())
        print('<Net> connected, Data sent')

    def data_received(self, data):
        print('<Net> Data received: {}'.format(data))

    def connection_lost(self, exc):
        print('<Net> The server closed the connection')
        self.loop.close()

def sender(obj, loop):
    loop.run_until_complete(loop.create_connection(
        lambda: infoWechatProtocol(obj, loop), '111.231.81.39', 12138))
