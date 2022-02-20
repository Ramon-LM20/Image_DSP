#pragma once
#include<iostream>
#include<chrono>
#include<string>

class timeControl
{
public:
	std::chrono::time_point<std::chrono::steady_clock> start, current, end;
	std::chrono::duration<float> duration;
	timeControl();
	~timeControl();
	float get_time();
	void printTimer(int modo); //modo: 1 ms / 2 us / 3 e 0 s
};

