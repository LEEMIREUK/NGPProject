#include "Bullet.h"
#include "../../protocol.h"
#include <iostream>
using namespace std;

void Bullet::Update(std::chrono::duration<double>& time)
{
	speed += BULLET_SPEED * time.count();
	bullet_x = start_x + (speed + 50) * cos(radian);
	bullet_y = start_y + speed * sin(radian);
}
