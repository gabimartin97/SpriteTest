#include "Character.h"

Character::Character( const Vec2& pos )
	:
	pos( pos ),
	hitbox(pos,(float)spriteWidth,(float)spriteWidth)
{
	for( int i = 0; i < (int)Sequence::StandingLeft; i++ )
	{
		animations.emplace_back( Animation( 0,64, spriteWidth, spriteHeight,8,sprite,0.16f ) );
	}
	for( int i = (int)Sequence::StandingLeft; i < (int)Sequence::Count; i++ )
	{
		animations.emplace_back( Animation(0,0, spriteWidth, spriteHeight,8,sprite,0.16f ) );
	}
}

void Character::Draw( Graphics& gfx ) const
{
	// if effect active, draw sprite replacing opaque pixels with red
	bool reversed = iCurSequence == Sequence::WalkingLeft || iCurSequence == Sequence::StandingLeft;
	if( effectActive )
	{
		animations[(int)iCurSequence].Draw((Vei2)pos, gfx, SpriteEffect::Damage(chroma),reversed);
	}
	else
	{
		animations[(int)iCurSequence].Draw((Vei2)pos, gfx, SpriteEffect::Chroma(chroma), reversed);
	}
	//gfx.DrawRect(hitbox);
}

void Character::SetDirection( const Vec2& dir )
{
	if( dir.x > 0.0f )
	{
		iCurSequence = Sequence::WalkingRight;
	}
	else if( dir.x < 0.0f )
	{
		iCurSequence = Sequence::WalkingLeft;
	}
	else if( dir.y != 0.0f )
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
		if( vel.x > 0.0f )
		{
			iCurSequence = Sequence::StandingRight;
		}
		else if( vel.x < 0.0f )
		{
			iCurSequence = Sequence::StandingLeft;
		}
		else 
		if( vel.y != 0 )
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

void Character::Update( float dt )
{
	pos += vel * dt;
	hitbox = RectF(pos+Vec2(5, 5), (float)(spriteWidth -10), ((float)spriteWidth -10));
	animations[(int)iCurSequence].Update( dt );
	
	// update effect time if active
	if( effectActive )
	{
		effectTime += dt;
		// deactivate effect if duration exceeded
		if( effectTime >= effectDuration )
		{
			effectActive = false;
		}
	}
	if (invulnerable)
	{
		invulnerabilityTime += dt;
		if (invulnerabilityTime >= invulnerabilityDuration)
		{
			invulnerable = false;
		}
	}
}

Vec2 Character::GetPosition() const
{
	return pos;
}

RectF Character::GetHitbox() const
{
	return hitbox;
}

void Character::ActivateEffect()
{
	if (!invulnerable)
	{
		effectActive = true;
		effectTime = 0.0f;
		invulnerable = true;
		invulnerabilityTime = 0.0f;
	}
}

bool Character::IsColliding(const RectF object)const
{
	return hitbox.IsOverlappingWith(object);
}
