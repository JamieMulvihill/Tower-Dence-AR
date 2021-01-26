#include "EnemyManager.h"


EnemyManager::EnemyManager()
{
	LaunchedEnemies = 0;
	launchDelay = 0.f;
	totalEnemies = 12;
	launchedAllEnemies = false;
	enemyWinners = 0;
}

EnemyManager::~EnemyManager()
{

}

void EnemyManager::InitEnemies(gef::Vector4 startPos, gef::Mesh* mesh, std::vector<gef::Vector4> waypoints, gef::Vector4 firstWaypoint) {

	launchedAllEnemies = false;
	LaunchedEnemies = 0;

	for (int i = 0; i < totalEnemies; i++) {
		
		enemies.push_back(new Enemy);
		enemies.back()->SetPosition(startPos);
		enemies.back()->SetAlive(true);
		enemies.back()->set_mesh(mesh);
		enemies.back()->InitializeWaypoints(waypoints);
		enemies.back()->SetWaypointDistance(startPos, firstWaypoint);
		enemies.back()->RecalculateVelocity();
		//enemies.back()->targetLock->set_mesh(builder->CreateBoxMesh(gef::Vector4(.05f, .05f, .05f), gef::Vector4(0,0,0)));
	}
}

void EnemyManager::UpdateEnemies(float deltaTime) {
	for (int i = 0; i < enemies.size(); i++) {
		if (enemies[i]->GetAlive()) {
			enemies[i]->Update(deltaTime);
			if (enemies[i]->GetreachedEndPoint())
				enemyWinners++;
		}
		else {
			DeleteEnemy(enemies[i]);
			break;
		}
	}
}

void EnemyManager::Render(gef::Renderer3D* renderer)
{

	for (int i = 0; i < enemies.size(); i++) {
		if (enemies[i]->GetAlive()) {
			renderer->DrawMesh(*enemies[i]);
			/*if (enemies[i]->isTargeted) {
				renderer->DrawMesh(*enemies[i]->targetLock);
			}*/
		}
	}
}

void EnemyManager::DeleteEnemy(Enemy* enemy)
{
	for (int i = 0; i < enemies.size(); i++) {
		if (enemies[i] == enemy) {
			enemies.erase(enemies.begin() + i);
			LaunchedEnemies--;
		}
	}
}

void EnemyManager::LaunchEnemies(float deltaTime)
{
	launchDelay += deltaTime;
	if (!launchedAllEnemies) {
		if (launchDelay > .5) {
			enemies[LaunchedEnemies]->SethasStarted(true);
			LaunchedEnemies++;
			launchDelay = 0.f;
		}
		if (LaunchedEnemies == totalEnemies) {
			launchedAllEnemies = true;
		}
	}
}
void EnemyManager::ReachedEnd() {
	enemyWinners++;
}
