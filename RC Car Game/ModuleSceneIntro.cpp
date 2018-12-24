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


	big_ball_prim.radius = 5; 
	big_ball_prim.SetPos(10, 10, 70); 
	big_ball_prim.color = Green; 
	big_ball_body = App->physics->AddBody(big_ball_prim, 0.0f);//6000.0f);

	//big_ball_body->Set_Speed(btVector3(0, 20, 0));
	
	test_ramp = CreateRamp((0, 0, 0), (10, 30, 30)); 

	// test timer
	test_timer.Start();
	

	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");
	return true;
}

// Update
update_status ModuleSceneIntro::Update(float dt)
{
	Plane p(0, 1, 0, 0);
	p.axis = true;
	p.Render();

	big_ball_prim.SetPos(big_ball_body->GetPos().x, big_ball_body->GetPos().y, big_ball_body->GetPos().z); 
	big_ball_prim.Render(); 
	
	// draw all circuit pieces
	for (int i = 0; i < circuit_cubes.prims.Count(); ++i)
		circuit_cubes.prims[i].Render();
	
	
	if (App->input->GetKey(SDL_SCANCODE_K) == KEY_DOWN) {
		float magnitude = 85000;
		big_ball_body->Push((App->player->vehicle->GetPos().x - big_ball_body->GetPos().x)*magnitude, (App->player->vehicle->GetPos().y - big_ball_body->GetPos().y)*magnitude, (App->player->vehicle->GetPos().z - big_ball_body->GetPos().z)*magnitude);
	}

	// check timer and adds mass to test sphere
	if (test_timer.Read() > 5000)
	{
		//check_point_body
		LOG("reset timer");
		test_timer.Start();
		// test mass
		//big_ball_body->SetBodyMass(500.0f);
		App->physics->SetBodyMass(big_ball_body, 100.0f);
		//big_ball_body->Set_Speed(btVector3(0, 20, 0));
	}

	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody3D* body1, PhysBody3D* body2)
{
	LOG("collision");
}

Cube ModuleSceneIntro::CreateRamp(vec3 origin, vec3 dest) {

	// create cube ----

	Cube ramp; 
	ramp.color = White;
	float angle = 20.0f * _PI / 180 ;
	             

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

