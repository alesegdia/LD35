#include "mapscreen.h"
#include "../ld35game.h"
#include "../assets.h"

MapScreen::MapScreen(LD35 *g)
	: m_game(g),
	  m_hud(m_game->m_player, m_game->m_font)
{
	nextLevel(1);
}

MapScreen::~MapScreen()
{

}

void MapScreen::show()
{
	Assets::instance->battle->pause();
	if( !Assets::instance->music->playing() )
	{
		Assets::instance->music->rewind();
		Assets::instance->music->play();
	}

	if( m_game->m_player->levelingNotification.isPresent )
	{
		infoMessage( m_game->m_player->levelingNotification.text1,
					 m_game->m_player->levelingNotification.text2,
					 m_game->m_player->levelingNotification.text3,
					 m_game->m_player->levelingNotification.text4 );
		Assets::instance->levelupSfx->play();
		m_game->m_player->levelingNotification.isPresent = false;
	}
	m_justEntered = true;
}

void MapScreen::update(double delta)
{
	if (Input::IsKeyDown(ALLEGRO_KEY_ESCAPE))
	{
		m_escapeState = true;
	}


	if (m_escapeState)
	{
		if (Input::IsKeyDown(ALLEGRO_KEY_Y))
		{
			m_game->close();
		}
		else if (Input::IsKeyDown(ALLEGRO_KEY_N))
		{
			m_escapeState = false;
		}
		return;
	}

	if( Input::IsKeyJustPressed(ALLEGRO_KEY_TAB) )
	{
		m_game->m_minimapScreen->config( m_mapRenderer->getVisible(), m_map, m_player->tile() );
		Matrix2DDebug<int>() << *(m_mapRenderer->getVisible());
		m_game->setScreen(m_game->m_minimapScreen);
	}

	if (Input::IsKeyJustPressed(ALLEGRO_KEY_F))
	{
		Assets::instance->music->pause();
	}

	if (Input::IsKeyJustPressed(ALLEGRO_KEY_G))
	{
		Assets::instance->music->resume();
	}

	m_mapRenderer->center(m_player->tile().x(), m_player->tile().y());
	if( Input::IsKeyJustPressed(ALLEGRO_KEY_F5) ) m_game->advanceFloor();
	if( Input::IsKeyJustPressed(ALLEGRO_KEY_F6) ) m_game->endGame();

	bool skipVisualUpdateXD = false;
	if( m_isInfo )
	{
		if( Input::IsKeyJustPressed(ALLEGRO_KEY_ENTER) )
		{
			skipVisualUpdateXD = true;
			m_player->clearInteract();
			m_isInfo = false;
			Vec2i front_tile_coords;
			int front_tile = getTileFrontPlayer(&front_tile_coords);
			if( front_tile == 2 )
			{
				//m_game->advanceFloor();
				m_game->setScreen(m_game->m_midFloorScreen);
			}
			// RED TILE
			else if( front_tile == 3 )
			{
				m_game->m_player->unlock(Fire);
				m_map->set(front_tile_coords.x(), front_tile_coords.y(), 0);
			}
			// GREEN TILE
			else if( front_tile == 4 )
			{
				m_game->m_player->unlock(Gaia);
				m_map->set(front_tile_coords.x(), front_tile_coords.y(), 0);
			}
			// BLUE TILE
			else if( front_tile == 5 )
			{
				m_game->m_player->unlock(Water);
				m_map->set(front_tile_coords.x(), front_tile_coords.y(), 0);
			}
		}
		if( Input::IsKeyJustPressed(ALLEGRO_KEY_BACKSPACE) )
		{
			m_isInfo = false;
		}
	}
	else
	{

		if( !m_checkAbis )
		{
			if(!Input::IsKeyJustPressed(ALLEGRO_KEY_ENTER)) m_player->update(delta);

			if( m_player->tile() != m_prevPlayerPos && !m_justEntered )
			{
				// try new battle!
				int r = RNG::rng->nextInt(30);
				if( r < 3 )
				{
					m_game->setScreen(m_game->m_battleScreen);
				}
			}
			m_justEntered = false;

			m_prevPlayerPos = m_player->tile();
			scroll(m_player->position(), m_game->m_camera1);
			if( Input::IsKeyJustPressed(ALLEGRO_KEY_ENTER) )
			{
				if( getTileFrontPlayer() == 2 )
				{
					infoMessage("ENTER: next floor", "BACKSPACE: cancel", "yo dawg...", "that was ez!");
				}
				else if( getTileFrontPlayer() == 3 )
				{
					infoMessage("Your skin burns!", "You learnt a new", "fire ability!", "");
				}
				else if( getTileFrontPlayer() == 4 )
				{
					infoMessage("You shine!", "You learnt a new", "gaia ability!", "");
				}
				else if( getTileFrontPlayer() == 5 )
				{
					infoMessage("Feeling light!", "You learnt a new", "water ability!", "");
				}
				else
				{
					infoMessage("There's nothing", "here, don't", "waste my time!", "Yo deaawg!!1");
				}
			}
		}
	}

	if( Input::IsKeyJustPressed(ALLEGRO_KEY_SPACE) )
	{
		m_checkAbis = !m_checkAbis;
	}

	if (!skipVisualUpdateXD)
	{
		m_player->visualUpdate(delta);
	}

	if( m_checkAbis )
	{

		if( !m_isInfo )
		{
			HudReturn hudReturn = m_hud.update();

			switch( hudReturn )
			{
			case HudSuccess:

				infoMessage(m_hud.getSelected()->desc1, m_hud.getSelected()->desc2, m_hud.getSelected()->desc3, m_hud.getSelected()->desc4);
				break;
			}
		}
		else
		{
			if( Input::IsKeyJustPressed(ALLEGRO_KEY_BACKSPACE) || Input::IsKeyJustPressed(ALLEGRO_KEY_ENTER) )
			{
				if( m_isInfo )
				{
					m_isInfo = false;
				}
			}
		}
	}


}

