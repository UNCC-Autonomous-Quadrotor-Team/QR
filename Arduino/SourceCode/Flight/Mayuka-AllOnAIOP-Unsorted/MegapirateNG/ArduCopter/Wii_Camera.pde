#include <Wire.h>
#include <math.h>
#include <APM_WiiCamera.h>

#define WII_CAMERA_PORT 0x21              //i2C address of Wii camera ( = 0xB0 >> 1 to convert twi to i2c?)
#define WII_CAMERA_TARGET_MAX_WIDTH 155   //width of IR blobs on IR target in mm
#define WII_CAMERA_TARGET_MIN_WIDTH 50    //width of IR blobs on IR target in mm
#define WII_CAMERA_YAW 0	          //rotation of the camera sensor from aircraft body in degrees

// Real world parameters
#define DEG2RAD  PI/180.0

// Camera parameters
#define WII_CAMERA_ADDRESS 0x21					// i2C address of Wii camera (0xB0 >> 1 to convert twi to i2c?)
#define X_PIX 1024								// number of pixels in X dimension of camera
#define Y_PIX 768								// number of pixels in Y dimension of camera
#define X_CENTRE X_PIX/2						// X coordinate of Centre Pixel in Camera
#define Y_CENTRE Y_PIX/2						// Y coordinate of Centre Pixel in Camera  
#define X_FOV 42.6								// Field of view of Camera across X dimension -> from http://forum.wiibrew.org/read.php?7,11114
#define Y_FOV (X_FOV * Y_PIX / X_PIX) 			// Field of view of Camera across Y dimension Calculated from X FOV 
#define PIX2DEG (X_FOV / X_PIX)					// Number of pixels per degree of view


WiiCamera               ircam;
int                     WiiRange=0;
int                     WiiRotation=0;
int                     WiiDisplacementX=0;
int                     WiiDisplacementY=0;

void setup()
{
  Serial.begin(115200);
  ircam.init();
}

void loop()
{
  ircam.read();
  if (ircam.blobcount>0)
  {
    Wii_calcPosn(WII_CAMERA_YAW,WII_CAMERA_TARGET_MAX_WIDTH,WII_CAMERA_TARGET_MIN_WIDTH);

    Serial.print(" RANGE:");
    Serial.print(WiiRange);
    Serial.print(" YAW:");
    Serial.println(WiiRotation);
    Serial.print("BLOBS:");
    Serial.print(ircam.blobcount);

  }
  delay(100);
}




void Wii_calcPosn(int _cameraYaw, int _targetMaxWidth, int targetMinWidth)
{

  // Calculated dimensions
  float theta = 0.0;
  float distance = 0.0;
  float HeightAGL = 0.0;
  float YawAngle = 0.0;
  float MidPointX = 0.0;
  float MidPointY = 0.0;
  float Pix2mm =0;

  if (ircam.blobcount==2) 
  {

    // CALCULATIONS    
    distance = sqrt(square(ircam.Blob[0].X-ircam.Blob[1].X) + square(ircam.Blob[0].Y-ircam.Blob[1].Y));    // Calculate the number of pixels between the two IR targets                            
    Pix2mm = distance/_targetMaxWidth;                                 // the number of pixels per cm at the target distance - will change with distance changes
    theta = distance * PIX2DEG / 2;                                // half the angle between the two outside targets in degrees
    HeightAGL = _targetMaxWidth * 0.5 / tan(theta*DEG2RAD);            // Calculate the height of camera above targets                    
    YawAngle = atan2(ircam.Blob[0].Y-ircam.Blob[1].Y, ircam.Blob[0].X-ircam.Blob[1].X) * 180.0 / PI;       // Angle of rotation of the two targets in the image
    YawAngle += _cameraYaw;                                        // add any sensor alignment angle to yaw angle  

    MidPointX = (ircam.Blob[0].X+ircam.Blob[1].X)/2.0;                                 // find the mid point of the line in pixels
    MidPointY = (ircam.Blob[0].Y+ircam.Blob[1].Y)/2.0;
    WiiDisplacementX = int(X_CENTRE-MidPointX);                       // calculate the displacement of the midpoint from the centre of image in pixels
    WiiDisplacementY = int(Y_CENTRE-MidPointY);
    WiiDisplacementX /= Pix2mm;									   // Convert the displacement of the midpoint from the centre of image to mm
    WiiDisplacementY /= Pix2mm;

    WiiRange = HeightAGL/10.0;
    WiiRotation = YawAngle;
  }
  else
  {
    WiiRange = 0;
    WiiRotation = 0;
  }
}



void showBlobs()
{
  for(int i=0; i<ircam.blobcount; i++)
  {
    Serial.print( int(ircam.Blob[i].X) );
    Serial.print(",");
    Serial.print( int(ircam.Blob[i].Y) );
    Serial.print(",");
    Serial.print( int(ircam.Blob[i].Size) );
    if (i<ircam.blobcount-1)
      Serial.print(",");
  }
}



void showall()
{
  Serial.print("Blobs:");
  Serial.print(ircam.blobcount);
  Serial.print(" Raw:");

  for(int i=0; i<4; i++)
  {
    Serial.print( int(ircam.Blob[i].X) );
    Serial.print(",");
    Serial.print( int(ircam.Blob[i].Y) );
    Serial.print(",");
    Serial.print( int(ircam.Blob[i].Size) );
    if (i<4-1)
      Serial.print(",");
  }

  Serial.print(" Range:");
  Serial.print(WiiRange);

  Serial.print(" Rotation:");
  Serial.print(WiiRotation);

  Serial.print(" Displacement X:");
  Serial.print(WiiDisplacementX);
  Serial.print(" Y:");
  Serial.print(WiiDisplacementY);
  Serial.print(" ");
}

