static void Divider()
{
  Serial.println("");  
  Serial.println("*******************************************");
}

static void WiiCameraInitialize()
{
  ircam.init();
  Serial.println("1");
  Serial.println("3");
}

static void ServoSetup()
{
  myservo2.attach(9);  // attaches the servo on pin 9 to the servo object 
  Serial.begin(9600);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
}
