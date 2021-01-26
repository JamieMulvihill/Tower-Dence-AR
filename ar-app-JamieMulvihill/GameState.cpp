#include "GameState.h"
#include "LoseState.h"
#include "WinState.h"
#include "ar_app.h"
#include <system/platform.h>
#include <graphics/sprite_renderer.h>
#include <graphics/texture.h>
#include <graphics/mesh.h>
#include <graphics/primitive.h>
#include <assets/png_loader.h>
#include <graphics/image_data.h>
#include <graphics/font.h>
#include <input/touch_input_manager.h>
#include <maths/vector2.h>
#include <input/input_manager.h>
#include <maths/math_utils.h>
#include <graphics/renderer_3d.h>
#include <graphics/render_target.h>
#include <gef.h>
#include <math.h>
#include <sony_sample_framework.h>
#include <sony_tracking.h>
#include <system/debug_log.h>
#include <input/sony_controller_input_manager.h>
#include <system/file.h>

GameState::GameState(gef::Platform& platform) :
	State(platform),
	renderer_3d_(NULL),
	primitive_builder_(NULL),
	font_(NULL),
	gameStarted(false),
	roundEnded(false),
	roundStarted(false),
	towerSelected(false),
	buildAvailable(false),
	isShaking(false),
	lostMarker(false),
	health(100),
	roundNum(1),
	startMarkers(3),
	markerIndex(1),
	selectedMarker(-1),
	shakeTime(.2f),
	lostMarkerIndex(-1),
	colorChange(0xff888888)
{
}

void GameState::Init(gef::InputManager* input_manager, gef::AudioManager* audio_manager)
{
	renderer_3d_ = gef::Renderer3D::Create(platform_);
	primitive_builder_ = new PrimitiveBuilder(platform_);
	audioManager = audio_manager;

	SetupLights();

	// initialise sony framework
	sampleInitialize();
	smartInitialize();

	// reset marker tracking
	AppData* dat = sampleUpdateBegin();
	smartTrackingReset();
	sampleUpdateEnd(dat);

	cameraImageAspectRatio_ = 640.f / 480.f;
	displayAspectRatio_ = 960.f / 544.f;
	cameraImageScaleFactor_ = displayAspectRatio_ / cameraImageAspectRatio_;

	cameraFeedSprite_.set_position(0, 0, 1);
	cameraFeedSprite_.set_width(2.f);
	cameraFeedSprite_.set_height(2.0f * cameraImageScaleFactor_);

	InitMesh();

	InitSound(audio_manager);

	LoadWaypointsFromFile("FileReader/waypoints");

	errorTex = CreateTextureFromPNG("Textures/MarkerLostError.png", platform_);


	totalMarkers = startMarkers;

	gef::Matrix44 initialize;
	initialize.SetIdentity();
	for (int i = 0; i < 6; i++) {
		Marker marker;
		markers[i].SetMarkerTransform(initialize);
		markers[i].SetMarkerRelativeto1(initialize);
		markers[i].SetInverseTransform(initialize);
		markers[i].SetSphereCentre(gef::Vector4(0, 0, 0, 0));
		markers[i] = marker;
	}

	/// initilize the GUI
	gui = new GUI(); 
	gui->Init(platform_, 0);
}

void GameState::Release(gef::AudioManager* audio_manager)
{
	delete primitive_builder_;
	primitive_builder_ = NULL;

	smartRelease();
	sampleRelease();

	delete towerToUpgrade;
	towerToUpgrade = NULL;

	delete renderer_3d_;
	renderer_3d_ = NULL;

	audio_manager->StopMusic();

	audio_manager->UnloadAllSamples();

	audio_manager->UnloadMusic();
}

