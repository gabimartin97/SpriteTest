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
#include <random>

Game::Game( MainWindow& wnd )
	:
	wnd( wnd ),
	gfx( wnd )
	
{
	std::random_device dev;
	std::mt19937 rng(dev());
	std::uniform_int_distribution<int> xDist(-200, 1000); // distribution in range [-200, 1000]
	std::uniform_int_distribution<int> yDist(-200, 800); // distribution in range [-200, 800]

	for (int i = 0; i < 5; i++)
	{
		enemies.emplace_back(Vec2((float)xDist(rng), (float)yDist(rng)), &enemySurf);
	}
	
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
		float dt = 1.0f / 60.0f;
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
	// update character
	link.SetDirection(dir);
	link.Update(dt);

	//Genero proyectiles
	if (!wnd.mouse.IsEmpty() )
	{
		if (wnd.mouse.Read().GetType() == Mouse::Event::Type::LPress )
		{
			Vec2 direction = Vec2(wnd.mouse.GetPos()) - link.GetPosition(); // Vector desde el personaje hasta el mouse
			projectiles.emplace_back(link.GetHitbox().GetCenter(), direction.Normalize(), (float)1000.0f, &fireball);
		}
	}

	//Actualizo proyectiles
	for (auto i = projectiles.begin();i<projectiles.end(); ++i)
	{
		i->Update(dt);
		auto bulletHitbox = i->GetHitbox();
		
		if (!bulletHitbox.IsOverlappingWith(gfx.GetScreenRect()))
		{
			//Proyectil fuera de la pantalla
			i->SetImpact();
		}
		
	}

	//-----------------------ACTUALIZO ENEMIGOS -----------------------//

	{
		auto removeFrom = std::remove_if(enemies.begin(), enemies.end(), [](Enemy& e) {return e.IsDead(); });
		enemies.erase(removeFrom, enemies.end());
	}

	for (auto enemyIterator = enemies.begin(); enemyIterator < enemies.end(); ++enemyIterator)
	{
		RectF enemyHitbox = enemyIterator->GetHitbox();
		Vec2 newEnemyDirection = Vec2(0,0);
		//Primero chequeo si los enemigos entran en contacto con el presonaje
		if (enemyHitbox.IsOverlappingWith(link.GetHitbox()))
		{
			link.ActivateEffect();
			newEnemyDirection = Vec2(0,0);		//Para que no se superpongan con el personaje

		}
		else
		{
			newEnemyDirection = (link.GetPosition() - enemyIterator->GetPosition()).GetNormalized(); //Persiguen al personaje
		}
		

		//Luego chequeo si los enemigos entran en contacto con los projectiles
		for (Projectile& p : projectiles)
		{
			if (enemyHitbox.IsOverlappingWith(p.GetHitbox()))
			{
				p.SetImpact();
				enemyIterator->GetDamage(10);
			}
		}
		//Por ultimo separo a los enemigos que se tocan entre si
		for (auto i = enemies.begin(); i < enemies.end(); ++i)
		{
			if (enemyIterator == i) continue;
			if (enemyHitbox.IsOverlappingWith(i->GetHitbox()))
			{
				//Hago que los enemigos se alejen entre si pero sigan persiguiendo al personaje
				newEnemyDirection = (newEnemyDirection -(i->GetPosition() - enemyIterator->GetPosition()).GetNormalized()).GetNormalized();
			}

		}


		enemyIterator->SetDirection(newEnemyDirection);
		enemyIterator->Update(dt);
	}
	//-----------------------ACTUALIZO ENEMIGOS -----------------------//
		
	//Destruyo projectiles
	auto eraseFrom = std::remove_if(projectiles.begin(), projectiles.end(), [](Projectile& p) {return p.isCollided();});
	projectiles.erase(eraseFrom, projectiles.end());

}

void Game::ComposeFrame()
{
	//gfx.DrawSprite(0, 0, background, SpriteEffect::Copy());

	link.Draw( gfx );
		
	for ( Enemy& e : enemies)
	{
		e.Draw(gfx);
	}
	for ( Projectile& p : projectiles)
	{
		p.Draw(gfx);
	}
}
