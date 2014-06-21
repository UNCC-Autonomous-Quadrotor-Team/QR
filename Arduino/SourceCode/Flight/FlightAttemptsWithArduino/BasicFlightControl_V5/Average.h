//struct Average{
//  int X;
//  int Y;
//} Blob1Avg, Blob2Avg, Blob3Avg, Blob4Avg;
//
//struct Trig{
//  int X;
//  int Y;
//} TBlob1, TBlob2, TBlob3;

int throttleValue;

Servo Throttle, Pitch, Roll, Yaw, Aux1; 

int throttleInitialValue = 1000;
int pitchInitialValue = 1500, rollInitialValue = 1500, yawInitialValue = 1500;

int aux1StableFlight = 1200;
int aux1AltitudeHold = 1400;

char xbeeMessage = ' ';
