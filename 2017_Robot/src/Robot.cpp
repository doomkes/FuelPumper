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
#include "Shooter.h"
using frc::SmartDashboard;
using namespace std;
using namespace frc;
class Robot: public frc::IterativeRobot {
	grip::BoilerVision m_boilerVision;
	frc::Solenoid m_gearShift;
	CANTalon m_leftMotor1;
	CANTalon m_leftMotor2;
	CANTalon m_rightMotor1;
	CANTalon m_rightMotor2;

	TankDrive m_tank;

    Shooter m_shooter;
	CANTalon m_shootWheel1;
	CANTalon m_shootWheel2;
	CANTalon m_indexMotor;

	grip::BoilerVision Vision;
	CameraServer *cameraServer = nullptr;
	cs::CvSource m_outputStream;
	cs::UsbCamera camera;
	GearManipulator m_gearManip;

	Pickup m_pickup;
	Joystick m_leftStick, m_rightStick;

	frc::Talon m_intakeMotor;
	// Hopper Motor pushs into Hopper
	frc::Talon m_hopperMotor;



public:
	Robot()
		:
		m_leftStick(LEFTSTICK) //todo:mjj use config constant instead of literal number
		,m_rightStick(RIGHTSTICK) //todo:mjj use config constant instead of literal number
		,m_gearShift(GEAR_SHIFT)
		,m_leftMotor1(LEFT_DRIVE1)
		,m_leftMotor2(LEFT_DRIVE2)
		,m_rightMotor1(RIGHT_DRIVE1)
		,m_rightMotor2(RIGHT_DRIVE2)
		, m_intakeMotor(0)
		, m_hopperMotor(1)
		,m_tank(
			m_leftStick
			, m_rightStick
			, m_gearShift
			, m_leftMotor1
			, m_leftMotor2
			, m_rightMotor1
			, m_rightMotor2
		)
		, m_shootWheel1(0)
		, m_shootWheel2(1)
		, m_indexMotor(2)
		, m_pickup(
			m_leftStick
			, PICKUP
			, m_intakeMotor
			, m_hopperMotor
		)
		, m_shooter(
			m_rightStick
			, m_shootWheel1 //todo:mjj chagne values to constant.
			, m_shootWheel2
			, m_indexMotor
		)


	{

	}

	void RobotInit() {
		SmartDashboard::PutNumber("Exposure", 1);
		if(!Preferences::GetInstance()->ContainsKey("Exposure")) {
			Preferences::GetInstance()->PutFloat("Exposure", 1);
		}
	}

	void TeleopInit() {

	}

	void TeleopPeriodic() {
		m_gearManip.Release(m_leftStick.GetRawButton(LStickMap::GEAR_RELEASE));
		m_tank.TeleopPeriodic();
		m_pickup.TeleopPeriodic();
		m_shooter.TeleopPeriodic();
	}

	void AutonomousInit() override {
		cameraServer = CameraServer::GetInstance();
		camera = cameraServer->StartAutomaticCapture(0);
		m_outputStream = CameraServer::GetInstance()->PutVideo("thresh", 640, 480);
		camera.SetResolution(640, 480);
		camera.SetExposureManual(1);
	}

	void AutonomousPeriodic() {
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

	void TestPeriodic() {

	}
};

START_ROBOT_CLASS(Robot)

