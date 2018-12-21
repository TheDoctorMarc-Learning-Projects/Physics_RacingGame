#include "Globals.h"
#include "Application.h"
#include "ModulePlayer.h"
#include "Primitive.h"
#include "PhysVehicle3D.h"
#include "PhysBody3D.h"

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

	VehicleInfo car;

	// Car properties ----------------------------------------
	car.chassis_size.Set(2.5, 1, 4);
	car.chassis_offset.Set(0, 1.0, 0);

	car.rear_wing_size.Set(4.8, 0.4, 0.3);
	car.rear_wing_offset.Set(0, 2.4, -2.5);

	car.rear_wing_support_size.Set(0.3, 1, 0.3); 
	car.rear_wing_support_offset.Set(0, 2, -2); 

	car.rear_wing_flag_size.Set(0.1, 1.1, 1.1); 
	car.rear_wing_flag_offset.Set(2.5, 2.3, -2.5); 

	car.mass = 500.0f;
	car.suspensionStiffness = 8.0f;  // 15.88f;
	car.suspensionCompression = 0.83f;
	car.suspensionDamping = 1.0f;  // 0.88f;
	car.maxSuspensionTravelCm = 200.0f;
	car.frictionSlip = 10; // 50.5;
	car.maxSuspensionForce = 6000.0f;

	// Wheel properties ---------------------------------------
	float front_connection_height = 0.75f;
	float front_wheel_radius = 0.75f;
	float front_wheel_width = 0.65f;
	float front_suspensionRestLength = 0.75f;

	float rear_connection_height = 0.8f;
	float rear_wheel_radius = 0.8f;
	float rear_wheel_width = 0.75f;
	float rear_suspensionRestLength = 0.8f; 

	// Don't change anything below this line ------------------

	/*float half_width = car.chassis_size.x*0.5f;
	float half_length = car.chassis_size.z*0.5f;*/

	float half_width = car.chassis_size.x*1.1f;
	float half_length = car.chassis_size.z*0.8f;
	
	vec3 direction(0,-1,0);
	vec3 axis(-1,0,0);
	
	car.num_wheels = 4;
	car.wheels = new Wheel[4];

	// FRONT-LEFT ------------------------
	car.wheels[0].connection.Set(half_width - 0.3f * front_wheel_width, front_connection_height, half_length - front_wheel_radius);
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
	car.wheels[1].connection.Set(-half_width + 0.3f * front_wheel_width, front_connection_height, half_length - front_wheel_radius);
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
	car.wheels[2].connection.Set(half_width - 0.3f * rear_wheel_width, rear_connection_height, -half_length + rear_wheel_radius);
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
	car.wheels[3].connection.Set(-half_width + 0.3f * rear_wheel_width, rear_connection_height, -half_length + rear_wheel_radius);
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

	inertia = (-vehicle->info.mass * vehicle->GetKmh()) / 50 + vehicle->GetKmh();

	if(App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
	{
		acceleration = MAX_ACCELERATION;
	}
	else if (vehicle->GetKmh() > 0) {
		acceleration += inertia;
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




	return UPDATE_CONTINUE;
}



