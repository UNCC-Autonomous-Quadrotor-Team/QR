#!/bin/bash
# Created by  : Terrill Massey 
# Description : Install script for UNCC QR Environment.

#REV:

# 3/4/2017 - Modified Header and added verbose comments. 


###################################################
#FUNCTION DEFINITIONS.
###################################################
function install_opencv () {

    # Created by  : Terrill Massey 
    # Description : Install script for opencv and its dependences. The script will remove source code 
    # once installation is complete. 
    #REV:
    # 2015 - Original
    # 3/3/2017 - Modified Header and added verbose comments. 
    # 3/4/2017 - Updated dependences for video I/O package pull, and added python 3.0 install. Updated opencv build to  3.2.0
    # 3/5/2017 - Removed clear command

    DESCRIPTION="This is a full environment setup for opencv. For questions, contact Terrill Massey at trrllmassey@gmail.com. "
    SUDOWARNING="You will need root priviliges to run this script. Please run this script with 'sudo' infront of the command. ex: sudo ./script.sh" 
    LINEBREAK="-----------------------------------------------------" 
    echo $DESCRIPTION
    echo $LINEBREAK


    #Determine if there is acess to root priviliges. 
    ROOTSTATUS=$(whoami)
    
    if [ "$ROOTSTATUS" != "root" ]; then
	echo "Please use sudo when running this script."
	exit

    fi
    
    case $ROOTSTATUS in
	"root") #We have access to sudo privilliges
	    
	    #check to see if a previous installation exists 
	    
	    src_dir=/etc/
	    cd $src_dir
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
	    
	    cd $DIR 

	    #Clean the system from any old debian packages for opencv. 
	    echo "Cleaning the system from any old opencv packages..."
            apt-get autoremove libopencv-dev python-opencv  --assume-yes
	    echo " INITIALIZING SETUP..."
            apt-get update --assume-yes
            apt-get upgrade --assume-yes
            apt-get dist-upgrade --assume-yes
            apt-get autoremove   --assume-yes
	    #install developer tools and packages just in case it is not on the platform.
            apt-get install build-essential cmake pkg-config  --assume-yes
	    
	    echo "INSTALLING DEPENDICIES..." 
	    echo $LINEBREAK
	    echo "Installing image I/O packages.."
            apt-get install libjpeg62-turbo-dev libtiff5-dev libjasper-dev libpng12-dev --assume-yes
	    echo "Installing GTK GUI development library.."
	    apt-get install libgtk2.0-dev --assume-yes
	    echo " Installing video I/O packages.."
            apt-get install libavcodec-dev libavformat-dev libswscale-dev libv4l-dev --assume-yes
	    apt-get install libxvidcore-dev libx264-dev
	    echo "Installing optimization libraries.."
            apt-get install libatlas-base-dev gfortran --assume-yes
	    echo " Installing pip.."
            wget https://bootstrap.pypa.io/get-pip.py
	    python get-pip.py
	    echo "Installing python 2.7-dev.."
            apt-get install python2.7-dev --assume-yes
	    echo "Installing python 3.0-dev.."
	    apt-get install python3.0-dev --assume-yes 
		
	    echo "Installing numpy.."
	    pip install numpy
	
	    #(sudo apt-get install build-essential) ||( break) ;
	    
	    #(sudo apt-get install cmake git libgtk2.0-dev pkg-config libavcodec-dev libavformat-dev libswscale-dev)  ||(break) 
	    
	    #(sudo apt-get install python-dev python-numpy libtbb2 libtbb-dev libjpeg-dev libpng-dev libtiff-dev libjasper-dev libdc1394-22-dev) ||(break) 


	    ########################################################################
	    # BUILD BINARIES FROM SOURCE CODE WITH CMAKE  
	    ########################################################################
	    echo "INSTALLING OPENCV.."
	    echo $LINEBREAK
	    
	    (wget -O opencv-3.2.0.zip http://sourceforge.net/projects/opencvlibrary/files/opencv-unix/3.2.0/opencv-3.2.0.zip/download) ||(echo "Filepath to repositiory is broken.Please update this link or check your connection... " &&  exit) 
	    #unzip package 
	    echo "Unzipping Package.."
	    (unzip opencv-*) || (echo "failed to unzip package.." && exit)
	    #clean zip file.
	    (rm *.zip) || (echo "failed to remove zip file.." && exit) 
	    # enter opencv directory
	    cd opencv* 
            
            
     
            
	    echo "Starting Build"
	    echo $LINEBREAK
	    mkdir build
            cd build
	  
	    ( cmake -D CMAKE_BUILD_TYPE=RELEASE -D  BUILD_NEW_PYTHON_SUPPORT=ON -D INSTALL_C_EXAMPLES=ON -D INSTALL_PYTHON_EXAMPLES=ON  -D BUILD_EXAMPLES=ON ..) ||(echo "cmake Failed.. Check cmake command" && exit)
            make 
	    
	    make install
	    ldconfig
	    echo "OpenCV installed correctly "
	    
	    #Cleanup the working environment. Optional
	    echo " Do you wish to keep the opencv source code?"
	    select decision in "Yes" "No"; do

		case $decision in
		    No)
			
			echo "Cleaning environment.."
			cd ..; cd ..; cd ..;
			(sudo rm -r $DIR) || (echo "Failed to cleanup.. Check the directory that this script resides in and manually remove the opencv directory."  && exit)
			echo "Done cleaning environment"
			;;
		    Yes)
			#exit the program.
			echo "Done.Source code can be found in the opencv directory under $src_dir"
			;;
		esac
	    done 
	    
	    ;;
	*)
	    
	    #need sudo privilliges
	    echo "install_opencv function requires sudo privilliges"
	    ;;
    esac
    

}

