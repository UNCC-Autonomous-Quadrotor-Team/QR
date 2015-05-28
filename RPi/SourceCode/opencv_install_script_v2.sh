
# Created by : Terrill Massey 

clear; 

DESCRIPTION="This is a full environment setup for opencv. For questions, contact Terrill Massey @trrllmassey@gmail.com. "
 
SUDOWARNING=" This operation will requrie sudo priviliges. Can you obtain sudo priviliges?"
echo $DESCRIPTION
LINEBREAK="-----------------------------------------------------" 
echo $LINEBREAK
echo $SUDOWARNING
 
select choice in "Yes" "No" ;

do 

case $choice in 
    Yes )
	#Elevate privlliges to root. 
        sudo su 


	#Clean the system from any old debian packages for opencv. 
	echo "Cleaning the system from any old opencv packages..."
        apt-get autoremove libopencv-dev python-opencv
	echo " INITIALIZING SETUP..."
        apt-get update
        apt-get upgrade --assume-yes
        apt-get dist-upgrade
        apt-get autoremove
	#install developer tools and packages just in case it is not on the platform.
        apt-get install build-essential cmake pkg-config
	
	echo "INSTALLING DEPENDICIES..." 
	echo $LINEBREAK
	echo "Installing image I/O packages.."
        apt-get install libjpeg8-dev libtiff4-dev libjasper-dev libpng12-dev --assume-yes
	echo "Installing GTK GUI development library.."
	apt-get install libgtk2.0-dev --assume-yes
	echo " Installing video I/O packages.."
        apt-get install libavcodec-dev libavformat-dev libswscale-dev libv4l-dev --assume-yes
	echo "Installing optimization libraries.."
        apt-get install libatlas-base-dev gfortran --assume-yes
	echo " Installing pip.."
        wget https://bootstrap.pypa.io/get-pip.py
	python get-pip.py
	echo "Installing python 2.7-dev.."
        apt-get install python2.7-dev
	echo "Installing numpy.."
	pip install numpy
	
#(sudo apt-get install build-essential) ||( break) ;

	#(sudo apt-get install cmake git libgtk2.0-dev pkg-config libavcodec-dev libavformat-dev libswscale-dev)  ||(break) 

	#(sudo apt-get install python-dev python-numpy libtbb2 libtbb-dev libjpeg-dev libpng-dev libtiff-dev libjasper-dev libdc1394-22-dev) ||(break) 


########################################################################
# START SOURCE CODE PULL 
########################################################################
#check to see if a previous installation exists 
	echo "INSTALLING OPENCV.."
	echo $LINEBREAK
	DIR=opencv 
	if [ -d "$DIR" ]; 
	then 
	    echo "A previous installation of the opencv source code is detected."
	    echo "This directory will be cleaned for a fresh install." 
	    # clean the directory 
	    rm -r $DIR
	    mkdir $DIR  #make a new directory  
       else 

	    mkdir $DIR #make a new directory
	fi 


	echo "Getting OpenCV source code from OPENCV repository.." 

	cd opencv 
       (wget -O opencv-2.4.10.zip http://sourceforge.net/projects/opencvlibrary/files/opencv-unix/2.4.10/opencv-2.4.10.zip/download) ||(echo "Filepath to repositiory is broken.Please update this link or check your connection... " &&  break) 
       #unzip package 
       echo "Unzipping Package.."
       (unzip opencv-*) || (echo "failed to unzip package.." && break)
       #clean directory.
       (rm *.zip) || (echo "failed to remove zip file.." && break) 
       # enter opencv directory
       cd opencv* 
             
                    
     
        
	echo "Starting Build"
	echo $LINEBREAK
	mkdir build
        cd build
	echo "Current Directory"
	pwd
       ( cmake -D CMAKE_BUILD_TYPE=RELEASE -D  BUILD_NEW_PYTHON_SUPPORT=ON -D INSTALL_C_EXAMPLES=ON -D INSTALL_PYTHON_EXAMPLES=ON  -D BUILD_EXAMPLES=ON ..) ||(echo "cmake Failed.. Check cmake command" && break)
         make 
	 make install
	 ldconfig
	echo "OpenCV installed correctly "

	#Cleanup the working environment.
	echo "Cleaning environment.."
	cd ..; cd ..; 
	(rm -r $DIR) || (echo "Failed to cleanup.. Check the directory that this script resides in and manually remove the opencv directory.")
	echo "Done"
	break
	;;

    No)  
	echo "You will need sudo privilliges to peform the environment build"
	break
	;; 

esac 

done 