void GameState::HandleInput(gef::InputManager * inputManager)
{
	TouchInput(inputManager, renderer_3d_, 960, 544);

	if (inputManager) {
		inputManager->Update();
		gef::SonyControllerInputManager* controller_input = inputManager->controller_input();
		if (controller_input) {
			const gef::SonyController* controller = controller_input->GetController(0);
			if (controller) {
				if (controller->buttons_pressed() & gef_SONY_CTRL_START) {
				}
				if (controller->buttons_pressed() & gef_SONY_CTRL_SELECT) {
				}
				if (controller->buttons_pressed() & gef_SONY_CTRL_UP) {
				}
				if (controller->buttons_pressed() & gef_SONY_CTRL_DOWN) {
				}
				if (controller->buttons_pressed() & gef_SONY_CTRL_LEFT) {
				}
				if (controller->buttons_pressed() & gef_SONY_CTRL_RIGHT) {
				}
				if (controller->buttons_pressed() & gef_SONY_CTRL_L2) {
				}
				if (controller->buttons_pressed() & gef_SONY_CTRL_R2) {
				}
				if (controller->buttons_pressed() & gef_SONY_CTRL_TRIANGLE) {
					gui->SetisOpen(!gui->GetisOpen()); ///if the player presses triangle, the gui will either oopen or close
				}
				if (controller->buttons_pressed() & gef_SONY_CTRL_CIRCLE) {
					if (!roundEnded)
						TargetEnemy(renderer_3d_); /// if the player presses the "O" button, call target enemy for firing from gate at selected enemy
				}
				if (controller->buttons_pressed() & gef_SONY_CTRL_CROSS) {
				}
				if (controller->buttons_pressed() & gef_SONY_CTRL_SQUARE) {
				}
			}
		}
	}
}

void GameState::InitMesh()
{
	/// load scene assest and set meshes
	ReadSceneAndAssignFirstMesh("FinalMap.scn", &mapScene_, &mapMesh_);
	ReadSceneAndAssignFirstMesh("Tower.scn", &towerScene_, &towerMesh_);
	ReadSceneAndAssignFirstMesh("TarTower.scn", &tarTowerScene_, &tarTowerMesh_);
	ReadSceneAndAssignFirstMesh("Enemy.scn", &enemyScene_, &enemyMesh_);
	ReadSceneAndAssignFirstMesh("Gate.scn", &gateScene_, &gateMesh_);
	ReadSceneAndAssignFirstMesh("Arrow.scn", &arrowScene_, &arrowMesh_);
	ReadSceneAndAssignFirstMesh("Cannon.scn", &cannonScene_, &cannonMesh_);
	ReadSceneAndAssignFirstMesh("CannonBall.scn", &cannonBallScene_, &cannonBallMesh_);

	map.set_mesh(mapMesh_); /// set map mesh

	enemyGoal.set_mesh(gateMesh_); /// enemy target point
	enemyGoal.SetPosition(gef::Vector4(.32, -.3, 0));
	enemyGoal.BuildZRotationMatrix(165);

	gate.Init(gateMesh_, &enemyManager); /// init gate tower

	tower.Init(towerMesh_); /// initilize projectile tower 
	tower.SetMarkerIndex(markerIndex); /// set its marker 
	markerIndex++; /// increase the marker count
	towers.push_back(&tower);  /// add tower to vector

	cannonTower.Init(cannonMesh_); /// initilize cannon tower
	cannonTower.SetMarkerIndex(markerIndex); /// set its marker 
	markerIndex++; /// increase the marker count
	cannons.push_back(&cannonTower);  /// add tower to vector


}

void GameState::InitSound(gef::AudioManager* audio_manager)
{
	audioManager = audio_manager;
	audioManager->LoadMusic("GameSFX/BackLoop.wav", platform_);
	audioManager->PlayMusic();

	roundStart_sfx_id = audioManager->LoadSample("GameSFX/BattleCry.wav", platform_);
	audioManager->SetSampleVoiceVolumeInfo(roundStart_sfx_id, volInfo);

	construction_sfx_id = audioManager->LoadSample("GameSFX/ConstDing.wav", platform_);
	audioManager->SetSampleVoiceVolumeInfo(construction_sfx_id, volInfo);

	cannon_sfx_id = audioManager->LoadSample("GameSFX/ExplodeSound.wav", platform_);
	audioManager->SetSampleVoiceVolumeInfo(cannon_sfx_id, volInfo);

	arrow_sfx_id = audioManager->LoadSample("GameSFX/ArrowSound.wav", platform_);
	audioManager->SetSampleVoiceVolumeInfo(arrow_sfx_id, volInfo);
}

