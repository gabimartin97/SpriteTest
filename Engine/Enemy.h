#pragma once
#include "Animation.h"
#include "Vec2.h"
#include "SpriteEffect.h"

class Enemy {
public:
	enum class Sequence
	{
		WalkingLeft,
		WalkingRight,
		WalkingUp,
		WalkingDown,
		StandingLeft,
		StandingRight,
		StandingUp,
		StandingDown,
		Count
	};
public:
	Enemy(const Vec2 pos, Surface& surf);
	void Draw(Graphics& gfx) const;
	void SetDirection(const Vec2& dir);
	void Update(float dt);
	
	Vec2 GetPosition()const;
	RectF GetHitbox()const;
	void GetDamage();
private:
	void ActivateEffect();
private:
	static constexpr int spriteWidth = 64;
	static constexpr int spriteHeight = 64;
private:
	Surface& sprite;
	Vec2 pos;
	Vec2 vel = { 0.0f,0.0f };
	RectF hitbox;
	const Color chroma = Colors::Magenta;
	std::vector<Animation> animations;
	Sequence iCurSequence = Sequence::StandingRight;
	
	static constexpr float effectDuration = 0.045f;
	static constexpr float sequenceCooldown = 0.200f;	//Para que la animacion no pase de izq a derecha tan rapido
	float sequenceTimer = 0.0f;
	float speed = 80.0f;
	float effectTime = 0.0f;
	bool effectActive = false;
	bool changedSequence = false;
};