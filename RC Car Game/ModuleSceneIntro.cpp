#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "ModulePhysics3D.h"
#include "ModulePlayer.h"
#include "Primitive.h"
#include "PhysBody3D.h"
#include "PhysVehicle3D.h"
#include <math.h>
#include <gl/GL.h>
#include <gl/GLU.h>


ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	App->camera->Move(vec3(1.0f, 1.0f, 0.0f));
	App->camera->LookAt(vec3(0, 0, 0));

	/*// indiana jones big ball ---
	big_ball_prim.radius = 5; 
	big_ball_prim.SetPos(10, 10, 70); 
	big_ball_prim.color = Green; 
	big_ball_body = App->physics->AddBody(big_ball_prim, 100.0f);//6000.0f);
	big_ball_body->SetStatic(true);
	//big_ball_body->Set_Speed(btVector3(0, 20, 0));
	
	// check point test TODO: make a specific function, actually unlocks the indiana ball ----

	Cube checkCube(10, 10, 1);
	checkCube.SetPos(10, 2, 60);
	circuit_cubes.prims.PushBack(checkCube);
	// physbody
	//PhysBody3D* b = App->physics->AddBody(checkCube, 0.0f, true);
	test_sensor = App->physics->AddBody(checkCube, 0.0f, true);
	// listener
	//b->collision_listeners.add(this);
	test_sensor->collision_listeners.add(this);
	check_point_bodies.PushBack(test_sensor);
	
	// ----------------------------------------------------------------------------------------
	
	// ramp
	test_ramp = CreateRamp((0, 0, 0), (10, 30, 30)); */

	// tunnel
	Create_Tunnel((100, 100, 100), (200, 200, 200));


	// test timer
	test_timer.Start();

	// sensors test
	/*CreateCheckSensor({ 0,0,40 }, { 0,0,0 });
	CreateCheckSensor({ 0,0,80 }, { 0,0,0 });
	CreateCheckSensor({ 0,0,120 }, { 0,0,0 });
	CreateCheckSensor({ 0,0,160 }, { 0,0,0 });*/

	// cannon sensors test
	/*CreateCannonSensor({ 0,0,40 }, { 0,0,0 });
	CreateCannonSensor({ 0,0,80 }, { 0,0,0 });
	CreateCannonSensor({ 0,0,120 }, { 0,0,0 });
	CreateCannonSensor({ 0,0,160 }, { 0,0,0 });*/

	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	for (int i = 0; i < cannon_sensors.Count(); ++i)
	{
		delete cannon_sensors[i].ball;
	}

	return true;
}

