#pragma once
#include "Module.h"
#include "p2DynArray.h"
#include "Globals.h"
#include "Primitive.h"

#define MAX_SNAKE 2

class btRigidBody;
struct PhysBody3D;
struct PhysMotor3D;
struct vec3; 

struct cubeObjects
{
	p2DynArray<Cube> prims;
	p2DynArray<PhysBody3D*> bodies;
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


	// ramps, make a list, for the moment individual
	/*p2List<Cube> ramp_cubes; 
	p2List<PhysBody3D*> ramps;*/
	cubeObjects circuit_cubes;
	// checkpoints, maybe its useful create another helper struct like circuit_cubes
	p2DynArray<PhysBody3D*> check_point_bodies;
	p2DynArray<Cube> check_point_prim;

	// individual useful check point
	PhysBody3D* test_sensor;


	Cube test_ramp; 

	Timer test_timer;
	



};
