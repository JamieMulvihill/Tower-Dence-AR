#include "Enemy.h"
#include <maths/math_utils.h>
#include <math.h>

Enemy::Enemy() {

	Init();
}
Enemy::~Enemy() {
	
}


void Enemy::Init() {

	speed = .1f;
	health = 100.f;

	/// setting waypoint values
	currentWaypoint = 0;
	nextWaypoint = 1;
	distanceToWaypoint = 0;
	distanceBetweenWaypoints = 0;
	totalWaypoints = 15;

	/// initializing bools
	isAlive = true;
	hasStarted = false;
	reachedEndPoint = false;
	isTargeted = false;
	
	scaleVector = gef::Vector4(.25f, .25f, .25f);
	
}
bool Enemy::Update(float deltaTime)
{
	if (hasStarted) {
		position_ += velocity_ * deltaTime;
	}
	BuildTransform();

	if (distanceToWaypoint > distanceBetweenWaypoints) { /// check enemy postion aganst the current and next waypoints
		currentWaypoint++;
		nextWaypoint++;
		SetWaypointDistance(waypointPositions[currentWaypoint], waypointPositions[nextWaypoint]);
		RecalculateVelocity();

		if (nextWaypoint == totalWaypoints) { /// check if enemy has reached end and if so, stop them
			reachedEndPoint = true;
			velocity_ = gef::Vector4(0, 0, 0);
		}
	}

	if (waypointPositions.size() > 0) {
		CalculateDistance(waypointPositions[currentWaypoint]);
	}

	return true;
}

void Enemy::SetWaypointDistance(gef::Vector4 start, gef::Vector4 end) {
	distanceBetweenWaypoints = (end - start).Length();
}

void Enemy::InitializeWaypoints(std::vector<gef::Vector4> waypoints)
{
	waypointPositions = waypoints;
}

void Enemy::CalculateDistance(gef::Vector4 start)
{
	distanceToWaypoint = (position_ - start).Length();
}

/// function to determind the velocity/direction of enemy
void Enemy::RecalculateVelocity()
{
	velocity_ = waypointPositions[nextWaypoint] - waypointPositions[currentWaypoint]; /// calculate vector between current and next waypoint
	velocity_.Normalise();
	BuildZRotationMatrix(atan2(velocity_.y(), velocity_.x()) + (3.1415 /2)); /// calculate rotation angle
	velocity_ *= speed; /// multiply by speed scaler value
}
