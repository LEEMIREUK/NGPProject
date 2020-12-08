#include "Bullet.h"
#include "../../protocol.h"
#include <iostream>
using namespace std;

void Bullet::Update(std::chrono::duration<double>& time)
{
	if (w_type == 1)
		speed += STRONG_BULLET_SPEED * time.count();
	else
		speed += BULLET_SPEED * time.count();
	bullet_x = start_x + (speed + 50) * cos(radian);
	bullet_y = start_y + speed * sin(radian);
	if ((bullet_x < 0 || bullet_x > 1200) || (bullet_y < 0 || bullet_y > 800))
		active = false;
}
