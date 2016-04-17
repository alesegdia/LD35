
#include "hud.h"

#include <allegro5/allegro_primitives.h>

PlayerHud::PlayerHud(Player::SharedPtr player, ALLEGRO_FONT *font)
	: m_player(player), m_font(font)
{

}

void PlayerHud::render()
{
	// draw base gui color
	al_draw_filled_rectangle(0, 44, 80, 80, m_player->shapeColor());

	// draw entries with cursor
	for( int i = m_top; i < m_bot; i++ )
	{
		if( m_selected == i )
		{
			al_draw_bitmap(Assets::instance->cursorRight, -10, 46 + (i - m_top) * 10, 0);
		}
		if( m_player->abilities().size() > i )
		{
			if( m_player->abilities()[i]->canUse() )
			{
				al_draw_text(m_font, al_map_rgb(255, 255, 255), 10, 50 + (i - m_top) * 10, 0, m_player->abilities()[i]->text());
			}
			else
			{
				al_draw_text(m_font, al_map_rgb(192, 192, 192), 10, 50 + (i - m_top) * 10, 0, m_player->abilities()[i]->text());
			}
		}
	}

	// draw helpers to show more
	if( m_top > 0 )
	{
		al_draw_bitmap(Assets::instance->cursorUp, 64, 50, 0);
	}

	if( m_bot < m_player->abilities().size() )
	{
		al_draw_bitmap(Assets::instance->cursorDown, 64, 60, 0);
	}

	// draw player health bar
	al_draw_filled_rectangle(3, 46, 77, 48, al_map_rgb(102, 57, 49));
	al_draw_filled_rectangle(3, 46, 3 + m_player->lifePercentage() * (77.f - 3.f), 48, al_map_rgb(223, 113, 38));

}

HudReturn PlayerHud::update()
{
	HudReturn done = HudNoOp;

	if( Input::IsKeyJustPressed(ALLEGRO_KEY_UP) )
	{
		if( tryCursorUp() )
		{
			m_selected--;
			m_player->setShape(m_player->abilities()[m_selected]->shapeType());
		}
	}

	if( Input::IsKeyJustPressed(ALLEGRO_KEY_DOWN) )
	{
		if( tryCursorDown() )
		{
			m_selected++;
			m_player->setShape(m_player->abilities()[m_selected]->shapeType());
		}
	}

	if( Input::IsKeyJustPressed(ALLEGRO_KEY_ENTER) )
	{
		if( m_player->abilities()[m_selected]->canUse() )
		{
			m_player->setShape(m_player->abilities()[m_selected]->shapeType());
			done = HudSuccess;
		}
		else
		{
			done = HudNoOp;
		}
	}

	return done;
}

bool PlayerHud::tryCursorUp()
{
	if( m_selected - 1 >= 0 )
	{
		if( m_selected - 1 < m_top )
		{
			m_top--;
			m_bot--;
		}
		return true;
	}
	return false;
}

int PlayerHud::numAbilities()
{
	return m_player->abilities().size();
}

bool PlayerHud::tryCursorDown()
{
	if( m_selected + 1 < numAbilities() )
	{
		if( m_selected + 1 >= m_bot )
		{
			m_bot++;
			m_top++;
		}
		return true;
	}
	return false;
}

Ability::SharedPtr PlayerHud::getSelected()
{
	return m_player->abilities()[m_selected];
}

EnemyHud::EnemyHud(Entity *player) : m_player(player), m_enemyLayout(LayoutWidth, LayoutHeight, nullptr)
{

}

void drawCursorOver( int x, int y )
{
	al_draw_bitmap(Assets::instance->cursorDown, 8 + x * 24, -12 + y * 21, 0);
}

void EnemyHud::render()
{
	// draw enemies
	for( int x = 0; x < m_enemyLayout.cols(); x++ )
	{
		for( int y = 0; y < m_enemyLayout.rows(); y++ )
		{
			if( m_enemyLayout.get(x, y) != nullptr )
			{
				al_draw_bitmap(Assets::instance->enemyPlaceholder, 8 + x * 24, 5 + y * 21, 0);
				drawHealthFor(m_enemyLayout.get(x, y), 8 + x * 24, 5 + y * 21);
			}
		}
	}


	if( !m_ignore )
	{
		if( m_hover )
		{
			drawCursorOver(m_selectedX, m_selectedY);
			for( Entity* e : getSelecteds(m_ability) )
			{
				if( e != nullptr )
				{
					drawCursorOver(e->position.x(), e->position.y());
				}
			}
		}
	}
}

