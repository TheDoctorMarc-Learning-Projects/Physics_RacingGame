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
#include "circuitSegments.h"


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

	// indiana jones big ball ---
	//big_ball_prim.radius = 5; 
	//big_ball_prim.SetPos(10, 10, 70); 
	//big_ball_prim.color = Green; 
	//big_ball_body = App->physics->AddBody(big_ball_prim, 100.0f);//6000.0f);
	//big_ball_body->SetStatic(true);
	//big_ball_body->Set_Speed(btVector3(0, 20, 0));
	
	// check point test TODO: make a specific function, actually unlocks the indiana ball ----

	//Cube checkCube(10, 10, 1);
	//checkCube.SetPos(10, 2, 60);
	//circuit_cubes.prims.PushBack(checkCube);
	//// physbody
	////PhysBody3D* b = App->physics->AddBody(checkCube, 0.0f, true);
	//test_sensor = App->physics->AddBody(checkCube, 0.0f, true);
	//// listener
	////b->collision_listeners.add(this);
	//test_sensor->collision_listeners.add(this);
	//check_point_bodies.PushBack(test_sensor);
	
	// ----------------------------------------------------------------------------------------
	
	// ramp
	// test_ramp = CreateRamp((0, 0, 0), (10, 30, 30)); 

	// tunnel
	// Create_Tunnel((50, 50, 50), (300, 300, 300));

	// test timer
	//test_timer.Start();

	// sensors test
	/*CreateCheckSensor({ 0,0,40 }, { 0,0,0 });
	CreateCheckSensor({ 0,0,80 }, { 0,0,0 });
	CreateCheckSensor({ 0,0,120 }, { 0,0,0 });
	CreateCheckSensor({ 0,0,160 }, { 0,0,0 });*/

	// handle lights
	/*App->renderer3D->lights[1].ref = GL_LIGHT1;
	App->renderer3D->lights[1].Init(); 
	App->renderer3D->lights[1].diffuse = Red; 
	App->renderer3D->lights[1].ambient = Red; 
	App->renderer3D->lights[1].SetPos(0, 3 , 0);
	App->renderer3D->lights[1].Active(true);*/

	//Create_Fence({ 10,0,10 }, { 60,5,40 });

	Create_Finish_Line_Elements({ 10, 0, -179 }); 

	// create individual fence items from array
	// similar way from how we use vertexBox2d ric app
	int circuitBoundaries[10] = {
	-200, 200,
	200, 200,
	200, -200,
	-200, -200,
	-200, 200
	};

	//for (int i = 0; i < 8; i+=2) // in pack of two, be sure to send correctly or got a crash for out of array bounds
	//{
	//	//CreateFence(&exteriorFences[i]);
	//	CreateBar(&circuitBoundaries[i]);
	//}
	for (int i = 0; i < 294; i += 2) // in pack of two, be sure to send correctly or got a crash for out of array bounds
	{
		CreateBar(&circuitSketch_segmentPG[i]);
	}
	for (int i = 0; i < 404; i += 2) // in pack of two, be sure to send correctly or got a crash for out of array bounds
	{
		CreateBar(&circuitSketch_segmentPR[i]);
	}

	// create tunnel
	Create_Tunnel({ -32, 0, 177} , { 110, 0, 269 });

	// create ramps ---
	CreateRampV2({49, 2, -69}, { 15, 15 }, -50.f, 15.0f);
	CreateRampV2({70, 2, -45 }, { 15, 15 }, -45.f, -15.0f);
	CreateRampV2({ -130, 2, 37 }, { 15, 20 }, -60.f, 15.0f);
	CreateRampV2({ -113, 2, 69 }, { 15, 15 }, -60.f, -15.0f);

	// create cannon sensors with balls
	CreateCannonSensor({ -150,0,-179 });
	CreateCannonSensor({ -130,0,-179 });
	CreateCannonSensor({ -115,0,-179 });
	CreateCannonSensor({ -100,0,-179 });
	CreateCannonSensor({ -90,0,-179 });
	CreateCannonSensor({ -85,0,-179 });
	CreateCannonSensor({ -80,0,-179 });

	// check points
	CreateCheckSensor({ 13,0,-192 }, {13,0,-165});
	CreateCheckSensor({ 132,0,-176 }, { 115,0,-159 });
	CreateCheckSensor({ 45,0,-108 }, { 20,0,-100 });
	CreateCheckSensor({ 72,0, 18 }, { 68,0,42 });
	CreateCheckSensor({ -146,0,188 }, { -145,0, 164 });
	CreateCheckSensor({ -138,0,68 }, { -147,0, 52 });
	CreateCheckSensor({ -172,0,15 }, { -158,0, 0 });

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


	for (int i = 0; i < circuit_cyls.prims.Count(); ++i)
		circuit_cyls.prims[i].Render();

	// draw all checkPoints primitives, debug draw purposes
	for (int i = 0; i < check_points.Count(); ++i)
		check_points[i].bodyPrim.Render();

	/*// render test light object
	test_light.Render();*/

	/*big_ball_prim.SetPos(big_ball_body->GetPos().x, big_ball_body->GetPos().y, big_ball_body->GetPos().z); 
	big_ball_prim.Render(); 
	
	
	if (App->input->GetKey(SDL_SCANCODE_K) == KEY_DOWN) {
		float magnitude = 85000;
		big_ball_body->Push((App->player->vehicle->GetPos().x - big_ball_body->GetPos().x)*magnitude, (App->player->vehicle->GetPos().y - big_ball_body->GetPos().y)*magnitude, (App->player->vehicle->GetPos().z - big_ball_body->GetPos().z)*magnitude);
	}*/

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
	}*/

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

	//
	/*Sphere s;
	s.SetPos(0, 10, -180);
	s.Render();*/
	Cube testC;
	testC.SetPos(0, 10, -180);
	testC.Render();

	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody3D* body1, PhysBody3D* body2)
{

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
		for (int i = 0; i < check_points.Count(); ++i)
		{
			if (body1 == check_points[i].body && !check_points[i].active)
			{
				LOG("basic check point collision");
				check_points[i].active = true;
				check_points[i].bodyPrim.color = Green;
				// if this is the last checkpoint, reset the first
				if (i > 0 && i == check_points.Count() - 1)
				{
					LOG("last checkpoint");
					check_points[0].active = false;
					check_points[0].bodyPrim.color = White;
				}
				// if this is the first checkpoint, deactivate the rest, for lap reset
				if (i == 0 && check_points.Count() > 1)
				{
					for (int j = 1; j < check_points.Count(); ++j)
					{
						check_points[j].active = false;
						check_points[j].bodyPrim.color = White;
					}
				}
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
				//int x = cannon_sensors[i].ball->body->GetPos().x;
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


void ModuleSceneIntro::Create_Finish_Line_Elements(const vec3 pos) {

	float height = 10; 
	float panel_height = 4; 
	uint separation = 1; 

	Cube Left, Right; 
	Left.color = Right.color = White; 
	Left.SetPos(pos.x, pos.y + height / 2, pos.z - 18);
	Right.SetPos(pos.x, pos.y + height / 2, pos.z + 18);
	Left.size = Right.size = { 1, height, 1 };


	PhysBody3D* l = App->physics->AddBody(Left, 0.0f);
	PhysBody3D* r = App->physics->AddBody(Right, 0.0f);

	uint j = 0; 
	while (j < 4) {
		while (separation < 38) {
			Cube e;
			if (j % 2 == 0) {
				e.color = (separation % 2 == 0) ? White : Black;
			}
			else {
				e.color = (separation % 2 == 0) ? Black : White;
			}
			
			e.SetPos(l->GetPos().x, l->GetPos().y + height / 2 + panel_height / 2 - j, l->GetPos().z - Left.size.z + separation);
			e.size = { 1, 1, 1 };

			PhysBody3D* e_body = App->physics->AddBody(e, 0.0f);
			circuit_cubes.bodies.PushBack(e_body);
			circuit_cubes.prims.PushBack(e);

			separation++;
		}
		separation = 1;
		j++; 
	}



	circuit_cubes.prims.PushBack(Left); 
	circuit_cubes.prims.PushBack(Right);
	
	circuit_cubes.bodies.PushBack(l);
	circuit_cubes.bodies.PushBack(r);
	

}

void ModuleSceneIntro::CreateRampV2(const vec3 mapPositionXZ, const vec2 plane_size, const float yawAngle, const float rollAngle)
{
	Cube c;
	c.size.x = plane_size.x; // width
	c.size.z = plane_size.y; // "height"
	c.size.y = 0.4f;

	PhysBody3D* b = App->physics->AddBody(c, 0.0f);
	b->SetEuler(yawAngle, rollAngle);
	b->SetPos(mapPositionXZ.x, mapPositionXZ.y, mapPositionXZ.z);

	const btQuaternion _q = *b->GetRotQuat();
	const btVector3 _axis = _q.getAxis();

	c.SetPos(b->GetPos().x, b->GetPos().y, b->GetPos().z);
	c.SetRotation(_q.getAngle() * 180 / _PI, { _axis.getX(), _axis.getY(), _axis.getZ() });

	circuit_cubes.prims.PushBack(c);
	circuit_cubes.bodies.PushBack(b);
}


void ModuleSceneIntro::CreateBar(const int* arrayDir)
{
	vec3 origin;
	vec3 dest;
	origin.Set(10, 0, 10);
	dest.Set(-10, 0, 30);

	// x,z
	origin.x = *arrayDir;
	arrayDir++;
	origin.z = *arrayDir;
	arrayDir++;
	dest.x = *arrayDir;
	arrayDir++;
	dest.z = *arrayDir;

	vec3 u = origin - dest;
	//u = normalize(u);
	float angle = atan2f(u.z, u.x);

	//find midpoint to sum to origin coords.
	vec3 midPoint;
	midPoint.Set((origin.x + dest.x) * 0.5f, 0, (origin.z + dest.z) * 0.5f);

	// cube primitive
	Cube cube;
	cube.size.x = length(u);
	cube.size.y = 2;
	// TODO: test color, needs special fence list to do this
	cube.color = (circuit_cubes.prims.Count() % 2 == 0) ? White : Red;
	cube.SetPos(midPoint.x, cube.size.y / 2, midPoint.z);
	cube.SetRotation(angle * RADTODEG, { 0,-1, 0 });

	// physic body
	PhysBody3D* b = App->physics->AddBody(cube, 0.f);
	b->SetPos(midPoint.x, cube.size.y / 2, midPoint.z);
	b->Set_Orientation(angle, { 0,-1,0 });

	circuit_cubes.prims.PushBack(cube);
	circuit_cubes.bodies.PushBack(b);
}

void ModuleSceneIntro::Create_Tunnel(vec3 origin, vec3 dest) {

	float corner_junction = 0.25f;   // wall width / 2 
	//float rot_angle_Z = asin((dest.x - origin.x) / sqrt(pow(dest.x - origin.x, 2) + pow(dest.z - origin.z, 2))) * 180 / _PI;  // 2D rot angle
	//float rot_angle_X = 90 - rot_angle_Z;


	// three parts with commonalities
	Cube top, left, right; 
	top.color = left.color = right.color = White;
	top.size = left.size = right.size = dest - origin; 
	top.size.y = left.size.y = right.size.y = 1.5f; 

	// top 
	top.size.z = TUNNEL_WIDTH;
	PhysBody3D* top_body = App->physics->AddBody(top); 
	top_body->SetStatic(true); 
	top_body->SetPos(origin.x, TUNNEL_HEIGHT, origin.z);
	top.SetPos(origin.x, TUNNEL_HEIGHT, origin.z);

	// left
	left.size.z = TUNNEL_HEIGHT;
	PhysBody3D* left_body = App->physics->AddBody(left, pow(10, 50));
	left_body->SetStatic(true);
	left_body->SetPos(origin.x, TUNNEL_HEIGHT / 2 - corner_junction, top_body->GetPos().z - top.size.z / 2 + corner_junction);
	left.SetPos(origin.x, TUNNEL_HEIGHT / 2 - corner_junction, top_body->GetPos().z - top.size.z / 2 + corner_junction);
	left.SetRotation(90.0f, { 1,0,0 });
	left_body->Set_Orientation(90.0f * _PI / 180, { 1,0,0 }); 

	// right
	right.size.z = TUNNEL_HEIGHT;
	PhysBody3D* right_body = App->physics->AddBody(right, pow(10, 50));
	right_body->SetStatic(true);
	right_body->SetPos(origin.x, TUNNEL_HEIGHT / 2 - corner_junction, top_body->GetPos().z + top.size.z / 2 - corner_junction);
	right.SetPos(origin.x, TUNNEL_HEIGHT / 2 - corner_junction, top_body->GetPos().z + top.size.z / 2 - corner_junction);
	right.SetRotation(90.0f, { 1,0,0 });
	right_body->Set_Orientation(90.0f * _PI / 180, { 1,0,0 });

    // add rotation 

	/*top.SetRotation(rot_angle_Z * 180 / _PI, {0, -1, 0});
	top_body->Set_Orientation(rot_angle_Z, { 0, -1, 0 });

	left.SetRotation(rot_angle_Z * 180 / _PI, { 0, -1, 0 });
	left_body->Set_Orientation(rot_angle_Z, { 0, -1, 0 });

	right.SetRotation(rot_angle_Z * 180 / _PI, { 0, -1, 0 });
	right_body->Set_Orientation(rot_angle_Z, { 0, -1, 0 });*/



	// store objects
	circuit_cubes.prims.PushBack(top);
	circuit_cubes.bodies.PushBack(top_body); 

	circuit_cubes.prims.PushBack(left);
	circuit_cubes.bodies.PushBack(left_body);

	circuit_cubes.prims.PushBack(right);
	circuit_cubes.bodies.PushBack(right_body);


	/*// lights
	test_light.radius = 1;
	test_light.color = Blue;
	test_light.SetPos(0, 0, 0);

	App->renderer3D->lights[1].Active(true);
	App->renderer3D->lights[1].ambient = Red;
	App->renderer3D->lights[1].diffuse = Red;
	App->renderer3D->lights[1].Init();
	App->renderer3D->lights[1].SetPos(2, 2, 2);
	App->renderer3D->lights[1].on = true;
	App->renderer3D->lights[1].ref = GL_LIGHT1;*/

	
}

vec3 ModuleSceneIntro::Create_Side_Fence_Limit_Segment(vec3 origin, vec3 dest) {

	vec3 return_last_pos(0,0,0); 

	float fence_height = 3; 
	float separation = 0; 
	float first_fence_offset = 0.15f; // fence depth / 2

	vec3 original_dir = dest - origin; 

	// for (uint i = 1; i<=2; ++i) {       // do the same for both laterals of the circuit

		Cube top, bottom;
		top.color = bottom.color = Black;
		vec3 dir = dest - origin;
		dir.y = 0.5f;
		dir.z = 0.5f;
		top.size = bottom.size = dir;

		float rot_angle_Z = asin((dir.x) / sqrt(pow(dir.x, 2) + pow(dest.z - origin.z, 2))) * 180 / _PI;  // 2D rot angle
		float rot_angle_X = 90 - rot_angle_Z;

		float z_in_world = original_dir.z / cos(rot_angle_Z * _PI / 180);
		float x_in_world = original_dir.x * sin(rot_angle_X);

		float proportion = z_in_world / x_in_world; 
		float real_dist_x = sqrt((pow(TUNNEL_WIDTH, 2) / (1 + proportion))); 
		float real_dist_z = proportion * real_dist_x; 

	//	if (i == 1) {
			
			top.SetPos(origin.x, fence_height, origin.z);
			bottom.SetPos(origin.x, 0, origin.z);
		/*}
		else {
			separation = 0;   // reset separation 
			top.SetPos(origin.x - real_dist_x , fence_height, origin.z + real_dist_z );
			bottom.SetPos(origin.x - real_dist_x , 0, origin.z + real_dist_z);

		}*/
		
		PhysBody3D* top_body = App->physics->AddBody(top, pow(10, 50));
		PhysBody3D* bottom_body = App->physics->AddBody(bottom, pow(10, 50));


		// add rotation
		top.SetRotation(rot_angle_Z, { 0, -1, 0 });
		top_body->Set_Orientation(rot_angle_Z * _PI / 180, { 0, -1, 0 });

		bottom.SetRotation(rot_angle_Z, { 0, -1, 0 });
		bottom_body->Set_Orientation(rot_angle_Z * _PI / 180, { 0, -1, 0 });



		while (separation < top.size.x) {

			Cube vertical(0.3f, fence_height, 0.3f);
			vertical.color = Black;
			vertical.SetPos(-first_fence_offset * sin(rot_angle_X * _PI / 180) + bottom_body->GetPos().x + (bottom.size.x / 2) * sin(rot_angle_X * _PI / 180) - separation * sin(rot_angle_X * _PI / 180), fence_height / 2, -first_fence_offset * sin(rot_angle_Z * _PI / 180) + bottom_body->GetPos().z + (bottom.size.x / 2) * sin(rot_angle_Z * _PI / 180) - separation * sin(rot_angle_Z * _PI / 180));
			PhysBody3D* vertical_body = App->physics->AddBody(vertical, pow(10, 50));

			// add rotation
			vertical.SetRotation(rot_angle_Z, { 0, -1, 0 });
			vertical_body->Set_Orientation(rot_angle_Z * _PI / 180, { 0, -1, 0 });

			circuit_cubes.prims.PushBack(vertical);
			circuit_cubes.bodies.PushBack(vertical_body);

			separation += 3;
			
			// store return position
			if (separation == 3) {
				return_last_pos.x = vertical_body->GetPos().x;
				return_last_pos.z = vertical_body->GetPos().z;
			}
	//	}

		// chop off the end if the vertical fences do not coincide

		circuit_cubes.prims.PushBack(top);
		circuit_cubes.bodies.PushBack(top_body);


		circuit_cubes.prims.PushBack(bottom);
		circuit_cubes.bodies.PushBack(bottom_body);


		// store last position 

		
		last_positions_to_snap.add(return_last_pos); 

	}

	return return_last_pos; 
}


cannonBalls* ModuleSceneIntro::SpawnCannonBall(const vec3 origin, vec3 direction)
{
	// creates, adds and set timer to cannon balls, adds to list and push the ball
	int offsetX = 9; // offset to far away the ball laterally
	int offsetY = 6;
	// creates sphere
	Sphere ball_prim;
	ball_prim.radius = 3;
	// checks if we have even or not on the cannon balls list, for spawning this shot from left or right
	float newZpos = 0.f;
	if (cannon_sensors.Count() % 2 != 0) {
		newZpos = origin.z + ball_prim.radius * 2 + offsetX;
	}
	else {
		newZpos = origin.z - ball_prim.radius * 2 - offsetX;
	}
	ball_prim.SetPos(origin.x, origin.y + ball_prim.radius + offsetY, newZpos);
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

void ModuleSceneIntro::CreateCheckSensor(const vec3 origin, vec3 destination)
{
	vec3 direction = origin - destination;
	// get angle xz
	float angle = atan2f(direction.z, direction.x);
	// cube primitive
	vec3 midPoint;
	midPoint.Set((origin.x + destination.x) * 0.5f, 0, (origin.z + destination.z) * 0.5f);
	Cube checkCube(10, 2, 1);
	checkCube.SetPos(midPoint.x, checkCube.size.y * 0.5f, midPoint.z); // y doesnt matter on this case
	checkCube.SetRotation(angle * RADTODEG, { 0,-1, 0 });
	checkCube.size.x = length(direction);
	// physbody
	PhysBody3D* b = App->physics->AddBody(checkCube, 0.0f, true);
	// listener
	b->collision_listeners.add(this);
	
	// adds to lists
	//circuit_cubes.prims.PushBack(checkCube); // for now (debug draw only)
	checkPoints newCheckPoint;
	newCheckPoint.body = b;
	newCheckPoint.bodyPrim = checkCube;
	check_points.PushBack(newCheckPoint);
	//check_point_bodies.PushBack(b); // this is a basic checkpoint
}

void ModuleSceneIntro::CreateCannonSensor(const vec3 position)
{
	Cube checkCube(24, 3, 1);
	checkCube.SetPos(position.x, position.y, position.z);
	checkCube.SetRotation(90, { 0,-1,0 });
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