###################################################
#START OF MAIN SCRIPT.
###################################################

DESCRIPTION="THIS IS THE MAIN ENVIRONMENT SETUP FOR THE UNCC QR SWARM FORMATION SOFTWARE. ENSURE THAT YOU CAN ACCESS SUDO PRIVILLEGES"
LINEBREAK="-----------------------------------------------------------------------------------"
uncc_quadrotor_repo="https://github.com/UNCC-Autonomous-Quadrotor-Team/QR/"



echo $DESCRIPTION
echo $LINEBREAK






###################################################
#Gather options and there respective arguments.
###################################################
#construct flags for the user options

create_usname=false #flag for -n option / create username
usrname="nothing"
clone_repo=false #flag for -g option / clone github repository
buildopencv=false # flag for -b option /build latest version of opencv. 

while  getopts ":n:gb" opt  ; do
    
    case $opt  in
	
	
	n) # Create a new username for the quadrotor Evnrionment.

	   create_usname=true
	   usrname=$OPTARG
	    ;;
	
	g) #Perform a github pull on the UNCC quadrotor repository.
	    clone_repo=true	
	    ;;
	
	b) #build opencv.
	    buildopencv=true
	    ;;
	
	\?) #Unknown was chosen.
	    echo "invalid option :-$OPTARG"
	    exit 4
	    ;;
	
	:) #no arguments for the option was chosen. Display Help.
	    echo "No options chosen for -$OPTARG"
	    exit 5
	    ;;
	
    esac
done
shift $((OPTIND-1))

###################################################
# Check the status of the flags, and process them.
###################################################
# If you're wondering, the processing of the flags is done in the if-else statement below
# instead of in the case statement above because the order of the processing is relevant.
# Below is the desired process order: 
# 1) We want to process the username first, then switch to it. 
# 2) clone the quadrotor repo
# 3) build opencv



if [ "$create_usname" == true  ]; then
    echo "in usname loop"
    
    if id $usrname; then
		echo "Username already exists. Choose an unused username."
		exit 2
	    fi
	    
	    #create username.
	    echo "Creating Username"
	    echo $LINEBREAK 
	    (sudo adduser $usrname) || (exit)
	    cd /home/$usrname
	    
fi

if [ "$clone_repo" == true ]; then

    
	    echo "Performing clone of UNCC Quadrotor Github Repo."
	    echo $LINEBREAK
	    clone_loc=$(pwd)
	    
	    echo "Current repo clone location is at: $clone_loc" 
	    echo "Is this okay?"
	     select choice in "Yes" "No"
	    do
		
		case $choice in
		    
		    Yes)
			#See if github is installed first. If not, install.
			sudo apt-get update  --assume-yes
			sudo apt-get upgrade --assume-yes
			(command -v git) || (echo "Git isn't installed.Installing now" && sudo apt-get install git --assume-yes) || (echo "Cannot install git. Check connection to debian repo." && exit 3)
			#Proceed to fetch a  clone from upstream repo.
			uncc_quadrotor_repo="https://github.com/UNCC-Autonomous-Quadrotor-Team/QR/"
			(sudo git clone $uncc_quadrotor_repo) ||(echo "Repository is already installed.")
			break 
			;;
		    
		    No)
			echo "Move this script to the desired location for github pull."
			exit
			;;
		esac
	     done
fi

	    

if [ "$buildopencv" == true ]; then
    
    
    install_opencv
fi


  
	      







