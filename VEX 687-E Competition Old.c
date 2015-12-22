#pragma config(Motor,  port2,           ForwardLeft,   tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port3,           ForwardRight,  tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port4,           BackwardLeft,  tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port5,           BackwardRight, tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port6,           LauncherLeft,  tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port7,           LauncherRight, tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port8,           Intake,        tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port9,           LauncherLeft2,  tmotorVex393_MC29, openLoop)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//
#pragma platform(VEX)
#pragma competitionControl(Competition)
#pragma autonomousDuration(20)
#pragma userControlDuration(120)
#include "Vex_Competition_Includes.c"
void Drive(int L, int R)
{
		motor[ForwardLeft] = L; motor[BackwardLeft] = L;
		motor[ForwardRight] = R; motor[BackwardRight] = R;
}
int Sensitivity(int Input, int Other)
{
	if (Input * Other <= -16 * 127)
		return Input/2;
	return Input;
}
int Threshold(int Input)
{
		if (abs(Input) > 16)
			return 127 * abs(Input)/Input;
		return 0;
}
void Shoot(bool Button)
{
	if (Button)
	{
			motor[LauncherLeft] = 127; motor[LauncherRight] = 127;
			motor[LauncherLeft2] = 127;
	}
	else
		{
			motor[LauncherLeft] = 0; motor[LauncherRight] = 0;
			motor[LauncherLeft2] = 0;
		}
}
int Steer(int Input, bool Direction)
{
		if (Direction)
			return Input/16;
		return Input;
}
void Reload(bool Forward, bool Backward)
{
		if (Forward)
			motor[Intake] = 127;
		else
			motor[Intake] = 0;
		if (Backward)
			motor[Intake] = -127;
}
void pre_auton()
{
  bStopTasksBetweenModes = true;
}

task autonomous()
{
	motor[ForwardLeft] = 96; motor[ForwardRight] = 127;
	motor[BackwardLeft] = 96; motor[BackwardRight] = 127;
	motor[LauncherLeft] = 96; motor[LauncherRight] = 96;
	motor[LauncherLeft2] = 96;
	wait1Msec(4000);
	motor[ForwardLeft] = 0; motor[ForwardRight] = 0;
	motor[BackwardLeft] = 0; motor[BackwardRight] = 0;
	motor[Intake] = 127;
	wait1Msec(11000);
}

task usercontrol()
{
	int L = 0; int R = 0;
	while (true)
	{
		L = Steer(Threshold(vexRT[Ch3]), vexRT[Btn5U]);
		R = Steer(Threshold(vexRT[Ch2]), vexRT[Btn6U]);
		L = Sensitivity(L, R);
		R = Sensitivity(R, L);
		Reload(vexRT[Btn5D], vexRT[Btn7D]);
		Shoot(vexRT[Btn6D]);
		Drive(L, R);
	}
}