void GameState::UpdateEnemies(float frameTime)
{
	if (gameStarted && !roundEnded) {
		enemyManager.UpdateEnemies(frameTime);
		enemyManager.LaunchEnemies(frameTime);

		for (int i = 0; i < enemyManager.enemies.size(); i++) {
			if (enemyManager.enemies[i]->GetAlive()) {
				enemyManager.enemies[i]->set_transform(enemyManager.enemies[i]->GetlocalTransform() * markers[0].GetMarkerTransform());
				if (enemyManager.enemies[i]->GetreachedEndPoint()) {
					health -= 10;
					isShaking = true;
					gui->SetHealth(health);
					enemyManager.enemies[i]->SetAlive(false);
				}
			}
		}
	}
}

void GameState::UpdateTowers(float frameTime)
{
	/// Update map game object
	map.SetScaleVector(gef::Vector4(.01f, .01f, .01f));
	map.Update(frameTime);
	map.set_transform(map.GetlocalTransform() * markers[0].GetMarkerTransform());

	enemyGoal.Update(frameTime);
	enemyGoal.set_transform(enemyGoal.GetlocalTransform() * markers[0].GetMarkerTransform());

	/// Update Gate tower update
	gate.SetMarkerTransform(markers[0].GetMarkerTransform());
	gate.Update(frameTime);
	gate.set_transform(gate.GetlocalTransform() * markers[0].GetMarkerTransform());
	
	/// Update Enemies before being used in the tower calculations
	UpdateEnemies(frameTime);

	/// loop through the vector of cannons and Update
	for (int i = 0; i < cannons.size(); i++) {
		cannons[i]->SetMarkerTransform(markers[cannons[i]->GetMarkerIndex()].GetMarkerTransform());
		cannons[i]->Update(frameTime);

		if (enemyManager.enemies.size() > 0) {
			/// check the vectors between each enemy and the cannons to get directional vectors
			if (cannons[i]->CheckRanges(enemyManager.enemies.front()->GetlocalTransform(), markers[0] , markers[cannons[i]->GetMarkerIndex()].GetMarkerTransform(), cannonBallMesh_, &enemyManager)) {
				audioManager->PlaySample(cannon_sfx_id);
			}
		}
	}

	/// if the game has started, used the fixed positions to update the projectile towers
	if (roundStarted) {
		for (int i = 0; i < towers.size(); i++) {
			towers[i]->Update(frameTime);
			towers[i]->set_transform(fixedPostions[i].localTransforms * fixedPostions[i].marker.GetMarkerRelativeto1() * markers[0].GetMarkerTransform());
			if (enemyManager.enemies.size() > 0) {
				for (int j = 0; j < enemyManager.enemies.size(); j++) {
					if (enemyManager.enemies[j]->GetAlive()) {
						if (towers[i]->CheckRanges(frameTime, enemyManager.enemies[j]->GetlocalTransform(), markers[0].GetMarkerTransform(), fixedPostions[i].marker.GetinverseTransform(),
							fixedPostions[i].marker.GetMarkerRelativeto1(), arrowMesh_, &enemyManager, enemyManager.enemies[j])) {
							audioManager->PlaySample(arrow_sfx_id);
						}
					}
				}
			}
		}
	}

	/// if round not started update the projectile towers using the marker positions
	else {
		for (int i = 0; i < towers.size(); i++) {
			towers[i]->Update(frameTime);
			towers[i]->set_transform(towers[i]->GetlocalTransform() * markers[towers[i]->GetMarkerIndex()].GetMarkerRelativeto1()* markers[0].GetMarkerTransform());
		}
	}


}

void GameState::Update(float frame_time, gef::InputManager * inputManager, State ** nextState)
{
	fps_ = 1.0f / frame_time;

	AppData* dat = sampleUpdateBegin();

	// use the tracking library to try and find markers
	smartUpdate(dat->currentImage);
	
	// Populating arrays of transforms from markers
	for (int i = 0; i < totalMarkers; i++) {
		if (sampleIsMarkerFound(i)) {
			// marker is being tracked, get its transform
			sampleGetTransform(i, &initTransform);
			markers[i].SetMarkerTransform(initTransform); /// set the current tracked marker transform to a marker in markers array
			initInverse.SetIdentity();
			initInverse.AffineInverse(initTransform); /// get the inverse of the current marker
			markers[i].SetInverseTransform(initInverse); /// set the inverse of the marker in array
			markers[i].SetMarkerRelativeto1(markers[i].GetMarkerTransform() * markers[0].GetinverseTransform()); /// set the marker relative to marker 1
			markers[i].SetSphereCentre(markers[i].GetMarkerTransform().GetTranslation()); /// set the sphere centre position for screen ray checks
			markerSphereCentre[i] = markers[i].GetMarkerTransform().GetTranslation();
			if (lostMarkerIndex == i) { /// track if a lost marker has been found
				lostMarkerIndex = -1;
				lostMarker = false;
			}
		}
		else {
			lostMarkerIndex = i;
			lostMarker = true;
			break;
		}
	}
	
	sampleUpdateEnd(dat);

	if (!lostMarker) { /// check if all the necessary markers are tracked, the game logic can be carried out if they are
		UpdateTowers(frame_time);

		HandleInput(inputManager);

		gui->Update(frame_time);

		Shake(frame_time);

		LoseCheck();

		RoundOver();

		WinCheck();
	}
}

