#include "mapscreen.h"
#include "../ld35game.h"
#include "../assets.h"

MapScreen::MapScreen(LD35 *g)
	: m_game(g)
{
	map(RandomWalkerGenerator().generate(60, 60));

	std::vector<ALLEGRO_BITMAP*> tiles;
	tiles.push_back(Assets::instance->tilesetSheet->getFrame(1, 0));
	tiles.push_back(Assets::instance->tilesetSheet->getFrame(2, 0));

	m_mapRenderer = std::make_shared<TileMapRenderer>(map(), tiles );
	Vec2i fw = findFirstWalkable();
	std::cout << fw << std::endl;
	Vec2f player_pos( fw.x() * 16, fw.y() * 16 );
	m_player.reset(new PlayerPawnMap(player_pos, map()));
}

MapScreen::~MapScreen()
{

}

void MapScreen::show()
{

}

void MapScreen::update(double delta)
{
	m_player->update(delta);
	scroll(m_player->position(), m_game->m_camera1);

}

void MapScreen::scroll(Vec2f new_cam_pos, Camera& cam)
{
	new_cam_pos = new_cam_pos + Vec2f(-16 - 16, -16 - 16);
	cam.position(Vec2f(round(new_cam_pos.x()), round(new_cam_pos.y())));
}

void MapScreen::render()
{
	al_clear_to_color(al_map_rgb(0,0,0));
	al_set_target_bitmap(al_get_backbuffer(m_game->display()));

	m_game->m_camera1.bind();
	m_mapRenderer->render();
	m_player->render();
}

void MapScreen::hide()
{

}
