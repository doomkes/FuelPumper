/*
 * GCode.pp
 *
 *  Created on: Feb 17, 2017
 *      Author: Joey
 */

#include <GCode.h>

GCode::GCode() {
	// TODO Auto-generated constructor stub

}

GCode::~GCode() {
	// TODO Auto-generated destructor stub
}

void GCode::G1(float xDist, float yDist, float speed) {
	m_tank->Position();
	float xPos = m_tank->m_xPosition;
	float yPos = m_tank->m_yPosition;
	static float xStart;
	static float yStart;

	static float angle;
	static float distance;

	switch(m_G1State) {
		case 0 : {
			angle=atan2(yDist,xDist);
			SmartDashboard::PutNumber("G0_TargetAngle", angle);
			xStart = m_tank->m_xPosition;
			yStart = m_tank->m_yPosition;
			SmartDashboard::PutNumber("G0_xStart", xStart);
			SmartDashboard::PutNumber("G0_yStart", yStart);

			float angleChangeIn = angle/6.283*95.819;
			float rightDrive = angleChangeIn/2;
			float leftDrive = -angleChangeIn/2;
			SmartDashboard::PutNumber("G0_AngleChange", angleChangeIn);
			SmartDashboard::PutNumber("G0_rightDrive", rightDrive);
			SmartDashboard::PutNumber("G0_leftDrive", leftDrive);

			m_move.SetAll(5, 5, 15, leftDrive);
			distance = sqrt((xDist*xDist)+(yDist*yDist));
			SmartDashboard::PutNumber("G0_Distance", distance);

			m_timer.Reset();
			m_timer.Start();

			m_G1State += 1;
			break;
		}
		case 1: {
			float actAngle = (m_tank->m_gyro.GetAngle()*3.14159)/180;
			float t = m_timer.Get();

			m_tank->PositionDrive(m_move.Position(t), -m_move.Position(t));
			SmartDashboard::PutNumber("G0_ActAngle", actAngle);
			if (fabs(fabs( actAngle) - fabs(angle))<=0.0349) {
				m_timer.Reset();
				m_timer.Start();
				m_move.SetAll(speed/3,speed/3,speed, distance);
				m_G1State += 1;
			}
			break;
		}
		case 2: {
			float t = m_timer.Get();
			float pos = m_move.Position(t);
			SmartDashboard::PutNumber("G0_time", t);
			m_tank->PositionDrive(pos, pos);
			SmartDashboard::PutNumber("G0_pos(t)", pos);
			if(fabs(xDist - (xStart -xPos)) <= 2 && fabs(yDist - (yStart - yPos)) <= 2) {
				m_G1State++;
			}

			break;
		}
		case 3: {

		}
	}
}
