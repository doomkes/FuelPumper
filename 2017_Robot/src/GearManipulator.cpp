/*
 * GearManipulator.cpp
 *
 *  Created on: Feb 6, 2017
 *      Author: Joey
 */

#include "GearManipulator.h"
#include "RobotMap.h"
#include "TankDrive.h"
GearManipulator::GearManipulator(
		frc::JoystickButton* joystickButton_gearRelease
		,OI *oi
		,CI *ci
		,frc::Servo* m_leftServo
		,frc::Servo* m_rightServo
)
:
joystickButton_gearRelease(joystickButton_gearRelease)
,m_oi (oi)
,m_ci (ci)
, m_leftServo(m_leftServo)
, m_rightServo(m_rightServo)
, m_kicker(SolenoidMap::SOLENOID_GEAR_KICKER)
//: m_leftServo(LEFT_GEAR_SERVO), m_rightServo(RIGHT_GEAR_SERVO)
{
}

GearManipulator::~GearManipulator() {
}

void GearManipulator::TeleopInit() {

}

void GearManipulator::TeleopPeriodic() {
	bool accept = false;
	float tankSpeed = m_ci->tankDrive->GetSpeed();
	if(m_oi->joystick_driverLeft->GetRawButton(BUTTON_L_ACCEPT_GEAR) || tankSpeed <= 36) {
		accept = true;
	}
	Release(joystickButton_gearRelease->Get(), accept);
}

void GearManipulator::Release(bool release, bool Accept) {
	// TODO find out if values passes to servos need to be flipped.
	double leftCloseAngle = Preferences::GetInstance()->GetDouble("GearLeftClosedAngle", 0);
	double rightCloseAngle = Preferences::GetInstance()->GetDouble("GearRightClosedAngle", 180);

	double leftOpenAngle = Preferences::GetInstance()->GetDouble("GearLeftOpenAngle", 180);
	double rightOpenAngle = Preferences::GetInstance()->GetDouble("GearRightOpenAngle", 0);

	if(release == true) {
		m_leftServo->SetAngle(leftOpenAngle);
		m_rightServo->SetAngle(rightOpenAngle);

		m_kicker.Set(true);
	}
	else {
		if(Accept) {
			leftCloseAngle += 20;
			rightCloseAngle -= 20;
		}
		m_leftServo->SetAngle(leftCloseAngle);
		m_rightServo->SetAngle(rightCloseAngle);
		m_kicker.Set(false);
	}
}
