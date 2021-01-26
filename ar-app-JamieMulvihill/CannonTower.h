#pragma once
#include "Tower.h"
#include "Projectile.h"
#include "ProjectileManager.h"
#include "Marker.h"

class CannonTower : public Tower {

public:
	void Init(gef::Mesh* mesh);
	void Update(float frametime);
	bool CheckRanges(gef::Matrix44 enemyLocalTransform, Marker enemyMarker,  gef::Matrix44 localMarkerTransform, gef::Mesh* mesh, EnemyManager* enemyManager_);

	///Getters and setters for marker transforms
	gef::Matrix44 GetMarkerTransform() { return markerTransform; };
	void SetMarkerTransform(gef::Matrix44 transform) { markerTransform = transform; };
	
	gef::Matrix44 GetMarkerTransformRelativeto1() { return markerTransformRelativeto1; };
	void SetMarkerTransformRelativeto1(gef::Matrix44 relativeTransform) { markerTransformRelativeto1 = relativeTransform; };
	
	gef::Matrix44 GetMarkerInverseTransform() { markerInverseTransform; };
	void SetMarkerInverseTransform(gef::Matrix44 inverseTransform) { markerInverseTransform = inverseTransform; };

	bool GethasFired() { return hasFired; }

	ProjectileManager* GetProjectileManager() { return &projectileManager; }

private:
	bool hasFired = false;
	float targetRange, frameTime, currentTime, fireRate;
	ProjectileManager projectileManager;
	gef::Matrix44 local_marker_transform, marker_Transform;
};