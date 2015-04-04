'''
@author: Jeeten
'''
"""*****************************************************************************************************************
1. Push buttons to enable and disable serial connection.
2. Read a byte of serial data every second.
3. Buffer 3 values (bytes) and update plot.
4. Push buttons to start and stop plotting
5. Use a timer interrupt to read data every second.
(If while loop is used to read data, the serial port does not free up memory and no other tasks can be performed)
******************************************************************************************************************"""


import sys
import numpy as np
import pyqtgraph as pg
import serial
from PyQt4 import QtGui, QtCore
from numpy import dtype
#from PyQt4Qt import QBrush, QColor

#app = QtGui.QApplication([])

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
        
        #Plot1
        self.plot1 = pg.PlotWidget(title="<font color='blue'><b>Position</b></font>")
        self.layout.addWidget(self.plot1,1,0,1,4)
        self.plot1.resize(440,100)
        
        #Connect Button
        self.Bconnect = QtGui.QPushButton("Connect",self)
        self.Bconnect.clicked.connect(self.Fconnect)        
        self.layout.addWidget(self.Bconnect,2,0)

        #Disconnect Button
        self.Bdisconnect = QtGui.QPushButton("Disconnect",self)
        self.Bdisconnect.clicked.connect(self.Fdisconnect)        
        self.layout.addWidget(self.Bdisconnect,2,1)
        
        #Start Button
        self.Bstart = QtGui.QPushButton("Start",self)
        self.Bstart.clicked.connect(self.Fstart)
        self.layout.addWidget(self.Bstart,2,2)
        
        #Stop Button
        self.Bstop = QtGui.QPushButton("Stop",self)
        self.Bstop.clicked.connect(self.Fstop)        
        self.layout.addWidget(self.Bstop,2,3)
        
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
        #self.status = "Status: Disconnected"

    def Fconnect(self):
        """Serial Port Open Connection"""
        try:
            self.port = serial.Serial('COM3', 9600) 
            print ("serial port opened") #print on the terminal
        except serial.SerialException as error:
            print ("could not open serial port '{}': {}".format('COM3', error))
        self.label.setText("<b>Serial Status:</b> <font color='green'>Connected</font>")
        self.Bconnect.setDisabled(True)
        self.Bdisconnect.setDisabled(False)
        self.Bstart.setDisabled(False)
        self.Bstop.setDisabled(False)
        
    def Fdisconnect(self):
        self.port.close()
        self.label.setText("<b>Serial Status:</b> <font color='red'>Disconnected</font>")
        self.Bconnect.setDisabled(False)
        self.Bdisconnect.setDisabled(True)
        
    def Fstart(self):
        self.plotstatus.setText("<b>Plotting Status:</b> <font color='green'>Plotting</font>")
        """one second timer"""
        self.timer=QtCore.QTimer()
        QtCore.QObject.connect(self.timer, QtCore.SIGNAL('timeout()'), self.readData)
        self.timer.start(1000)
        
    def Fstop(self):
        self.plotstatus.setText("<b>Plotting Status:</b> <font color='red'>Not Plotting</font>")
        self.timer.stop()
        
    def readData(self):
        """Read Serial Data"""
        #to read a complete line ended by '\n'
        self.line = self.port.readline()
        

        if self.i<3:
            self.buffer.append(int(self.line))
            self.i+=1
        
        elif self.i==3:
            self.update()
            self.i=0
            self.yvalue=[]
            self.buffer=[]
            
        print (self.i)

    
    def update(self):
        #global curve, data
      
        print ("buffer", self.buffer)
        
        self.plot1.clear()
        self.plot1.plot(self.buffer, pen=None, symbol='x')

        
def main():
    app = QtGui.QApplication(sys.argv)
    On = Oneplot()
    On.show()
    sys.exit(app.exec_())
             
if __name__ == '__main__':
    main()