#include "Tower.h"
#include <graphics/mesh.h>
#include <maths/math_utils.h>
#include <math.h>


void Tower::Init(gef::Mesh* mesh)
{
	isSelected = false;
}

void Tower::Update(float frametime)
{
	
}

void Tower::CheckRanges(gef::Matrix44 enemyLocalTransform, gef::Matrix44 marker1transform, gef::Matrix44 inverseTowerMarker, gef::Matrix44 localMarkerTransform, PrimitiveBuilder * primitive_Builder, gef::Mesh* mesh, EnemyManager* enemyManager_, Enemy* targetEnemy)
{
}

gef::Matrix44 Tower::GetMarkerTransform()
{
	return gef::Matrix44();
}

gef::Matrix44 Tower::GetMarkerTransformRelativeto1()
{
	return gef::Matrix44();
}

gef::Matrix44 Tower::GetMarkerInverseTransform()
{
	return gef::Matrix44();
}

void Tower::SetMarkerTransform(gef::Matrix44)
{
}

void Tower::SetMarkerTransformRelativeto1(gef::Matrix44)
{
}

void Tower::SetMarkerInverseTransform(gef::Matrix44)
{
}

