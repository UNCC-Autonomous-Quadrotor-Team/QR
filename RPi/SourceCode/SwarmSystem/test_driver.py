import serial 
import time as t
import Xbee

xbee = Xbee.Xbee('/dev/ttyAMA0',9600)
#time.sleep(1.5)
msg = 'Hello from Raspberry Pi'
address = 0x0000
options = 0x00
frame_id = 0x00

result = xbee.SendTransmitRequest(msg,address,frame_id,options,1)

while 1:
    t.sleep(10)
    xbee.receive_packet()

