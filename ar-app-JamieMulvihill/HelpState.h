#include <graphics/sprite.h>
#include <maths/vector2.h>
#include <maths/vector4.h>
#include <maths/matrix44.h>
#include <vector>
#include "State.h"

class HelpState : public State
{
public:
	HelpState(gef::Platform& p);

	void Init(gef::InputManager* input_manager, gef::AudioManager* audio_manager);
	void Release(gef::AudioManager* audio_manager);
	void Update(float frame_time, gef::InputManager * inputManager, State ** nextState);
	void Render(gef::SpriteRenderer * spriteRenderer, float width, float height, gef::Font * font);
private:
	void DrawHUD(gef::SpriteRenderer * spriteRenderer, gef::Font * font, float width, float height);

	gef::Texture* helpScreen;
	gef::Sprite helpSprite;

	float fps_;
};