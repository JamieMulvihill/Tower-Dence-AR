#pragma once

#include <system/application.h>
#include <graphics/sprite.h>
#include <maths/vector2.h>
#include <vector>
#include <graphics/mesh_instance.h>
#include <platform/vita/graphics/texture_vita.h>
#include "GameObject.h"
#include "State.h"
#include "primitive_builder.h"
#include <graphics/Scene.h>
#include "Marker.h"
#include "Enemy.h"
#include "EnemyManager.h"
#include "ProjectileTower.h"
#include "GateTower.h"
#include "CannonTower.h"
#include "Projectile.h"
#include "ProjectileManager.h"
#include "GUI.h"
// Vita AR includes
#include <camera.h>
#include <gxm.h>
#include <motion.h>
#include <libdbg.h>
#include <libsmart.h>


/// Struct used to store the positions of the tower game object whrn round starts so it can be fixed during rounds
struct FixedPosition{ 

	gef::Matrix44 localTransforms;
	Marker marker;
	gef::Matrix44 markerTransforms;
	gef::Matrix44 markerInverseTransforms;
	gef::Matrix44 markerTransformsRelativeto1;
};

// FRAMEWORK FORWARD DECLARATIONS
namespace gef
{
	class Platform;
	class Font;
	class Renderer3D;
	class Mesh;
	class RenderTarget;
	class TextureVita;
	class InputManager;
	class Scene;
}


class GameState : public State
{
public:

	GameState(gef::Platform& p);

	///functions to initialize, render, update and delete 
	void Init(gef::InputManager* input_manager, gef::AudioManager* audio_manager);
	void Render(gef::SpriteRenderer * spriteRenderer, float width, float height, gef::Font * font);
	void Update(float frame_time, gef::InputManager * inputManager, State ** nextState);
	void Release(gef::AudioManager* audio_manager);
	void HandleInput(gef::InputManager * inputManager);
private:
	
	void InitMesh();
	void InitSound(gef::AudioManager* audio_manager);

	void UpdateEnemies(float frameTime);

	void UpdateTowers(float frameTime);

	/// functions for initializing Mesh
	void ReadSceneAndAssignFirstMesh(const char* filename, gef::Scene** scene, gef::Mesh** mesh);
	gef::Mesh* GetFirstMesh(gef::Scene* scene);

	void DrawHUD(gef::SpriteRenderer * spriteRenderer);
	void RenderOverlay(gef::SpriteRenderer * spriteRenderer);
	void SetupLights();

	/// functions for reading from a file
	char *FileReader(const char * filename);
	void LoadWaypointsFromFile(const char* filename);

	/// functions for handling touchInput and screen to wworld space interaction
	bool TouchInput(gef::InputManager * inputManager, gef::Renderer3D * renderer, float width, float height);
	void GetScreenPosRay(const gef::Vector2 & screen_position, const gef::Matrix44 & projection, const gef::Matrix44 & view, gef::Vector4 & startPoint, gef::Vector4 & direction, float width, float height);
	bool RaySphereIntersect(gef::Vector4 & startPoint, gef::Vector4 & direction, gef::Vector4 & sphere_centre, float sphere_radius);
	void TargetEnemy(gef::Renderer3D * renderer);

	/// functions for game logic behaviour
	void LoseCheck();
	void WinCheck();
	void RoundOver();
	void RoundStart();

	/// functions for tower controls
	void SpawnTower(gef::Matrix44 markerTransform, gef::Matrix44 relativeTransform, gef::Matrix44 inverseTransform, int towerChoice);
	void UpgradeTower(gef::Vector2 screenPos, gef::Renderer3D * renderer);
	void BuildTower(gef::Vector2 screenPos, gef::Renderer3D * renderer);
	void SetTowerPosition();


	/// functions for screen shake
	void Shake(float frameTime);
	float RandomRange(float min, float max);

	/// Variables for rendering camera feed
	gef::Sprite cameraFeedSprite_;
	SceGxmTexture cameraFeed_;
	gef::TextureVita cameraFeedTexture_;
	float cameraImageAspectRatio_, displayAspectRatio_, cameraImageScaleFactor_, ndc_z_min_, shakeTime;

	/// gef related variables
	class gef::Renderer3D* renderer_3d_;
	PrimitiveBuilder* primitive_builder_;
	gef::Font* font_;

	EnemyManager enemyManager;

	/// variables for player health, th current round, the marker selected to build on or upgrade and the last marker to be built on
	int health, roundNum, startMarkers, totalMarkers, selectedMarker, markerIndex, lostMarkerIndex;

	float fps_;

	bool gameStarted, roundEnded, roundStarted, buildAvailable, isShaking, towerSelected, lostMarker;

	Int32 roundStart_sfx_id, construction_sfx_id, cannon_sfx_id, arrow_sfx_id;
	UInt32 colorChange;

	gef::Texture* errorTex;
	gef::Sprite errorSpite;

	/// Games Starting Towers and map
	GateTower gate;
	ProjectileTower tower;
	CannonTower cannonTower;
	GameObject map, enemyGoal;

	/// tower vectors to store all towers in game
	std::vector<ProjectileTower*>towers;
	std::vector<CannonTower*> cannons;

	/// pointers to towers that are chosen for upgrades
	ProjectileTower* towerToUpgrade;
	CannonTower* cannonToUpgrade;
	
	/// waypoiints read from file and stored in vector and used for enemy mobvement
	GameObject waypoints[16];
	std::vector<gef::Vector4> waypointPositions;
	
	/// in game UI
	GUI* gui;

	/// Ingame mesh and scene files
	gef::Scene* mapScene_;
	gef::Mesh* mapMesh_;

	gef::Scene* towerScene_;
	gef::Mesh* towerMesh_;

	gef::Scene* tarTowerScene_;
	gef::Mesh* tarTowerMesh_;

	gef::Scene* enemyScene_;
	gef::Mesh* enemyMesh_;

	gef::Scene* gateScene_;
	gef::Mesh* gateMesh_;

	gef::Scene* arrowScene_;
	gef::Mesh* arrowMesh_;

	gef::Scene* cannonScene_;
	gef::Mesh* cannonMesh_;

	gef::Scene* cannonBallScene_;
	gef::Mesh* cannonBallMesh_;

	/// vector of fixed positions for towers that dont move during a round
	std::vector<FixedPosition> fixedPostions;

	Marker markers[6];
	gef::Matrix44 initInverse, initTransform;

	/// arrays for storing the required matrices for marker calculations
	gef::Matrix44 markerTransforms[6];
	gef::Matrix44 markerInverseTransforms[6];
	gef::Matrix44 markerTransformsRelativeto1[6];

	/// array for setting postions for sphere centres for screen to world space interaction
	gef::Vector4 markerSphereCentre[6];
};
