//*************************************************************************************

void GetAverageBlobXYCoordinates()
{
  ClearPrevious();

  SumBlobCoordinates();

  AverageXYSumIfBlobAlwaysPresent();

  delay(50);
}

//*************************************************************************************
//*************************************************************************************

void ClearPrevious()
{
  X1 = 0,Y1 = 0,X2 = 0,Y2 = 0,X3 = 0,Y3 = 0,X4 = 0,Y4 = 0;
  n1 = 0,n2 = 0,n3 = 0,n4 = 0;
  Blob1Avg.X = 0,Blob1Avg.Y = 0,Blob2Avg.X = 0,Blob2Avg.Y = 0,Blob3Avg.X = 0,Blob3Avg.Y = 0,Blob4Avg.X = 0,Blob4Avg.Y = 0;
}

//*************************************************************************************

void SumBlobCoordinates()
{
  for (int takeSample = 0; takeSample < numberOfSamples; takeSample++)
  {
    result = ircam.read();  
    
    if (result & BLOB1){ X1 += ircam.Blob1.X; Y1 += ircam.Blob1.Y; n1++; }
    if (result & BLOB2){ X2 += ircam.Blob2.X; Y2 += ircam.Blob2.Y; n2++; }
    if (result & BLOB3){ X3 += ircam.Blob3.X; Y3 += ircam.Blob3.Y; n3++; }
    if (result & BLOB4){ X4 += ircam.Blob4.X; Y4 += ircam.Blob4.Y; n4++; }
  } 
}

//*************************************************************************************
//*************************************************************************************

void AverageXYSumIfBlobAlwaysPresent()
{
  if ((X1 != 0) && (Y1 != 0)){ Blob1Avg.X = X1 / numberOfSamples; Blob1Avg.Y = Y1 / numberOfSamples; }
  if ((X2 != 0) && (Y2 != 0)){ Blob2Avg.X = X2 / numberOfSamples; Blob2Avg.Y = Y2 / numberOfSamples; }
  if ((X3 != 0) && (Y3 != 0)){ Blob3Avg.X = X3 / numberOfSamples; Blob3Avg.Y = Y3 / numberOfSamples; }
  if ((X4 != 0) && (Y4 != 0)){ Blob4Avg.X = X4 / numberOfSamples; Blob4Avg.Y = Y4 / numberOfSamples; }
}

//*************************************************************************************
