#include "ProjectileManager.h"

ProjectileManager::ProjectileManager()
{
	numProjectiles = 0;
}

ProjectileManager::~ProjectileManager()
{
}

void ProjectileManager::InitProjectiles(gef::Vector4 startPos, gef::Mesh* mesh, gef::Vector4 targtPos, Enemy* target, float angle, float offset)
{

	std::list<Projectile*>::iterator it; /// iterator for the projectile in list
	projectiles.push_back(new Projectile); /// add projectile to list
	it = projectiles.begin();
	advance(it, numProjectiles); /// move the iterator to the desired position in the list
	projectiles.back()->set_mesh(mesh);
	projectiles.back()->BuildZRotationMatrix(angle);
	projectiles.back()->Init(mesh, gef::Vector4(startPos.x(), startPos.y(), startPos.z() + offset), targtPos, target);
	projectiles.back()->SetAlive(true);
	projectiles.back()->SetIterator(it); /// set iterator position
	numProjectiles++;
}

void ProjectileManager::UpdateProjectiles(float frame_time)
{
	for (std::list<Projectile*>::iterator it = projectiles.begin(); it != projectiles.end(); ++it) {

		Projectile* current = *it;
		if ((*it)->GetAlive()) /// if the projectile is alive update otherwise delete it
		{
			current->Update(frame_time);
		}
		else
		{
			DeleteProjectile(current);
			break;
		}
	}
}

void ProjectileManager::Render(gef::Renderer3D * renderer)
{
	for (std::list<Projectile*>::iterator it = projectiles.begin(); it != projectiles.end(); ++it) {
		if ((*it)->GetAlive())
		{
			Projectile *current = *it;
			renderer->DrawMesh(*current);
			
		}
	}
}

void ProjectileManager::DeleteProjectile(Projectile * projectile)
{
	projectiles.erase(projectile->GetIterator()); /// erase the projectile from list
	
	numProjectiles--;
}
