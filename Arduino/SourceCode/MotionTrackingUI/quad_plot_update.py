'''
@author: Jeeten
'''
"""*****************************************************************************************************************
1. Push buttons to enable and disable serial connection.
2. Graphs are plotted by reading data available on the serial port every second.
3. Push buttons to start and stop plotting
4. An API packet is received in every serial transmission. Every packet has the data in the following format:
<Command_ID><Height Data><Perperndicularity><Distance>
<  1 byte  ><  4 bytes  ><     4 bytes     >< 4 bytes>
5. The data is parsed and four different graphs are plotted to study and analyze the properties of the swarming robots.
******************************************************************************************************************"""


import sys
import numpy as np
import pyqtgraph as pg
import serial
import time
from PyQt4 import QtGui, QtCore
from numpy import dtype, inf

class Oneplot(QtGui.QWidget):
    def __init__(self):
        super(Oneplot, self).__init__()
        
        self.initUI()
        
    def initUI(self):

        """Form Layout"""
        QtGui.QWidget.__init__(self, parent=None)
        self.resize(640,480)
        self.setWindowTitle("Quadrotor Swarm Status")
        self.layout = QtGui.QGridLayout()
        self.setLayout(self.layout)
        
        #Plot1 - Plot the nodes w.r.t coordinator quadrotor as origin
        self.plot1 = pg.PlotWidget(title="<font color='blue'><b>Position</b></font>")
        self.layout.addWidget(self.plot1,1,0,1,2)
        self.plot1.resize(320,240)
        self.plot1.setXRange(-60, 60)
        self.plot1.setYRange(-60, 60)
        self.plot1.setLabel("bottom","<font size='4'>Distance (x)</font>")
        self.plot1.setLabel("left","<font size='4'>Perpendicularity (y)</font>")
        
        #Plot2 - Plot Rssi v/s time
        self.plot2 = pg.PlotWidget(title="<font color='blue'><b>RSSI</b></font>")
        self.layout.addWidget(self.plot2,1,2,1,2)
        self.plot2.resize(320,240)
        self.plot2_node1 = []
        self.plot2_node2 = []
        self.plot2.setLabel("bottom", "<font size='4'>Time (x)</font>")
        self.plot2.setLabel("left", "<font size='4'>Received Signal Strnegth (RSSI)</font>")
        
        #Plot3 - Plot Height v/s time
        self.plot3 = pg.PlotWidget(title="<font color='blue'><b>Height</b></font>")
        self.layout.addWidget(self.plot3,2,0,1,2)
        self.plot3.resize(320,240)
        self.plot3.setLabel("bottom", "<font size='4'>Time (x)</font>")
        self.plot3.setLabel("left", "<font size='4'>Height</font>")
        self.plot3_node1 = []
        self.plot3_node2 = []
        
        #Plot4 - Plot the path variance of the node quadrotors v/s the coordinator quadrotor
        self.plot4 = pg.PlotWidget(title="<font color='blue'><b>Variance</b></font>")
        self.layout.addWidget(self.plot4,2,2,1,2)
        self.plot4.resize(320,240)
        self.plot4.setLabel("bottom", "<font size='4'>Time (x)</font>")
        self.plot4.setLabel("left", "<font size='4'>Error</font>")
        self.plot4_node1 = []
        self.plot4_node2 = []
        
        
        #Connect Button
        self.Bconnect = QtGui.QPushButton("Connect",self)
        self.Bconnect.clicked.connect(self.Fconnect)        
        self.layout.addWidget(self.Bconnect,3,0)

        #Disconnect Button
        self.Bdisconnect = QtGui.QPushButton("Disconnect",self)
        self.Bdisconnect.clicked.connect(self.Fdisconnect)        
        self.layout.addWidget(self.Bdisconnect,3,1)
        
        #Start Button
        self.Bstart = QtGui.QPushButton("Start",self)
        self.Bstart.clicked.connect(self.Fstart)
        self.layout.addWidget(self.Bstart,3,2)
        
        #Stop Button
        self.Bstop = QtGui.QPushButton("Stop",self)
        self.Bstop.clicked.connect(self.Fstop)        
        self.layout.addWidget(self.Bstop,3,3)
        
        #Button Status
        self.Bdisconnect.setDisabled(True)
        self.Bstart.setDisabled(True)
        self.Bstop.setDisabled(True)
        
        #Serial Label
        self.label = QtGui.QLabel("<b>Serial Status:</b> <font color='red'>Disconnected</font>")
        self.layout.addWidget(self.label,0,0)
        
        #Plot Status Label
        self.plotstatus = QtGui.QLabel("<b>Plotting Status:</b> <font color='red'>Not Plotting</font>")
        self.layout.addWidget(self.plotstatus,0,3)
        
        #variables
        self.buffer=[]
        self.i=0
        self.buffer1=[1,3,5]
        self.count = 0
        self.count2 = 0
        #self.status = "Status: Disconnected"
        #added for XBEE data parsing and storage
        self.receive_buffer = []
        self.node1 = []
        self.node2 = []
        
        # define grid
        self.plt_grid = pg.GridItem()

    def Fconnect(self):
        """Serial Port Open Connection"""
        try:
            self.port = serial.Serial('COM4', 9600) 
            print ("serial port opened") #print on the terminal
        except serial.SerialException as error:
            print ("could not open serial port '{}': {}".format('COM4', error))
        self.label.setText("<b>Serial Status:</b> <font color='green'>Connected</font>")
        self.Bconnect.setDisabled(True)
        self.Bdisconnect.setDisabled(False)
        self.Bstart.setDisabled(False)
        self.Bstop.setDisabled(False)
        
    def Fdisconnect(self):
        self.port.close()
        self.label.setText("<b>Serial Status:</b> <font color='red'>Disconnected</font>")
        self.plotstatus.setText("<b>Plotting Status:</b> <font color='red'>Not Plotting</font>")
        self.Bconnect.setDisabled(False)
        self.Bdisconnect.setDisabled(True)
        self.Bstart.setDisabled(True)
        self.Bstop.setDisabled(True)
        
    def Fstart(self):
        self.plotstatus.setText("<b>Plotting Status:</b> <font color='green'>Plotting</font>")
        """one second timer"""
        self.timer=QtCore.QTimer()
        self.Bdisconnect.setDisabled(True)
        self.Bstart.setDisabled(True)
        QtCore.QObject.connect(self.timer, QtCore.SIGNAL('timeout()'), self.readData)
        self.timer.start(1000)

    def Fstop(self):
        self.plotstatus.setText("<b>Plotting Status:</b> <font color='red'>Not Plotting</font>")
        self.Bstart.setDisabled(False)
        self.Bdisconnect.setDisabled(False)
        self.timer.stop()

    
    """****************************Read Data Frame(self)***********************************"""
    def readData(self):
        """Read Serial Data"""
        #to read a complete line ended by '\n'
        print("Waiting")
        self.ValidRxFrames = []
        
        a = self.port.inWaiting()
        while a:
            print(a)
            data = self.port.read(a)
            a-=len(data)
            self.receive_buffer.extend(data)
        print ('\t'.join([hex(i) for i in self.receive_buffer]))
 
        self.receive_buffer = bytearray(self.receive_buffer)    
        dataframes = self.receive_buffer.split(bytes(b'\x7E'))
        print ("dataframes")
        print(dataframes)
        
        #####VALIDATION####################
        for dataframe in dataframes[1:]: 
            valid = self.Validate_frame(dataframe)
            #print ("Validity")
            #print (valid)
                

        #for valid_dataframe  in self.ValidRxFrames :
        self.ExtractMessage(self.ValidRxFrames[len(self.ValidRxFrames) - 1])
            
        print("node 1")
        print (self.node1)
        print("node 2")
        print (self.node2)
        self.update()
        print("Plotted")

    """************************\Validate Data Frame(Called from ReadData(self))***********************"""        
    def Validate_frame(self,data_frame):
        
        # 9 bytes is Minimum length to be a valid Rx frame
        #  LSB, MSB, Type, Source Address(2), RSSI,
        #  Options, 1 byte data, checksum
      
        # use the escape characters to count the number of bytes in the data frame
        if len(data_frame) < 9:
            return False
        
        #print("length")
        #print(len(data_frame))

        LSB = data_frame[1]

        # Check to make sure that the data frame is not smaller than the LSB specified. 
        if LSB > (len(data_frame[2:]) - 1):
            return False
        
        # Validate checksum
        if (sum(data_frame[2:3+LSB]) & 0xFF) != 0xFF:
            return False
        
        #append all correct data frames to the RxMessages buffer 
        self.ValidRxFrames.append(data_frame)
        return True

    """**********************Extract Message(Called from readDate())******************************"""    
    def ExtractMessage(self, data_frame):
        #self.node1 = []                                 #reinitialize on every call to ExtractMessage()
        #self.node2 = []
        #<MSB_length><LSB_length><API><MSB_addr><LSB_addr><RSSI><Options><Cmd_ID><DATA_height><DATA_per><DATA_dist><chcksum>
        #<0000000000><1111111111><222><33333333><44444444><5555><6666666><777777><8.9.10...11><12131415><161718.19><.....20>        
        
        if data_frame[7] == 3:                          #command ID for base station, should be 0x03
            length = data_frame[0]*255 + data_frame[1]
            address = data_frame[3]*255 + data_frame[4]
            rssi = data_frame[5]
            height = self.bytearray_to_int(data_frame[8:12])
            perpend =  self.bytearray_to_int(data_frame[12:16])
            dist = self.bytearray_to_int(data_frame[16:20])
        
        if len(self.node1) == 0:
            self.node1+= [address] + [rssi] + [height] + [perpend] + [dist]
        elif len(self.node1) != 0:
            if address == self.node1[0]:
                print("node 1 is here")
                self.node1 = []
                self.node1+= [address] + [rssi] + [height] + [perpend] + [dist]
            else:
                self.node2 = []
                print("node 2 is here")
                self.node2+= [address] + [rssi] + [height] + [perpend] + [dist]
        
        
    """****************Convert ByteArray to Integer (Called by ExtractMessage())******************"""
    def bytearray_to_int(self, input_bytearray):
        print(input_bytearray)
        offset = len(input_bytearray) * 8 
        output_integer = 0 
        for i in range(0,len(input_bytearray)):
            offset -= 8
            output_integer += input_bytearray[i] << offset   
        return output_integer
        """*******************************************************************************************"""
    """*********************************Timer ISR*************************************************"""
    def update(self):
        #global curve, data
        print ("buffer", self.buffer)
        #Pens and brushes
        self.pen0 = pg.mkPen('w')
        self.pen_blue = pg.mkPen('b', width=3)
        self.pen_blue_t = pg.mkPen('b', width=2)
        self.pen_red = pg.mkPen('r', width=3)
        self.pen_green = pg.mkPen('g', width=3)
        self.pen_green_t = pg.mkPen('g', width=2)
        self.pen_yellow = pg.mkPen('y', width=3)
        self.pen_o = pg.mkPen('g',width=0.3)
        self.brush1 = pg.mkBrush('y')
        
        
        gridPen = pg.mkPen(color=(200, 200, 255), style=QtCore.Qt.DotLine)
        
        
        #self.plot.addItem(self.plt_grid)
        
        
		
        """Plot 1"""
        self.plot1.clear()
        self.plot1.addItem(self.plt_grid)
        #self.plot1.plot(self.buffer1, self.buffer, pen=None, symbol='x')
        self.plot1.addLine(x=0, pen=0.5)
        self.plot1.addLine(y=0, pen=0.5)
        
        if len(self.node1)>3:
            plot1_node1_y = [0,self.node1[3]]
            plot1_node1_x = [0,self.node1[4]]
        if len(self.node2) >3:
            plot1_node2_y = [0,-self.node2[3]]
            plot1_node2_x = [0,-self.node2[4]]
        
        self.plot1.addLegend()
        
        if len(self.node1)>3:
            if self.node1[4] < 15:      #if quad is too close, plot in red.. WARNING!!
                self.plot1.plotItem.plot(plot1_node1_x, plot1_node1_y, pen = None, symbol = 'x', symbolPen = self.pen_red,symbolSize=25,name = '......NODE1')
            else:
                self.plot1.plotItem.plot(plot1_node1_x, plot1_node1_y, pen = None, symbol = 'x', symbolPen = self.pen_green,symbolSize=25,name = '......NODE1')

        if len(self.node2)>3:
            if self.node2[4] < 15:      #if quad is too close, plot in red.. WARNING!!
                self.plot1.plotItem.plot(plot1_node2_x, plot1_node2_y, pen = None, symbol = 'x', symbolPen = self.pen_red,symbolSize=25,name = '......NODE2')
            else:
                self.plot1.plotItem.plot(plot1_node2_x, plot1_node2_y, pen = None, symbol = 'x', symbolPen = self.pen_blue,symbolSize=25,name = '......NODE2')                
                
        #self.plot.plotItem.plot(self.data1, pen= None, symbol = 'x', symbolPen = self.pen1, symbolSize = 30, name = '.......NODE2')
        
        """Plot 2 - RSSI"""
        self.plot2.addLegend()
        if len(self.node1)>3:
            self.plot2_node1.append(self.node1[1])
            self.plot2.plotItem.plot(self.plot2_node1,pen = self.pen_green_t, symbol = 'o',symbolSize=5,name = '......NODE1')

        if len(self.node2)>3:
            self.plot2_node2.append(self.node2[1])
            self.plot2.plotItem.plot(self.plot2_node2, pen = self.pen_blue_t, symbol = 'o',symbolSize=5,name = '......NODE2')
            
        """Plot 3 - Height"""
        self.plot3.addLegend()
        if len(self.node1)>3:
            self.plot3_node1.append(self.node1[2])
            self.plot3.plotItem.plot(self.plot3_node1, pen = self.pen_green_t, symbol = 'o',symbolSize=5,name = '......NODE1')

        if len(self.node2)>3:
            self.plot3_node2.append(self.node2[2])
            self.plot3.plotItem.plot(self.plot3_node2, pen = self.pen_blue_t, symbol = 'o',symbolSize=5,name = '......NODE2')    
            
        
        """Plot 4 - Variance"""
        self.plot4.addLegend()
        self.plot4.addItem(self.plt_grid)
        #lineplot = np.sqrt(100**2 + 20**2)
        self.plot4.addLine(y=0, pen=0.5)
        self.count+=1
        if len(self.node1)>3:
            val1 = np.sqrt((self.node1[2]-100)**2 + self.node1[3]**2 + (self.node1[4] - 20)**2) 
            self.plot4_node1.append(val1)
            #self.plot4_node1_y.append(self.node1[3] +1+20)
            self.plot4.plotItem.plot(self.plot4_node1, pen = self.pen_green_t, symbol = 'o',symbolSize=5,name = '......NODE1')

        if len(self.node2)>3:
            #val2 = self.node2[4] - 20 + self.count
            val2 = np.sqrt((self.node2[2]-100)**2 + self.node2[3]**2 + (self.node2[4] - 20)**2)
            self.plot4_node2.append(val2)
            #self.plot4_node2_y.append(self.node2[3] +1+20)
            self.plot4.plotItem.plot(self.plot4_node2, pen = self.pen_blue_t, symbol = 'o',symbolSize=5,name='......NODE2')
        



        
def main():
    app = QtGui.QApplication(sys.argv)
    On = Oneplot()
    On.show()
    sys.exit(app.exec_())
             
if __name__ == '__main__':
    main()