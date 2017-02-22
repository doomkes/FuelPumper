/*
 * Vision.cpp
 *
 *  Created on: Feb 13, 2017
 *      Author: administrator
 */

#include <Vision.h>

Vision::Vision(
		frc::CameraServer* cameraServer,
		cs::UsbCamera* usbCamera_1,
		cs::UsbCamera* usbCamera_2,
		cs::UsbCamera* usbCamera_3,
		frc::DigitalOutput* digitalOutput_cameraLight1,
		frc::DigitalOutput* digitalOutput_cameraLight2,
		frc::DigitalOutput* digitalOutput_cameraLight3,
		frc::JoystickButton* joystickButton_camera1,
		frc::JoystickButton* joystickButton_camera2,
		frc::JoystickButton* joystickButton_camera3,
		frc::JoystickButton* joystickButton_cameraLight1,
		frc::JoystickButton* joystickButton_cameraLight2,
		frc::JoystickButton* joystickButton_cameraLight3
)
:
cameraServer(cameraServer),
usbCamera_1(usbCamera_1),
usbCamera_2(usbCamera_2),
usbCamera_3(usbCamera_3),
digitalOutput_cameraLight1(digitalOutput_cameraLight1),
digitalOutput_cameraLight2(digitalOutput_cameraLight2),
digitalOutput_cameraLight3(digitalOutput_cameraLight3),
joystickButton_camera1(joystickButton_camera1),
joystickButton_camera2(joystickButton_camera2),
joystickButton_camera3(joystickButton_camera3),
joystickButton_cameraLight1(joystickButton_cameraLight1),
joystickButton_cameraLight2(joystickButton_cameraLight2),
joystickButton_cameraLight3(joystickButton_cameraLight3)

{
	// TODO Auto-generated constructor stub

}

Vision::~Vision() {
	// TODO Auto-generated destructor stub
}

void Vision::TeleopInit() {

}

void Vision::TeleopPeriodic() {
	//TODO:MJJ should we just have one light button, and it illuminates that light for the camera that is currently active?
	if (this->joystickButton_cameraLight1->Get()) {
		this->CameraLight1(true);
	}  else {
		this->CameraLight1(false);
	}

	if (this->joystickButton_cameraLight2->Get()) {
		this->CameraLight2(true);
	}  else {
		this->CameraLight2(false);
	}

	if (this->joystickButton_cameraLight3->Get()) {
		this->CameraLight3(true);
	}  else {
		this->CameraLight3(false);
	}

	if (this->joystickButton_camera1) {
		this->Camera(this->usbCamera_1);
	}
}

void Vision::CameraLight(frc::DigitalOutput* cameraLight, bool state) {

	if(state)
		cameraLight->UpdateDutyCycle(0.7);
	else
		cameraLight->UpdateDutyCycle(0);
}

void Vision::CameraLight1(bool state) {
	this->CameraLight(this->digitalOutput_cameraLight1, state);
}

void Vision::CameraLight2(bool state) {
	this->CameraLight(this->digitalOutput_cameraLight2, state);
}

void Vision::CameraLight3(bool state) {
	this->CameraLight(this->digitalOutput_cameraLight3, state);
}

void Vision::Camera(cs::UsbCamera* usbCamera) {
	// ...
}



