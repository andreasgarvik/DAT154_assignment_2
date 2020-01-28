#include "Car.h"
class Car
{
private:
	int x, y;
	bool drivingSouth;

public:
	Car(int xIn, int yIn, bool drivingSouthIn) {
		x = xIn;
		y = yIn;
		drivingSouth = drivingSouthIn;
	}
	void Move() {
		if (drivingSouth) {
			y = y + 3;
		}
		else {
			x = x + 3;
		}
		
	}
	bool GetDrivingSouth() {
		return drivingSouth;
	}
	int GetX() {
		return x;
	}
	int GetY() {
		return y;
	}
};
