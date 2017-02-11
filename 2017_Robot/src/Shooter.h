/*
 * Shooter.h
 *
 *  Created on: Jan 16, 2017
 *      Author: Joey
 */

#ifndef SRC_SHOOTER_H_
#define SRC_SHOOTER_H_

#include <CANTalon.h>

class Shooter {
	CANTalon m_shootWheel1, m_shootWheel2;
	CANTalon m_indexMotor;
public:
	Shooter();
	virtual ~Shooter();

	//Want to allow it to activate shooter motors let
	//then get up to speed then activate index motor
	void Shoot();
    void Stop();
    void ReverseIndex();
    void Init();


};

#endif /* SRC_SHOOTER_H_ */
