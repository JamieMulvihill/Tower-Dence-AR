#pragma once
#include "Tower.h"

class GateTower : public Tower {

public:
	void Init(gef::Mesh* mesh, EnemyManager* enemyManager_);
	void Update(float frametime);
	
	gef::Matrix44 GetMarkerTransform() { return markerTransform; };
	gef::Matrix44 GetMarkerTransformRelativeto1() { return markerTransformRelativeto1; };
	gef::Matrix44 GetMarkerInverseTransform() { markerInverseTransform; };

	void SetMarkerTransform(gef::Matrix44 transform) { markerTransform = transform; };
	void SetMarkerTransformRelativeto1(gef::Matrix44 relativeTransform) { markerTransformRelativeto1 = relativeTransform; };
	void SetMarkerInverseTransform(gef::Matrix44 inverseTransform) { markerInverseTransform = inverseTransform; };

	void Attack(gef::Vector4 startPos, gef::Mesh* mesh, gef::Vector4 targtPos, Enemy* target, float angle);
	
	ProjectileManager* GetProjectileManager() { return &projectileManager; }

	void SetCanFire(bool fire) { canFire = fire; }
	bool GetCanFire() { return canFire; }

private:

	ProjectileManager projectileManager;
	gef::Vector4 targetVector;
	float targetRange, reloadTime, currentTime;
	bool canFire, hasShot;

	gef::Matrix44 local_marker_transform, marker_Transform;
};