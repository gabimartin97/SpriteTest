#include "Projectile.h"

Projectile::Projectile(const Vec2& pos, const Vec2& vel, float speed, Surface* surf)
	:
	pos(pos),
	vel(vel),
	speed(speed),
	hitbox(RectF(pos,8,8)),
	surf(surf)
{
	
}

void Projectile::Update(float dt)
{
	pos += vel * speed * dt;
	hitbox = RectF(pos, 8, 8);
}

void Projectile::Draw( Graphics& gfx)
{
	gfx.DrawSprite((int)pos.x,(int)pos.y, RectI(0, 8, 0, 8), gfx.GetScreenRect(), *surf, SpriteEffect::Chroma(chroma));
}

RectF Projectile::GetHitbox() const
{
	return hitbox;
}
