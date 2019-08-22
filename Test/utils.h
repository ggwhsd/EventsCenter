#pragma once
#include <iostream>
#include <thread>
using namespace std;

void SleepMilliSec(int t)
{
	this_thread::sleep_for(chrono::milliseconds(t));
}