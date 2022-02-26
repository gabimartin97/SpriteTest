#pragma once

#include "Colors.h"
#include "Graphics.h"

namespace SpriteEffect
{
	class Chroma
	{
	public:
		Chroma( Color c )
			:
			chroma( c )
		{}
		void operator()( Color cSrc,int xDest,int yDest,Graphics& gfx ) const
		{
			if( cSrc != chroma )
			{
				gfx.PutPixel( xDest,yDest,cSrc );
			}
		}
	private:
		Color chroma;
	};
	class Substitution
	{
	public:
		Substitution( Color c,Color s )
			:
			chroma( c ),
			sub( s )
		{}
		void operator()( Color cSrc,int xDest,int yDest,Graphics& gfx ) const
		{
			if( cSrc != chroma )
			{
				gfx.PutPixel( xDest,yDest,sub );
			}
		}
	private:
		Color chroma = Colors::Magenta;
		Color sub;
	};
	class Copy
	{
	public:
		void operator()( Color cSrc,int xDest,int yDest,Graphics& gfx ) const
		{
			gfx.PutPixel( xDest,yDest,cSrc );
		}
	};
	class Ghost
	{
	public:
		Ghost( Color c )
			:
			chroma( c )
		{}
		void operator()( Color src,int xDest,int yDest,Graphics& gfx ) const
		{
			if( src != chroma )
			{
				const Color dest = gfx.GetPixel( xDest,yDest );
				const Color blend = {
					unsigned char( (src.GetR() + dest.GetR()) / 2 ),
					unsigned char( (src.GetG() + dest.GetG()) / 2 ),
					unsigned char( (src.GetB() + dest.GetB()) / 2 )
				};
				gfx.PutPixel( xDest,yDest,blend );
			}
		}
	private:
		Color chroma;
	};
	class Damage
	{
	public:
		Damage(Color c)
			:
			chroma(c)
		{}
		void operator()(Color src, int xDest, int yDest, Graphics& gfx) const
		{
			
			if (src != chroma)
			{
				const Color blend = { 255, src.GetG(),src.GetB() };
				gfx.PutPixel(xDest, yDest, blend);
			}
		}
	private:
		Color chroma;
	};

	class Luminate
	{
	public:
		Luminate(Color c, unsigned char value)
			:
			chroma(c),
			value(value)
		{}
		void operator()(Color src, int xDest, int yDest, Graphics& gfx) const
		{
			if (src != chroma)
			{
				unsigned char red = (src.GetR() + value);
				unsigned char green = (src.GetG() + value);
				unsigned char blue = (src.GetB() + value);
				if ((src.GetR() + value) >= 255) red = 255;
				if ((src.GetG() + value) >= 255) green = 255;
				if ((src.GetB() + value) >= 255) blue = 255;
				const Color blend = {red,green,blue};
				
				gfx.PutPixel(xDest, yDest, blend);
			}
		}
	private:
		Color chroma;
		unsigned char value;
	};
}