void MapScreen::scroll(Vec2f new_cam_pos, Camera& cam)
{
	new_cam_pos = new_cam_pos + Vec2f(-16 - 16, -16 - 16);
	cam.position(Vec2f(round(new_cam_pos.x()), round(new_cam_pos.y())));
}

void MapScreen::map(const Matrix2Di &map)
{
	if( m_map == nullptr )
	{
		m_map.reset(new Matrix2Di(map));
	}
	else
	{
		*m_map = map;
	}

	visible.reset(new Matrix2Di(map.cols(), map.rows(), 0));

	m_freeTiles.clear();

	for( int x = 0; x < m_map->cols(); x++ )
	{
		for( int y = 0; y < m_map->rows(); y++ )
		{
			if( m_map->get(x, y) == 0 )
			{
				m_freeTiles.push_back({x, y});
			}
		}
	}

	Vec2i end = this->randomFreeTile();
	m_map->set(end.x(), end.y(), 2);

	if( m_game->m_player->m_shapeLevel[0] < 4 )
	{
		end = this->randomFreeTile();
		m_map->set(end.x(), end.y(), 3);
	}

	if( m_game->m_player->m_shapeLevel[1] < 4 )
	{
		end = this->randomFreeTile();
		m_map->set(end.x(), end.y(), 4);
	}

	if( m_game->m_player->m_shapeLevel[2] < 4 )
	{
		end = this->randomFreeTile();
		m_map->set(end.x(), end.y(), 5);
	}

}

int MapScreen::getTileFrontPlayer(Vec2i* out)
{
	Vec2i tocheck(m_player->position().x() / 16, m_player->position().y() / 16);
	if( m_player->facing() == 'u' )
	{
		tocheck = tocheck + Vec2i( 0, -1 );
	}
	else if( m_player->facing() == 'd' )
	{
		tocheck = tocheck + Vec2i( 0, 1 );
	}
	else if( m_player->facing() == 'l' )
	{
		tocheck = tocheck + Vec2i( -1, 0 );
	}
	else if( m_player->facing() == 'r' )
	{
		tocheck = tocheck + Vec2i( 1, 0 );
	}
	if( out != nullptr )
	{
		out->set(tocheck.x(), tocheck.y());
	}
	return m_map->get(tocheck.x(), tocheck.y());
}

