#pragma once
#include <graphics/sprite.h>
#include "load_texture.h"
#include <maths/vector2.h>
#include <maths/vector4.h>
#include <maths/matrix44.h>
#include <vector>
#include "State.h"

// FRAMEWORK FORWARD DECLARATIONS
namespace gef
{
	class Platform;
	class SpriteRenderer;
	class Font;
	class Renderer3D;
	class Mesh;
	class InputManager;
	class Scene;
}

class MenuState : public State
{
public:
	/// enum for selected button
	enum BUTTONS { EXIT, PLAY, HELP };
	/// enum variable of buttons
	BUTTONS button;

	MenuState(gef::Platform& p);

	void Init(gef::InputManager* input_manager, gef::AudioManager* audio_manager);
	void Release(gef::AudioManager* audio_manager);
	void Update(float frame_time, gef::InputManager * inputManager, State ** nextState);
	void Render(gef::SpriteRenderer * spriteRenderer, float width, float height, gef::Font * font);
private:
	void HandleInput(gef::InputManager* input_manager_);
	void ButtonSelect();                                  /// function to change the color of the selected button, uses a switch statement to check current button slected
	void ButtonColorChange(gef::Sprite * sprite);		 /// function to change the color of the selected button, increases the alpha value of sprite color
	float Lerp(float start, float end, float time);
	void DrawHUD(gef::SpriteRenderer * spriteRenderer, gef::Font * font, float width, float height);


	/// variables for sound effects
	Int32 buttonMove_sfx_id, buttonSelect_sfx_id;

	/// variables for colors
	UInt32 colorChange, defaultColor;

	bool inMenu, playSelected;
	float valueLeft, valueTop, valueRight, scale;

	float fps_;

	/// texture* variables
	gef::Texture* splashScreen;
	gef::Texture* playButton;
	gef::Texture* helpButton;
	gef::Texture* exitButton;

	gef::Sprite helpBtn, playBtn, exitBtn;

};