void GameState::Render(gef::SpriteRenderer * spriteRenderer, float width, float height, gef::Font * font)
{
	font_ = font;

	AppData* dat = sampleRenderBegin();

	gef::Matrix44 OrthographicProjectionMatrix;
	OrthographicProjectionMatrix.SetIdentity();
	OrthographicProjectionMatrix = platform_.OrthographicFrustum(-1, 1, -1, 1, -1, 1);

	// Render the camera feed
	if (dat->currentImage) {
		cameraFeedTexture_.set_texture(dat->currentImage->tex_yuv);
		spriteRenderer->set_projection_matrix(OrthographicProjectionMatrix);
		cameraFeedSprite_.set_texture(&cameraFeedTexture_);
		spriteRenderer->Begin(true);
		// DRAW CAMERA FEED SPRITE HERE
		spriteRenderer->DrawSprite(cameraFeedSprite_);
		spriteRenderer->End();
	}

	//// REMEMBER AND SET THE PROJECTION MATRIX HERE
	gef::Matrix44 ProjectionMatrix;
	ProjectionMatrix.SetIdentity();
	ProjectionMatrix = platform_.PerspectiveProjectionFov(SCE_SMART_IMAGE_FOV, (float)SCE_SMART_IMAGE_WIDTH / (float)SCE_SMART_IMAGE_HEIGHT, .1f, 10);

	gef::Matrix44 scaleMatrix_;
	scaleMatrix_.SetIdentity();
	scaleMatrix_.Scale(gef::Vector4(1, cameraImageScaleFactor_, 1, 1));

	gef::Matrix44 finalProjectionMatrix;
	finalProjectionMatrix.SetIdentity();
	finalProjectionMatrix = ProjectionMatrix * scaleMatrix_;

	gef::Matrix44 ViewMatrix_;
	ViewMatrix_.SetIdentity();

	// SET VIEW AND PROJECTION MATRIX HERE
	renderer_3d_->set_projection_matrix(finalProjectionMatrix);

	renderer_3d_->set_view_matrix(ViewMatrix_);

	// Begin rendering 3D meshes, don't clear the frame buffer
	renderer_3d_->Begin(false);

	enemyManager.Render(renderer_3d_);

	/// render map and enemy target gate mesh
	renderer_3d_->DrawMesh(map);
	renderer_3d_->DrawMesh(enemyGoal);

	/// loop through all the connons in cannons vector and render all their projectils
	for (int i = 0; i < cannons.size(); i++) {
		renderer_3d_->DrawMesh(*cannons[i]);
		cannons[i]->GetProjectileManager()->Render(renderer_3d_);
	}
	
	/// loop for rendering all the towers in towers vctor, if the tower is seleccted, set to red
	for (int i = 0; i < towers.size(); i++) {

		if (towers[i]->isSelected) {
			renderer_3d_->set_override_material(&primitive_builder_->red_material());
			renderer_3d_->DrawMesh(*towers[i]);
		}
		renderer_3d_->set_override_material(NULL);
		if (!towers[i]->isSelected) {
			
			renderer_3d_->DrawMesh(*towers[i]);
		}

		
		ProjectileTower* temp = (ProjectileTower*)towers[i];
		temp->projectileManager.Render(renderer_3d_);
	}

	///Render gate mesh and gates projectiles
	renderer_3d_->DrawMesh(gate);
	gate.GetProjectileManager()->Render(renderer_3d_);


	renderer_3d_->set_override_material(NULL);

	renderer_3d_->End();

	RenderOverlay(spriteRenderer);

	sampleRenderEnd();
}

