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
#include <math.h>
using namespace PlayerCc;  

/**
 * Function headers
 *
 **/

player_pose2d_t readPosition(LocalizeProxy& lp);
void printRobotData(BumperProxy& bp, player_pose2d_t pose);

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
		
      		// Print data on the robot to the terminal
      		printRobotData(bp, pose);
        	
      		// This part of the code should be very familiar by now.
      		//
      		// If either bumper is pressed, stop. Otherwise just go forwards
	
      		if(bp[0] || bp[1]){
			// we reverse first
			speed= -0.1;
			turnrate= 0;
			pp.SetSpeed(speed, turnrate);
			
			speed = 0; //reset speed to 0 for turning

			// Left Bumper hit, turn right
			if (bp[0] && !bp[1]) {
				turnrate = dtor(-10);
			}
			// Right bumper hit, turn left
			else if (!bp[0] && bp[1])
			{
				turnrate = dtor(10);
			}
			// Both bumpes hit, random move
			if (bp[0] && bp[1])
			{
				if(rand()%2 > 0){
					turnrate=dtor(-10);
				} else {
					turnrate=dtor(10);
				}
			}
		} 
      		else 
		{
			//turn robot counter-clockwise if the robot's current angle is less than the target position, X = 5, Y = -3.5
			if(pose.pa < tan(2.5/11)) {
				turnrate = dtor(10);
			}
			//if robot is facing the target position, go towards it
			else if(pose.pa >= tan(2.5/11)) {
            		speed = 0.5;
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

void printRobotData(BumperProxy& bp, player_pose2d_t pose)
{

  // Print out what the bumpers tell us:
  std::cout << "Left  bumper: " << bp[0] << std::endl;
  std::cout << "Right bumper: " << bp[1] << std::endl;

  // Print out where we are
  std::cout << "We are at" << std::endl;
  std::cout << "X: " << pose.px << std::endl;
  std::cout << "Y: " << pose.py << std::endl;
  std::cout << "A: " << pose.pa << std::endl;

  
} // End of printRobotData()

double getTan(double xPos, double yPos, double xTarget, double yTarget)
{
    return tan( abs(yPos - yTarget) / abs(xPos-xTarget) );
}
