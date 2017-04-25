/*
 * Shooter.cpp
 *
 *  Created on: Jan 16, 2017
 *      Author: Joey
 */


#include "Shooter.h"
#include "RobotMap.h"
using namespace std;
Shooter::Shooter(
		CANTalon* m_shootWheel1
		, CANTalon* m_shootWheel2
		, CANTalon* m_indexMotor
		, frc::Talon* m_shooterFeeder
		, OI *oi
		, CI *ci
		, frc::DigitalOutput* m_aimLight
		, frc::JoystickButton* joystickButton_shoot
		, frc::JoystickButton* joystickButton_reverseIndex
		, frc::JoystickButton* joystickButton_aimingLight
		, float m_shooterSpeed
)
	:
	m_particleAccelerator(m_shootWheel1)
	, m_afterBurner(m_shootWheel2)
	, m_indexMotor(m_indexMotor)
	, m_aimLight(m_aimLight)
	, m_OI(oi)
	, m_CI(ci)
	, joystickButton_shoot(joystickButton_shoot)
	, joystickButton_reverseIndex(joystickButton_reverseIndex)
	, joystickButton_aimingLight(joystickButton_aimingLight)
	, m_shooterSpeed(m_shooterSpeed)
	, m_shooterFeeder(m_shooterFeeder)
{
	m_shootWheel1->SetControlMode(frc::CANSpeedController::kSpeed);
	m_shootWheel2->SetControlMode(frc::CANSpeedController::kSpeed);
	m_shootWheel1->SetCurrentLimit(30);
	m_shootWheel2->SetCurrentLimit(30);
	m_shootWheel1->EnableCurrentLimit(true);
	m_shootWheel2->EnableCurrentLimit(true);
	m_indexMotor->SetControlMode(frc::CANSpeedController::kSpeed);
}

Shooter::~Shooter() {
	// TODO Auto-generated destructor stub
}

void Shooter::TeleopInit() {
	Spinup(2950, false);
}

void Shooter::TeleopPeriodic() {
	enum ShootMode {
		STOP,
		SPINUP,
		TRIM,

	};
	static string shooterStatus;
	static bool firstTime = true;
	static int state = 0;
	static Timer pickupTimer;
	if(firstTime) {
		pickupTimer.Start();
	}

	static bool pickingUp = false;

	if (m_OI->joystick_manipulator->GetRawButton(1)) {
		Shoot();

	}
	else {
		SetIndexer(0);
	}
	if(m_OI->joystick_manipulator->GetRawButton(7)) {
		state = 1;
	}

	else if(m_OI->joystick_manipulator->GetRawButton(11)) {
		state = 0;
	}
	else if(m_OI->joystick_manipulator->GetRawButton(9)) {
		state = 2;
	}
	if(m_OI->joystickButton_reverseIndex->Get()) {
		ReverseIndex();
	}
	constexpr float shooterSpeed = 2950;
	switch(state){
	case STOP:
		Stop();
		shooterStatus = "Stopped";
		break;
	case SPINUP:
		Spinup(shooterSpeed, false);
		shooterStatus = "Running, no trim";
		break;
	case TRIM:
		Spinup(shooterSpeed, true);
		shooterStatus = "Running, trim";
		break;
	}
	SmartDashboard::PutString("Shooter Status",  shooterStatus);
	SmartDashboard::PutBoolean("Shooter_ShootBtn", joystickButton_shoot->Get());
}


void Shooter::Shoot() {
	double IndexVoltageFactor = 1;
	double IndexMotorAmps = m_pdp.GetCurrent(4);
	SmartDashboard::PutNumber("Index Current", IndexMotorAmps);

	// TODO Figure out actual threshold
//	if (abs(IndexMotorAmps) > 7.5) {
//		IndexVoltageFactor -= .005;
//		SmartDashboard::PutBoolean ("Indexer Is Jammed.", true);
//	}
//	else {
//		SmartDashboard::PutBoolean ("Indexer Is Jammed.", false);
//		IndexVoltageFactor += .005;
//	}
	float ballsPS;
	float speedAdjust = 0;
	static int ballsShot = 0;
	static bool countingBalls = false;
	float paSpeed = m_particleAccelerator->GetSpeed();
	float abSpeed = m_afterBurner->GetSpeed();

	SmartDashboard::PutNumber("PASpeed", paSpeed);
	SmartDashboard::PutNumber("ABSpeed", abSpeed);
	static bool firstTime = true;
	static Timer pickupTimer;
	m_CI->canTalon_hopper->Set(-.6);
	m_CI->canTalon_intake->Set(-.6);
	static bool pickingUp = false;
	ballsPS = Preferences::GetInstance()->GetDouble("IndexBallsPS", 4);
	SetIndexer(15*ballsPS);
	//Pulse pickup.
	if(firstTime) {
		pickupTimer.Start();
	}
//		if(pickupTimer.Get() >= 1) {
//			m_CI->canTalon_hopper->Set(-1);
//			m_CI->canTalon_intake->Set(-1);
//			pickingUp = true;
//			pickupTimer.Reset();
//		}
//		if (pickingUp == true && pickupTimer.Get() >= 0.5) {
//			m_CI->canTalon_hopper->Set(0);
//			m_CI->canTalon_intake->Set(0);
//		}
		if (abSpeed <= -3600 && countingBalls) {
			countingBalls = false;
			ballsShot +=1;
		}
		else if (abSpeed >= -3700 && !countingBalls) {
			countingBalls = true;
		}
		SmartDashboard::PutNumber("Balls Shot", ballsShot);
		SmartDashboard::PutBoolean("Counting Balls?", countingBalls);
		SmartDashboard::PutBoolean("Should Index", shouldIndex);


}

