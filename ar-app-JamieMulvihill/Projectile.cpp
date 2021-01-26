#include "Projectile.h"

Projectile::Projectile()
{
}

Projectile::~Projectile()
{
}

void Projectile::Init(gef::Mesh * mesh, gef::Vector4 spawn, gef::Vector4 targetPosition, Enemy* target)
{
	speed = .5f; 
	projectileDamage_ = 100.f;
	position_ = spawn;
	targetPos = targetPosition;

	/// this targetEnemy is used for checking the collison with a specific enemy instead of looping though all enemies
	/// due to performance limitations
	targetEnemy = target; 

	velocity = targetPos - position_; /// set the velocity vector of the projectile to be the vector from tower position to the target position 
	velocity.Normalise();
	velocity *= speed;
	scaleVector = gef::Vector4(1.f, 1.f, 1.f);
	range = .25; /// range for despawn
}

void Projectile::Update(float frameTime)
{
	position_ += velocity * frameTime; /// move projectile along velocity vector
	BuildTransform();

	if (IsCollidingAABB(*targetEnemy, *this)) { /// check the collision between the projectile nd its taget enemy
		SetAlive(false);
		targetEnemy->SetHealth(projectileDamage_); /// deal damage
		targetEnemy->SetAlive(false);
	}

	/// check if the projectile has grone of range and need sto be despawned
	OutofRange();
}

void Projectile::OutofRange()
{
	if ((spawnPos - position_).Length() > range) {
		SetAlive(false);
	}
}

bool Projectile::IsCollidingAABB(const GameObject& meshInstance1, const GameObject& meshInstance2) {

	gef::Aabb aaBB_1 = meshInstance1.mesh()->aabb();
	gef::Aabb aaBB_2 = meshInstance2.mesh()->aabb();

	gef::Aabb aaBB1_Transformed = aaBB_1.Transform(meshInstance1.transform());
	gef::Aabb aaBB2_Transformed = aaBB_2.Transform(meshInstance2.transform());

	if (aaBB1_Transformed.max_vtx().x() > aaBB2_Transformed.min_vtx().x() && aaBB1_Transformed.min_vtx().x() < aaBB2_Transformed.max_vtx().x() &&
		aaBB1_Transformed.max_vtx().y() > aaBB2_Transformed.min_vtx().y() && aaBB1_Transformed.min_vtx().y() < aaBB2_Transformed.max_vtx().y() &&
		aaBB1_Transformed.max_vtx().z() > aaBB2_Transformed.min_vtx().z() && aaBB1_Transformed.min_vtx().z() < aaBB2_Transformed.max_vtx().z()) {

		return true;
	}

	return false;

}