#include "Enemy.h"

Enemy::Enemy(const Vec2 pos, Surface* surf) 
	:
	pos(pos),
	sprite(surf),
	hitbox(pos, (float)spriteWidth, (float)spriteWidth)
{
	hitbox = hitbox.GetExpanded(-2.0f);	//Reduzco la hitbox

	for (int i = 0; i < (int)Sequence::Count; i++)
	{
		animations.emplace_back(0, 0, spriteWidth, spriteHeight, 7, *sprite, 0.16f);
	}
	
}

void Enemy::Draw(Graphics& gfx) const
{
	// if effect active, draw sprite replacing opaque pixels with red
	bool reversed = iCurSequence == Sequence::WalkingLeft || iCurSequence == Sequence::StandingLeft;
	if (DmgEffectActive)
	{
		animations[(int)iCurSequence].Draw((Vei2)pos, gfx, SpriteEffect::Damage(chroma), reversed);
	}
	else
	{
		animations[(int)iCurSequence].Draw((Vei2)pos, gfx, SpriteEffect::Chroma(chroma), reversed);
	}
	//gfx.DrawRect(hitbox);
}

void Enemy::SetDirection(const Vec2& dir)
{
	if (!changedSequence)
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
		changedSequence = true;
	}
				vel = dir * speed;
}

void Enemy::Update(float dt)
{
	pos += vel * dt;
	animations[(int)iCurSequence].Update(dt);
	hitbox = RectF(pos, (float)spriteWidth, (float)spriteWidth);
	// update effect time if active
	if (DmgEffectActive)
	{
		DmgEffectTime += dt;
		// deactivate effect if duration exceeded
		if (DmgEffectTime >= effectDuration)
		{
			DmgEffectActive = false;
		}
	}
	// solo cambiar la animacion de la dirección cada x tiempo
	if (changedSequence)
	{
		sequenceTimer += dt;
		// deactivate effect if duration exceeded
		if (sequenceTimer >= sequenceCooldown)
		{
			changedSequence = false;
			sequenceTimer = 0.0f;
		}
	}

}



Vec2 Enemy::GetPosition() const
{
	return pos;
}

RectF Enemy::GetHitbox() const
{
	return hitbox;
}

void Enemy::GetDamage(const int dmg)
{
	DmgEffectActive = true;
	DmgEffectTime = 0.0f;
	health -= dmg;
}

bool Enemy::IsDead() const
{
	return health <= 0;
}