Matrix2Di::SharedPtr MapScreen::map()
{
	return m_map;
}

Vec2i MapScreen::findFirstWalkable()
{
	for( int x = 0; x < m_map->cols(); x++ )
	{
		for( int y = 0; y < m_map->rows(); y++ )
		{
			if( m_map->get(x, y) == 0 )
			{
				return Vec2i(x, y);
			}
		}
	}
	return Vec2i(-1, -1);
}

Vec2i MapScreen::randomFreeTile()
{
	int index = RNG::rng->nextInt(m_freeTiles.size() - 1);
	Vec2i ret = m_freeTiles[index];

	m_freeTiles.erase(m_freeTiles.begin() + index);

	return ret;
}

void MapScreen::nextLevel(int level)
{
	static constexpr int n = 5;
	map(RandomWalkerGenerator().generate(10 + n * level, 10 + n * level));
	//map(RandomWalkerGenerator().generate(n, n));

	std::vector<ALLEGRO_BITMAP*> tiles;
	tiles.push_back(Assets::instance->tilesetSheet->getFrame(1, 0));
	tiles.push_back(Assets::instance->tilesetSheet->getFrame(2, 0));
	tiles.push_back(Assets::instance->tilesetSheet->getFrame(1, 1));

	tiles.push_back(Assets::instance->tilesetSheet->getFrame(0, 2));
	tiles.push_back(Assets::instance->tilesetSheet->getFrame(0, 0));
	tiles.push_back(Assets::instance->tilesetSheet->getFrame(0, 1));

	m_mapRenderer = std::make_shared<TileMapRenderer>(map(), tiles );
	Vec2i fw = findFirstWalkable();
	Vec2f player_pos( fw.x() * 16, fw.y() * 16 );
	m_player.reset(new PlayerPawnMap(player_pos, map()));
}

void MapScreen::render()
{

	if (m_escapeState)
	{
		m_game->m_camera2.bind();

		al_clear_to_color(al_map_rgb(63, 63, 116));
		al_set_target_bitmap(al_get_backbuffer(m_game->display()));

		al_draw_text(m_game->m_font, al_map_rgb(203, 219, 252), 15, 11, 0, "Quit game?");
		al_draw_text(m_game->m_font, al_map_rgb(203, 219, 252), 20, 20, 0, "(Y/N)");

		return;
	}


	al_clear_to_color(al_map_rgb(0,0,0));
	al_set_target_bitmap(al_get_backbuffer(m_game->display()));

	m_game->m_camera1.bind();
	m_mapRenderer->render();
	m_player->render();

	m_game->m_camera2.bind();

	al_draw_text(m_game->m_font, al_map_rgb(255, 255, 255), 2, 70, 0, (std::string("Floor ") + std::to_string(m_game->m_floor)).c_str());


	if( m_checkAbis )
	{
		m_hud.render();
	}

	if( m_isInfo )
	{
		al_draw_filled_rectangle(0, 44, 80, 80, m_game->m_player->shapeColor());
		int off = 46;
		al_draw_text(m_game->m_font, al_map_rgb(255, 255, 255), 4, off, 0, m_infomessage1.c_str());
		al_draw_text(m_game->m_font, al_map_rgb(255, 255, 255), 4, off + 8, 0, m_infomessage2.c_str());
		al_draw_text(m_game->m_font, al_map_rgb(255, 255, 255), 4, off + 16, 0, m_infomessage3.c_str());
		al_draw_text(m_game->m_font, al_map_rgb(255, 255, 255), 4, off + 24, 0, m_infomessage4.c_str());
	}

	al_draw_text(m_game->m_font, al_map_rgb(255, 255, 255), 2, 2, 0, (std::string("LV.") + std::to_string(m_game->m_player->level())).c_str());

	float offx = 18;
	al_draw_filled_rectangle(offx, 5, 78, 7, al_map_rgb(172, 50, 50));

	al_draw_filled_rectangle(offx, 5, offx + m_game->m_player->expPercentage() * (78 - offx), 7, al_map_rgb(217, 87, 99));



}

void MapScreen::hide()
{

}
