#pragma once
#include <graphics/mesh_instance.h>
#include <vector>

class GameObject : public gef::MeshInstance
{
public:
	GameObject();
	~GameObject();

	void Init();
	bool Update(float deltaTime);
	void BuildTransform();

	gef::Matrix44 BuildXRotationMatrix(float angle);
	gef::Matrix44 BuildYRotationMatrix(float angle);
	gef::Matrix44 BuildZRotationMatrix(float angle);

	gef::Vector4 GetPosition() { return position_; }
	void SetPosition(gef::Vector4 position) { position_ = position; }

	gef::Vector4 GetVelocity() { return velocity_; }
	void SetVelocity(gef::Vector4 velocity) { velocity_ = velocity; }

	gef::Vector4 GetScaleVector() { return scaleVector; }
	void SetScaleVector(gef::Vector4 scalevecotr) { scaleVector = scalevecotr; }

	gef::Matrix44 GetXRotationMatrix() { return XRotationMatrix; }
	void SetXRotationMatrix(gef::Matrix44 XRotate) { XRotationMatrix = XRotate; }

	gef::Matrix44 GetYRotationMatrix() { return YRotationMatrix; }
	void SetYRotationMatrix(gef::Matrix44 YRotate) { XRotationMatrix = YRotate; }

	gef::Matrix44 GetZRotationMatrix() { return ZRotationMatrix; }
	void SetZRotationMatrix(gef::Matrix44 ZRotate) { ZRotationMatrix = ZRotate; }

	gef::Matrix44 GetlocalScaleMatrix() { return localScaleMatrix; }
	void SetlocalScaleMatrix(gef::Matrix44 localScale) { localScaleMatrix = localScale; }

	gef::Matrix44 GetlocalRotateMatrix() { return localRotateMatrix; }
	void SetlocalRotateMatrix(gef::Matrix44 localRotate) { localRotateMatrix = localRotate; }

	gef::Matrix44 GetlocalTransform() { return localTransform; }
	void SetlocalTransform(gef::Matrix44 localtrans) { localTransform = localtrans; }

	gef::Matrix44 GetlocalPosition() { return localPosition; }
	void SetlocalPosition(gef::Matrix44 localpos) { localPosition = localpos; }

	gef::Matrix44 markerTransform;
protected:


	gef::Vector4 position_;
	gef::Vector4 velocity_;
	gef::Vector4 scaleVector;

	gef::Matrix44 XRotationMatrix;
	gef::Matrix44 YRotationMatrix;
	gef::Matrix44 ZRotationMatrix;

	float x_rotation_radians;
	float y_rotation_radians;
	float z_rotation_radians;

	gef::Matrix44 localScaleMatrix;
	gef::Matrix44 localRotateMatrix;
	gef::Matrix44 localTransform;
	gef::Matrix44 localPosition;

};
