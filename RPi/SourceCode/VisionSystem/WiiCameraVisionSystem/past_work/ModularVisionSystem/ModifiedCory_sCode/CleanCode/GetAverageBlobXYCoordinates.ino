//*************************************************************************************

void GetAverageBlobXYCoordinates(int samples)
{
  SampleAndSumXYCoordinates(samples);
  
  AverageXYSumIfBlobAlwaysPresent(samples);
  
  delay(50);
}

//*************************************************************************************
//*************************************************************************************

void SampleAndSumXYCoordinates(int samples)
{
  for (int i = 0; i < samples; i++)
  {
    ClearPreviousAndSampleWiiCamera(); 
    
    SumBlobCoordinates();
  }
}

//*************************************************************************************
//*************************************************************************************

void AverageXYSumIfBlobAlwaysPresent(int samples)
{
  if ((X1 != 0) && (Y1 != 0)) { Blob1Avg.X = X1 / samples; Blob1Avg.Y = Y1 / samples; }
  if ((X2 != 0) && (Y2 != 0)) { Blob2Avg.X = X2 / samples; Blob2Avg.Y = Y2 / samples; }
  if ((X3 != 0) && (Y3 != 0)) { Blob3Avg.X = X3 / samples; Blob3Avg.Y = Y3 / samples; }
  if ((X4 != 0) && (Y4 != 0)) { Blob4Avg.X = X4 / samples; Blob4Avg.Y = Y4 / samples; } 
}

//*************************************************************************************
//*************************************************************************************

void ClearPreviousAndSampleWiiCamera()
{
  Blob1Avg.X = 0,Blob1Avg.Y = 0,Blob2Avg.X = 0,Blob2Avg.Y = 0, Blob3Avg.X = 0,Blob3Avg.Y = 0,Blob4Avg.X = 0,Blob4Avg.Y = 0; 
  result = ircam.read();  
}

//*************************************************************************************
//*************************************************************************************

void SumBlobCoordinates()
{
  if (result & BLOB1)
  {
    X1 += ircam.Blob1.X;
    Y1 += ircam.Blob1.Y;
    n1++;
  }
  if (result & BLOB2)
  {
    X2 += ircam.Blob2.X;
    Y2 += ircam.Blob2.Y;
    n2++;
  }
  if (result & BLOB3)
  {
    X3 += ircam.Blob3.X;
    Y3 += ircam.Blob3.Y;
    n3++;
  }
  if (result & BLOB4)
  {
    X4 += ircam.Blob4.X;
    Y4 += ircam.Blob4.Y;
    n4++;
  }
}

//*************************************************************************************
