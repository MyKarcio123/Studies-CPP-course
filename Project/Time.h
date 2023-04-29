#pragma once
class Time
{
public:
	static double getDeltaTime() {
		return delta;
	}
	static void setDelta(double delta_) {
		delta = delta_;
	}
private:
	static double delta;
};

