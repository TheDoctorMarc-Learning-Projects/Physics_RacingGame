#pragma once
#include "Module.h"
#include "p2DynArray.h"
#include "Globals.h"
#include "Primitive.h"

#define MAX_SNAKE 2
#define TUNNEL_WIDTH 26.5f
#define TUNNEL_HEIGHT 11.5f

class btRigidBody;
struct PhysBody3D;
struct PhysMotor3D;
struct vec3; 

struct cubeObjects
{
	p2DynArray<Cube> prims;
	p2DynArray<PhysBody3D*> bodies;
};

struct cannonBalls // helper struct, spawn balls and deletes by timer on scene
{
	vec3 original_pos;
	Timer timer;
	Sphere primitive;
	PhysBody3D* body = nullptr;
};

struct cannonSensors
{
	Timer timer; // activate next collision with cooldown
	bool collision = false;
	PhysBody3D* body = nullptr;
	Cube cubePrim;
	// each cannon sensor has their associate cannon ball // never delete individual bodies without a complete thinkered method, not do for now
	cannonBalls* ball = nullptr;
};

class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(Application* app, bool start_enabled = true);
	~ModuleSceneIntro();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	void OnCollision(PhysBody3D* body1, PhysBody3D* body2);
	Cube CreateRamp(vec3 origin, vec3 dest);
	void CreateCheckSensor(const vec3 position, vec3 direction);
	cannonBalls* SpawnCannonBall(const vec3 origin, vec3 direction);
	void CreateCannonSensor(const vec3 position, vec3 direction);
	void Create_Tunnel(vec3 origin, vec3 dest); 
	void Create_Side_Fence_Limit_Segment(vec3 origin, vec3 dest);

public:
	/*
	PhysBody3D* pb_snake[MAX_SNAKE];
	Sphere s_snake[MAX_SNAKE];

	PhysBody3D* pb_snake2[MAX_SNAKE];
	Sphere s_snake2[MAX_SNAKE];
	*/

	PhysBody3D* pb_chassis;
	Cube p_chassis;

	PhysBody3D* pb_wheel;
	Cylinder p_wheel;

	PhysBody3D* pb_wheel2;
	Cylinder p_wheel2;

	PhysMotor3D* left_wheel;
	PhysMotor3D* right_wheel;


	PhysBody3D* big_ball_body; 
	Sphere big_ball_prim; 


	// list of all cubes
	cubeObjects circuit_cubes;
	// checkpoints, maybe its useful create another helper struct like circuit_cubes
	p2DynArray<PhysBody3D*> check_point_bodies;
	p2DynArray<Cube> check_point_prim;

	// individual useful check point
	PhysBody3D* test_sensor;

	// ramps
	Cube test_ramp; 

	// timers
	Timer test_timer;

	// tunnel
	cubeObjects tunnel;
	
	// list of current cannonballs
	p2List<cannonBalls> cannon_balls;
	// check points for cannon zone
	//cubeObjects cannon_sensors;
	p2DynArray<cannonSensors> cannon_sensors;




	// lights

	Sphere test_light;
	
	
};
