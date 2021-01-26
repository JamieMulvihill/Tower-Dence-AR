#include "MenuState.h"
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
#include "HelpState.h"
#include "ar_app.h"


MenuState::MenuState(gef::Platform& platform) :
	State(platform),
	valueLeft(0),
	valueTop(0),
	valueRight(960),
	scale(1.f),
	buttonMove_sfx_id(-1),
	buttonSelect_sfx_id(-1),
	inMenu(true),
	playSelected(false),
	colorChange(0x66008800)
{
}

void MenuState::Init(gef::InputManager* input_manager, gef::AudioManager* audio_manager)
{
	splashScreen = CreateTextureFromPNG("Textures/BackGround.png", platform_);
	playButton = CreateTextureFromPNG("Textures/PlayBTN.png", platform_);
	helpButton = CreateTextureFromPNG("Textures/HelpBTN.png", platform_);
	exitButton = CreateTextureFromPNG("Textures/ExitButton.png", platform_);

	audioManager = audio_manager;

	volInfo.volume = 1;

	buttonMove_sfx_id = audioManager->LoadSample("GameSFX/KeySound.wav", platform_);
	audioManager->SetSampleVoiceVolumeInfo(buttonMove_sfx_id, volInfo);
	audioManager->LoadMusic("GameSFX/MenuMusic.wav", platform_);
	audioManager->PlayMusic();
	
	/// store uint32 value of default colour
	defaultColor = playBtn.colour();

	/// initialize seleted button
	button = PLAY;
	
}

void MenuState::Release(gef::AudioManager* audio_manager)
{

	delete splashScreen;
	splashScreen = NULL;

	delete playButton;
	playButton = NULL;

	delete helpButton;
	helpButton = NULL;

	delete exitButton;
	exitButton = NULL;

	audio_manager->StopMusic();

	audio_manager->UnloadAllSamples();

	audio_manager->UnloadMusic();
}
void MenuState::Update(float frame_time, gef::InputManager * inputManager, State ** nextState)
{
	fps_ = 1.0f / frame_time;


	if (inMenu) {
		valueTop = Lerp(valueTop, 80, 0.05); // lerp value to make text fall from top of screen
		ButtonSelect();
	}
	HandleInput(inputManager);
}

void MenuState::Render(gef::SpriteRenderer * spriteRenderer, float width, float height, gef::Font * font)
{
	spriteRenderer->Begin();

	gef::Sprite background;
	background.set_texture(splashScreen);
	background.set_position(gef::Vector4(width*0.50f, height*0.5f, -0.99f));
	background.set_height(height);
	background.set_width(width);
	spriteRenderer->DrawSprite(background);

	font->RenderText(
		spriteRenderer,
		gef::Vector4(width*0.5f, valueTop, -0.99f),
		2.0f,
		0xff000000,
		gef::TJ_CENTRE,
		"Main Menu");

	/// Render button icon
	playBtn.set_texture(playButton);
	playBtn.set_position(gef::Vector4(width*0.5f, height*0.5f, -0.99f));
	playBtn.set_height(180.0f);
	playBtn.set_width(180.0f);
	spriteRenderer->DrawSprite(playBtn);



	font->RenderText(
		spriteRenderer,
		gef::Vector4(width*0.5f, height*0.7f, -0.99f),
		1.0f,
		0x88008800,
		gef::TJ_CENTRE,
		"Play Game");

	/// Render button icon
	exitBtn.set_texture(exitButton);
	exitBtn.set_position(gef::Vector4(width*0.25f, height*0.5f, -0.99f));
	exitBtn.set_height(180.0f);
	exitBtn.set_width(180.0f);
	spriteRenderer->DrawSprite(exitBtn);

	font->RenderText(
		spriteRenderer,
		gef::Vector4(width*0.25f, height*0.7f, -0.99f),
		1.0f,
		0x88008800,
		gef::TJ_CENTRE,
		"Exit");

	helpBtn.set_texture(helpButton);
	helpBtn.set_position(gef::Vector4(width*0.75f, height*0.5f, -0.99f));
	helpBtn.set_height(180.0f);
	helpBtn.set_width(180.0f);
	spriteRenderer->DrawSprite(helpBtn);

	font->RenderText(
		spriteRenderer,
		gef::Vector4(width*0.75f, height*0.7f, -0.99f),
		1.0f,
		0x88008800,
		gef::TJ_CENTRE,
		"How to Play");

	DrawHUD(spriteRenderer, font, width, height);
	spriteRenderer->End();
}

float MenuState::Lerp(float start, float end, float time)
{
	float lerpValue = start + (end - start) * time;

	return lerpValue;
}

void MenuState::DrawHUD(gef::SpriteRenderer * spriteRenderer, gef::Font * font, float width, float height)
{
	if (font)
	{
		font->RenderText(spriteRenderer, gef::Vector4(850.0f, 510.0f, -0.9f), 1.0f, 0xffffffff, gef::TJ_LEFT, "FPS: %.1f", fps_);
	}
}

void MenuState::HandleInput(gef::InputManager* input_manager_)
{
	if (input_manager_) {
		input_manager_->Update();
		gef::SonyControllerInputManager* controller_input = input_manager_->controller_input();
		if (controller_input) {
			const gef::SonyController* controller = controller_input->GetController(0);
			if (controller) {
				if (controller->buttons_pressed() & gef_SONY_CTRL_CROSS) {
					/// switch to control what happens when the cross button is pressed
					/// button input sets the state based on what has been selected
					switch (button) {
					case EXIT:
						exit(0);
						break;
					case PLAY:
						ARApp::SetState(new GameState(platform_));
						break;
					case HELP:
						ARApp::SetState(new HelpState(platform_));
						break;
					}
				}
				/// switch to control what happens when the left button is pressed
				else if (controller->buttons_pressed() & gef_SONY_CTRL_LEFT) {
					audioManager->PlaySample(buttonMove_sfx_id); /// play sound
					switch (button) {
					case EXIT:
						button = HELP;
						exitBtn.set_colour(defaultColor);
						break;
					case PLAY:
						button = EXIT;
						playBtn.set_colour(defaultColor);
						break;
					case HELP:
						button = PLAY;
						helpBtn.set_colour(defaultColor);
						break;
					}
				}

				///switch to control what happens when the right button is pressed
				else if (controller->buttons_pressed() & gef_SONY_CTRL_RIGHT) {
					audioManager->PlaySample(buttonMove_sfx_id); /// play sound

					switch (button) {
					case EXIT:
						button = PLAY;
						exitBtn.set_colour(defaultColor);
						break;
					case PLAY:
						button = HELP;
						playBtn.set_colour(defaultColor);
						break;
					case HELP:
						button = EXIT;
						helpBtn.set_colour(defaultColor);
						break;
					}
				}
			}
		}
	}
}

/// function to change the color of the selected button, increases the alpha value of sprite color
void MenuState::ButtonColorChange(gef::Sprite * sprite)
{
	sprite->set_colour(colorChange);
	colorChange += 0x05000000;
}
/// function to change the color of the selected button, uses a switch statment to check current button slected
void MenuState::ButtonSelect()
{
	switch (button) {
	case EXIT:
		button = EXIT;
		ButtonColorChange(&exitBtn);
		break;
	case PLAY:
		button = PLAY;
		ButtonColorChange(&playBtn);
		break;
	case HELP:
		button = HELP;
		ButtonColorChange(&helpBtn);
		break;
	}
}