void GameState::RenderOverlay(gef::SpriteRenderer * spriteRenderer)
{
	gef::Matrix44 proj_matrix2d;
	proj_matrix2d = platform_.OrthographicFrustum(0.0f, platform_.width(), 0.0f, platform_.height(), -1.0f, 1.0f);
	spriteRenderer->set_projection_matrix(proj_matrix2d);
	spriteRenderer->Begin(false);
	

	DrawHUD(spriteRenderer);

	if (lostMarker) {
		errorSpite.set_texture(errorTex);
		errorSpite.set_position(gef::Vector4(480, 277, 0.f));
		errorSpite.set_height(560);
		errorSpite.set_width(960);
		spriteRenderer->DrawSprite(errorSpite);

	}
	else if(!lostMarker) {
		gui->Render(spriteRenderer, 960, 544, font_, 0, gate.GetCanFire());
	}

	spriteRenderer->End();
}

void GameState::DrawHUD(gef::SpriteRenderer * spriteRenderer)
{
	if (font_)
	{
		font_->RenderText(spriteRenderer, gef::Vector4(650.0f, 0.0f, 1.f), 1.0f, 0xffffffff, gef::TJ_LEFT, "Round: %i/3", roundNum);
		font_->RenderText(spriteRenderer, gef::Vector4(850.0f, 510.0f, 1.f), 1.0f, 0xffffffff, gef::TJ_LEFT, "FPS: %.1f", fps_);

		if (lostMarker) {
			font_->RenderText(spriteRenderer, gef::Vector4(320.0f, 400.f, 1.f), 1.5f, colorChange, gef::TJ_LEFT, "Marker %i", lostMarkerIndex + 1);
		}
		
	}
}

void GameState::SetupLights()
{
	gef::PointLight default_point_light;
	default_point_light.set_colour(gef::Colour(0.7f, 0.7f, 1.0f, 1.0f));
	default_point_light.set_position(gef::Vector4(-300.0f, -500.0f, 100.0f));

	gef::Default3DShaderData& default_shader_data = renderer_3d_->default_shader_data();
	default_shader_data.set_ambient_light_colour(gef::Colour(0.5f, 0.5f, 0.5f, 1.0f));
	default_shader_data.AddPointLight(default_point_light);
}

gef::Mesh* GameState::GetFirstMesh(gef::Scene* scene)
{
	gef::Mesh* mesh = NULL;

	if (scene)
	{
		// now check to see if there is any mesh data in the file, if so lets create a mesh from it
		if (scene->mesh_data.size() > 0)
			mesh = scene->CreateMesh(platform_, scene->mesh_data.front());
	}

	return mesh;
}

void GameState::ReadSceneAndAssignFirstMesh(const char* filename, gef::Scene** scene, gef::Mesh** mesh) {

	gef::Scene* scn = new gef::Scene;
	scn->ReadSceneFromFile(platform_, filename);
	// we do want to render the data stored in the scene file so lets create the materials from the material data present in the scene file
	scn->CreateMaterials(platform_);

	*mesh = GetFirstMesh(scn);
	*scene = scn;
}

///functions to populate vector using FileReader function, creates a vector of floats, converts the char* returned by FileReader function to string
/// loops through the string and converts to float, converts floats to gef::Vector 4
char* GameState::FileReader(const char* filename) {

	std::vector<float> values;
	std::string config_filename(filename);
	config_filename += ".txt";
	void* file_data = NULL;
	Int32 file_size = 0;
	gef::File* file = gef::File::Create();

	bool success = true;
	success = file->Open(config_filename.c_str());
	if (success)
	{
		success = file->GetSize(file_size);
		if (success)
		{
			file_data = malloc(file_size);
			success = file_data != NULL;
			if (success)
			{
				Int32 bytes_read;
				success = file->Read(file_data, file_size, bytes_read);
				if (success) {
					success = bytes_read == file_size;
				}
			}
		}

		char* point = (char*)file_data;

		file->Close();
		delete file;
		file = NULL;

		return point;
	}

	return nullptr;
}
void GameState::LoadWaypointsFromFile(const char* filename)
{
	char* input = FileReader(filename);

	std::vector<float> positionsValues;
	std::string string = input;
	std::string output;

	for (int i = 0; i < string.size(); i++)
	{
		char currentChar = string[i];


		if (currentChar >= 45 && currentChar <= 57)
		{
			output += currentChar;
		}

		if (currentChar == ',')
		{

			char LE = 13;
			char CR = 10;

			output += LE;
			output += CR;

			float number;
			number = atof(output.c_str());
			positionsValues.push_back(number);
			output.clear();
		}

	}

	for (int i = 0; i < positionsValues.size(); i += 3)
	{
		waypointPositions.push_back(gef::Vector4(positionsValues[i], positionsValues[i + 1], positionsValues[i + 2]));
	}

	delete input;
	input = NULL;
}

