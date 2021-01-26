#pragma once
#include "GameObject.h"
#include "EnemyManager.h"
#include "ProjectileManager.h"
#include "primitive_builder.h"

class Tower : public GameObject
{
public:
	
	virtual void Init(gef::Mesh* mesh);
	virtual void Update(float frametime);
	virtual void CheckRanges(gef::Matrix44 enemyLocalTransform, gef::Matrix44 marker1transform, gef::Matrix44 inverseTowerMarker, gef::Matrix44 localMarkerTransform,  PrimitiveBuilder* primitive_Builder, gef::Mesh* mesh, EnemyManager* enemyManager_, Enemy* targetEnemy);
	
	virtual gef::Matrix44 GetMarkerTransform(); 
	virtual gef::Matrix44 GetMarkerTransformRelativeto1();
	virtual gef::Matrix44 GetMarkerInverseTransform();

	virtual void SetMarkerTransform(gef::Matrix44);
	virtual void SetMarkerTransformRelativeto1(gef::Matrix44);
	virtual void SetMarkerInverseTransform(gef::Matrix44);

	void SetMarkerIndex(int marker) { markerIndex = marker; }
	int GetMarkerIndex() { return markerIndex; }

	bool isSelected;
protected:
	
	gef::Matrix44 markerTransformRelativeto1;
	gef::Matrix44 markerInverseTransform;

	int markerIndex;
};

