#include "Enemy.h"

Enemy::Enemy(const Vec2 pos, Surface& surf)
	:
	pos(pos),
	sprite(surf),
	hitbox(pos, (float)spriteWidth, (float)spriteWidth)

{
	for (int i = 0; i < (int)Sequence::Count; i++)
	{
		animations.emplace_back(0, 0, spriteWidth, spriteHeight, 7, sprite, 0.16f);
	}
	
}

void Enemy::Draw(Graphics& gfx) const
{
	// if effect active, draw sprite replacing opaque pixels with red
	bool reversed = iCurSequence == Sequence::WalkingLeft || iCurSequence == Sequence::StandingLeft;
	if (effectActive)
	{
		animations[(int)iCurSequence].Draw((Vei2)pos, gfx, SpriteEffect::Damage(chroma), reversed);
	}
	else
	{
		animations[(int)iCurSequence].Draw((Vei2)pos, gfx, SpriteEffect::Chroma(chroma), reversed);
	}
	gfx.DrawRect(hitbox);
}

void Enemy::SetDirection(const Vec2& dir)
{
	if (dir.x > 0.0f)
	{
		iCurSequence = Sequence::WalkingRight;
	}
	else if (dir.x < 0.0f)
	{
		iCurSequence = Sequence::WalkingLeft;
	}
	else if (dir.y != 0.0f)
	{
		if (iCurSequence == Sequence::StandingRight)
		{
			iCurSequence = Sequence::WalkingRight;
		}
		else if (iCurSequence == Sequence::StandingLeft)
		{
			iCurSequence = Sequence::WalkingLeft;
		}

	}

	else
	{
		if (vel.x > 0.0f)
		{
			iCurSequence = Sequence::StandingRight;
		}
		else if (vel.x < 0.0f)
		{
			iCurSequence = Sequence::StandingLeft;
		}
		else
			if (vel.y != 0)
			{
				if (iCurSequence == Sequence::WalkingLeft)
				{
					iCurSequence = Sequence::StandingLeft;
				}
				else
				{
					iCurSequence = Sequence::StandingRight;
				}

			}

	}
	vel = dir * speed;
}

void Enemy::Update(float dt)
{
	pos += vel * dt;
	animations[(int)iCurSequence].Update(dt);
	hitbox = RectF(pos, (float)spriteWidth, (float)spriteWidth);
	// update effect time if active
	if (effectActive)
	{
		effectTime += dt;
		// deactivate effect if duration exceeded
		if (effectTime >= effectDuration)
		{
			effectActive = false;
		}
	}

}

void Enemy::ActivateEffect()
{
	effectActive = true;
	effectTime = 0.0f;
}

Vec2 Enemy::GetPosition() const
{
	return pos;
}

RectF Enemy::GetHitbox() const
{
	return hitbox;
}