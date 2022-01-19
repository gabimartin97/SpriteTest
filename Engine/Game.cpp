/****************************************************************************************** 
 *	Chili DirectX Framework Version 16.07.20											  *	
 *	Game.cpp																			  *
 *	Copyright 2016 PlanetChili.net <http://www.planetchili.net>							  *
 *																						  *
 *	This file is part of The Chili DirectX Framework.									  *
 *																						  *
 *	The Chili DirectX Framework is free software: you can redistribute it and/or modify	  *
 *	it under the terms of the GNU General Public License as published by				  *
 *	the Free Software Foundation, either version 3 of the License, or					  *
 *	(at your option) any later version.													  *
 *																						  *
 *	The Chili DirectX Framework is distributed in the hope that it will be useful,		  *
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of						  *
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the						  *
 *	GNU General Public License for more details.										  *
 *																						  *
 *	You should have received a copy of the GNU General Public License					  *
 *	along with The Chili DirectX Framework.  If not, see <http://www.gnu.org/licenses/>.  *
 ******************************************************************************************/
#include "MainWindow.h"
#include "Game.h"

Game::Game( MainWindow& wnd )
	:
	wnd( wnd ),
	gfx( wnd ),
	enemy(Vec2(50,50),enemySurf)
{
	
}

void Game::Go()
{
	gfx.BeginFrame();	
	UpdateModel();
	ComposeFrame();
	gfx.EndFrame();
}

void Game::UpdateModel()
{
	#ifdef NDEBUG
		float dt = ft.Mark();
	#else
		float dt = 1.0f / 144.0f;
	#endif

	// process arrow keys state
	Vec2 dir = { 0.0f,0.0f };
	if (wnd.kbd.KeyIsPressed(VK_UP))
	{
		dir.y -= 1.0f;
	}
	if (wnd.kbd.KeyIsPressed(VK_DOWN))
	{
		dir.y += 1.0f;
	}
	if (wnd.kbd.KeyIsPressed(VK_LEFT))
	{
		dir.x -= 1.0f;
	}
	if (wnd.kbd.KeyIsPressed(VK_RIGHT))
	{
		dir.x += 1.0f;
	}
	link.SetDirection(dir);
	// update character
	
	link.Update(dt);

	//Genero proyectiles
	if (!wnd.mouse.IsEmpty() )
	{
		if (wnd.mouse.Read().GetType() == Mouse::Event::Type::LPress )
		{
			Vec2 direction = Vec2(wnd.mouse.GetPos()) - link.GetPosition();
			projectiles.emplace_back(link.GetHitbox().GetCenter(), direction.Normalize(), (float)1000.0f, &fireball);
		}
	}

	//Actualizo proyectiles
	for (auto i = projectiles.begin();i<projectiles.end();)
	{
		i->Update(dt);
		auto bulletHitbox = i->GetHitbox();
		
		if (!bulletHitbox.IsOverlappingWith(gfx.GetScreenRect()))
		{
			//Proyectil fuera de la pantalla
			i=projectiles.erase(i);
		}
		else if(bulletHitbox.IsOverlappingWith(enemy.GetHitbox()))
		{
			//Proyectil impacta enemigo
			i = projectiles.erase(i);
			enemy.GetDamage();
		}
		else
		{
			++i;
		}
		
	}


	enemy.SetDirection((link.GetPosition() - enemy.GetPosition()).GetNormalized());
	enemy.Update(dt);
	

	if (link.IsColliding(enemy.GetHitbox()))
	{
		link.ActivateEffect();
	}
}

void Game::ComposeFrame()
{
	//gfx.DrawSprite(0, 0, background, SpriteEffect::Copy());
	link.Draw( gfx );
	enemy.Draw(gfx);
	for (Projectile& p : projectiles)
	{
		p.Draw(gfx);
	}
}
