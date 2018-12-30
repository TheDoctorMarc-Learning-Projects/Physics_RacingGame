#pragma once
#include "Module.h"
#include "p2DynArray.h"
#include "Globals.h"
#include "Primitive.h"

#define MAX_LAPS 3
#define MAX_PARTY_BALLS 11
#define TUNNEL_WIDTH 28.5f
#define TUNNEL_HEIGHT 11.5f
#define MAX_WRECKING__CBALLS 10 // the first is the "static one", the last is the big one

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
	Sphere lightSphere;
};

struct FallingSnakesData
{
	p2DynArray<Sphere> s;
	p2DynArray<PhysBody3D*> b = { NULL };
	int lastChangedColorId = 0;
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

	// circuit elements creations ------------
	void CreateCheckPoint(const vec3 position, vec3 direction);
	void Create_Tunnel_Sensors(const vec3 position); 
	cannonBalls* SpawnCannonBall(const vec3 origin, vec3 direction);
	void CreateCannonSensor(const vec3 position);
	void Create_Tunnel(vec3 origin, vec3 dest); 
	void Create_Side_Fence_Limit_Segment(vec3 origin, vec3 dest); 
	void CreateBar(const int*);
	void CreateRampV2(const vec3 mapPositionXZ, const vec2 plane_size, const float yawAngle = 0.f, const float rollAngle = 0.f);
	void CreatePartyBall(const vec3 position, float radius = 1.0f);
	void CreateFallingSnake(const vec3 position, const float size = 0.5f, int numOfBalls = 3);
	void Create_Finish_Line_Elements(const vec3);
	// utils
	void RepositionPartyBalls();
	void FallingSnakesColorCascade();
	bool UpdateGameState();
	Uint32 GetBestLap();
	void GetStandardTimeFormat(uint& min, uint& sec, uint &ms, Uint32 time) const;
	void AddPenalizationTime(Uint32 time);
	// -----------------------------------------

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
	uint loseSFX = 0u; 
	uint winSFX = 0u;
	uint checkpointSFX = 0u;
	uint cannonShotSFX = 0u;

	// list of all cubes
	cubeObjects circuit_cubes;

	// and cyls
	CylObjects circuit_cyls; 

	// and spheres
	SphObjects circuit_sphrs; 
	// checkpoints
	p2DynArray<checkPoints> check_points;

	// tunnel
	cubeObjects tunnel;
	
	// list of current cannonballs
	p2List<cannonBalls> cannon_balls;
	// check points for cannon zone
	p2DynArray<cannonSensors> cannon_sensors;
	SphObjects partyBallsZone;
	vec3 partyBallDefPositions[MAX_PARTY_BALLS] = { {152,0,64},{129,0,107},{140,0,120}, {126,0,151}, {132,0,38},
													{85,0,164},{120,0,170}, {136,0,100}, {90, 0,182}, {56,0,180}, {136,0,136} };
	float partyBallSpecificRadius[MAX_PARTY_BALLS] = { 4.0f, 2.0f, 0.5f, 3.0f, 1.5f, 0.75f, 1.25f, 1.0f, 0.85f, 1.0f, 1.0f };

	p2DynArray<FallingSnakesData> fallingSnakes;
	Timer color_delayer_timer;
	Uint32 color_swapping_timer_fsnakes = 60u;

	// wrecking ball pendulum test
	PhysBody3D* wreckingBallBs[10] = { NULL };
	Sphere wreckingBallSs[10];
	
};
