#!bin/python
#Modified and constructed by Terrill Massey tmasse11@uncc.edu
#Sources used: Steven James,serdmanczyk.gihub.io/XBee-API-PythonArduino-Tutorial/>, Xbee Documentation
#
#
import serial as ser
import time as t
import numpy as np
class Xbee: 
    def __init__(self,serialport,baudrate):
        self.ser_conn = ser.Serial(
            port = serialport,
            baudrate=baudrate,
            stopbits = 1,
            parity = 'N',
            bytesize = 8,
            )
    
        

    def SendTransmitRequest(self,raw_data_msg,destination_address,frameid,options,verbose):
        #setup the packet frame
        if not raw_data_msg:
            return 0
        if type(raw_data_msg) is int:  #Convert int into chr equivalent. Note Chr(1 byte) - Translates to 0x00 hex. 
            first_byte_msg  = (raw_data_msg & 0xFF00) >> 8 
            second_byte_msg = (raw_data_msg & 0x00FF)
            data_msg = bytearray.fromhex('{:02X}{:02X}'.format(first_byte_msg,second_byte_msg))
        else:
            data_msg = raw_data_msg
        

        #convert to bytearray
        
#FORMAT: <start delimiter (7E)] <Length: MSB LSB> <Frame Identifier> <Destination address High> <Destination Address Low> <Options>        <MSG>
#SIZE:         <1 Byte>            < 2 bytes>     < 1 byte>              <1 byte>                    <1 byte>             <1 byte>  <up to 100 bytes>
        packet_header = '7E 00 {:02X} 01 {:02X} {:02X} {:02X} {:02X}'.format(
            len(data_msg) + 5,  # LSB LENGTH
            frameid,
            (destination_address & 0xFF00) >> 8 ,  # Destination MSB/High 
            (destination_address & 0x00FF),       # Destination Low Address
            options,                  # options byte 
            )


        #construct the Data Frame using packet header and data msg. 
        data_frame = bytearray.fromhex(packet_header)
        
        data_frame.extend(data_msg)
        
        #calculate the checksum byte
        
        data_frame.append(0xFF - (sum(data_frame[3:]) & 0xFF))
  
        
        #Print the packet if verbose is set. 
        
        if(verbose):
            print "TX Packet:" + self.format_to_string(data_frame)
        

    
        #Send the packet to the xbee for transmission. 
        return self.ser_conn.write(data_frame)
    


    
    def format_to_string(self,data_frame):
        #convert the hexadecimals to a string format.
        return " ".join("{:02x}".format(i) for i in data_frame)


    def receive_packet(self):



        #Define lists  that will be used to store the byte arrays. 
        self.ValidRxDataFrames = [] # initalize a list to store validated data frames that were received.
        self.receive_buffer =[] #initialize a list variable to append bytes from the physical receive buffer in the UART register.
        self.rxmessages =[]  # initalize a list variable that will store all fully processed messages received. 
            
           


            
            
            ############# READ ALL BYTES FROM THE UART RECEIVE BUFFER ####################
             
            #See how many bytes are in the receive buffer. 
        content_size_in_rx_buffer = self.ser_conn.inWaiting()
            
            #for debugging only. This prints out the amount of bytes in buffer.
        print content_size_in_rx_buffer
        print '------------------------'

            
        while content_size_in_rx_buffer:
            data_chunk = self.ser_conn.read(content_size_in_rx_buffer)
            content_size_in_rx_buffer -= len(data_chunk)
            self.receive_buffer.extend(data_chunk)
            



            ##### TRIM OFF ESCAPE CHARACTERS #######################
            #unescape all contents in the receive buffer. This essentially removes the '\' character before everything. The output will be a bytearray.
        self.receive_buffer_unescaped = self.Unescape(self.receive_buffer)

            ####### BREAK APART THE DATA IN BUFFER TO CORRECT DATA FRAMES########################
            #split all contents in the receive buffer into seperate data frames delimited b 0x7E
        dataframes = self.receive_buffer_unescaped.split(bytes(b'\x7E'))
            
        print  len(dataframes) - 1 
            #print type(dataframes) DEBUG ONLY 
        
       #####VALIDATION####################
        for dataframe in dataframes[1:]: 
                #validate all of the frames. 
            valid = self.Validate_frame(dataframe)
            print '----------------------'
            # print self.format_to_string(msg)
            # print 'Type:'
            # print type(msg)

                #For Debugging Only
            print 'Validity'
            print valid

            
       #Extract the valid data frames 

        for valid_dataframe  in self.ValidRxDataFrames :
            print '------------------------------------'
            print self.format_to_string(valid_dataframe)
            self.ExtractMessage(valid_dataframe)

        # Print rxmessages <for debugging only>    

        for rxmessage in self.rxmessages :
            print' --------------------------'
            print self.format_to_string(rxmessage)
        
                
        
         
        return self.rxmessages
                                              

                
            
            
            
           
            

            
                            
            
    def Validate_frame(self,data_frame):
        

        
        # 9 bytes is Minimum length to be a valid Rx frame
        #  LSB, MSB, Type, Source Address(2), RSSI,
        #  Options, 1 byte data, checksum
      
        # use the escape characters to count the number of bytes in the data frame
        if len(data_frame) < 9:
            return False
        
     
        

        LSB = data_frame[1]

        # Check to make sure that the data frame is not smaller than the LSB specified. 
        if LSB > (len(data_frame[2:]) - 1):
            return False
        
         # Validate checksum
        if (sum(data_frame[2:3+LSB]) & 0xFF) != 0xFF:
            return False
        

        #append all correct data frames to the RxMessages buffer 
        self.ValidRxDataFrames.append(data_frame)
        return True
   
      
    def Unescape(self, dataframe):  # Remove all escape characters in buffer
        output = bytearray()

        if(len(dataframe) == 0) :
            # Last byte indicates an escape, can't unescape that
            return output
        
      
        skip = False
        for i in range(len(dataframe)):
            if skip:
                skip = False
                continue
            
            if dataframe[i] == 0x7D:
                output.append(dataframe[i+1] ^ 0x20)
                skip = True
            else:
                output.append(dataframe[i])
                
        return output 

    def ExtractMessage(self,dataframe):
        #
       ## #THIS FUNCTION ASSUMES THAT THE START DELIMITER OF A VALIDATED DATAFRAME IS ALREADY STRIPPED OFF OF THE DATA FRAME############################################


       end_of_message_length = dataframe[1] + 1 # Since this is a verified data frame, the second byte will always be the LSB. Since the MSB and LSB are first. An offset of 1 is applied. TODO: read the MSB and LSB for super large data frames. 


       #note that list indexing is non inclusive for the upper bound. Although that is strange, to compensate this you will need to  add one to the upper bound.  
       data = dataframe[7:(end_of_message_length + 1)] #Actual Data
       nodeid = dataframe[3:5] #node id
       data_message = nodeid + data # node id with actual data. nodeid will always come first and will be the first two bytes (16 bit addressing is used) 
        
       return self.rxmessages.append(data_message)
         
        
        
    

    
