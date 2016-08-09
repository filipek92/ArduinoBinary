import threading

ESCAPE_CHAR = 27
START_CHAR = 1
END_CHAR = 4

ESCAPE_BYTE = bytes([ESCAPE_CHAR])

class Parser:
    def __init__(self, stream=None, callback=None, irqCallback=None):
        self.stream = stream    # instance variable unique to each instance
        self.callback = callback
        self.irqCallback = irqCallback
        self.buffer = bytearray()
        self.decoding = False
        self.escaped = False

        if stream is not None:
            self.thread = threading.Thread(target=self.worker)
            self.thread.daemon = True
            self.thread.start()

    def worker(self):
        if self.stream is None:
            return
        print("Parser started")
        try:
            while True:
                byte = self.stream.read(1)[0]
                self.parseByte(byte)
        except Exception as e:
            raise e


    def parse(self, data):
        packets = []
        for byte in data:
            tmp = self.parseByte(byte)
            if tmp is not None:
                packets.append(tmp)
        return packets

    def parseByte(self, byte):
        packet = None
        if self.escaped:
            if byte == START_CHAR:
                if self.decoding:
                    packet = bytes(self.buffer)
                    self.fireCallback(packet)
                self.buffer = bytearray()
                self.decoding = True
            elif byte == END_CHAR:
                if self.decoding:
                    packet = bytes(self.buffer)
                    self.fireCallback(packet)
                self.decoding = False
            elif byte == ESCAPE_CHAR:
                self.addToBuffer(ESCAPE_CHAR)
            else:
                self.fireIrqCallback(byte)
            self.escaped = False
        elif byte == ESCAPE_CHAR:
            self.escaped = True
        else:
            self.addToBuffer(byte)
        return packet


    def addToBuffer(self, byte):
        if self.decoding:
            self.buffer.append(byte)

    def fireIrqCallback(self, byte):
        if(self.irqCallback):
            self.irqCallback(byte)

    def fireCallback(self, packet):
        if(self.callback):
            self.callback(packet)

class Sender(object):
    def __init__(self, stream=None):
        self.stream = stream

    def encode(self, packet):
        data = bytes([ESCAPE_CHAR,START_CHAR])+packet.replace(ESCAPE_BYTE, ESCAPE_BYTE*2)+bytes([ESCAPE_CHAR,END_CHAR])

        if self.stream:
            self.stream.write(data)

        return data



def main():
    import serial
    import sys

    portname = sys.argv[1]
    print("Otviram port {}".format(portname))
    port = serial.Serial(portname, baudrate=115200)

    sender = Sender(port)
    parser = Parser(port, callback=callback, irqCallback=callbackIRQ)

    while True:
        try:
            data = bytes(map(int, input("Zadej data v packetu: ").split()))
            sender.encode(data)
        except ValueError as e:
            print(e)
        except KeyboardInterrupt:
            break


def callback(packet):
    print("Packet: {}".format(packet))

def callbackIRQ(code):
    print("Received IRQ: {}".format(code))

if __name__ == '__main__':
    main();
