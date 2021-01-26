#include "ProjectileTower.h"

#include <graphics/mesh.h>
#include <maths/math_utils.h>
#include <math.h>

void ProjectileTower::Init(gef::Mesh* mesh)
{
	gef::Sphere bounds(gef::Vector4(0.0f, 0.0f, 0.0f), 2.f);
	mesh->set_bounding_sphere(bounds);
	set_mesh(mesh);
	currentTime = 0.f;
	fireRate = 0.4f;
	targetRange = .1;
	isSelected = false;
}

void ProjectileTower::Update(float frametime)
{
	if (projectileManager.projectiles.size() > 0) {
		projectileManager.UpdateProjectiles(frametime);
		/// iterate through list of projectiles and set the transforms
		for (std::list<Projectile*>::iterator it = projectileManager.projectiles.begin(); it != projectileManager.projectiles.end(); ++it) {
			if ((*it)->GetAlive())
			{
				Projectile *current = *it;
				current->set_transform(current->GetlocalTransform() * local_marker_transform * marker_Transform);
			}
		}
	}

}

bool ProjectileTower::CheckRanges(float frameTime, gef::Matrix44 enemyLocalTransform, gef::Matrix44 marker1transform, gef::Matrix44 inverseTowerMarker, gef::Matrix44 localMarkerTransform, gef::Mesh* mesh, EnemyManager* enemyManager_, Enemy* targetEnemy)
{
	local_marker_transform = localMarkerTransform;
	marker_Transform = marker1transform;

	/// convert the enemy from its marker to tower maker space get the vector from the enemy to the tower
	targetVector = (enemyLocalTransform * marker1transform * inverseTowerMarker).GetTranslation() - position_; 

	if (targetVector.Length() < targetRange && enemyManager_->enemies.size() > 0) { /// check vector is within range and there is enemy

		float angle = atan2(targetVector.y(), targetVector.x()); /// angle for rotation of proectile towards enemy
		
		currentTime += frameTime;
		if (currentTime > fireRate) { /// check if tower is ready to fire
			projectileManager.InitProjectiles(gef::Vector4(0.f, 0.f, 0.f), mesh, (enemyLocalTransform * marker1transform * inverseTowerMarker).GetTranslation(), targetEnemy, (angle + 3.1415), 0.05f);
			currentTime = 0;
			return true;
		}
		return false;
	}
	return false;
}

void ProjectileTower::UpgradeTower(int choice)
{
	int upgradeChoice = choice; 

	switch (upgradeChoice) { /// switch for choosing what upgrade to apply to tower
		case 0:
			targetRange *= 1.5f; /// increase range
			break;
		case 1:
			fireRate *= .8f; /// increase rate of fire
			break;
		default:
			break;
	}

}
