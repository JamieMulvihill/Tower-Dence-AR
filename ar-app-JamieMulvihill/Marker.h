#pragma once
#include <system/platform.h>


class Marker
{
public:
	Marker();
	~Marker();

	gef::Matrix44 GetMarkerTransform() { return markerTransform; }
	void SetMarkerTransform(gef::Matrix44 transform) { markerTransform = transform; }

	gef::Matrix44 GetMarkerRelativeto1() { return markerRelativeto1; }
	void SetMarkerRelativeto1(gef::Matrix44 transform) { markerRelativeto1 = transform; }

	gef::Matrix44 GetinverseTransform() { return inverseTransform; }
	void SetInverseTransform(gef::Matrix44 transform) { inverseTransform = transform; }

	gef::Vector4 GetSphereCentre() { markerSphereCentre; }
	void SetSphereCentre(gef::Vector4 centre) { markerSphereCentre = centre; }

	bool GetisDetected() { return isDetected; }
	void SetisDetected(bool detected) { isDetected = detected; }
private:
	bool isDetected;
	gef::Matrix44 markerTransform, markerRelativeto1, inverseTransform;
	gef::Vector4 markerSphereCentre;

};

