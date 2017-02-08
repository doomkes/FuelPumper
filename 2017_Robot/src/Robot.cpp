#include <iostream>
#include <memory>
#include <string>
//mjj004
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
using frc::SmartDashboard;
using namespace std;
using namespace frc;
class Robot: public frc::IterativeRobot {
	TankDrive m_tank;
	Vision m_vision;
	grip::BoilerVision m_boilerVision;
	CameraServer *cameraServer = nullptr;
	cs::CvSource m_outputStream;
	cs::UsbCamera camera;
	GearManipulator m_gearManip;

	Pickup m_pickup;
	Joystick m_leftStick, m_rightStick;
public:
	Robot()
		: m_leftStick(1), m_rightStick(2)
	{

	}

	void RobotInit() {
		SmartDashboard::PutNumber("Exposure", 1);
		if(!Preferences::GetInstance()->ContainsKey("Exposure")) {
			Preferences::GetInstance()->PutFloat("Exposure", 1);
		}
		m_vision.Init();
	}

	void TeleopInit() {

	}

	void TeleopPeriodic() {
		m_tank.Drive(-m_leftStick.GetY(), m_rightStick.GetY());
		if(m_leftStick.GetRawButton(PICKUP)) {
			m_pickup.Intake(true);
		} else m_pickup.stop();

		if(m_leftStick.GetRawButton(CAMERA1)) {
			m_vision.SwitchCamera(0);
		}
		else if(m_leftStick.GetRawButton(CAMERA2)) {
			m_vision.SwitchCamera(1);
		}

		m_gearManip.Release(m_leftStick.GetRawButton(LStickMap::GEAR_RELEASE));
//		if(m_leftStick.GetRawButton(LStickMap::GEAR_RELEASE)) {
//
//		}
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