HudReturn EnemyHud::update()
{
	if( m_ignore )
	{
		return HudSuccess;
	}
	HudReturn done = HudNoOp;

	if( Input::IsKeyJustPressed(ALLEGRO_KEY_UP) )
	{
		if( m_selectedY - 1 >= 0 )
		{
			m_selectedY--;
		}
	}

	if( Input::IsKeyJustPressed(ALLEGRO_KEY_DOWN) )
	{
		if( m_selectedY + 1 < LayoutHeight )
		{
			m_selectedY++;
		}
	}

	if( Input::IsKeyJustPressed(ALLEGRO_KEY_LEFT) )
	{
		if( m_selectedX - 1 >= 0 )
		{
			m_selectedX--;
		}
	}

	if( Input::IsKeyJustPressed(ALLEGRO_KEY_RIGHT) )
	{
		if( m_selectedX + 1 < LayoutWidth )
		{
			m_selectedX++;
		}
	}

	if( Input::IsKeyJustPressed(ALLEGRO_KEY_ENTER) )
	{
		m_hover = false;
		done = HudSuccess;
		if( getSelecteds(m_ability).size() == 0 )
		{
			done = HudFailure;
		}
	}

	if( Input::IsKeyJustPressed(ALLEGRO_KEY_BACKSPACE) )
	{
		done = HudBack;
	}

	return done;
}

void EnemyHud::clear()
{
	m_enemyLayout.fill(nullptr);
}

void EnemyHud::checkAlive()
{
	for( int x = 0; x < LayoutWidth; x++ )
	{
		for( int y = 0; y < LayoutHeight; y++ )
		{
			Enemy::SharedPtr e = m_enemyLayout.get(x, y);
			if( e != nullptr )
			{
				if( e->getStat(HP)  <= 0 )
				{
					m_enemyLayout.set(x, y, nullptr);
				}
			}
		}
	}
}

void EnemyHud::set(Enemy::SharedPtr enemy, int x, int y)
{
	assert( x >= 0 && x < LayoutWidth && y >= 0 && y < LayoutHeight );
	m_enemyLayout.set(x, y, enemy);
	enemy->position.set(x, y);
}

std::vector<Entity *> EnemyHud::getSelecteds(Ability::SharedPtr ability)
{
	std::vector<Entity*> enemies_selected;
	Enemy::SharedPtr enemy = m_enemyLayout.get(m_selectedX, m_selectedY);
	switch( ability->pickType() )
	{
	case Pick2x2Block:
		std::cout << "entering 2x2 pick" << std::endl;
		for( int x = m_selectedX; x < m_selectedX + 2; x++ )
		{
			for( int y = m_selectedY; y < m_selectedY + 2; y++ )
			{
				if( x >= 0 && x < m_enemyLayout.cols() && y >= 0 && y < m_enemyLayout.rows() )
				{
					Enemy::SharedPtr e = m_enemyLayout.get(x, y);
					if( e != nullptr )
					{
						enemies_selected.push_back(e.get());
					}
				}
			}
		}
		break;
	case PickAll:
		for( int x = 0; x < LayoutWidth; x++ )
		{
			for( int y = 0; y < LayoutHeight; y++ )
			{
				Enemy::SharedPtr e = m_enemyLayout.get(x, y);
				enemies_selected.push_back(e.get());
			}
		}
		break;
	default:
	case PickSingle:
		if( enemy != nullptr ) enemies_selected.push_back(enemy.get());
		break;
	case PickSelf:
		enemies_selected.push_back(m_player);
		break;
	}
	return enemies_selected;
}

void EnemyHud::setupSelection(Ability::SharedPtr ability)
{
	m_ignore = ability->pickType() == PickSelf;
	m_ability = ability;
	m_selectedX = m_selectedY = 0;
	m_hover = true;
}

void EnemyHud::setHover(bool hover)
{
	m_hover = hover;
}

void EnemyHud::drawHealthFor(Enemy::SharedPtr enemy, int x, int y)
{
	al_draw_filled_rectangle(x, y-2, x + 16, y-1, al_map_rgba(153, 229, 80, 255));
	al_draw_filled_rectangle(x, y-2, x + 16 * enemy->lifePercentage(), y-1, al_map_rgba(55, 148, 110, 255));
}

GenericHud::~GenericHud()
{

}
