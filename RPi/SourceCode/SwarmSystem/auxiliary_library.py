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
       
              


        
       #NOTES:        
       #IT IS ASSUMED THAT BYTEARRAYS ARE ORDERED WITH MOST SIGNIFICANT BIT FIRST (READING LEFT TO RIGHT)
       #UPDATE: Configured program to handle signed integers. However the length of the integers cannot be larger than 32 bits/ 4 bytes.
        #INPUT: BYTEARRAY
        #OUTPUT: INTEGER
        
        if len(input_bytearray) < 4:

            zero_padding = bytearray(4 - len(input_bytearray))
            input_bytearray = zero_padding + input_bytearray
            
            output_int, =  struct.unpack('>i',str(input_bytearray))
           # print type(output_int)
            return output_int
        elif len(input_bytearray) > 4: 
            print "Integer cannot be larger than 32 bits / 4 bytes " 
            return 0 
        elif len(input_bytearray) == 4:
            output_int, =  struct.unpack('>i',str(input_bytearray)) # struct.unpack returns a tuple of length 1. 
            #print type(output_int)
            return output_int
    #print "bytearrays must be 4 bytes long/ Can only handle 32 bit integers"
            
       # Variable sized bytearray (works for unsigned integers only)      
      #  offset = len(input_bytearray) * 8 
       # output_integer = 0 
       # for i in range(0,len(input_bytearray)):
        #    offset -=  8
         #   output_integer += input_bytearray[i] << offset
            
       # if len(input_bytearray) == 4 : 
        #    output_integer =  (input_bytearray[0] << 24) + (input_bytearray[1] << 16) + (input_bytearray[2] << 8) + (input_bytearray[3])
       # elif len(input_bytearray) == 2 : 
        #    output_integer = (input_bytearray[0] << 8) + input_bytearray[1]
       # else:
        #    output_integer = -1 
            
       # return output_integer



    def bytearray_to_string(self,byte_msg):
        return " ".join("{:02X}".format(i) for i in byte_msg)


    
