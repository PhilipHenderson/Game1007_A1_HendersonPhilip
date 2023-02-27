#pragma once
#ifndef __STATES_H__
#define __STATES_H__
#include <vector>
#include <string>
#include "GameObject.h"

class State // This is the abstract base class for all state subclasses.
{
public:
	virtual ~State() = default;
	virtual void Enter() = 0; // = 0 means pure virtual. Method MUST be defined in subclass.
	virtual void Update();
	virtual void Render();
	virtual void Exit();
	virtual void Resume() {};
	void AddChild(std::string, GameObject*);
	GameObject* GetChild(const std::string&);
	void RemoveChild(const std::string&);

protected: // Private but inherited.
	std::vector<std::pair<std::string, GameObject*>> m_objects;
	State() = default;
};

class TitleState : public State // Abstract Base Class
{
public:
	TitleState();
	virtual void Enter(); // Pure Virtual method
	virtual void Update();
	virtual void Render();
	virtual void Exit();
private:
	//Map for music track goes here
};

class GameState : public State // Abstract Base Class
{
public:
	GameState();
	virtual void Enter(); // Pure Virtual method
	virtual void Update();
	virtual void Render();
	virtual void Exit();
	virtual void Resume();
private:
	//Map for music track goes here
	//Map for sound effects goes here
};

class PauseState : public State // Abstract Base Class
{
public:
	virtual void Enter(); // Pure Virtual method
	virtual void Update();
	virtual void Render();
	virtual void Exit();
private:
};

class EndState : public State // Abstract Base Class
{
public:
	virtual void Enter(); // Pure Virtual method
	virtual void Update();
	virtual void Render();
	virtual void Exit();
private: 
};

#endif // States //
