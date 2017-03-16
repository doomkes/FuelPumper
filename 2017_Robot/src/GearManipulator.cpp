/*
 * GearManipulator.cpp
 *
 *  Created on: Feb 6, 2017
 *      Author: Joey
 */

#include "GearManipulator.h"
#include "RobotMap.h"
GearManipulator::GearManipulator(
		frc::JoystickButton* joystickButton_gearRelease
		,frc::Servo* m_leftServo
		,frc::Servo* m_rightServo
)
:
joystickButton_gearRelease(joystickButton_gearRelease)
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
	Release(joystickButton_gearRelease->Get());
}

void GearManipulator::Release(bool release) {
	// TODO find out if values passes to servos need to be flipped.
	double leftCloseAngle = Preferences::GetInstance()->GetDouble("GearLeftClosedAngle", 0);
	double rightCloseAngle = Preferences::GetInstance()->GetDouble("GearRightClosedAngle", 180);

	double leftOpenAngle = Preferences::GetInstance()->GetDouble("GearLeftOpenAngle", 180);
	double rightOpenAngle = Preferences::GetInstance()->GetDouble("GearRightOpenAngle", 0);

	if(release == true) {
		m_leftServo->SetAngle(leftOpenAngle);
		m_rightServo->SetAngle(rightOpenAngle);
	}
	else {
		m_leftServo->SetAngle(leftCloseAngle);
		m_rightServo->SetAngle(rightCloseAngle);
	}
}