bool GameState::TouchInput(gef::InputManager * inputManager, gef::Renderer3D * renderer, float width, float height)
{
	const gef::TouchInputManager * touch_manager = inputManager->touch_manager();
	/// get the active touches for this panel
	const gef::TouchContainer& panel_touches = touch_manager->touches(0);

	inputManager->touch_manager()->EnablePanel(0);
	if (panel_touches.size() > 0)
	{
		/// just grabbing the first touch for simplicity here
		/// normally we go through all active touches and check the id
		gef::Touch touch = panel_touches.front();

		/// only process this touch if it is NEW or ACTIVE
		if ((touch.type == gef::TT_NEW)) {
			/// convert the touch position to a ray that starts on the camera near plane
			/// and shoots into the camera view frustum
			/// check for the tap on the play button
			gef::Vector2 screen_position = touch.position;
			if (gui->GetisOpen()){
				gef::Vector2 spritePos = gef::Vector2(100, 410);
				if ((screen_position - spritePos).Length() < 20) {
					if (!gameStarted || roundEnded) {
						RoundStart();
					}
					gameStarted = true;
					roundEnded = false;

				}
			}
			
			/// checks the GUI button pushes on screen

			if (gui->GetCanBuild()) {  /// check if there is a build/upgrade available
				UpgradeTower(screen_position, renderer); /// Call function to set a tower for upgrade
				if (towerSelected) { /// chack if there is a sletec tower
					gui->ButtonInteraction(screen_position, buildAvailable);
					if (gui->GetUpgradeSelection()) { /// check that an upgrade is selected
						towerToUpgrade->UpgradeTower(gui->ButtonInteraction(screen_position, buildAvailable)); /// upgrade tower
						audioManager->PlaySample(construction_sfx_id); 
						towerSelected = false; /// reset variables to false
						towerToUpgrade->isSelected = false;
						towerToUpgrade = nullptr;
						buildAvailable = false;
						gui->SetUpgradeSelection(false);
						gui->SetCanBuild(false);
					}
				}

				BuildTower(screen_position, renderer); /// call function to build a tower on a selected marker
				if (selectedMarker > -1) { /// check if a marker has been selected
					gui->ButtonInteraction(screen_position, buildAvailable); 
					if (gui->GetUpgradeSelection()) { /// check that tower is selected to build
						SpawnTower(markers[selectedMarker].GetMarkerTransform(), markers[selectedMarker].GetMarkerRelativeto1(), markers[selectedMarker].GetinverseTransform(),
							gui->ButtonInteraction(screen_position, buildAvailable)); /// spawn toawer
						audioManager->PlaySample(construction_sfx_id);
						selectedMarker = -1; /// reset variables
						buildAvailable = false;
						gui->SetUpgradeSelection(false);
						gui->SetCanBuild(false);
					}
				}
			}
		}
	}
	return false;
}

void GameState::SpawnTower(gef::Matrix44 markerTransform, gef::Matrix44 relativeTransform, gef::Matrix44 inverseTransform, int towerChoice){

	/// switch to control which tower gets spawned
	switch (towerChoice) {
	case 2:
		ProjectileTower * tower = new ProjectileTower; 
		tower->Init(towerMesh_);
		tower->SetMarkerIndex(markerIndex); /// set the marker of the new tower
		towers.push_back(tower); /// add to vector of towers
		markerIndex++;
		break;
	case 3:
		CannonTower* cannon = new CannonTower;
		cannon->Init(cannonMesh_);
		cannon->SetMarkerIndex(markerIndex); /// set the marker of the new tower
		cannons.push_back(cannon);  /// add to vector of towers
		markerIndex++;
		break;
	default:
		break;
	}

	
}

