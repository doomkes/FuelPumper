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
#include "TrapezoidalMove.h"
#include "RelativeMovement.h"
#include "ChainCommands.h"
#include "Pickup.h"
#include "BoilerVision.h"
#include "GearManipulator.h"
#include "Vision.h"
#include "Shooter.h"
#include "Autonomous.h"
#include "Climber.h"
#include "OI.h"
#include "CI.h"

using frc::SmartDashboard;
using namespace std;
using namespace frc;

class Robot: public frc::IterativeRobot {
	OI* oi = new OI();
	CI* ci = new CI();
	Vision* m_vision = nullptr;
	grip::BoilerVision m_boilerVision;

	TankDrive m_tank;
	TrapezoidalMove m_move;
	Autonomous *m_autonomous = nullptr;
	Climber m_climber;
	Shooter m_shooter;// = nullptr;

	//grip::BoilerVision Vision;
	CameraServer *cameraServer = nullptr;
	cs::CvSource m_outputStream;
	cs::UsbCamera camera;
	GearManipulator m_gearManipulator;
	Pickup m_pickup;


	float m_shooterSpeed = 0;

public:
	Robot()
:
	m_tank(
			oi->joystick_driverLeft,
			oi->joystick_driverRight,
			oi->joystickButton_reverseDrive,
			oi->joystickButton_shiftLow,
			oi->joystickButton_shiftHigh,
			oi->joystickButton_vbus,
			ci->solenoid_gearShift,
			ci->canTalon_leftMotor1,
			ci->canTalon_leftMotor2,
			ci->canTalon_rightMotor1,
			ci->canTalon_rightMotor2
	),
	m_move(),
	m_pickup(
			oi->joystickButton_pickup,
			oi->joystickButton_reversePickup,
			oi,
			ci->canTalon_intake,
			ci->canTalon_hopper
	),
	m_climber(
			oi->joystick_manipulator,
			oi->joystickButton_climb,
			ci->canTalonclimbMotor
	),
	m_gearManipulator(
			oi->joystickButton_gearRelease,
			oi,
			ci->servo_leftGear,
			ci->servo_rightGear
	),
	m_shooter(
			ci->canTalon_shootWheel1
			, ci->canTalon_shootWheel2
			, ci->canTalon_indexMotor
			, ci->canTalon_shooterFeeder
			, oi
			, ci
			, ci->digitalOutput_aimLight
			, oi->joystickButton_shoot
			, oi->joystickButton_reverseIndex
			, oi->joystickButton_aimLight
			, m_shooterSpeed
	)
{

}

	void RobotInit() override {
		float exposure = Preferences::GetInstance()->GetFloat("Exposure", 25);
		cameraServer = CameraServer::GetInstance();

		camera = cameraServer->StartAutomaticCapture(0);
		m_outputStream = CameraServer::GetInstance()->PutVideo("thresh", 320, 240);
		camera.SetResolution(320, 240);
		camera.SetExposureManual(exposure);

		SmartDashboard::PutNumber("drive_P", 0);
		SmartDashboard::PutNumber("drive_I", 0);
		SmartDashboard::PutNumber("drive_D", 0);
		m_vision = new Vision(
				cameraServer,
				ci->usbCamera_1,
				ci->usbCamera_2,
				ci->usbCamera_3,
				ci->digitalOutput_light1,
				ci->digitalOutput_light2,
				ci->digitalOutput_light3,
				oi->joystickButton_camera1,
				oi->joystickButton_camera2,
				oi->joystickButton_camera3,
				oi->joystickButton_cameraLight1,
				oi->joystickButton_cameraLight2,
				oi->joystickButton_cameraLight3
		);

		m_shooterSpeed = frc::Preferences::GetInstance()->GetFloat("ShooterSpeed",0);



		m_autonomous = new Autonomous(cameraServer, m_outputStream, camera, m_tank, m_shooter, m_gearManipulator);
		m_shooter.Init();


	}

	void TeleopInit() override {
		m_gearManipulator.TeleopInit();
		m_pickup.TeleopInit();
		m_shooter.TeleopInit();
		m_tank.Init();
		m_climber.TeleopInit();
		m_vision->TeleopInit();
		m_tank.SetMode(DriveMode::TELEPOSITION);
	}

	void TeleopPeriodic() override {
		//m_gearManipulator.Release(m_leftStick.GetRawButton(LStickMap::GEAR_RELEASE));
		static double lastTime = 0;
		const float time = Timer::GetFPGATimestamp();
		double dt = time - lastTime;
		if (dt > .2) {
			dt = .2;
		};
		SmartDashboard::PutNumber("teleop latency", dt);
		lastTime = time;

		m_gearManipulator.TeleopPeriodic();
		m_tank.TeleopPeriodic(dt);
		m_pickup.TeleopPeriodic();
		m_shooter.TeleopPeriodic();
		m_climber.TeleopPeriodic();
		m_vision->TeleopPeriodic();
	}

	void AutonomousInit() override {
		m_autonomous->AutonomousInit();
	}

	void AutonomousPeriodic() override {
		m_autonomous->AutonomousPeriodic();

	}
	void DisabledPeriodic() override {
		m_autonomous->DisabledPeriodic();
	}
	void TestPeriodic() override {

	}
};

START_ROBOT_CLASS(Robot)

