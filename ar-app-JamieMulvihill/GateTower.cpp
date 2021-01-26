#include "GateTower.h"
#include <graphics/mesh.h>
#include <maths/math_utils.h>
#include <math.h>



void GateTower::Init(gef::Mesh* mesh, EnemyManager* enemyManager_)
{
	gef::Sphere bounds(gef::Vector4(0.0f, 0.0f, 0.0f), 2.f);
	mesh->set_bounding_sphere(bounds);
	set_mesh(mesh);
	targetRange = .1;
	canFire = true;
	hasShot = false;
	currentTime = 0.f;
	reloadTime = .75f;
}

void GateTower::Update(float frametime)
{
	if (hasShot) {
		canFire = false;
		currentTime += frametime;

		if (currentTime > reloadTime) {
			canFire = true;
			hasShot = false;
			currentTime = 0.f;
		}
	}

	if (projectileManager.projectiles.size() > 0) {
		projectileManager.UpdateProjectiles(frametime);
		for (std::list<Projectile*>::iterator it = projectileManager.projectiles.begin(); it != projectileManager.projectiles.end(); ++it) {
			if ((*it)->GetAlive())
			{
				Projectile *current = *it;
				current->set_transform(current->GetlocalTransform() * markerTransform);
			}
		}
	}
	
}

void GateTower::Attack(gef::Vector4 startPos, gef::Mesh * mesh, gef::Vector4 targtPos, Enemy * target, float aglee)
{
	
	targetVector = targtPos - position_;

	if (target) {

		float angle = atan2(targetVector.y(), targetVector.x());
		hasShot = true;
		projectileManager.InitProjectiles(gef::Vector4(0.f, 0.f, 0.f), mesh, targtPos, target, (angle + 3.1415), 0.05f);
	}
}

