#pragma once
#include "Module.h"
#include "p2DynArray.h"
#include "Globals.h"
#include "Primitive.h"

#define MAX_LAPS 3
#define TUNNEL_WIDTH 28.5f
#define TUNNEL_HEIGHT 11.5f

class btRigidBody;
class btVector3;
struct PhysBody3D;
struct PhysMotor3D;
struct vec3; 

enum GameState : int
{
	PREPARATION, // places camera somewhere, reset states etc
	COUNTDOWN, // countdown timer
	GO, // locks camera to vehicle and starts crono
	WIN,
	LOSE
};

struct cubeObjects
{
	p2DynArray<Cube> prims;
	p2DynArray<PhysBody3D*> bodies;
};


struct CylObjects
{
	p2DynArray<Cylinder> prims;
	p2DynArray<PhysBody3D*> bodies;
};


struct SphObjects
{
	p2DynArray<Sphere> prims;
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

struct checkPoints
{
	bool active = false;
	PhysBody3D* body = nullptr;
	Cube bodyPrim;
	Cube visualIndicator;
};

struct countdownSFX
{
	bool played = false;
	uint32 fx = 0u;
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
	//Cube CreateRamp(vec3 origin, vec3 dest);
	void CreateCheckPoint(const vec3 position, vec3 direction);
	void Create_Tunnel_Sensors(const vec3 position); 
	cannonBalls* SpawnCannonBall(const vec3 origin, vec3 direction);
	void CreateCannonSensor(const vec3 position);
	void Create_Tunnel(vec3 origin, vec3 dest); 
	void Create_Side_Fence_Limit_Segment(vec3 origin, vec3 dest); 
	//void Create_Curve(vec3 origin, vec3 dest, float factor = 0.0f, uint elems = 10); 

	//void Create_Fence(vec3 origin, vec3 dest);

	//void CreateFence(int*);
	void CreateBar(const int*);
	void CreateRampV2(const vec3 mapPositionXZ, const vec2 plane_size, const float yawAngle = 0.f, const float rollAngle = 0.f);
	//void CreateCheckPointArc(checkPoints*);

	void Create_Finish_Line_Elements(const vec3);

	bool UpdateGameState();
	Uint32 GetBestLap();
	void GetStandardTimeFormat(uint& min, uint& sec, uint &ms, Uint32 time) const;
	void AddPenalizationTime(Uint32 time);

public:

	GameState game_state = GameState::PREPARATION;
	Timer countdownTimer;
	Timer lapTimer;
	Uint32 AllLapsTime[MAX_LAPS] = { 0u }; // for show on title bar when race ends
	Uint32 TotalRaceTime = 0u;
	Uint32 penalizationTime = 0u;
	Uint32 maxTimeForWin = 180000; // 3minutes for now TODO: adjust
	int lap = 0;
	// sfx
	countdownSFX cdsfx[3]; // 3 beep
	uint cameraMoveSFX = 0u;
	uint loseSFX = 0u; // TODO
	uint winSFX = 0u; // TODO
	uint checkpointSFX = 0u;

	/*
	PhysBody3D* pb_snake[MAX_SNAKE];
	Sphere s_snake[MAX_SNAKE];

	PhysBody3D* pb_snake2[MAX_SNAKE];
	Sphere s_snake2[MAX_SNAKE];
	*/

	//PhysBody3D* pb_chassis;
	//Cube p_chassis;

	//PhysBody3D* pb_wheel;
	//Cylinder p_wheel;

	//PhysBody3D* pb_wheel2;
	//Cylinder p_wheel2;

	//PhysMotor3D* left_wheel;
	//PhysMotor3D* right_wheel;


	PhysBody3D* big_ball_body; 
	Sphere big_ball_prim; 

	// list of all cubes
	cubeObjects circuit_cubes;

	// and cyls
	CylObjects circuit_cyls; 

	// and spheres
	SphObjects circuit_sphrs; 


	// checkpoints, maybe its useful create another helper struct like circuit_cubes
	/*p2DynArray<PhysBody3D*> check_point_bodies;
	p2DynArray<Cube> check_point_prim;*/
	p2DynArray<checkPoints> check_points;

	// individual useful check point
	PhysBody3D* test_sensor;

	// timers
	Timer test_timer;

	// tunnel
	cubeObjects tunnel;
	
	// list of current cannonballs
	p2List<cannonBalls> cannon_balls;
	// check points for cannon zone
	p2DynArray<cannonSensors> cannon_sensors;


	// primitive "lights"
	Sphere test_light;
	
};
