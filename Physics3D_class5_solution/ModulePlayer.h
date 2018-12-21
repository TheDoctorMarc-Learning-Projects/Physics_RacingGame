#pragma once
#include "Module.h"
#include "Globals.h"
#include "p2Point.h"

struct PhysVehicle3D;

#define MAX_ACCELERATION 6000.0f           // MAX_ACCELERATION 1000.0f
#define MAX_ACC_DRS 9000.0f
#define TURN_DEGREES 25.0f * DEGTORAD      // TURN_DEGREES 15.0f * DEGTORAD 
#define BRAKE_POWER 3500.0f                // BRAKE_POWER 1000.0f

class ModulePlayer : public Module
{
public:
	ModulePlayer(Application* app, bool start_enabled = true);
	virtual ~ModulePlayer();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

public:

	PhysVehicle3D* vehicle;
	float turn;
	float acceleration;
	float brake;
	float inertia; 

	// fxs
	uint engine_fx; 
};