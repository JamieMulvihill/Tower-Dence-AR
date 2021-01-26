#include "GUI.h"

void GUI::Init(gef::Platform& platform, int numPlayers)
{

	/// create GUI textures
	popOutTex = CreateTextureFromPNG("Textures/Paper.png", platform);
	reticalTex = CreateTextureFromPNG("RedRetical.png", platform);
	guiBar = CreateTextureFromPNG("Textures/LoadAmount.png", platform);
	guiBarBG = CreateTextureFromPNG("Textures/LoadBar.png", platform);
	playBtnTex = CreateTextureFromPNG("Textures/Play.png", platform);
	bannerTex = CreateTextureFromPNG("Textures/Banner.png", platform);
	cannonBtnTex = CreateTextureFromPNG("Textures/Canon.png", platform);
	cannonBtnPressed = CreateTextureFromPNG("Textures/CannonPressed.png", platform);
	archerBtnTex = CreateTextureFromPNG("Textures/Bow.png", platform);
	archerBtnPressed = CreateTextureFromPNG("Textures/ArcherPressed.png", platform);
	fireRateBtnTex = CreateTextureFromPNG("Textures/Fire.png", platform);
	fireRateBtnPressed = CreateTextureFromPNG("Textures/FirePressed.png", platform);
	rangeBtnTex = CreateTextureFromPNG("Textures/RangeBtn.png", platform);
	rangeBtnPressed = CreateTextureFromPNG("Textures/RangePressed.png", platform);
	reticalTex = CreateTextureFromPNG("Textures/Retical.png", platform);
	reticalTexRed = CreateTextureFromPNG("Textures/RedRetical.png", platform);

	barBG.set_texture(guiBarBG);
	barBG.set_height(42);
	barBG.set_width(355);
	barBG.set_position(gef::Vector4(760, 65, -0.99f));

	damage = 0.f;
	health = 100.f;

	bar.set_texture(guiBar);
	bar.set_height(32.f);
	barwidth = 350;
	bar.set_width(health * 3.5);
	barPos = 780;
	bar.set_position(gef::Vector4((barPos + (bar.width() / 2)), 65, -0.99f));

	isOpen = true;
	fullyOpen = false;
	upgradeSelected = false;
	hittingEnemy = false;
	canBuild = false;

	closedWidth = 5.f;
	popOutWidth = closedWidth;

	/// initialize side screen gui
	popOut.set_texture(popOutTex);
	popOut.set_height(680.f);
	popOut.set_width(closedWidth);
	popOut.set_position(gef::Vector4(popOut.width() / 2, platform.height() / 2, -0.99f));

	playBtn.set_texture(playBtnTex);
	playBtn.set_height(48);
	playBtn.set_width(128);
	playBtn.set_position(gef::Vector4(100, 410, -0.99f));

	bannerUpgrade.set_texture(bannerTex);
	bannerUpgrade.set_height(128);
	bannerUpgrade.set_width(128);
	bannerUpgrade.set_position(gef::Vector4(100, 95, -0.99f));

	bannerBuild.set_texture(bannerTex);
	bannerBuild.set_height(128);
	bannerBuild.set_width(128);
	bannerBuild.set_position(gef::Vector4(100, 250, -0.99f));

	archerBtn.set_texture(archerBtnTex);
	archerBtn.set_height(64);
	archerBtn.set_width(64);
	archerBtn.set_position(gef::Vector4(60, 310, -0.99f));

	cannonBtn.set_texture(cannonBtnTex);
	cannonBtn.set_height(64);
	cannonBtn.set_width(64);
	cannonBtn.set_position(gef::Vector4(140, 310, -0.99f));

	rangeBtn.set_texture(rangeBtnTex);
	rangeBtn.set_height(64);
	rangeBtn.set_width(64);
	rangeBtn.set_position(gef::Vector4(60, 155, -0.99f));

	fireRateBtn.set_texture(fireRateBtnTex);
	fireRateBtn.set_height(64);
	fireRateBtn.set_width(64);
	fireRateBtn.set_position(gef::Vector4(140, 155, -0.99f));

	retical.set_texture(reticalTex);
	retical.set_height(32);
	retical.set_width(32);
	retical.set_position(gef::Vector4(480, 277, -0.99f));

	buttons[0].buttonSprite = rangeBtn;
	buttons[0].buttonTex = rangeBtnTex;
	buttons[0].pressedTex = rangeBtnPressed;
	buttons[0].position = rangeBtn.position();

	buttons[1].buttonSprite = fireRateBtn;
	buttons[1].buttonTex = fireRateBtnTex;
	buttons[1].pressedTex = fireRateBtnPressed;
	buttons[1].position = fireRateBtn.position();

	buttons[2].buttonSprite = archerBtn;
	buttons[2].buttonTex = archerBtnTex;
	buttons[2].pressedTex = archerBtnPressed;
	buttons[2].position = archerBtn.position();

	buttons[3].buttonSprite = cannonBtn;
	buttons[3].buttonTex = cannonBtnTex;
	buttons[3].pressedTex = cannonBtnPressed;
	buttons[3].position = cannonBtn.position();
}

