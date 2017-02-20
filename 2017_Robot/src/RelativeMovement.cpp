/*
 * GCode.pp
 *
 *  Created on: Feb 17, 2017
 *      Author: Joey
 */

#include "RelativeMovement.h"
#include <Map>

using namespace std;

RelativeMovement::RelativeMovement(TankDrive& Tank, ChainCommands& Chain):m_tank(Tank),m_chain(Chain) {
	// TODO Auto-generated constructor stub

}

RelativeMovement::~RelativeMovement() {
	// TODO Auto-generated destructor stub
}

void RelativeMovement::Linear(float xDist, float yDist, float speed) {
	static float angle;
	static float distance;
	//calc turn

	angle=atan2(yDist,xDist);
	if (angle > 3.14159) {
		angle-=6.28318;
	}
	float angleChangeInches = angle/6.28318*95.819;
	float turn = angleChangeInches/2;

	SmartDashboard::PutNumber("G0_TargetAngle", angle);
	SmartDashboard::PutNumber("G0_AngleChange", angleChangeInches);
	SmartDashboard::PutNumber("G0_rightDrive", turn);

	//add turn command to array
	m_chain.AddCommand(DRIVE_TURN,turn,0);

	//calc drive distance
	distance = sqrt((xDist*xDist)+(yDist*yDist));
	SmartDashboard::PutNumber("G0_Distance", distance);

	//add drive command to array
	m_chain.AddCommand(DRIVE_STRAIGHT, distance, speed);
}
void RelativeMovement::Arc(bool Clockwise,float xEndPoint,float yEndPoint,float xCircle,float yCircle, float speed) {
	//sanity saving constants
	const float radius = sqrt((xCircle*xCircle)+(yCircle*yCircle));
	const float diameter = 2*radius;
	const float circumference = 3.14159*diameter;
	const float distBtwnWhl = 30.5; /*Distance Between Wheels*/

	//calc starting angle
	float centerAngle = atan2(xCircle,yCircle);
	float angleChange = centerAngle;
	if(xCircle>0 && Clockwise) {
		angleChange+=1.57079;
	}
	else if(xCircle>0 && !Clockwise) {
		angleChange-=1.57079;
	}
	else if(xCircle<0 && Clockwise) {
		angleChange-=1.57079;
	}
	else if(xCircle<0 && !Clockwise) {
		angleChange+=1.57079;
	}
	//remove redundant paths
	if (angleChange > 3.14159) {
			angleChange-=6.28318;
	}
	//calc turn in inches and add to command array
	float angleChangeInches = angleChange/6.28318*95.819;
	float turn = angleChangeInches/2;
	m_chain.AddCommand(DRIVE_TURN,turn,0);

	//calc length of arc
	float
	-1h/c
}

