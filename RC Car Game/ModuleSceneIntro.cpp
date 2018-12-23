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


	chaser_sph.radius = 5; 
	chaser_sph.SetPos(10, 0, 10); 
	chaser_sph.color = Green; 
	chaser = App->physics->AddBody(chaser_sph, 6000.0f);
	
	test_ramp = SpawnRamp((0, 0, 0), (10, 10, 10)); 
	

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

	chaser_sph.SetPos(chaser->GetPos().x, chaser->GetPos().y, chaser->GetPos().z); 
	// chaser_sph.Render(); 
	

	test_ramp.Render(); 


	if (App->input->GetKey(SDL_SCANCODE_K) == KEY_DOWN) {
		float magnitude = 85000;
		chaser->Push((App->player->vehicle->GetPos().x - chaser->GetPos().x)*magnitude, (App->player->vehicle->GetPos().y - chaser->GetPos().y)*magnitude, (App->player->vehicle->GetPos().z - chaser->GetPos().z)*magnitude);
	}

	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody3D* body1, PhysBody3D* body2)
{
}

Cube ModuleSceneIntro::SpawnRamp(vec3 origin, vec3 dest) {

	Cube ramp; 
	ramp.color = Blue;
	float angle = 20.0f * _PI / 180 ;

	vec3 floor_distance = dest - origin; 
	vec3 height = floor_distance * sin(angle) / sin(90 - angle); 
	vec3 dest_in_air = dest + height; 
	vec3 actual_Size = dest_in_air - origin; 

	ramp.size = actual_Size;
	ramp.size.y = 0.5f; 
	
	ramp.SetPos(origin.x, origin.y + height.y, origin.z); 
	ramp.SetRotation(20.0f, { 1, 0, 0 });


	ramp_body = App->physics->AddBody(ramp, 500000.0f); 



	return ramp; 
}