/// function to Render sprites and display score
void GUI::Render(gef::SpriteRenderer* sprite_renderer, float width, float height, gef::Font* font, int score, bool canRetical)
{
	sprite_renderer->DrawSprite(barBG);
	sprite_renderer->DrawSprite(bar);
	sprite_renderer->DrawSprite(popOut);

	if (canRetical) {
		retical.set_texture(reticalTex);
		sprite_renderer->DrawSprite(retical);
	}

	if (fullyOpen) {

		font->RenderText(
			sprite_renderer,
			gef::Vector4(95.0f, 350.0f, -0.99f),
			.6f,
			0xffffffff,
			gef::TJ_CENTRE,
			"Build Available: %i", canBuild);

		font->RenderText(
			sprite_renderer,
			gef::Vector4(97.0f, 200.0f, -0.99f),
			.5f,
			0xffffffff,
			gef::TJ_CENTRE,
			"Upgrade Available: %i", canBuild);

		sprite_renderer->DrawSprite(playBtn);

		font->RenderText(
			sprite_renderer,
			gef::Vector4(100, 400, -0.99f),
			.5f,
			0xffffffff,
			gef::TJ_CENTRE,
			"Play");

		sprite_renderer->DrawSprite(bannerUpgrade);

		font->RenderText(
			sprite_renderer,
			gef::Vector4(100, 80, -0.99f),
			.5f,
			0xffffffff,
			gef::TJ_CENTRE,
			"Upgrade");

		sprite_renderer->DrawSprite(bannerBuild);

		font->RenderText(
			sprite_renderer,
			gef::Vector4(100, 235, -0.99f),
			.5f,
			0xffffffff,
			gef::TJ_CENTRE,
			"Build");

		sprite_renderer->DrawSprite(buttons[0].buttonSprite);
		sprite_renderer->DrawSprite(buttons[1].buttonSprite);
		sprite_renderer->DrawSprite(buttons[2].buttonSprite);
		sprite_renderer->DrawSprite(buttons[3].buttonSprite);
	}

	

	font->RenderText(
		sprite_renderer,
		gef::Vector4(760, 52.5, -0.99f),
		.8f,
		0xffffffff,
		gef::TJ_CENTRE,
		"Health");
}

/// function that returns a float, lerps values from a start value to an end value
float GUI::Lerp(float start, float end, float time)
{
	float lerpValue = start + (end - start) * time;

	return lerpValue;
}

void GUI::Update(float frame_time)
{
	if (health > 0.f) {
		bar.set_width(health * 3.5);
		barPos = 1020;
		bar.set_position(gef::Vector4((barPos - (bar.width() / 2) - (barBG.width() / 4)), 65, -0.99f));

	}

	if (!isOpen) {
		popOut.set_width(closedWidth);
		popOutWidth = closedWidth;
		popOut.set_position(gef::Vector4(popOut.width() / 2, 544 / 2, -0.99f));
		fullyOpen = false;
	}

	if (isOpen && popOut.width() < 250) {
		popOutWidth += 10.f;
		popOut.set_width(popOutWidth);
		popOut.set_position(gef::Vector4(popOut.width() / 2, 544 / 2, -0.99f));
	}

	if (popOutWidth >= 250) {
		fullyOpen = true;
		if (!canBuild) {
			for (int i = 0; i < NUMBUTTONS; i++) {
				buttons[i].buttonSprite.set_texture(buttons[i].pressedTex);
			}
		}
		else {
			for (int i = 0; i < NUMBUTTONS; i++) {
				buttons[i].buttonSprite.set_texture(buttons[i].buttonTex);
			}
		}

	}

	
}

void GUI::Release()
{
}

int GUI::ButtonInteraction(gef::Vector2 touchPosition, bool buildAvailble) {

	if (fullyOpen) {
		gef::Vector4 touch = gef::Vector4(touchPosition.x, touchPosition.y, -.99f);
		for (int i = 0; i < NUMBUTTONS; i++) {
			
			if ((touch - buttons[i].position).Length() < 20) {
				upgradeSelected = true;
				return i;
			}
		}
	}
}

