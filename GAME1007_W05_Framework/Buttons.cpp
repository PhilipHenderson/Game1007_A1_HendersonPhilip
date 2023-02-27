#include "Buttons.h"
#include "CollisionManager.h"
#include "EventManager.h"
#include "RenderManager.h"
#include "TextureManager.h"
#include "MathManager.h"
#include "StateManager.h"

Buttons::Buttons(SDL_Rect s, SDL_FRect d, const char* k) :Sprite(s, d), m_state(STATE_UP), m_key(k) {}

void Buttons::Update()
{
	bool col = COMA::PointAABBCheck(EVMA::GetMousePos(), m_dst);
	switch (m_state)
	{
	case STATE_UP:
		if (col)
		{
			m_state = STATE_OVER;
			EVMA::SetCursor(SDL_SYSTEM_CURSOR_HAND);
		}
		break;
	case STATE_OVER:
		if (!col)
		{
			m_state = STATE_UP;
			EVMA::SetCursor(SDL_SYSTEM_CURSOR_ARROW);
		}
		else if (col && EVMA::MousePressed(1)) m_state = STATE_DOWN; // 1 is left mouse.
		break;
	case STATE_DOWN:
		if (EVMA::MouseReleased(1)) // Left mouse released.
		{
			if (col)
			{
				m_state = STATE_OVER;
				EVMA::SetCursor(SDL_SYSTEM_CURSOR_ARROW);
				Execute();
			}
			else
			{
				m_state = STATE_UP;
				EVMA::SetCursor(SDL_SYSTEM_CURSOR_ARROW);
			}
		}
		break;
	}
}

void Buttons::Render()
{
	m_src.x = m_src.w * (int)m_state;
	SDL_RenderCopyF(REMA::GetRenderer(), TEMA::GetTexture(m_key), &m_src, &m_dst);
}

PlayButton::PlayButton(SDL_Rect s, SDL_FRect d, const char* k) :Buttons(s, d, k) {}

void PlayButton::Execute()
{
	STMA::ChangeState(new GameState());
}

ResumeButton::ResumeButton(SDL_Rect s, SDL_FRect d, const char* k) :Buttons(s, d, k) {}

void ResumeButton::Execute()
{
	STMA::PopState();
	return;
}

MenuButton::MenuButton(SDL_Rect s, SDL_FRect d, const char* k) :Buttons(s, d, k) {}

void MenuButton::Execute()
{
	STMA::ChangeState(new TitleState());
}

