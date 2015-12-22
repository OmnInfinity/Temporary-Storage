#pragma config(I2C_Usage, I2C1, i2cSensors)
#pragma config(Sensor, I2C_1,  ,               sensorQuadEncoderOnI2CPort,    , AutoAssign )
#pragma config(Motor,  port2,           ForwardLeft,   tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port3,           ForwardRight,  tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port4,           BackwardLeft,  tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port5,           BackwardRight, tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port6,           LauncherLeft,  tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port7,           LauncherRight, tmotorVex393_MC29, openLoop, encoderPort, I2C_1)
#pragma config(Motor,  port8,           Intake,        tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port9,           Debug,         tmotorVex393_MC29, openLoop)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#pragma platform(VEX)
#pragma competitionControl(Competition)
#pragma autonomousDuration(20)
#pragma userControlDuration(120)
#include "Vex_Competition_Includes.c"

const float Gain = 0.025;
const float Tolerance = 20;
short Speed;
float Input;
float Output;
float pOutput = 0;
float Error;
float pError = Input;
const float dTime = 0.05;

void Drive(int LVertical, int RVertical) {
	motor[ForwardLeft] = LVertical; 	motor[ForwardRight] = RVertical;
	motor[BackwardLeft] = LVertical;	motor[BackwardRight] = RVertical;
}

int Sensitivity(int Input, int Other) {
	if (Input * Other <= -16 * 127)
		return Input/2;
	return Input;
}

int Threshold(int Input) {
		if (abs(Input) > 16)
			return 127 * abs(Input)/Input;
		return 0;
}

float Controller(float Current) {
	Error = Input - Current;
	Output += Error * Gain;
	if (Output > 127)
		Output = 127;
	if (Output < 0)
		Output = 0;
	if (Error * pError < 0) {
		if (abs(pOutput - Output) > Tolerance)
			Output = pOutput;
		else
			Output = (pOutput + Output)/2;
	}
	pOutput = Output;
	pError = Error;
	return Output;
}

void Shoot(bool Button, float Target, float Power) {
	Input = Target;
	if (Button) {
		motor[LauncherLeft] = Power; motor[LauncherRight] = Power;
	}
	else {
		motor[LauncherLeft] = 0; motor[LauncherRight] = 0;
	}
}

int Steer(int Input, bool Direction) {
	if (Direction)
		return Input/16;
	return Input;
}

void Reload(bool Forward, bool Backward) {
	if (Forward)
		motor[Intake] = 127;
	else if (Backward)
		motor[Intake] = -127;
	else
		motor[Intake] = 0;
}

void Debugger(bool Enable) {
	motor[Debug] = 127;
}

task second() {
	while (true) {
		Speed = getMotorVelocity(LauncherRight);
		Output = Controller(Speed);
		wait1Msec(dTime * 1000);
	}
}

void pre_auton()
{
  bStopTasksBetweenModes = true;
}

task autonomous()
{
	startTask(second);
	Shoot(true, 10, 10);
	wait1Msec(100);
	Shoot(vexRT[Btn6D], 96, Output);
	motor[ForwardLeft] = 96; motor[ForwardRight] = 127;
	motor[BackwardLeft] = 96; motor[BackwardRight] = 127;
	motor[LauncherLeft] = Output; motor[LauncherRight] = Output;
	wait1Msec(4000);
	motor[ForwardLeft] = 0; motor[ForwardRight] = 0;
	motor[BackwardLeft] = 0; motor[BackwardRight] = 0;
	motor[Intake] = 127;
	wait1Msec(11000);
}

task usercontrol()
{
	bool Debug = true;
	startTask(second);
	Shoot(true, 10, 10);
	wait1Msec(100);
	int L = 0; int R = 0;
	while (true)
	{
		L = Sensitivity(Steer(Threshold(vexRT[Ch3]), vexRT[Btn5U]), R);
		R = Sensitivity(Steer(Threshold(vexRT[Ch2]), vexRT[Btn6U]), L);
		Reload(vexRT[Btn5D], vexRT[Btn7D]);
		Shoot(vexRT[Btn6D], 96, Output);
		Drive(L, R);
		if (vexRT[Btn7L])
			Debug = !Debug;
		Debugger(Debug);
	}
}
