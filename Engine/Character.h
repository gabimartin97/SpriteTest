#pragma once

#include "Animation.h"
#include "Vec2.h"
#include "SpriteEffect.h"

class Character
{
private:
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
	Character( const Vec2& pos );
	void Draw( Graphics& gfx ) const;
	void SetDirection( const Vec2& dir );
	void Update( float dt );
	Vec2 GetPosition()const;
	// activates a damage visual effect
	void ActivateEffect();
	bool IsColliding(const RectF object)const;
private:
	static constexpr int spriteWidth = 64;
	static constexpr int spriteHeight = 64;
	static constexpr float effectDuration = 0.045f;
	static constexpr float invulnerabilityDuration = 0.5f;
private:
	Surface sprite = "Images\\Character64x64.bmp";
	Vec2 pos;
	Vec2 vel = { 0.0f,0.0f };
	RectF hitbox;
	const Color chroma = Colors::Magenta;
	std::vector<Animation> animations;
	Sequence iCurSequence = Sequence::StandingRight;
	float speed = 110.0f;
	
	float effectTime = 0.0f;
	float invulnerabilityTime = 0.0f;
	bool effectActive = false;
	bool invulnerable = false;
};