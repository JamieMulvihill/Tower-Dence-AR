#pragma once
#include "Projectile.h"
#include <graphics/renderer_3d.h>
#include <list>
#include "Enemy.h"

class ProjectileManager {
public:
	ProjectileManager();
	~ProjectileManager();

	void InitProjectiles(gef::Vector4 startPos, gef::Mesh* mesh, gef::Vector4 targtPos,  Enemy* target, float angle, float offset);
	void UpdateProjectiles(float frame_time);
	void Render(gef::Renderer3D* renderer);
	void DeleteProjectile(Projectile* projectile);

	std::list<Projectile*> projectiles;
private:
	int numProjectiles;
};