void GameState::GetScreenPosRay(const gef::Vector2& screen_position, const gef::Matrix44& projection, const gef::Matrix44& view, gef::Vector4& startPoint, gef::Vector4& direction, float width, float height) {
	gef::Vector2 ndc;

	float hw = width * 0.5f;
	float hh = height * 0.5f;

	ndc.x = (static_cast<float>(screen_position.x) - hw) / hw;
	ndc.y = (hh - static_cast<float>(screen_position.y)) / hh;

	gef::Matrix44 projectionInverse;
	projectionInverse.Inverse(view * projection);

	gef::Vector4 nearPoint, farPoint;

	nearPoint = gef::Vector4(ndc.x, ndc.y, ndc_z_min_, 1.0f).TransformW(projectionInverse);
	farPoint = gef::Vector4(ndc.x, ndc.y, 1.0f, 1.0f).TransformW(projectionInverse);

	nearPoint /= nearPoint.w();
	farPoint /= farPoint.w();

	startPoint = gef::Vector4(nearPoint.x(), nearPoint.y(), nearPoint.z());
	direction = farPoint - nearPoint;
	direction.Normalise();
}

// modified from https://gamedev.stackexchange.com/questions/96459/fast-ray-sphere-collision-code
bool GameState::RaySphereIntersect(gef::Vector4& startPoint, gef::Vector4& direction, gef::Vector4& sphere_centre, float sphere_radius) {
	gef::Vector4 m = startPoint - sphere_centre;
	float b = m.DotProduct(direction);
	float c = m.LengthSqr() - sphere_radius * sphere_radius;

	// Exit if ray’s origin outside sphere (c > 0) and ray pointing away from sphere (b > 0) 
	if (c > 0.0f && b > 0.0f)
		return false;
	float discr = b * b - c;

	if (std::isnan(discr))
		return false;
		
	// A negative discriminant corresponds to ray missing sphere 
	if (discr < 0.0f)
		return false;

	// Ray now found to intersect sphere, compute smallest t value of intersection
	float t = -b - sqrtf(discr);

	// If t is negative, ray started inside sphere so clamp t to zero 
	if (t < 0.0f)
		t = 0.0f;

	gef::Vector4 hitpoint = startPoint + direction * t;

	return true;
}

void GameState::LoseCheck() {

	if (health <= 0) {
		ARApp::SetState(new LoseState(platform_));
	}
}
void GameState::WinCheck() {
	if (roundNum > 3 && roundEnded && health > 0) {
		ARApp::SetScore(health);
		ARApp::SetState(new WinState(platform_));
	}
}
void GameState::RoundOver() {
	
	if (enemyManager.enemies.size() == 0 && !roundEnded && gameStarted) {
		roundEnded = true;
		roundStarted = false;
		enemyManager.SetTotalEnemies(4); /// increase the number of enemies
		totalMarkers = startMarkers + roundNum; /// add a new marker for the player to build on
		roundNum++;
		fixedPostions.clear();
		gui->SetisOpen(true);
		gui->SetCanBuild(true);
		buildAvailable = true;
	}
}

/// Function for the start behaviours
void GameState::RoundStart() {
	
	gui->SetisOpen(false);
	SetTowerPosition(); /// set the fixed positions for the towers that dont move
	enemyManager.InitEnemies(waypoints[0].GetPosition(), enemyMesh_, waypointPositions, waypointPositions[1]);
	audioManager->PlaySample(roundStart_sfx_id);
	roundStarted = true;
	gui->SetCanBuild(false);
	
	if (towerToUpgrade != NULL) {
		towerToUpgrade = nullptr;
		towerSelected = false;
	}
}

