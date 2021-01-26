#include "CannonTower.h"
#include <graphics/mesh.h>
#include <maths/math_utils.h>
#include <math.h>

void CannonTower::Init(gef::Mesh* mesh)
{
	gef::Sphere bounds(gef::Vector4(0.0f, 0.0f, 0.0f), 2.f);
	mesh->set_bounding_sphere(bounds);
	set_mesh(mesh);
	targetRange = .1;
	currentTime = 0.f;
	frameTime = 0.f;
	fireRate = 1.5f;
}

void CannonTower::Update(float frametime)
{
	frameTime = frametime;
	if (projectileManager.projectiles.size() > 0) {
		projectileManager.UpdateProjectiles(frametime);
		/// iterate through list of projectiles and set the transforms
		for (std::list<Projectile*>::iterator it = projectileManager.projectiles.begin(); it != projectileManager.projectiles.end(); ++it) {
			if ((*it)->GetAlive())
			{
				Projectile *current = *it;
				current->set_transform(current->GetlocalTransform() * markerTransform);
			}
		}
	}
	set_transform(localTransform * markerTransform); /// set the cannons transofrm
}

bool CannonTower::CheckRanges(gef::Matrix44 enemyLocalTransform, Marker enemyMarker, gef::Matrix44 localMarkerTransform, gef::Mesh* mesh, EnemyManager* enemyManager_)
{
	gef::Vector4 targetVector = gef::Vector4(100,100,100); /// initailze the target vector
	gef::Matrix44 inverse;
	inverse.SetIdentity();
	inverse.AffineInverse(markerTransform); /// Initilize the inverse of the markerTransofrm

	currentTime += frameTime;

	Enemy* closestEnemy; /// pointer to enemy
	if (currentTime > fireRate) { /// check if the cannon is ready to fire

		if (enemyManager_->enemies.size() > 0) {
			for (int i = 0; i < enemyManager_->enemies.size(); i++) {
				/// get the vector from the caannon to enemy
				gef::Vector4 lengthVector = (enemyManager_->enemies[i]->GetlocalTransform() * enemyMarker.GetMarkerTransform() * inverse).GetTranslation() - position_;
				if (lengthVector.Length() < targetVector.Length()) { /// check is vector less than target vector
					targetVector = lengthVector; /// set new vector to e target vector
					closestEnemy = enemyManager_->enemies[i]; /// set enemy to be this enemy
				}
			}
			projectileManager.InitProjectiles(gef::Vector4(0, 0, 0), mesh, gef::Vector4(0, -1, 0), closestEnemy, 0, 0); /// initilize the projectile
			currentTime = 0;
			return true;
		}
	}

	return false;
}
