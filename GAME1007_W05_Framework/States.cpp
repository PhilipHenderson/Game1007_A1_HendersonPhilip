#include "States.h"
#include "StateManager.h"
// GameObject includes.
#include "AsteroidField.h"
#include "BulletPool.h"
#include "Buttons.h"
#include "Primitives.h"
#include "Ship.h"
//Remaning managers
#include "CollisionManager.h"
#include "EventManager.h"
#include "RenderManager.h"
#include "SoundManager.h"
#include "TextureManager.h"
#include "Engine.h"

#include <iostream>
using namespace std;

// STATES //
void State::Update()
{
	for (auto const& i : m_objects)
	{
		i.second->Update();
		if (STMA::StateChanging()) return;
	}
}
void State::Render()
{
	for (auto const& i : m_objects)
		i.second->Render();
	if (dynamic_cast<GameState*>(this) && dynamic_cast<PauseState*>(STMA::GetStates().back()))
		return; // If GameState is rendering but PauseState is the current state, return.
	SDL_RenderPresent(REMA::GetRenderer());
}
void State::Exit()
{
	for (auto& i : m_objects)
	{
		delete i.second;
		i.second = nullptr;
	}
	m_objects.clear();
	m_objects.shrink_to_fit();
}
void State::AddChild(std::string key, GameObject* obj)
{
	m_objects.push_back(pair<string, GameObject*>(key, obj));
}
GameObject* State::GetChild(const std::string& key)
{
	auto it = std::find_if(m_objects.begin(), m_objects.end(),
		[&](const std::pair<std::string, GameObject*>& element)
		{
			return element.first == key;
		});
	if (it != m_objects.end())
		return it->second;
	else return nullptr;
}
void State::RemoveChild(const std::string& key)
{
	auto it = std::find_if(m_objects.begin(), m_objects.end(),
		[&](const std::pair<std::string, GameObject*>& element)
		{
			return element.first == key;
		});
	if (it != m_objects.end())
	{
		delete it->second; // Deallocate GameObject.
		it->second = nullptr; // Wrangle dangle.
		m_objects.erase(it);
		m_objects.shrink_to_fit();
	}
}
// STATES END //


// TITLE STATE //
TitleState::TitleState(){}
void TitleState::Enter()
{
	cout << "Entering TitleState!" << endl;
	TEMA::Load("../Assets/img/TitleBack.png", "TitleBackgroundImage");
	TEMA::Load("../Assets/img/NameTitle.png", "NameImage");
	TEMA::Load("../Assets/img/PlayButton.png", "PlayButton");

	SOMA::Load("../Assets/aud/guile.mp3", "title", SOUND_MUSIC);
	SOMA::AllocateChannels(16);
	SOMA::SetMusicVolume(32);
	SOMA::PlayMusic("title", -1, 2000);

	AddChild("TitleBackgroundImage", new Image({ 0, 0, 1920, 1200 }, { 0.0f, 0.0f, 1024.0f, 768.0f }, "TitleBackgroundImage"));
	AddChild("NameImage", new Image({ 0, 0, 800, 156 }, { 112.0f, 600.0f, 800.0f, 156.0f }, "NameImage"));
	AddChild("PlayButton", new PlayButton({ 0, 0, 400, 100 }, { 412.0f, 550.0f, 200.0f, 50.0f }, "PlayButton"));
}
void TitleState::Update()
{
	State::Update();
}
void TitleState::Render()
{
	SDL_SetRenderDrawColor(REMA::GetRenderer(), 0, 0, 255, 255); 
	SDL_RenderClear(REMA::GetRenderer());
	State::Render(); // this invokes SDL_RenderPresent;
}
void TitleState::Exit()
{
	cout << "Exiting TitleState!" << endl;
	TEMA::Unload("TitleBackgroundImage");
	TEMA::Unload("NameImage");
	TEMA::Unload("PlayButton");
	SOMA::StopMusic();
	SOMA::Unload("title", SOUND_MUSIC);
	State::Exit();
}
// TITLE STATE END //


