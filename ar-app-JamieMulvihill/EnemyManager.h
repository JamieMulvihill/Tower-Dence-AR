#pragma once
#include "Enemy.h"
#include <graphics/renderer_3d.h>
#include "primitive_builder.h"
#include <list>

class EnemyManager {
public:

	EnemyManager();
	~EnemyManager();

	void InitEnemies(gef::Vector4 startPos, gef::Mesh* mesh, std::vector<gef::Vector4> waypoints, gef::Vector4 firstWaypoint);
	void UpdateEnemies(float deltaTime);
	void Render(gef::Renderer3D* renderer);
	void DeleteEnemy(Enemy * enemy);
	void LaunchEnemies(float deltaTIme);
	void SetTotalEnemies(int newEnemies) { totalEnemies += newEnemies; }
	int GetTotalEnemies() { return totalEnemies; }
	void ReachedEnd();

	std::vector<Enemy*> enemies;
private:
	int totalEnemies, LaunchedEnemies, enemyWinners;
	bool launchedAllEnemies;
	float launchDelay;
};