/// fucntion for upgrading towers
void GameState::UpgradeTower(gef::Vector2 screenPos, gef::Renderer3D * renderer) {

	if (roundEnded && roundNum > 0) {
		gef::Vector4 ray_start_position, ray_direction;
		GetScreenPosRay(screenPos, renderer->projection_matrix(), renderer->view_matrix(), ray_start_position, ray_direction, 960, 544);
		/// Create a sphere around the position of the player body
		/// the radius can be changed for larger objects
		float  sphere_radius = .04f;
		
		for (int i = 0; i < totalMarkers; i++) { 
			gef::Vector4 spherePos = markers[i].GetSphereCentre(); 
			/// check to see if the ray intersects with the bound sphere that is around tower marker
			if (RaySphereIntersect(ray_start_position, ray_direction, markerSphereCentre[i], sphere_radius)) {
				int selectedTowerMarker = i;
				for (int j = 0; j < towers.size(); j++) {

					/// check if one of the already built towers is on the selected marker
					if (selectedTowerMarker == towers[j]->GetMarkerIndex()) {
						towerToUpgrade = towers[j];
						towerToUpgrade->isSelected = true;
						towerSelected = true;
					}
				}
			}
		}
	}
}

/// function for building towers
void GameState::BuildTower(gef::Vector2 screenPos, gef::Renderer3D * renderer) {
	if (roundEnded && roundNum > 0) {
		gef::Vector4 ray_start_position, ray_direction;
		GetScreenPosRay(screenPos, renderer->projection_matrix(), renderer->view_matrix(), ray_start_position, ray_direction, 960, 544);
		/// Create a sphere around the position of the player body
		/// the radius can be changed for larger objects
		float  sphere_radius = .04f;
		int markerIndex = 0;
		for (int i = 0; i < totalMarkers; i++) {
			gef::Vector4 spherePos = markers[i].GetSphereCentre();
			/// check to see if the ray intersects with the bound sphere that is around marker
			if (RaySphereIntersect(ray_start_position, ray_direction, markerSphereCentre[i], sphere_radius)) {
				selectedMarker = i; /// set the selected marker
			}
		}
	}
}

/// function for casting the ray fro mthe centre of the screen towards the player
void GameState::TargetEnemy(gef::Renderer3D * renderer) {

	gef::Vector4 ray_start_position, ray_direction, enemyPos;
	gef::Matrix44 enemyWorldSpace;

	gef::Vector2 screenCentre_ = gef::Vector2(480, 277);
	GetScreenPosRay(screenCentre_, renderer->projection_matrix(), renderer->view_matrix(), ray_start_position, ray_direction, 960, 544);
	/// Create a sphere around the position of the enmey body
	float  sphere_radius = .025f;
	for (int i = 0; i < enemyManager.enemies.size(); i++) {
		enemyWorldSpace = enemyManager.enemies[i]->GetlocalTransform() * markers[0].GetMarkerTransform();
		enemyPos = enemyWorldSpace.GetTranslation();
		/// check to see if the ray intersects with the bound sphere that is around an enemy
		if (RaySphereIntersect(ray_start_position, ray_direction, enemyPos, sphere_radius)) {
			gui->SetHittingEnemy(true);
			enemyManager.enemies[i]->SetisTargeted(true); 
			/// call the Gate towers attack function and set taarget to targetted enemy
			gate.Attack(gef::Vector4(0.f, 0.f, 0.f), arrowMesh_, enemyManager.enemies[i]->GetPosition(),enemyManager.enemies[i], 0);

		}
		else {
			gui->SetHittingEnemy(true);
		}
	}
}

/// Funciton to store the tower positions of the non moving towers whilst in the play phase
void GameState::SetTowerPosition() {

	for (int i = 0; i < towers.size(); i++) {
		FixedPosition towerPosition;
		towerPosition.marker = markers[towers[i]->GetMarkerIndex()];
		towerPosition.localTransforms = towers[i]->GetlocalTransform();
		fixedPostions.push_back(towerPosition);
	}
	
}

/// screen shake funciton
void GameState::Shake(float frameTime)
{
	float shakeForceX = .005f;
	float shakeForceY = .1f;

	if (shakeTime > 0.f && isShaking) {
		shakeTime -= frameTime;
		/// set the sprite position of the camera sprite to a random value in the shake range
		cameraFeedSprite_.set_position(gef::Vector4(RandomRange(-shakeForceX, shakeForceX), RandomRange(-shakeForceY, shakeForceY), 1));
	}
	else {
		isShaking = false;
		cameraFeedSprite_.set_position(gef::Vector4(0, 0, 1));
		shakeTime = .2f;
	}
}
float GameState::RandomRange(float min, float max)
{
	float r = (float)rand() / (float)RAND_MAX;
	r = (r * (max - min)) + min;
	return r;
}