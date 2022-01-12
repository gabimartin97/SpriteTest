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
	void ActivateEffect();
	Vec2 GetPosition()const;
private:
	Surface& sprite;
	Vec2 pos;
	Vec2 vel = { 0.0f,0.0f };
	const Color chroma = Colors::Magenta;
	std::vector<Animation> animations;
	Sequence iCurSequence = Sequence::StandingRight;
	float speed = 80.0f;
	static constexpr float effectDuration = 0.045f;
	float effectTime = 0.0f;
	bool effectActive = false;
};