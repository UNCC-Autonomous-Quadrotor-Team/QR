import serial as ser 
import time as t
#establish serial comm to the TX/RX pins (/dev/ttyAMA0)

xbee_com = ser.Serial(
    port = '/dev/ttyAMA0',
    baudrate = 115200,
    stopbits = 1,
    parity = 'N',
    bytesize = 8,
    
    )


#open the serial port. 

#xbee_com.open()
t.sleep(1.1)


#Enter AT command mode
xbee_com.write('+++')
#t.sleep(3.0)
t.sleep(1.7)
recieve_buffer_len = xbee_com.inWaiting()

while ( recieve_buffer_len > 0) : 
    
        recieved_byte = xbee_com.read(recieve_buffer_len)
        recieve_buffer_len -= len(recieved_byte)
        print recieved_byte


#Get pan ID 

    
 
xbee_com.close()
