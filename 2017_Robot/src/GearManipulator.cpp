/*
 * GearManipulator.cpp
 *
 *  Created on: Feb 6, 2017
 *      Author: Joey
 */

#include "GearManipulator.h"
#include "RobotMap.h"
GearManipulator::GearManipulator(
		frc::Joystick* m_joystick
		,int m_button
		,frc::Servo& m_leftServo
		,frc::Servo& m_rightServo
)
	:
	m_joystick(m_joystick)
	, m_button(m_button)
	, m_leftServo(m_leftServo)
	, m_rightServo(m_rightServo)
	//: m_leftServo(LEFT_GEAR_SERVO), m_rightServo(RIGHT_GEAR_SERVO)
{
}

GearManipulator::~GearManipulator() {
}

void GearManipulator::TeleopInit() {

}

void GearManipulator::TeleopPeriodic() {
	Release(m_joystick->GetRawButton(m_button));
}

void GearManipulator::Release(bool release) {
	// TODO find out if values passes to servos need to be flipped.
	if(release == true) {
		m_leftServo.SetAngle(180);
		m_rightServo.SetAngle(0);
	}
	else {
		m_leftServo.SetAngle(0);
		m_rightServo.SetAngle(180);
	}
}
