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
	Climber m_climber;
    Shooter m_shooter;
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
		, m_shooter(
			m_rightStick
			, m_shootWheel1 //todo:mjj change values to constant.
			, m_shootWheel2
			, m_indexMotor
			, m_leftStick
			, m_aimLight
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
		m_shooter.Init();
	}

	void TeleopInit() override {
		m_gearManipulator.TeleopInit();
		m_pickup.TeleopInit();
		m_shooter.TeleopInit();
		m_tank.TeleopInit();
		m_vision.TeleopInit();
	}

	void TeleopPeriodic() override {
		//m_gearManipulator.Release(m_leftStick.GetRawButton(LStickMap::GEAR_RELEASE));
		m_gearManipulator.TeleopPeriodic();
		m_tank.TeleopPeriodic();
		m_pickup.TeleopPeriodic();
		m_shooter.TeleopPeriodic();
		m_climber.TeleopPeriodic();
	}

	void AutonomousInit() override {
		cameraServer = CameraServer::GetInstance();
		camera = cameraServer->StartAutomaticCapture(0);
		m_outputStream = CameraServer::GetInstance()->PutVideo("thresh", 640, 480);
		camera.SetResolution(640, 480);
		camera.SetExposureManual(1);
	}

	void AutonomousPeriodic() override {
		cv::Mat frame;

		cameraServer->GetVideo().GrabFrame(frame);
		m_boilerVision.process(frame);
		std::vector<std::vector<cv::Point>> foundContours;
		foundContours = *m_boilerVision.getfindContoursOutput();

		vector<cv::Moments> mu(foundContours.size());

		vector<cv::Point2f> mc(foundContours.size());

		for( unsigned i = 0; i < foundContours.size(); i++ ) {
			mu[i] = moments( foundContours[i], false );
		}

		for(unsigned i = 0; i < foundContours.size(); i++) {
			mc[i] = cv::Point2f(mu[i].m10 / mu[i].m00, mu[i].m01 / mu[i].m00);
		}

		for(unsigned i = 0; i < foundContours.size(); i++) {
			printf("Found Contours!:  x: %f, y: %f\n", mc[i].x, mc[i].y);
		}
		for(unsigned i = 0; i < foundContours.size(); i++) {
			cv::Rect boundingRect = cv::boundingRect(foundContours[i]);
			printf("X: %i, Y: %i, W: %i, H: %i\n",boundingRect.x,boundingRect.y,boundingRect.width,boundingRect.height);
		}


//		cv::Mat contourImg;
//		cv::Mat mymat;
//		for(auto contour : foundContours) {
//			cv::drawContours(contourImg, contour, {255, 0, 0});
//			cv::Mat mymat = *Vision.gethsvThresholdOutput();
//		}
//		m_outputStream.PutFrame(mymat);
	}

	void TestPeriodic() override {

	}
};

START_ROBOT_CLASS(Robot)

