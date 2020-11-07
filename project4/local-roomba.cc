/*
 * local-roomba.cc
 * 
 * Group 12: Jennie Kang, Edmund Lam, Jamila Toaha
 *
 * Project 4: Part 1 - Blobs
 *
 * 
 * 
 * Sample code for a roomba-like robot that has two front bumpers and
 * magically knows where it is. 
 *
 * Written by: Simon Parsons
 * Date:       24th October 2011
 *  
 **/


#include <iostream>
#include <libplayerc++/playerc++.h>
using namespace PlayerCc;  

/**
 * Function headers
 *
 **/

player_pose2d_t readPosition(LocalizeProxy& lp);
void printRobotData(BumperProxy& bp, player_pose2d_t pose, float distance);
double getTan(double xPos, double yPos, double xTarget, double yTarget);
float getDistance (double xPos, double yPos, double xTarget, double yTarget);

/**
 * main()
 *
 **/

int main(int argc, char *argv[])
{  


  // Variables
  int counter = 0;
  double speed;            // How fast do we want the robot to go forwards?
  double turnrate;         // How fast do we want the robot to turn?
  player_pose2d_t  pose;   // For handling localization data
  float distance; 	   // How far is robot from the target?
  double targetTan;  	   // Save the tan result for our target
  // Set up proxies. These are the names we will use to connect to 
  // the interface to the robot.
  PlayerClient    robot("localhost");  
  BumperProxy     bp(&robot,0);  
  Position2dProxy pp(&robot,0);
  LocalizeProxy   lp (&robot, 0);

  // Allow the program to take charge of the motors (take care now)
  pp.SetMotorEnable(true);

  // Main control loop
  while(true) 
    {    
      // Update information from the robot.
      robot.Read();
      // Read new information about position
      pose = readPosition(lp);

      distance = getDistance(pose.px, pose.py, 5, -3.5);
      targetTan = getTan(pose.px, pose.py, 5, -3.5) -.1;
      // Print data on the robot to the terminal
      printRobotData(bp, pose, distance);

      // This part of the code should be very familiar by now.
      //
      // If either bumper is pressed, stop. Otherwise just go forwards
		
      if(bp[0] || bp[1]){
	speed= 0;
	turnrate= 0;
      } 
      else {
	//turn robot counter-clockwise if the robot's current angle is less than the target position, X = 5, Y = -3.5
//getTan   (2.5/11)

//subtracting .1 to make angle less
	
	if(pose.pa < targetTan ) {



	double degrees = 1 +  30 * (1- pose.pa/targetTan); //proportional control version 2 // 11 is maximum distance we think it will be. 

		turnrate = dtor(degrees);

	}
	//if robot is facing the target position, go towards it
	else if(pose.pa >= getTan (pose.px, pose.py, 5, -3.5) -.1) {

	 //speed = 0.1 + (100 / (double) distance);	 //proportional control, version 1
	 speed = 0.1 +  0.9 * ((double)distance/11); //proportional control version 2 // 11 is maximum distance we think it will be. 
								//maximum speed: .5, minimum speed: .1
	
	    turnrate = 0;
		//robot comes to a stop around X = 4.8, Y = -3.3
		if(pose.px > 4.8 && pose.py > -4) {
	           turnrate = 0;
		   speed = 0;
	        }
	        		

        } 
      }   
	

      // What are we doing?
      std::cout << "Speed: " << speed << std::endl;      
      std::cout << "Turn rate: " << turnrate << std::endl << std::endl;

      // Send the commands to the robot
      pp.SetSpeed(speed, turnrate);  
      // Count how many times we do this
      counter++;
    }
  
} // end of main()


/**
 * readPosition()
 *
 * Read the position of the robot from the localization proxy. 
 *
 * The localization proxy gives us a hypothesis, and from that we extract
 * the mean, which is a pose. 
 *
 **/

player_pose2d_t readPosition(LocalizeProxy& lp)
{

  player_localize_hypoth_t hypothesis;
  player_pose2d_t          pose;
  uint32_t                 hCount;

  // Need some messing around to avoid a crash when the proxy is
  // starting up.

  hCount = lp.GetHypothCount();

  if(hCount > 0){
    hypothesis = lp.GetHypoth(0);
    pose       = hypothesis.mean;
  }

  return pose;
} // End of readPosition()


/**
 *  printRobotData
 *
 * Print out data on the state of the bumpers and the current location
 * of the robot.
 *
 **/

void printRobotData(BumperProxy& bp, player_pose2d_t pose, float distance)
{

  // Print out what the bumpers tell us:
  std::cout << "Left  bumper: " << bp[0] << std::endl;
  std::cout << "Right bumper: " << bp[1] << std::endl;

  // Print out where we are
  std::cout << "We are at" << std::endl;
  std::cout << "X: " << pose.px << std::endl;
  std::cout << "Y: " << pose.py << std::endl;
  std::cout << "A: " << pose.pa << std::endl;
  std::cout << "Distance from target: " << distance << std::endl;

  
} // End of printRobotData()

double getTan (double xPos, double yPos, double xTarget, double yTarget){
	return tan ((yTarget - yPos) /(xTarget -xPos));
}


float getDistance (double xPos, double yPos, double xTarget, double yTarget) {
	return sqrt (pow(xTarget-xPos, 2) + 
		     pow(yTarget-yPos, 2) );
}



