#pragma once

#include "Surface.h"
#include "Graphics.h"
#include <vector>
#include "SpriteEffect.h"

class Animation
{
public:
	Animation( int x,int y,int width,int height,int count,const Surface& sprite,float holdTime,Color chroma = Colors::Magenta );
	
	
	// this version of draw replaces all opaque pixels with specified color
	
	void Update( float dt );
public:
	template<typename E>
	void Draw(const Vei2& pos, Graphics& gfx, E effect) const
	{
		Draw(pos,gfx, effect, gfx.GetScreenRect());
	}

	template<typename E>
	void Draw(const Vei2& pos, Graphics& gfx, E effect, const RectI& clip) const
	{
		gfx.DrawSprite(pos.x, pos.y, frames[iCurFrame], clip, sprite, effect);
	}
private:
	void Advance();
private:
	Color chroma;
	const Surface& sprite;
	std::vector<RectI> frames;
	int iCurFrame = 0;
	float holdTime;
	float curFrameTime = 0.0f;
};