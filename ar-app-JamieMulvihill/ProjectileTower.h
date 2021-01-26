#pragma once
#include "Tower.h"

class ProjectileTower : public Tower {

public:

	void Init(gef::Mesh* mesh);
	void Update(float frametime);
	bool CheckRanges(float frameTime, gef::Matrix44 enemyLocalTransform, gef::Matrix44 enemyMarkerTransform, gef::Matrix44 inverseTowerMarker, gef::Matrix44 localMarkerTransform, gef::Mesh* mesh, EnemyManager* enemyManager_, Enemy* targetEnemy);

	gef::Matrix44 GetMarkerTransform() { return markerTransform; };
	gef::Matrix44 GetMarkerTransformRelativeto1() { return markerTransformRelativeto1; };
	gef::Matrix44 GetMarkerInverseTransform() { markerInverseTransform; };

	void SetMarkerTransform(gef::Matrix44 transform) { markerTransform = transform; };
	void SetMarkerTransformRealativto1(gef::Matrix44 relativeTransform) { markerTransformRelativeto1 = relativeTransform; };
	void SetMarkerInverseTransform(gef::Matrix44 inverseTransform) { markerInverseTransform = inverseTransform; };

	void UpgradeTower(int choice);
	ProjectileManager projectileManager;

private:
	gef::Vector4 targetVector;
	float targetRange;
	float currentTime, fireRate;
	gef::Matrix44 local_marker_transform, marker_Transform;

};