// GAME STATE //
GameState::GameState(){}
void GameState::Enter()
{
	cout << "Entering GameState..." << endl;
	TEMA::Load("../Assets/img/GameBackground.png", "bg");
	TEMA::Load("../Assets/img/Sprites.png", "sprites");
	SOMA::Load("../Assets/aud/Engines.wav", "engines", SOUND_SFX);
	SOMA::Load("../Assets/aud/Fire.wav", "fire", SOUND_SFX);
	SOMA::Load("../Assets/aud/Explode.wav", "explode", SOUND_SFX);
	SOMA::Load("../Assets/aud/Teleport.wav", "teleport", SOUND_SFX);
	SOMA::Load("../Assets/aud/guile.mp3", "wings", SOUND_MUSIC);
	SOMA::SetSoundVolume(16);
	SOMA::SetMusicVolume(32);
	SOMA::PlayMusic("wings", -1, 2000);

	AddChild("bg", new Image({ 0, 0, 1024, 768 }, { 0, 0, 1024, 768 }, "bg"));
	AddChild("bullets", new BulletPool());
	AddChild("ship", new Ship({ 0, 0, 100, 100 }, { 462.0f, 334.0f, 100.0f, 100.0f }));
	AddChild("field", new AsteroidField(8));
}
void GameState::Update()
{
	if (EVMA::KeyPressed(SDL_SCANCODE_X))
	{
		STMA::ChangeState(new TitleState()); // Change to new TitleState
		return;
	}
	else if (EVMA::KeyPressed(SDL_SCANCODE_P))
	{
		STMA::PushState(new PauseState()); // Add new PauseState
		SOMA::PauseMusic();
		return;
	}
	// Check collision.
	vector<Asteroid*>& field = static_cast<AsteroidField*>(GetChild("field"))->GetAsteroids();
	Ship* ship = static_cast<Ship*>(GetChild("ship"));
	// Collision of ship and asteroids.
	if (ship != nullptr)
	{
		for (unsigned int i = 0; i < field.size(); i++)
		{
			Asteroid* ast = field.at(i);
			if (COMA::CircleCircleCheck(ship->GetCenter(), ast->GetCenter(),
				ship->GetRadius(), ast->GetRadius()))
			{
				SOMA::PlaySound("explode");
				RemoveChild("ship");
			}
		}
	}

	// Collision of ship and asteroids.

	// Collision of bullets and asteroids.
	vector<Bullet*>& bullets = static_cast<BulletPool*>(GetChild("bullets"))->GetBullets();
	for (unsigned int i = 0; i < bullets.size(); i++)
	{
		Bullet* bul = bullets.at(i);
		for (unsigned int j = 0; j < field.size(); j++)
		{
			Asteroid* ast = field.at(j);
			if (COMA::CircleCircleCheck(bul->GetCenter(), ast->GetCenter(),
				bul->GetRadius(), ast->GetRadius()))
			{
				// Spawn new asteroid chunks.
				// Hints: If the asteroid size is > 0, spawn a new left and right chunks
				//        You can get the angle of the bullet

				// End new asteroid block.
				SOMA::PlaySound("explode");
				bul->SetEnabled(false);
				ast->SetEnabled(false);
				goto end;
			}
		}
	}
end:
	// End collision checks.
	State::Update();
}
void GameState::Render()
{
	if (dynamic_cast<GameState*>(STMA::GetStates().back())) // only render if current state is GameState
	{
		SDL_SetRenderDrawColor(REMA::GetRenderer(), 100, 255, 0, 255);
		SDL_RenderClear(REMA::GetRenderer());
		State::Render();
	}
}

void GameState::Exit()
{
	cout << "Exiting GameState..." << endl;
	TEMA::Unload("bg");
	TEMA::Unload("sprites");
	SOMA::StopSound();
	SOMA::StopMusic();
	SOMA::Unload("engines", SOUND_SFX);
	SOMA::Unload("fire", SOUND_SFX);
	SOMA::Unload("explode", SOUND_SFX);
	SOMA::Unload("teleport", SOUND_SFX);
	SOMA::Unload("wings", SOUND_MUSIC);
	State::Exit();
}
void GameState::Resume()
{
	cout << "Resuming GameState" << endl;
	// resume playing music track
}
// GAME STATE END //

// PAUSE STATE //
void PauseState::Enter()
{
	cout << "Entering PauseState!" << endl;
	TEMA::Load("../Assets/img/ResumeButton.png", "ResumeButton");
	AddChild("ResumeButton", new ResumeButton({ 0, 0, 400, 100 }, { 412.0f, 550.0f, 200.0f, 50.0f }, "ResumeButton"));
}
void PauseState::Update()
{
	State::Update();
}
void PauseState::Render()
{
	// render the gamestate first.
	if (!STMA::GetStates().empty() && dynamic_cast<GameState*>(STMA::GetStates().back()))
		STMA::GetStates().back()->Render();

	SDL_SetRenderDrawBlendMode(REMA::GetRenderer(), SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(REMA::GetRenderer(), 0, 0, 28, 10);
	SDL_Rect rect = { 256, 128, 512, 512 };
	SDL_RenderFillRect(REMA::GetRenderer(), &rect);
	State::Render();
}
void PauseState::Exit()
{
	cout << "Exiting PauseState!" << endl;
	State::Exit();
	SOMA::ResumeMusic();
	// Start music Again
}
// PAUSE STATE END //

// END STATE //
void EndState::Enter()
{
	cout << "Entering EndState!" << endl;
}
void EndState::Update()
{
	if (EVMA::KeyPressed(SDL_SCANCODE_T))
		STMA::ChangeState(new TitleState);
}
void EndState::Render()
{
	SDL_SetRenderDrawColor(REMA::GetRenderer(), 125, 0, 125, 255);
	SDL_RenderClear(REMA::GetRenderer());

	// any unique rendering in GameState goes here...
	if (dynamic_cast<EndState*>(STMA::GetStates().back()))
		State::Render();
}
void EndState::Exit()
{
	cout << "Exiting EndState!" << endl;
}
// ENDSTATE... END :D //