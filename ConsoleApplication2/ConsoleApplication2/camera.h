#include <math.h>
float g_rotation;
class Camera {
	double theta;
	double y;
	double dTheta;
	double dy;
public:
	Camera() :theta(0), y(3), dTheta(0.04), dy(0.2) {}
	double getX() { return 10 * cos(theta); }
	double getY() { return y; }
	double getZ() { return 10 * sin(theta); }
	void moveRight() { theta += dTheta; }
	void moveLeft() { theta -= dTheta; }
	void moveUp() { y += dy; }
	void moveDown() { if (y > dy)y -= dy; }
};