#include "GameObject.h"
#include <maths/math_utils.h>


GameObject::GameObject()
{
	Init();
	
}


GameObject::~GameObject()
{

}

void GameObject::Init()
{
	position_ = gef::Vector4(.0f, 0.f, 0.f);
	scaleVector = gef::Vector4(1.f, 1.f, 1.f);
	y_rotation_radians = 0.f;
	velocity_ = gef::Vector4(0.f, 0.f, 0.f);
	localTransform.SetIdentity();
	localPosition.SetIdentity();
	
	BuildXRotationMatrix(0);
	BuildYRotationMatrix(0);
	BuildZRotationMatrix(0);
}

bool GameObject::Update(float deltaTime)
{
	BuildTransform();

	return true;
}

void GameObject::BuildTransform() {
	
	localScaleMatrix.Scale(scaleVector);
	localRotateMatrix = XRotationMatrix * YRotationMatrix * ZRotationMatrix;
	localPosition.SetTranslation(position_);
	localTransform = localScaleMatrix * localRotateMatrix * localPosition;
}

gef::Matrix44 GameObject::BuildXRotationMatrix(float angle)
{
	XRotationMatrix.SetIdentity();
	XRotationMatrix.RotationX(angle);
	return XRotationMatrix;
}

gef::Matrix44 GameObject::BuildYRotationMatrix(float angle)
{
	YRotationMatrix.SetIdentity();
	YRotationMatrix.RotationY(angle);
	return YRotationMatrix;
}

gef::Matrix44 GameObject::BuildZRotationMatrix(float angle)
{
	ZRotationMatrix.SetIdentity();
	ZRotationMatrix.RotationZ(angle);
	return ZRotationMatrix;
}

