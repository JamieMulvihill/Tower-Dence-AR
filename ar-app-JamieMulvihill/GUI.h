#include "load_texture.h"
#include <graphics/font.h>
#include <graphics/sprite.h>
#include <system/application.h>
#include <maths/vector2.h>
#include <vector>
#include <graphics/sprite_renderer.h>

#ifndef NUMBUTTONS
#define NUMBUTTONS 4
#endif // !NUMBUTTONS


struct Button
{
	gef::Sprite buttonSprite;
	gef::Texture* buttonTex;
	gef::Texture* pressedTex;
	gef::Vector4 position;
	bool isPressed = false;
};

class GUI
{
public:
	///functions to initialize, render, update and delete 
	void Init(gef::Platform& platform, int numPlayers);
	void Render(gef::SpriteRenderer* spriteRenderer, float width, float height, gef::Font* font, int score, bool canRetical);
	void Update(float frame_time);
	void Release();

	int ButtonInteraction(gef::Vector2 touchPosition, bool buildAvailble);

	void SetHealth(int healthLevel) { health = healthLevel; }


	/// function for lerping values
	float Lerp(float start, float end, float time);
	/// vector of Sprite* 
	std::vector<gef::Sprite*> sprites;
	
	bool GetisOpen() { return isOpen; }
	void SetisOpen(bool open) { isOpen = open; }

	bool GetfullyOpen() { return fullyOpen; }
	void SetfullyOpen(bool open) { fullyOpen = open; }

	bool GetUpgradeSelection() { return upgradeSelected; }
	void SetUpgradeSelection(bool upgrade) { upgradeSelected = upgrade; }

	bool GetCanBuild() { return canBuild; }
	void SetCanBuild(bool build) { canBuild = build; }
	
	void SetHittingEnemy(bool hit) { hittingEnemy = hit; }
private:
	
	gef::Sprite bar;
	gef::Sprite barBG;
	gef::Texture* guiBar;
	gef::Texture* guiBarBG;

	gef::Sprite popOut;
	gef::Texture* popOutTex;

	gef::Sprite retical;
	gef::Texture* reticalTex;
	gef::Texture* reticalTexRed;

	gef::Sprite playBtn;
	gef::Texture* playBtnTex;

	gef::Sprite archerBtn;
	gef::Texture* archerBtnTex;
	gef::Texture* archerBtnPressed;

	gef::Sprite cannonBtn;
	gef::Texture* cannonBtnTex;
	gef::Texture* cannonBtnPressed;

	gef::Sprite fireRateBtn;
	gef::Texture* fireRateBtnTex;
	gef::Texture* fireRateBtnPressed;

	gef::Sprite rangeBtn;
	gef::Texture* rangeBtnTex;
	gef::Texture* rangeBtnPressed;

	gef::Sprite bannerUpgrade;
	gef::Sprite bannerBuild;
	gef::Texture* bannerTex;

	bool isOpen, fullyOpen, upgradeSelected, hittingEnemy, canBuild;
	int numPlayers, offset, random;
	float barwidth, barPos, damage, health, closedWidth, popOutWidth;

	gef::Vector4 positions[4];
	Button buttons[NUMBUTTONS];
};

