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
	void Draw(const Vei2& pos, Graphics& gfx, E effect, bool reversed = false) const
	{
		Draw(pos,gfx, effect, gfx.GetScreenRect(), reversed);
	}

	template<typename E>
	void Draw(const Vei2& pos, Graphics& gfx, E effect, const RectI& clip, bool reversed = false) const
	{
		gfx.DrawSprite(pos.x, pos.y, frames[iCurFrame], clip, sprite, effect, reversed);
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