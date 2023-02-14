import time

import numpy as np
import serial
from helpers.serial_helper import SerialClass

def test_1():
    com = serial.Serial('/dev/ttyUSB0', baudrate=9600)
    time.sleep(3)
    print("send")
    print(com.write(b"fill"))
    print(com.readline())
    time.sleep(1)
    print("read")
    print(com.write(b"send"))
    print(com.readline())
    print("bin")
    print(com.write(b"bin"))
    buffer = com.read(20)
    print(np.frombuffer(buffer, dtype=np.uint16))
    print("done")

def struct_test():
    struct = np.dtype(
        [
            ('A0', np.uint16),
            ('A1', np.uint16),
            ('A2', np.uint16),
            ('A3', np.uint16),
            ('A4', np.uint16),
            ('A5', np.uint16)
        ]
    )
    com = serial.Serial('/dev/ttyUSB0', baudrate=115200)
    time.sleep(2)
    print(struct.itemsize)
    com.write(b'start')
    # time.sleep(1)
    list_ = []
    for i in range(1000):
        buffer = com.read(struct.itemsize)
        dat = np.frombuffer(buffer, dtype=struct)
        list_.append(dat)
        # print(dat)

    print(np.shape(list_))
    print(list_)
    com.close()
    print("done")

def str_test():
    com = serial.Serial('/dev/ttyUSB0', baudrate=115200)
    time.sleep(2)
    print(com.write(b'start\r\n'))
    print(com.readline())

if __name__ == '__main__':
    struct_test()

