#include "SplashScreenState.h"
#include "MenuState.h"
#include "ar_app.h"

///intializer list to set values
SplashScreenState::SplashScreenState(gef::Platform& p) :
	State(p),
	valueLeft(0),
	valueTop(0),
	valueRight(960),
	scale(1.f),
	buttonPush_sfx_id(-1),
	isWaiting(true),
	valueBottom(544),
	textfade(0x01000000)
{
	state_ = MENU;
}
SplashScreenState::~SplashScreenState()
{
}
void SplashScreenState::Init(gef::InputManager* input_manager, gef::AudioManager* audio_manager) {

	splashScreen = CreateTextureFromPNG("Textures/Tile.png", platform_);
}

/// function to render the prites and text to screen for the SplashScreen
void SplashScreenState::Render(gef::SpriteRenderer * spriteRenderer, float width, float height, gef::Font * font)
{
	spriteRenderer->Begin();

	gef::Sprite checkersprite;
	checkersprite.set_texture(splashScreen);
	checkersprite.set_height(64);
	checkersprite.set_width(64);

	static int s_frame = 0;

	for (int i = 0; i < 16; i++)
	{
		for (int j = 0; j < 16; j++)
		{
			checkersprite.set_position(i * 64 + (s_frame % 64) - 32, j * 64 + (s_frame % 64) - 32, 0);
			spriteRenderer->DrawSprite(checkersprite);
		}
	}
	s_frame++;

	font->RenderText(
		spriteRenderer,
		gef::Vector4(width*0.5f, 200, -0.99f),
		2.f,
		0xff000000,
		gef::TJ_CENTRE,
		"Tow      Defence");

	font->RenderText(
		spriteRenderer,
		gef::Vector4(width*0.38f, valueTop, -0.99f),
		2.5f,
		0xff000000,
		gef::TJ_CENTRE,
		"AR");

	font->RenderText(
		spriteRenderer,
		gef::Vector4(width*0.5f, valueBottom, -0.99f),
		1.5f,
		textfade,
		gef::TJ_CENTRE,
		"Press any button to continue");

	spriteRenderer->End();
}

void SplashScreenState::Update(float frame_time, gef::InputManager * input_manager, State ** nextState)
{
	HandleInput(input_manager);

	textfade += 0x02000000; /// change the fade value for for the text colour

	if (isWaiting) {
		valueLeft = Lerp(valueLeft, (960 * 0.5f) - 120, 0.05);
		valueTop = Lerp(valueTop, 185, 0.05); /// lerp value to make text fall from top of screen
		valueBottom = Lerp(valueBottom, 350, 0.05); /// lerp value to make text rise from top of screen
		valueRight = Lerp(valueRight, (960 * 0.5f) + 120, 0.05);
		scale = Lerp(scale, 5.4, 0.0015);
	}
}

void SplashScreenState::Release(gef::AudioManager* audio_manager)
{
	delete splashScreen;
	splashScreen = NULL;
}

void SplashScreenState::HandleInput(gef::InputManager* input_manager_)
{
	const gef::SonyController* controller = input_manager_->controller_input()->GetController(0);
	if (input_manager_) {
		input_manager_->Update();
		gef::SonyControllerInputManager* controller_input = input_manager_->controller_input();
		if (controller_input) {
			const gef::SonyController* controller = controller_input->GetController(0);
			if (controller) {
				if (controller->buttons_pressed()) {
					/// set bool to false
					///set state to MenuState
					isWaiting = false;
					ARApp::SetState(new MenuState(platform_));
				}
			}
		}
	}
}
