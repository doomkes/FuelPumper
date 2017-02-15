#include <iostream>
#include <memory>
#include <string>
//mjj005
#include <IterativeRobot.h>
#include <LiveWindow/LiveWindow.h>
#include <SmartDashboard/SendableChooser.h>
#include <SmartDashboard/SmartDashboard.h>
#include <WPILib.h>
#include "RobotMap.h"
#include "TankDrive.h"
#include "Pickup.h"
#include "BoilerVision.h"
#include "GearManipulator.h"
#include "Vision.h"
#include "Shooter.h"
#include "Autonomous.h"
#include "Climber.h"
using frc::SmartDashboard;
using namespace std;
using namespace frc;
class Robot: public frc::IterativeRobot {
	Vision m_vision;
	grip::BoilerVision m_boilerVision;
	frc::Solenoid m_gearShift;
	CANTalon m_leftMotor1;
	CANTalon m_leftMotor2;
	CANTalon m_rightMotor1;
	CANTalon m_rightMotor2;
	CANTalon m_climbMotor;
	TankDrive m_tank;
	Autonomous *m_autonomous = nullptr;
	Climber m_climber;
	Shooter *m_shooter;
	CANTalon m_shootWheel1;
	CANTalon m_shootWheel2;
	CANTalon m_indexMotor;
	DigitalOutput m_aimLight;
	//grip::BoilerVision Vision;
	CameraServer *cameraServer = nullptr;
	cs::CvSource m_outputStream;
	cs::UsbCamera camera;
	GearManipulator m_gearManipulator;
	Pickup m_pickup;
	Joystick m_leftStick, m_rightStick, m_manStick;

	frc::Talon m_intakeMotor;
	// Hopper Motor pushs into Hopper
	frc::Talon m_hopperMotor;

	frc::Servo m_leftGearServo;
	frc::Servo m_rightGearServo;

	float m_shooterSpeed;

public:
	Robot()
		:
		m_leftStick(STICK_LEFT) //todo:mjj use config constant instead of literal number
		,m_rightStick(STICK_RIGHT)//todo:mjj use config constant instead of literal number
		,m_manStick(STICK_MAN)
		,m_gearShift(SOLENOID_GEAR_SHIFT)
		,m_leftMotor1(MOTOR_LEFT_DRIVE1)
		,m_leftMotor2(MOTOR_LEFT_DRIVE2)
		,m_rightMotor1(MOTOR_RIGHT_DRIVE1)
		,m_rightMotor2(MOTOR_RIGHT_DRIVE2)
		,m_intakeMotor(MOTOR_PICKUP_INTAKE)
		,m_climbMotor(MOTOR_CLIMB)
		,m_hopperMotor(MOTOR_PICKUP_HOPPER)
		,m_tank(
			m_leftStick
			, m_rightStick
			, m_gearShift
			, m_leftMotor1
			, m_leftMotor2
			, m_rightMotor1
			, m_rightMotor2
		)
		, m_aimLight(LIGHT_AIM)
		, m_shootWheel1(0)
		, m_shootWheel2(1)
		, m_indexMotor(2)
		, m_pickup(
			m_leftStick
			, BUTTON_L_PICKUP
			, m_intakeMotor
			, m_hopperMotor
		)
		, m_climber(
			m_manStick
			, m_climbMotor
		)
		, m_leftGearServo(MOTOR_LEFT_GEAR_SERVO)
		, m_rightGearServo(MOTOR_RIGHT_GEAR_SERVO)
		, m_gearManipulator(
			m_rightStick
			, BUTTON_L_GEAR_RELEASE
			, m_leftGearServo
			, m_rightGearServo
		)
		, m_vision(

		)

	{

	}

	void RobotInit() override {
		SmartDashboard::PutNumber("Exposure", 1);
		if(!Preferences::GetInstance()->ContainsKey("Exposure")) {
			Preferences::GetInstance()->PutFloat("Exposure", 1);
		}
		cameraServer = CameraServer::GetInstance();
		m_autonomous = new Autonomous(cameraServer, m_outputStream, camera);

		m_shooterSpeed = frc::Preferences::GetInstance()->GetFloat("ShooterSpeed",0);
		m_shooter = new Shooter(
			m_shootWheel1
			, m_shootWheel2
			, m_indexMotor
			, m_aimLight
			, BUTTON_M_SHOOT
			, m_manStick
			, BUTTON_M_REVERSEINDEX
			, m_manStick
			, BUTTON_L_AIM_LIGHT
			, m_leftStick
			, m_shooterSpeed
		);


		m_shooter->Init();
	}

	void TeleopInit() override {
		m_gearManipulator.TeleopInit();
		m_pickup.TeleopInit();
		m_shooter->TeleopInit();
		m_tank.TeleopInit();
		m_climber.TeleopInit();
		m_vision.TeleopInit();
	}

	void TeleopPeriodic() override {
		//m_gearManipulator.Release(m_leftStick.GetRawButton(LStickMap::GEAR_RELEASE));
		m_gearManipulator.TeleopPeriodic();
		m_tank.TeleopPeriodic();
		m_pickup.TeleopPeriodic();
		m_shooter->TeleopPeriodic();
		m_climber.TeleopPeriodic();
		m_vision.TeleopPeriodic();
}

	void AutonomousInit() override {
	    m_autonomous->AutonomousInit();
	}

	void AutonomousPeriodic() override {
		m_autonomous->AutonomousPeriodic();

	}

	void TestPeriodic() override {

	}
};

START_ROBOT_CLASS(Robot)

