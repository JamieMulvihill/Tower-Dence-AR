#include "WinState.h"
#include "ar_app.h"
#include "MenuState.h"
#include "SplashScreenState.h"
#include <system/platform.h>
#include <graphics/sprite_renderer.h>
#include <graphics/texture.h>
#include <graphics/mesh.h>
#include <graphics/primitive.h>
#include <assets/png_loader.h>
#include <graphics/image_data.h>
#include <graphics/font.h>
#include <graphics/Scene.h>
#include <maths/vector2.h>
#include <input/input_manager.h>
#include <input/sony_controller_input_manager.h>
#include <input/keyboard.h>
#include <maths/math_utils.h>
#include <math.h>
#include <graphics/renderer_3d.h>
#include "ar_app.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <sstream>
#include <iostream>
#include <cstdlib>
#include <cmath>


WinState::WinState(gef::Platform& platform) :
	State(platform),
	font_(NULL),
	score(0),
	leveledUP(false),
	finalScore(0),
	oneStar(1),
	twoStar(39),
	threeStar(79),
	currentLevel(0)

{
}

void WinState::Init(gef::InputManager* input_manager, gef::AudioManager* audio_manager)
{
	winTex = CreateTextureFromPNG("Textures/WinScreen.png", platform_);

	starLevel = 0;

	/// storing the players final score
	finalScore = ARApp::getScore();

	audioManager = audio_manager;
	audioManager->LoadMusic("GameSFX/WinMusic.wav", platform_);
	audioManager->PlayMusic();

	scoreUp_sfx_id = audioManager->LoadSample("GameSFX/ScoreIncrease.wav", platform_);
	audioManager->SetSampleVoiceVolumeInfo(scoreUp_sfx_id, volInfo);

	starLevel_sfx_id = audioManager->LoadSample("GameSFX/LevelUpSound.wav", platform_);
	audioManager->SetSampleVoiceVolumeInfo(starLevel_sfx_id, volInfo);

	/// create animations
	CreateSpriteAnimation(starsArray, 4, "Textures/StarAnim/star", gef::Vector4(480, 180, -0.99f), 250, 480, currentStar, platform_);
}

void WinState::Release(gef::AudioManager* audio_manager)
{
	font_ = NULL;
	delete font_;

	delete winTex;
	winTex = NULL;

	audio_manager->StopMusic();

	audio_manager->UnloadAllSamples();

	audio_manager->UnloadMusic();

}
void WinState::Update(float frame_time, gef::InputManager * inputManager, State ** nextState)
{
	fps_ = 1.0f / frame_time;
	/// lerp values of the current screo the final score
	score = Lerp(score, finalScore + 1, 0.01);
	if (score <= finalScore) {
		LevelUp();
		audioManager->PlaySample(scoreUp_sfx_id);
	}

	/// update the starLevel 
	starLevel = CalculateStars();

	HandleInput(inputManager);
}

void WinState::Render(gef::SpriteRenderer * spriteRenderer, float width, float height, gef::Font * font)
{
	font_ = font;

	spriteRenderer->Begin();

	gef::Sprite background;
	background.set_texture(winTex);
	background.set_position(gef::Vector4(width*0.50f, height*0.5f, -0.99f));
	background.set_height(height);
	background.set_width(960);
	spriteRenderer->DrawSprite(background);

	///render the sprite in the array at the index equal to StarLevel
	spriteRenderer->DrawSprite(starsArray[starLevel]);

	
	DrawHUD(spriteRenderer, font_, width, height);
	spriteRenderer->End();
}

float WinState::Lerp(float start, float end, float time)
{
	float lerpValue = start + (end - start) * time;

	return lerpValue;
}
void WinState::HandleInput(gef::InputManager* input_manager_)
{
	if (input_manager_) {
		input_manager_->Update();
		// controller input
		gef::SonyControllerInputManager* controller_manager = input_manager_->controller_input();
		if (controller_manager) {
			const gef::SonyController* controller = controller_manager->GetController(0);
			if (controller) {
				if (controller->buttons_pressed() & gef_SONY_CTRL_CROSS) {
					/// switch to control what happens when the cross button is pressed
					/// button input sets the state based on what has been selected
					ARApp::SetState(new SplashScreenState(platform_));
				}
			}
		}
	}
}
void WinState::DrawHUD(gef::SpriteRenderer * spriteRenderer, gef::Font * font, float width, float height)
{
	if (font_)
	{
		font_->RenderText(
			spriteRenderer,
			gef::Vector4(480, 285, -0.99f),
			2.f,
			0xffffffff,
			gef::TJ_CENTRE,
			"%i", (int)score);

		font_->RenderText(spriteRenderer, gef::Vector4(850.0f, 510.0f, -0.99f), 1.0f, 0xffffffff, gef::TJ_LEFT, "FPS: %.1f", fps_);
	}
}


/// function to clalculate how mant stars player has got by checking score
int WinState::CalculateStars()
{
	int stars = 0;

	if (score < oneStar) {
		stars = 0;
		return stars;
	}
	else if (score > oneStar && score < twoStar) {
		stars = 1;
		return stars;
	}
	else if (score > twoStar && score < threeStar) {
		stars = 2;
		return stars;
	}
	else if (score > threeStar) {
		stars = 3;
		return stars;
	}
	else {
		return 0;
	}
}

/// function to play a specif sound if the starLevel changes 
void WinState::LevelUp() {
	///check if the int currentLevel is less than StarLevel,
	///play sounds and set currentLevel to be the starlevel
	if (currentLevel < starLevel) {
		audioManager->StopPlayingSampleVoice(scoreUp_sfx_id);
		audioManager->PlaySample(starLevel_sfx_id);
		currentLevel = starLevel;
	}

}

/// function to create animantion of sprites, uses the size int parameter to loop though and populate an array of sprites, converts the int i value from an int to string, 
/// adds this string to the name char* parameter, adds ".png" to name string, intializes a texture with name string, assigns texture to sprite in array at i index
void WinState::CreateSpriteAnimation(gef::Sprite * array, int size, const char * name, gef::Vector4 position, int height, int width, gef::Texture * texture, gef::Platform& platform_) {

	for (int i = 0; i < size; i++) {
		array[i].set_position(position);
		array[i].set_width(width);
		array[i].set_height(height);

		int value = i;
		std::string s;
		std::stringstream out;
		out << value;
		s = out.str();

		std::string num = s;
		std::string temp = name;
		std::string extendeer = ".png";
		temp += num;
		temp += extendeer;
		texture = CreateTextureFromPNG(temp.c_str(), platform_);
		array[i].set_texture(texture);
	}
}
