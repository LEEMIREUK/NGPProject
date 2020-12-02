#include "Bullet.h"
#include "../../protocol.h"

void Bullet::Update(std::chrono::system_clock::time_point::duration& time)
{
	speed += BULLET_SPEED * time.count();
	bullet_x = start_x + (speed + 50) * cos(radian);
	bullet_y = start_y + speed * sin(radian);
}