// Update
update_status ModuleSceneIntro::Update(float dt)
{
	Plane p(0, 1, 0, 0);
	p.axis = true;
	p.Render();


	// draw all circuit pieces
	for (int i = 0; i < circuit_cubes.prims.Count(); ++i)
		circuit_cubes.prims[i].Render();

/*	big_ball_prim.SetPos(big_ball_body->GetPos().x, big_ball_body->GetPos().y, big_ball_body->GetPos().z); 
	big_ball_prim.Render(); 
	
	
	if (App->input->GetKey(SDL_SCANCODE_K) == KEY_DOWN) {
		float magnitude = 85000;
		big_ball_body->Push((App->player->vehicle->GetPos().x - big_ball_body->GetPos().x)*magnitude, (App->player->vehicle->GetPos().y - big_ball_body->GetPos().y)*magnitude, (App->player->vehicle->GetPos().z - big_ball_body->GetPos().z)*magnitude);
	}

	/*if (App->input->GetKey(SDL_SCANCODE_P) == KEY_DOWN) {
		float magnitude = 8500;
		big_ball_body->Push((App->player->vehicle->GetPos().x - big_ball_body->GetPos().x)*magnitude, (App->player->vehicle->GetPos().y - big_ball_body->GetPos().y)*magnitude, (App->player->vehicle->GetPos().z - big_ball_body->GetPos().z)*magnitude);
	}*/

	// big ball reposition test
	/*if (test_timer.Read() > 5000 && !big_ball_body->isStatic())
	{
		big_ball_body->SetPos(10, 10, 70);
		big_ball_body->SetStatic(true);
		test_timer.Start();
	}

	// Cannon sensors collision cooldown states
	for (int i = 0; i < cannon_sensors.Count(); ++i)
	{
		if (cannon_sensors[i].collision) // if previous state is collision, checks timer cooldown
		{
			if (cannon_sensors[i].timer.Read() > 5000) // random value for testing
			{
				// be ready for next collision
				cannon_sensors[i].collision = false;
				// reposition the associated ball
				cannon_sensors[i].ball->body->SetPos(cannon_sensors[i].ball->original_pos.x, cannon_sensors[i].ball->original_pos.y, cannon_sensors[i].ball->original_pos.z);
				//cannon_sensors[i].ball->body->Set_Speed(btVector3(0, 0, 0));
				cannon_sensors[i].ball->body->SetStatic(true);
			}
		}

		// draw associated ball
		cannon_sensors[i].ball->primitive.SetPos(cannon_sensors[i].ball->body->GetPos().x, cannon_sensors[i].ball->body->GetPos().y, cannon_sensors[i].ball->body->GetPos().z);
		cannon_sensors[i].ball->primitive.Render();
	}

	if (App->input->GetKey(SDL_SCANCODE_P) == KEY_DOWN) {
		float magnitude = 8500;
		//cannon_sensors[0].ball->body->SetPos(0, 0, 0);
		cannon_sensors[0].ball->body->Push(1000, 1000, 10);
	}

	// Cannon ball primitives print // doesnt needs now, now draw associated fixed ball with each cannon sensor
	//for (int i = 0; i < cannon_balls.count(); ++i)
	//{
	//	cannon_balls[i].primitive.SetPos(cannon_balls[i].body->GetPos().x, cannon_balls[i].body->GetPos().y, cannon_balls[i].body->GetPos().z);
	//	cannon_balls[i].primitive.Render();

	//	// deletes by timer
	//	if (cannon_balls[i].timer.Read() > 5000) // test life value
	//	{
	//		//delete cannon_balls[i].body;
	//		LOG("");
	//	}
	//}*/

	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody3D* body1, PhysBody3D* body2)
{
	//if (body2 == App->player->vehicle)

	// specific sensors check
	if(body1 == test_sensor)
	{
		if (big_ball_body->isStatic())
		{
			LOG("Indiana jones ball is coming");
			big_ball_body->SetStatic(false);
			test_timer.Start();
		}
	}

	// if a collision is coming from a sensor call
	if (body1->is_sensor)
	{
		// iterates all sensor array
		for (int i = 0; i < check_point_bodies.Count(); ++i)
		{
			if (body1 == check_point_bodies[i])
			{
				LOG("basic check point collision");
			}
		}

		// iterates all cannon sensors
		for(int i = 0; i < cannon_sensors.Count() && body2 == App->player->vehicle; ++i) // always that we are colliding with player
		{
			if (body1 == cannon_sensors[i].body && !cannon_sensors[i].collision)
			{
				LOG("cannon sensor collision");
				// sets collision timer
				cannon_sensors[i].timer.Start();
				cannon_sensors[i].collision = true;

				// adds impulse
				float force = 5600.0f;
				int x = cannon_sensors[i].ball->body->GetPos().x;
				vec3 newDir(App->player->vehicle->GetPos().x - cannon_sensors[i].ball->body->GetPos().x ,
							App->player->vehicle->GetPos().y - cannon_sensors[i].ball->body->GetPos().y ,
							App->player->vehicle->GetPos().z - cannon_sensors[i].ball->body->GetPos().z);
				newDir = normalize(newDir);
				cannon_sensors[i].ball->body->SetStatic(false);
				cannon_sensors[i].ball->body->Push(newDir.x * force, newDir.y * force, newDir.z * force);
			}
		}

	}
}


Cube ModuleSceneIntro::CreateRamp(vec3 origin, vec3 dest) {

	// create cube ----

	Cube ramp;
	ramp.color = White;
	float angle = 20.0f * _PI / 180;


	vec3 floor_distance = dest - origin;
	vec3 height = floor_distance * sin(angle) / sin(90 - angle);
	vec3 dest_in_air = dest + height;
	vec3 actual_Size = dest_in_air - origin;
	ramp.size = actual_Size;
	ramp.size.y = 0.5f;

	ramp.SetPos(origin.x, height.y / 2, origin.z);
	ramp.SetRotation(20.0f, { 1, 0, 0 });

	// create physbody ----

	PhysBody3D* ramp_body = App->physics->AddBody(ramp, pow(10, 50));
	ramp_body->Set_Orientation(angle, { 1, 0, 0 });
	ramp_body->Get_Rigid_Body()->setGravity({ 0, 0, 0 });

	// adds primitive and respective body to circuit pieces dynamic array
	circuit_cubes.prims.PushBack(ramp);
	circuit_cubes.bodies.PushBack(ramp_body);

	// adds listener
	ramp_body->collision_listeners.add(this);

	return ramp;
}

