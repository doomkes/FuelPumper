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
	float angleChangeInches = angle/6.283*95.819;
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
