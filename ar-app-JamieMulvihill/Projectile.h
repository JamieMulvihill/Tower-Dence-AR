#pragma once
#include "GameObject.h"
#include <list>
#include <maths/aabb.h>
#include <graphics/mesh.h>
#include <graphics/mesh_instance.h>
#include "Enemy.h"
#include "EnemyManager.h"

class Projectile : public GameObject
{
public:
	Projectile();
	~Projectile();

	void Init(gef::Mesh * mesh, gef::Vector4 spawn, gef::Vector4 targetPos, Enemy* target);
	void Update(float frameTime);

	void SetIterator(std::list<Projectile*>::iterator iter) { projectileIter = iter; } ///set the postion for each projtile in the list of projectiles
	std::list<Projectile*>::iterator GetIterator() { return projectileIter; }
	void SetAlive(bool alive) { isAlive = alive; }
	bool GetAlive() { return isAlive; }
	void OutofRange();
	bool IsCollidingAABB(const GameObject& meshInstance1, const GameObject& meshInstance2); /// fucntion for collision checks 

private:
	gef::Vector4 spawnPos;
	gef::Vector4 velocity;
	gef::Vector4 targetPos;
	Enemy* targetEnemy;
	float speed, range, projectileDamage_;
	std::list<Projectile*>::iterator projectileIter;
	bool isAlive;
};