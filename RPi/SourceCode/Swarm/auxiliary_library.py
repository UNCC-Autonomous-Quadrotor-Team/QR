import struct

class Data_manipulation:
    
    def int_to_bytearray(self,input_int):
       # most_significant_byte = (input_int & 0xFF00) >> 8
        #least_significant_byte = (input_int  & 0x00FF)
        #return bytearray().fromhex('{:02X}{:02X}'.format(most_significant_byte,least_significant_byte))
        
        #ATTENTION, PYTHON BY DEFAULT USES 32 BIT INTEGERS.NOT 16 BIT INTEGERS.        #HOWEVER NOTE THAT ZIGBEE USES 16 BIT ADDRESSING, SO THIS CANNOT BE USED.

        return bytearray(struct.pack('>i',input_int))
      

    def bytearray_to_int(self,input_bytearray):

        #**************************************************************************************************************************************************
        # DESCRIPTION: This function only handles 16 bit integers or 32 bit integers. Therefore the input must be a bytearray with 2 bytes or 4 bytes.
        #**************************************************************************************************************************************************
        


        
       #NOTES:        
       #IT IS ASSUMED THAT BYTEARRAYS ARE ORDERED WITH MOST SIGNIFICANT BIT FIRST (READING LEFT TO RIGHT)

        

        offset = len(input_bytearray) * 8 
        output_integer = 0 
        for i in range(0,len(input_bytearray)):
            offset -= 8
            output_integer += input_bytearray[i] << offset
            
       # if len(input_bytearray) == 4 : 
        #    output_integer =  (input_bytearray[0] << 24) + (input_bytearray[1] << 16) + (input_bytearray[2] << 8) + (input_bytearray[3])
       # elif len(input_bytearray) == 2 : 
        #    output_integer = (input_bytearray[0] << 8) + input_bytearray[1]
       # else:
        #    output_integer = -1 
            
        return output_integer



    def bytearray_to_string(self,byte_msg):
        return " ".join("{:02X}".format(i) for i in byte_msg)



class Visison_System:

    def __init__(self):

        print "Container for Vision System."
