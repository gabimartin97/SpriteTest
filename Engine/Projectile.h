#pragma once
#include "Animation.h"
#include "Vec2.h"
#include "SpriteEffect.h"
#include "Graphics.h"
class Projectile
{
public:
	Projectile(const Vec2& pos, const Vec2& vel, float speed, Surface& surf);
	void Update(float dt);
	void Draw(Graphics& gfx);
	RectF GetHitbox() const;

private:
	Vec2 pos;
	Vec2 vel;
	float speed;
	RectF hitbox;
	Surface& surf;
	Color chroma = Colors::Magenta;
};