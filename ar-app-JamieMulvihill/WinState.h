#pragma once
#include <graphics/sprite.h>
#include <system/application.h>
#include "load_texture.h"
#include <maths/vector2.h>
#include <maths/vector4.h>
#include <maths/matrix44.h>
#include <vector>
#include "State.h"

class WinState : public State
{
public:
	WinState(gef::Platform& p);

	void Init(gef::InputManager* input_manager, gef::AudioManager* audio_manager);
	void Release(gef::AudioManager* audio_manager);
	void Update(float frame_time, gef::InputManager * inputManager, State ** nextState);
	void Render(gef::SpriteRenderer * spriteRenderer, float width, float height, gef::Font * font);
	void SetScore(int finalScore) { score = finalScore; }
private:
	float Lerp(float start, float end, float time);
	/// function to clalculate how many stars player has got by checking score
	int CalculateStars();
	/// function to create an animation from an arry of sprites
	void CreateSpriteAnimation(gef::Sprite * array, int size, const char * name, gef::Vector4 position, int height, int width, gef::Texture * texture, gef::Platform& platform_);
	/// function to play a specif sound if the starLevel changes 
	void LevelUp();
	void HandleInput(gef::InputManager* input_manager_);
	void DrawHUD(gef::SpriteRenderer * spriteRenderer, gef::Font * font, float width, float height);

	gef::Font* font_;

	float fps_;

	/// declaring variables for creating animations
	gef::Sprite starsArray[4];
	gef::Texture* currentStar;
	gef::Texture* winTex;

	/// ints used for changing the rendered sprite
	int starLevel, increment, oneStar, twoStar, threeStar, currentLevel;
	bool hasIncreased, leveledUP;
	float score, finalScore;
	Int32 scoreUp_sfx_id, starLevel_sfx_id;
};