#pragma once
#include "Module.h"
#include "Globals.h"
#include "p2Point.h"

struct PhysVehicle3D;
struct btVector3; 

#define MAX_ACCELERATION 4500.0f           // MAX_ACCELERATION 1000.0f
#define MAX_ACC_DRS 5500.0f
#define TURN_DEGREES 25.0f * DEGTORAD      // TURN_DEGREES 15.0f * DEGTORAD 
#define BRAKE_POWER 3500.0f                // BRAKE_POWER 1000.0f
#define MAX_SPEED 200.0f
#define MAX_SPEED_DRS 250.0f

class ModulePlayer : public Module
{
public:
	ModulePlayer(Application* app, bool start_enabled = true);
	virtual ~ModulePlayer();

	bool Start();
	update_status Update(float dt);
	void Keys();
	bool CleanUp();

	void CameraLogic();
	

public:

	PhysVehicle3D* vehicle;
	float turn;
	float acceleration;
	float brake;
	float inertia; 
	float acc_factor = 1.0f; 
	// fxs
	uint engine_fx; 
	Light focus; 

};