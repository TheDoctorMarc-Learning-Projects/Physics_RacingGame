#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "ModulePhysics3D.h"
#include "ModulePlayer.h"
#include "Primitive.h"
#include "PhysBody3D.h"
#include "PhysVehicle3D.h"
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
	chaser_sph.Render(); 
	
	if (App->input->GetKey(SDL_SCANCODE_K) == KEY_DOWN) {
		float magnitude = 85000;
		chaser->Push((App->player->vehicle->GetPos().x - chaser->GetPos().x)*magnitude, (App->player->vehicle->GetPos().y - chaser->GetPos().y)*magnitude, (App->player->vehicle->GetPos().z - chaser->GetPos().z)*magnitude);
	}

	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody3D* body1, PhysBody3D* body2)
{
}

