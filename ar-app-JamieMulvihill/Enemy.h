#pragma once
#include "GameObject.h"
#include <list>

class Enemy : public GameObject

{
public :
	Enemy();
	~Enemy();

	void Init();
	bool Update(float deltaTime);
	void RecalculateVelocity(); /// function to calculate the direction/velocity of the enemy
	void CalculateDistance(gef::Vector4 start); 
	void SetWaypointDistance(gef::Vector4 start, gef::Vector4); /// function to set the distance between waypoints
	void InitializeWaypoints(std::vector<gef::Vector4> waypoints);  /// fucntion to  set the waypoint  values for the enemies

	/// Getters and setters for variable values
	void SetHealth(float damage) { health -= damage; }
	
	float GetSpeed() { return speed; }
	void SetSpeed(float speed) { speed = speed; }
	
	bool GetAlive() { return isAlive; }
	void SetAlive(bool alive) { isAlive = alive; }

	bool GethasStarted() { return hasStarted; }
	void SethasStarted(bool start) { hasStarted = start; }

	bool GetreachedEndPoint() { return reachedEndPoint; }
	void SetreachedEndPoint(bool reached) { reachedEndPoint = reached; }

	bool GetisTargeted() { return isTargeted; }
	void SetisTargeted(bool target) { isTargeted = target; }

private:
	float speed, health;
	int currentWaypoint, nextWaypoint, totalWaypoints;
	float distanceToWaypoint;
	float distanceBetweenWaypoints;
	std::vector<gef::Vector4> waypointPositions;
	bool hasStarted, reachedEndPoint, isTargeted, isAlive;
};