void ModuleSceneIntro::Create_Tunnel(vec3 origin, vec3 dest) {

	float corner_junction = 0.25f;   // wall width / 2 
	float height = 11.5f; 
	float width = 26.5f; 

	// three parts with commonalities
	Cube top, left, right; 
	top.color = left.color = right.color = Blue;
	top.size = left.size = right.size = dest - origin; 
	top.size.y = left.size.y = right.size.y = 0.5f; 
	

	// top 
	top.size.z = width;
	PhysBody3D* top_body = App->physics->AddBody(top, pow(10, 50)); 
	top_body->SetStatic(true); 
	top_body->SetPos(origin.x, height, origin.z);
	top.SetPos(origin.x, height, origin.z);

	// left
	left.size.z = height; 
	PhysBody3D* left_body = App->physics->AddBody(left, pow(10, 50));
	left_body->SetStatic(true);
	left_body->SetPos(top_body->GetPos().z, height / 2 - corner_junction, top_body->GetPos().x - top.size.z / 2 + corner_junction);
	left.SetPos(top_body->GetPos().z, height / 2 - corner_junction, top_body->GetPos().x - top.size.z / 2 + corner_junction);
	left.SetRotation(90.0f, { 1,0,0 });
	left_body->Set_Orientation(90.0f * _PI / 180, { 1,0,0 }); 

	// right
	right.size.z = height;
	PhysBody3D* right_body = App->physics->AddBody(right, pow(10, 50));
	right_body->SetStatic(true);
	right_body->SetPos(top_body->GetPos().z, height / 2 - corner_junction, top_body->GetPos().x + top.size.z / 2 - corner_junction);
	right.SetPos(top_body->GetPos().z, height / 2 - corner_junction, top_body->GetPos().x + top.size.z / 2 - corner_junction);
	right.SetRotation(90.0f, { 1,0,0 });
	right_body->Set_Orientation(90.0f * _PI / 180, { 1,0,0 });



	/*cubeObjects* c = nullptr; 

	c->bodies.PushBack(top_body); 
	c->prims.PushBack(top);*/

	circuit_cubes.prims.PushBack(top);
	circuit_cubes.bodies.PushBack(top_body); 

	circuit_cubes.prims.PushBack(left);
	circuit_cubes.bodies.PushBack(left_body);

	circuit_cubes.prims.PushBack(right);
	circuit_cubes.bodies.PushBack(right_body);
	
}






cannonBalls* ModuleSceneIntro::SpawnCannonBall(const vec3 origin, vec3 direction)
{
	// creates, adds and set timer to cannon balls, adds to list and push the ball
	int offset = 7; // offset to far away the ball laterally
	// creates sphere
	Sphere ball_prim;
	ball_prim.radius = 3;
	// checks if we have even or not on the cannon balls list, for spawning this shot from left or right
	float newXpos = 0.f;
	if (cannon_sensors.Count() % 2 != 0) {//cannon_balls.count() % 2 != 0) {
		newXpos = origin.x + ball_prim.radius * 2 + offset;
	}
	else {
		newXpos = origin.x - ball_prim.radius * 2 - offset;
	}
	ball_prim.SetPos(newXpos, origin.y + ball_prim.radius, origin.z);
	ball_prim.color = Orange;
	// creates the body for collision
	PhysBody3D* b = App->physics->AddBody(ball_prim, 100.0f);
	//b->SetPos(newXpos, origin.y, origin.z);
	b->SetStatic(true);
	// creates new cannon ball data
	cannonBalls* newCannonBall = new cannonBalls();
	newCannonBall->body = b;
	newCannonBall->primitive = ball_prim;
	newCannonBall->original_pos = b->GetPos();
	// adds to cannonballs list
	//cannon_balls.add(newCannonBall);

	// adds impulse
	/*float force = 5000.0f;
	vec3 newDir(newXpos - App->player->vehicle->GetPos().x, origin.y - App->player->vehicle->GetPos().y, origin.z - App->player->vehicle->GetPos().z);
	newDir = normalize(newDir);
	b->Push(-newDir.x * force, newDir.y * force, newDir.y * force);*/

	return newCannonBall;

}

void ModuleSceneIntro::CreateCheckSensor(const vec3 position, vec3 direction)
{
	Cube checkCube(10, 3, 1);
	checkCube.SetPos(position.x, position.y, position.z);
	//checkCube.SetRotation();
	circuit_cubes.prims.PushBack(checkCube); // for now (debug draw only)
	// physbody
	PhysBody3D* b = App->physics->AddBody(checkCube, 0.0f, true);
	// listener
	b->collision_listeners.add(this);
	check_point_bodies.PushBack(b); // this is a basic checkpoint
}

void ModuleSceneIntro::CreateCannonSensor(const vec3 position, vec3 direction)
{
	Cube checkCube(10, 3, 1);
	checkCube.SetPos(position.x, position.y, position.z);
	//checkCube.SetRotation();
	circuit_cubes.prims.PushBack(checkCube); // for now (debug draw only)
	// physbody
	PhysBody3D* b = App->physics->AddBody(checkCube, 0.0f, true);
	// listener
	b->collision_listeners.add(this);

	// cannon sensor relative
	cannonSensors newCannon;
	newCannon.body = b;
	newCannon.cubePrim = checkCube;

	// adds a cannon ball associated
	newCannon.ball = SpawnCannonBall({ b->GetPos().x, b->GetPos().y, b->GetPos().z }, { 0,0,0 });
	
	cannon_sensors.PushBack(newCannon);
}



