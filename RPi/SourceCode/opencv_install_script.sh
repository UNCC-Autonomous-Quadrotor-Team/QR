
# Created by : Terrill Massey 

clear; 

DESCRIPTION="This is a full environment setup for opencv. For questions, contact Terrill Massey @trrllmassey@gmail.com"

echo $DESCRIPTION
echo "-----------------------------------------------------" 

echo "Before Begining you will need sudo privileges. Do you have these privilleges? "
 
select choice in "Yes" "No" ;do 

case $choice in 
    Yes ) 
	echo " Initalizing.." 
	echo "Installing Dependicies..." 
	(sudo apt-get install build-essential) ||( break) ;

	(sudo apt-get install cmake git libgtk2.0-dev pkg-config libavcodec-dev libavformat-dev libswscale-dev)  ||(break) 

	(sudo apt-get install python-dev python-numpy libtbb2 libtbb-dev libjpeg-dev libpng-dev libtiff-dev libjasper-dev libdc1394-22-dev) ||(break) 


########################################################################
# START SOURCE CODE PULL 
########################################################################
#check to see if a previous installation exists 
	DIR=opencv 
	if [ -d "$DIR" ]; 
	then 
	    echo "A previous installation of the opencv source code is detected."
	    echo "This directory will be cleaned for a fresh install." 
	    # clean the directory 
	    sudo rm -r $DIR
	    mkdir $DIR opencv #make a new directory 
	else 
	    mkdir $DIR  opencv # make a new directory
	fi 


	echo "Getting OpenCV source code from OPENCV repository.." 

	cd opencv 
       (wget -O OpenCV-2.4.9.zip http://sourceforge.net/projects/opencvlibrary/files/opencv-unix/2.4.9/opencv-2.4.9.zip/download ||(echo "Filepath to repositiory is broken.Please update this link or check your connection... " &&  break) 
       #unzip package 
       unzip OpenCV-*
        
     

	echo "Starting Build"
	echo "------------------------------->" 
	cd OpenCV-*
	mkdir build 
	cd build 
#	(cmake -D BUILD_NEW_PYTHON_SUPPORT=ON) || (echo "Build Failed" && break)
	(make -j5)   || (echo "Build Failed!" && break)
       
	(cmake ../)  || (echo "Build Failed!" && break)
	sudo make install  
	

	echo "OpenCV installed correctly "
	
	break;;

    No )  
	echo "You will need sudo privilliges to peform the environment build"
	break;; 

esac 

done 



