#include "LoseState.h"
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


LoseState::LoseState(gef::Platform& platform) :
	State(platform)
	
{
}

void LoseState::Init(gef::InputManager* input_manager, gef::AudioManager* audio_manager)
{
	loseTex = CreateTextureFromPNG("Textures/LoseScreen.png", platform_);
	audioManager = audio_manager;
	audioManager->LoadMusic("GameSFX/LoseScreen.wav", platform_);
	audioManager->PlayMusic();
}

void LoseState::Release(gef::AudioManager* audio_manager)
{
	delete loseTex;
	loseTex = NULL;

	audio_manager->StopMusic();
	audio_manager->UnloadMusic();

}
void LoseState::Update(float frame_time, gef::InputManager * inputManager, State ** nextState)
{
	fps_ = 1.0f / frame_time;

	// read input devices
	if (inputManager) {
		inputManager->Update();
		// controller input
		gef::SonyControllerInputManager* controller_manager = inputManager->controller_input();
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

void LoseState::Render(gef::SpriteRenderer * spriteRenderer, float width, float height, gef::Font * font)
{
	spriteRenderer->Begin();

	gef::Sprite background;
	background.set_texture(loseTex);
	background.set_position(gef::Vector4(width*0.50f, height*0.5f, -0.99f));
	background.set_height(height);
	background.set_width(960);
	spriteRenderer->DrawSprite(background);

	DrawHUD(spriteRenderer, font, width, height);
	spriteRenderer->End();
}

void LoseState::DrawHUD(gef::SpriteRenderer * spriteRenderer, gef::Font * font, float width, float height)
{
	if (font)
	{
		// display frame rate
		font->RenderText(spriteRenderer, gef::Vector4(width / 2, height / 2, -0.9f), 1.f, 0xffffffff, gef::TJ_LEFT, "LOSE");

		font->RenderText(spriteRenderer, gef::Vector4(850.0f, 510.0f, -0.9f), 1.0f, 0xffffffff, gef::TJ_LEFT, "FPS: %.1f", fps_);
	}
}
