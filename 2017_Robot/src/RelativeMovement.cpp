/*
 * GCode.pp
 *
 *  Created on: Feb 17, 2017
 *      Author: Joey
 */

#include "RelativeMovement.h"

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

	angle=atan2(yDist,xDist)-1.57079;
	if (angle > 3.14159) {
		angle-=6.28318;
	}
	float angleChangeInches = angle/6.28318*95.819;
	float turn = angleChangeInches/2;

	SmartDashboard::PutNumber("G0_TargetAngle", angle);
	SmartDashboard::PutNumber("G0_AngleChange", angleChangeInches);
	SmartDashboard::PutNumber("G0_rightDrive", turn);

	//add turn command to array
	m_chain.AddCommand(DRIVE_TURN,turn,0,0);

	//calc drive distance
	distance = sqrt((xDist*xDist)+(yDist*yDist));
	SmartDashboard::PutNumber("G0_Distance", distance);

	//add drive command to array
	m_chain.AddCommand(DRIVE_STRAIGHT, distance, speed,0);
}
void RelativeMovement::Arc(bool Clockwise,float xEndPoint,float yEndPoint,float xCircle,float yCircle, float speed) {
	//sanity saving constants
	const float radius = sqrt((xCircle*xCircle)+(yCircle*yCircle));
	const float diameter = 2*radius;
	const float circumference = 3.14159*diameter;

	//calc starting angle
	float centerAngle = atan2(yCircle,xCircle)-1.57079;
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
	m_chain.AddCommand(DRIVE_TURN,turn,0,0);

	//calc angle of arc
	float otherAngle=atan2(yEndPoint,xEndPoint)-1.57079;
	float isosAngle = fabs(centerAngle)+fabs(otherAngle);
	float arcAngle = 3.14159 - (2*isosAngle);

	//calc arc length and add to command array
	float arcLength = arcAngle/6.28318 * circumference;
	if (Clockwise) {
		m_chain.AddCommand(DRIVE_ARC_CLOCKWISE,arcLength,speed,radius);
	}
	else {
		m_chain.AddCommand(DRIVE_ARC_COUNTERCLOCKWISE,arcLength,speed,radius);
	}

}

