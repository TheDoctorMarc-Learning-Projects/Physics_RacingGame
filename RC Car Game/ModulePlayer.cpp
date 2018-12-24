#include "Globals.h"
#include "Application.h"
#include "ModulePlayer.h"
#include "Primitive.h"
#include "PhysVehicle3D.h"
#include "PhysBody3D.h"
#include <gl/GL.h>
#include <gl/GLU.h>

ModulePlayer::ModulePlayer(Application* app, bool start_enabled) : Module(app, start_enabled), vehicle(NULL)
{
	turn = acceleration = brake = 0.0f;
}

ModulePlayer::~ModulePlayer()
{}

// Load assets
bool ModulePlayer::Start()
{
	LOG("Loading player");


	// Fx's
	engine_fx = App->audio->LoadFx("Sound/engine.wav");
	App->audio->Change_Fx_Volume(engine_fx, 40);
	App->audio->PlayFx(engine_fx, -1);
	

	// lights
	/*glEnable(GL_LIGHTING);
	focus.Init();
	focus.Active(true);
	focus.ambient = { 0.4f, 0.5f, 0.6f, 1.0f };
	focus.diffuse = { 0.6f, 0.5f, 0.4f, 1.0f };
	focus.ref */

	VehicleInfo car;


	// Car properties ----------------------------------------
	car.chassis_size.Set(2.5, 1, 4.5);
	car.chassis_offset.Set(0, 1.0, 0);

	/*car.cabin_size.Set(2.0, 0.3, 2); 
	car.cabin_offset.Set(0, 1.8, -1);

	car.front_light_radius_height = (0.2, 0.4); 
	car.front_light_offset = (1.5, 0, 3);*/

	car.rear_wing_size.Set(4.8, 0.4, 0.3);
	car.rear_wing_offset.Set(0, 2.6, -2.5);

	car.rear_wing_support_size.Set(0.2, 1.0, 0.2); 
	car.rear_wing_support_offset.Set(0, 1.9, -2.45); 

	car.rear_wing_flag_size.Set(0.1, 1.1, 1.1); 
	car.rear_wing_flag_offset.Set(2.5, 2.3, -2.5); 

	car.mass = 500.0f;
	car.suspensionStiffness = 8.0f;  // 15.88f;
	car.suspensionCompression = 0.83f;
	car.suspensionDamping = 0.3f;  // 0.88f;
	car.maxSuspensionTravelCm = 200.0f;
	car.frictionSlip = 10; // 50.5;
	car.maxSuspensionForce = 6000.0f;

	// Wheel properties ---------------------------------------
	float front_connection_height = 0.75f;
	float front_wheel_radius = 0.75f;
	float front_wheel_width = 0.65f;
	float front_suspensionRestLength = 0.4f;

	float rear_connection_height = 0.8f;
	float rear_wheel_radius = 0.8f;
	float rear_wheel_width = 0.75f;
	float rear_suspensionRestLength = 0.8f; 

	// lights


	// car properties

	float half_rear_width = car.chassis_size.x*1.0f;
	float half_front_width = car.chassis_size.x*0.9f;

	float half_length = car.chassis_size.z*0.8f;
	
	vec3 direction(0,-1,0);
	vec3 axis(-1,0,0);
	
	car.num_wheels = 4;
	car.wheels = new Wheel[4];

	// FRONT-LEFT ------------------------
	car.wheels[0].connection.Set(half_front_width - 0.3f * front_wheel_width, front_connection_height, half_length - front_wheel_radius);
	car.wheels[0].direction = direction;
	car.wheels[0].axis = axis;
	car.wheels[0].suspensionRestLength = front_suspensionRestLength;
	car.wheels[0].radius = front_wheel_radius;
	car.wheels[0].width = front_wheel_width;
	car.wheels[0].front = true;
	car.wheels[0].drive = true;
	car.wheels[0].brake = false;
	car.wheels[0].steering = true;

	// FRONT-RIGHT ------------------------
	car.wheels[1].connection.Set(-half_front_width + 0.3f * front_wheel_width, front_connection_height, half_length - front_wheel_radius);
	car.wheels[1].direction = direction;
	car.wheels[1].axis = axis;
	car.wheels[1].suspensionRestLength = front_suspensionRestLength;
	car.wheels[1].radius = front_wheel_radius;
	car.wheels[1].width = front_wheel_width;
	car.wheels[1].front = true;
	car.wheels[1].drive = true;
	car.wheels[1].brake = false;
	car.wheels[1].steering = true;

	// REAR-LEFT ------------------------
	car.wheels[2].connection.Set(half_rear_width - 0.3f * rear_wheel_width, rear_connection_height, -half_length + rear_wheel_radius);
	car.wheels[2].direction = direction;
	car.wheels[2].axis = axis;
	car.wheels[2].suspensionRestLength = rear_suspensionRestLength;
	car.wheels[2].radius = rear_wheel_radius;
	car.wheels[2].width = rear_wheel_width;
	car.wheels[2].front = false;
	car.wheels[2].drive = false;
	car.wheels[2].brake = true;
	car.wheels[2].steering = false;

	// REAR-RIGHT ------------------------
	car.wheels[3].connection.Set(-half_rear_width + 0.3f * rear_wheel_width, rear_connection_height, -half_length + rear_wheel_radius);
	car.wheels[3].direction = direction;
	car.wheels[3].axis = axis;
	car.wheels[3].suspensionRestLength = rear_suspensionRestLength;
	car.wheels[3].radius = rear_wheel_radius;
	car.wheels[3].width = rear_wheel_width;
	car.wheels[3].front = false;
	car.wheels[3].drive = false;
	car.wheels[3].brake = true;
	car.wheels[3].steering = false;

	vehicle = App->physics->AddVehicle(car);
	vehicle->SetPos(0, 12, 10);

	 
	
	return true;
}

