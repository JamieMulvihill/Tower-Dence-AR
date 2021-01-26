#pragma once
#include "Tower.h"

class FanTower : public Tower {

public:
	void Init(gef::Mesh* mesh, EnemyManager* enemyManager_);
	void Update(float frametime);
	void CheckRanges(gef::Matrix44 enemyLocalTransform, gef::Matrix44 marker1transform, gef::Matrix44 inverseTowerMarker, gef::Matrix44 localMarkerTransform, PrimitiveBuilder* primitive_Builder, gef::Mesh* mesh, EnemyManager* enemyManager_);

	gef::Matrix44 GetMarkerTransform() { return markerTransform; };
	gef::Matrix44 GetMarkerTransformRelativeto1() { return markerTransformRelativeto1; };
	gef::Matrix44 GetMarkerInverseTransform() { markerInverseTransform; };

	void SetMarkerTransform(gef::Matrix44 transform) { markerTransform = transform; };
	void SetMarkerTransformRelativeto1(gef::Matrix44 relativeTransform) { markerTransformRelativeto1 = relativeTransform; };
	void SetMarkerInverseTransform(gef::Matrix44 inverseTransform) { markerInverseTransform = inverseTransform; };

	//EnemyManager * enemyManager;

private:

	gef::Vector4 targetVector;
	std::vector<GameObject> enemies;
	std::vector<GameObject> targetEnemies;
	float targetRange;

	gef::Matrix44 local_marker_transform, marker_Transform;
};