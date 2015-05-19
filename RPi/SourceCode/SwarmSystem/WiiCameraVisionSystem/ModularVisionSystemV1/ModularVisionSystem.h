struct Average{
  int X;
  int Y;
} Blob1Avg, Blob2Avg, Blob3Avg, Blob4Avg;

struct Trig{
  int X;
  int Y;
} TBlob1, TBlob2, TBlob3;

PVision ircam;
byte result;

// Used in MainCode
  int numberOfSamples = 1;


// Used in GetAverageBlobXYCoordinates
  int X1 = 0,Y1 = 0,X2 = 0,Y2 = 0,X3 = 0,Y3 = 0,X4 = 0,Y4 = 0;
  int n1 = 0,n2 = 0,n3 = 0,n4 = 0;

// Used in SendMovementCommand

  // User preference
  int idealDistanceFromBeacon    = 48; // inches
  
  // Varied by execution
  boolean hasNotMoved;
  int pixelsFromB1ToB2;
  int pixelsFromB1ToB3;
  int pixelsFromB2ToB3;
  int yawBeaconDifference;
  int distanceFromBeacon;
  int middleBeaconX;
  int middleBeaconY;
  
  // Fact
  int idealMiddleOfXAxis = 512;
  
  // Calibrated through testing
  int yawTolerance = 5; // pixels
  int lateralMoveTolerance = 30; // pixels
  int distanceFromBeaconTolerance = 1; // inch