void Shooter::SetIndexer(float speed) {
	shouldIndex = true;
	m_indexMotor->SetSetpoint(speed);
	m_shooterFeeder->Set(.5);
	float indexSpeed = m_indexMotor->GetSpeed();
	SmartDashboard::PutNumber("Indexer Speed", indexSpeed);
}

void Shooter::ReverseIndex() {
	m_indexMotor->SetSetpoint(-60);
}


void Shooter::Stop() {
	m_particleAccelerator->SetSetpoint(0);
	m_afterBurner->SetSetpoint(0);
	m_shooterFeeder->Set(0);
}

void Shooter::Init() {
	m_particleAccelerator->SetFeedbackDevice(CANTalon::FeedbackDevice::QuadEncoder);
	m_afterBurner->SetFeedbackDevice(CANTalon::FeedbackDevice::QuadEncoder);
	m_indexMotor->SetFeedbackDevice(CANTalon::CtreMagEncoder_Relative);

	m_particleAccelerator->SetVelocityMeasurementPeriod(CANTalon::VelocityMeasurementPeriod::Period_100Ms);
	m_afterBurner->SetVelocityMeasurementPeriod(CANTalon::VelocityMeasurementPeriod::Period_100Ms);
	m_particleAccelerator->SetVelocityMeasurementWindow(64);
	m_afterBurner->SetVelocityMeasurementWindow(64);

	m_particleAccelerator->ConfigEncoderCodesPerRev(20);
	m_afterBurner->ConfigEncoderCodesPerRev(20);

	m_particleAccelerator->SetControlMode(frc::CANSpeedController::kSpeed);
	m_afterBurner->SetControlMode(frc::CANSpeedController::kSpeed);
	m_indexMotor->SetControlMode(frc::CANSpeedController::kSpeed);

	m_particleAccelerator->SetPID(4,.005,0,1.5);
	m_afterBurner->SetPID(4,.005,0,1.5);
	m_particleAccelerator->SetIzone(200);
	m_afterBurner->SetIzone(200);

	m_indexMotor->SetPID(1,0,0, 1);
	m_indexMotor->ConfigPeakOutputVoltage(+12,-12);
	m_indexMotor->SetVoltageRampRate(50);
	//the encoder is on backwards
	m_indexMotor->SetSensorDirection(true);
	m_indexMotor->ConfigEncoderCodesPerRev(4096);

	m_indexMotor->SetSetpoint(0);
	m_particleAccelerator->SetSetpoint(0);
	m_afterBurner->SetSetpoint(0);
}
void Shooter::Spinup(float speed, bool trimable) {
	//speed is the accelerator speed. Afterburner is calculated based on 380/315 ratio
	float sliderPos =  -m_OI->joystick_manipulator->GetRawAxis(3);
	if(trimable) {
		speedAdjust = 150 * (sliderPos);
	}
	speed += speedAdjust;
	const float acceleratorSpeed = -speed;
	const float afterBurnerSpeed = -(380/315*(speed));
	float paSpeed = m_particleAccelerator->GetSpeed();
	float abSpeed = m_afterBurner->GetSpeed();
	SmartDashboard::PutNumber("PASpeed", paSpeed);
	SmartDashboard::PutNumber("ABSpeed", abSpeed);
	m_particleAccelerator->SetSetpoint(acceleratorSpeed);
	m_afterBurner->SetSetpoint(afterBurnerSpeed);
}
void Shooter::AimLight(bool state) {

	if(state)
		m_aimLight->UpdateDutyCycle(0.7);
	else
		m_aimLight->UpdateDutyCycle(0);

}