// Unload assets
bool ModulePlayer::CleanUp()
{
	LOG("Unloading player");

	return true;
}

// Update: draw background
update_status ModulePlayer::Update(float dt)
{

	turn = acceleration = brake = 0.0f;

	// handle light

	focus.position.x = vehicle->GetPos().x;
	focus.position.y = vehicle->GetPos().y + 2;
	focus.position.z = vehicle->GetPos().z;
	
	focus.Render();

	// handle special keys
	Keys(); 

	
	//App->camera->Look((10, 50, 20), vehicle->GetPos(), false);

	// App->camera->Move(vehicle->GetPos());
	
	// handle movement

	inertia = (-vehicle->info.mass * vehicle->GetKmh()) / 50 + vehicle->GetKmh();

	if(App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
	{
		  App->audio->Change_Fx_Volume(engine_fx, MIX_MAX_VOLUME);    // louder sound 

		  if(acc_factor > 0.1)
		  acc_factor -= 0.008; 

		    if(vehicle->GetKmh() <= MAX_SPEED)
			acceleration = MAX_ACCELERATION * acc_factor;

			if (App->input->GetKey(SDL_SCANCODE_P) == KEY_REPEAT) {
				vehicle->info.rear_wing_size.y = 0.05f;                   // open rear wing (DRS)

				if (vehicle->GetKmh() <= MAX_SPEED_DRS)
				acceleration = MAX_ACC_DRS * acc_factor;
			}

	}

	else {
		acc_factor = 1; 

		App->audio->Change_Fx_Volume(engine_fx, 40);  // default engine sound

		vehicle->info.rear_wing_size.y = 0.4f;  // close DRS

		if (vehicle->GetKmh() > 0) {
			acceleration += inertia;
		}
		
	}

	if(App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
	{
		if(turn < TURN_DEGREES)
			turn +=  TURN_DEGREES;
	}

	if(App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
	{
		if(turn > -TURN_DEGREES)
			turn -= TURN_DEGREES;
	}

	if(App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
	{
		
		if (vehicle->GetKmh() > 0) {                      // brake 
			brake = BRAKE_POWER;
		}
		else {
			acceleration = -MAX_ACCELERATION / 4;             // go backwards 
		}

	}
	else if (vehicle->GetKmh() < 0) {
		acceleration += inertia / 4;
	}

	
	/*if (vehicle->GetKmh() > 0) {
		acceleration += inertia; 
	}
	else if (vehicle->GetKmh() < 0) {
		acceleration += inertia / 4;
	}*/


	vehicle->ApplyEngineForce(acceleration);
	vehicle->Turn(turn);
	vehicle->Brake(brake);

	vehicle->Render();
	char title[80];
	sprintf_s(title, "%.1f Km/h", vehicle->GetKmh());
	App->window->SetTitle(title);

	CameraLogic();


	return UPDATE_CONTINUE;
}


void ModulePlayer::Keys() {

	// reset player position

	if (App->input->GetKey(SDL_SCANCODE_O) == KEY_DOWN) {
		vehicle->Set_Speed(btVector3(0,0,0)); 
		vehicle->SetPos(0, 0, 0); 
	}

	// reset overturned car

}

void ModulePlayer::CameraLogic()
{
	vec3 pos = vehicle->GetPos();
	pos.y = vehicle->GetPos().y + 10; 
	vec3 direction = vehicle->GetForwardVector();
	direction = normalize(direction);
	App->camera->Position = pos - direction * 20;
	App->camera->Position.y = 17;
	App->camera->LookAt(pos);

}



