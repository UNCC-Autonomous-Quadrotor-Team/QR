boolean IsLogical(){
  
  int tolerance = 15;
  
  //((Blob1Avg.Y != 0) && (Blob2Avg.Y !=0)) || ((Blob1Avg.Y != 0) && (Blob3Avg.Y != 0)) || ((Blob2Avg.Y != 0) && (Blob3Avg.Y != 0))
  
  
  //Compute variables to be used in algorithm
  if(true)
  {
    boolean comp12 = compareY(Blob1Avg.Y,Blob2Avg.Y,tolerance);
    boolean comp13 = compareY(Blob1Avg.Y,Blob3Avg.Y,tolerance);
    boolean comp14 = compareY(Blob1Avg.Y,Blob4Avg.Y,tolerance);
    boolean comp23 = compareY(Blob2Avg.Y,Blob3Avg.Y,tolerance);
    boolean comp24 = compareY(Blob2Avg.Y,Blob4Avg.Y,tolerance);

    //Compare the 5 relevant pairs of points to determine if an axis exists
    if(comp12 && comp13){
      compareX(Blob1Avg,Blob2Avg,Blob3Avg);
      //    Serial.println("Blobs 1,2, and 3 are on an axis");
      return true;
    }
    else if(comp12 && comp14){
      compareX(Blob1Avg,Blob2Avg,Blob4Avg);
      //    Serial.println("Blobs 1,2, and 4 are on an axis");
      return true;
    }
    else if(comp13 && comp14){
      compareX(Blob1Avg,Blob3Avg,Blob4Avg);
      //    Serial.println("Blobs 1,3, and 4 are on an axis");
      return true;
    }
    else if(comp23 && comp24){
      compareX(Blob2Avg,Blob3Avg,Blob4Avg);
      //    Serial.println("Blobs 2,3, and 4 are on an axis");
      return true;
    }
    else
      return false;
  }
}


//Compare function to determine if two points are on the same X axis
boolean compareY(int one,int two,int tolerance){
  if(abs(one - two) <= tolerance)
    return true;
  else
    return false;
}

boolean compareX(Average one,Average two, Average three){
  int array_x[3] = {
    one.X,two.X,three.X  };
  int array_y[3] = {
    one.Y,two.Y,three.Y  };
  int t,n,i,s;

  for(n = 0; n < 3; n++){
    for(i = 0; i < 2; i++){
      if(array_x[i] > array_x[i+1]){
        t = array_x[i];
        array_x[i] = array_x[i+1];
        array_x[i+1] = t;
        s = array_y[i];
        array_y[i] = array_y[i+1];
        array_y[i+1] = s;
      }
    }
  }
  //  Serial.println("The ordered points are:");
  //  for(i = 0; i < 3; i++){
  //    Serial.print(array_x[i]);
  //    Serial.print(", ");
  //    Serial.println(array_y[i]);
  //  }
  TBlob1.X = array_x[0];
  TBlob2.X = array_x[1];
  TBlob3.X = array_x[2];
  TBlob1.Y = array_y[0];
  TBlob2.Y = array_y[1];
  TBlob3.Y = array_y[2];
}
