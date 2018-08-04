import signal
signal.signal(signal.SIGINT, signal.SIG_DFL)
import asyncio
import json
import random
from convert import convert
import re
import regex

class VerifyServerWechatProtocol(asyncio.Protocol):
    def __init__(self, que):
        self.queue = que

    def connection_made(self, transport):
        peername = transport.get_extra_info('peername')
        print('<Wechat Server> Connection from {}'.format(peername))
        self.transport = transport

    def data_received(self, data):
        message = data.decode()
        jItem = json.loads(message)
        try:
            jItem['script'] = convert(jItem['script'])
            jItem['function'] = self.validFunction(jItem['function'], jItem['varlist'], jItem['colist'])
            self.transport.write("<Wechat Server> correct data".encode())
            self.queue.put_nowait(jItem)
        except RuntimeError as r:
            print(r.args)
            self.transport.write("<Wechat Server> compiling error".encode())
        except asyncio.QueueFull:
            self.transport.write("<Wechat Server> full".encode())
        finally:
            self.transport.close()

    def validFunction(self, func, varlist, colist):
        pat = r"(^|[+\-*^(\ ]+)(%s)([+\-*/^\ )]+|$)"
        xx, yy, zz = zip(*regex.findall(r"(^|[+\-*^(\ ]+)([a-zA-Z]+)([+\-*/^\ )]+|$)", func, overlapped=True))
        for i in varlist:
            if not i in yy:
                raise RuntimeError("varlist mismatch")
        px = re.compile(pat % varlist[0])
        py = re.compile(pat % varlist[1])
        pz = re.compile(pat % varlist[2])
        func = re.sub(px, lambda m: m.group(0).replace("x", "pos.x"), func)
        func = re.sub(py, lambda m: m.group(0).replace("y", "pos.y"), func)
        func = re.sub(pz, lambda m: m.group(0).replace("z", "pos.z"), func)
        for j in colist:
            if not j in yy:
                raise RuntimeError("colist mismatch")
        return func
        
                


class AcquireServerPlatformProtocol(asyncio.Protocol):
    def __init__(self, que):
        self.queue = que

    def connection_made(self, transport):
        peername = transport.get_extra_info('peername')
        print('<Platform Server> Connection from {}'.format(peername))
        self.transport = transport

    def data_received(self, data):  
        try:
            item = queue.get_nowait()
            self.transport.write(("true " + json.dumps(item)).encode())
            print("true")
            self.transport.close()
        except asyncio.QueueEmpty:
            self.transport.write("false".encode())
            print("false")
            self.transport.close()


queue = asyncio.Queue(500)

loop = asyncio.get_event_loop()
wServer = loop.create_server(
    lambda: VerifyServerWechatProtocol(queue), '0.0.0.0', 12138)
pServer = loop.create_server(
    lambda: AcquireServerPlatformProtocol(queue), '0.0.0.0', 12139)
server = loop.run_until_complete(asyncio.gather(wServer, pServer))
server1 = server[0]
server4 = server[1]
# Serve requests until Ctrl+C is pressed
print('wServer Serving on {}'.format(server1.sockets[0].getsockname()))
print('pServer Serving on {}'.format(server4.sockets[0].getsockname()))

try:
    loop.run_forever()
except KeyboardInterrupt:
    loop.stop()


server1.close()
server4.close()
loop.run_until_complete(server4.wait_closed())
loop.run_until_complete(server1.wait_closed())
loop.close()
