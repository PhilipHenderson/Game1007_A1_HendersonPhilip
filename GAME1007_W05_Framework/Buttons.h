#ifndef __BUTTONs_H__
#define __BUTTONs_H__
#include <string>
#include "SDL.h"
#include "GameObject.h"

class Buttons : public Sprite
{
public:
	void Update();
	void Render();

protected:
	std::string m_key;
	enum BtnState { STATE_UP, STATE_OVER, STATE_DOWN } m_state; // enum defined locally.
	virtual void Execute() = 0; // Button3 is an abstract base class.
	Buttons(SDL_Rect, SDL_FRect, const char*);
};

class PlayButton : public Buttons
{
private:
	void Execute();
public:
	PlayButton(SDL_Rect, SDL_FRect, const char*);
};

class ResumeButton : public Buttons
{
private:
	void Execute();
public:
	ResumeButton(SDL_Rect, SDL_FRect, const char*);
};

#endif
