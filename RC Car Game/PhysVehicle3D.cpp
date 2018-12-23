#include "PhysVehicle3D.h"
#include "Primitive.h"
#include "Bullet/include/btBulletDynamicsCommon.h"

// ----------------------------------------------------------------------------
VehicleInfo::~VehicleInfo()
{
	//if(wheels != NULL)
		//delete wheels;
}

// ----------------------------------------------------------------------------
PhysVehicle3D::PhysVehicle3D(btRigidBody* body, btRaycastVehicle* vehicle, const VehicleInfo& info) : PhysBody3D(body), vehicle(vehicle), info(info)
{
}

// ----------------------------------------------------------------------------
PhysVehicle3D::~PhysVehicle3D()
{
	delete vehicle;
}

// ----------------------------------------------------------------------------
void PhysVehicle3D::Render()
{
	Cylinder wheel;

	wheel.color = {0.2f, 0.2f, 0.2f, 1.0f};

	for(int i = 0; i < vehicle->getNumWheels(); ++i)
	{
		// wheel.radius = info.wheels[0].radius;
		// wheel.height = info.wheels[0].width;

		wheel.radius = info.wheels[i].radius;
		wheel.height = info.wheels[i].width;

		vehicle->updateWheelTransform(i);
		vehicle->getWheelInfo(i).m_worldTransform.getOpenGLMatrix(&wheel.transform);

		wheel.Render();
	}

	// chassis

	Cube chassis(info.chassis_size.x, info.chassis_size.y, info.chassis_size.z);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&chassis.transform);
	btQuaternion q = vehicle->getChassisWorldTransform().getRotation();
	btVector3 offset(info.chassis_offset.x, info.chassis_offset.y, info.chassis_offset.z);
	offset = offset.rotate(q.getAxis(), q.getAngle());

	chassis.transform.M[12] += offset.getX();
	chassis.transform.M[13] += offset.getY();
	chassis.transform.M[14] += offset.getZ();
	chassis.color = Red; 

	chassis.Render();

	// cabin
	/*Cube cabin(info.cabin_size.x, info.cabin_size.y, info.cabin_size.z);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&cabin.transform); 
	btVector3 offset_C(info.cabin_offset.x, info.cabin_offset.y, info.cabin_offset.z);
	offset_C = offset_C.rotate(q.getAxis(), q.getAngle());

	cabin.transform.M[12] += offset_C.getX();
	cabin.transform.M[13] += offset_C.getY();
	cabin.transform.M[14] += offset_C.getZ();
	cabin.color = Red; 

	cabin.Render();*/



	// rear wing 

	Cube rear_wing(info.rear_wing_size.x, info.rear_wing_size.y, info.rear_wing_size.z);

	vehicle->getChassisWorldTransform().getOpenGLMatrix(&rear_wing.transform);
	btVector3 offset_q_rw(info.rear_wing_offset.x, info.rear_wing_offset.y, info.rear_wing_offset.z);
	offset_q_rw = offset_q_rw.rotate(q.getAxis(), q.getAngle());

	rear_wing.transform.M[12] += offset_q_rw.getX();
	rear_wing.transform.M[13] += offset_q_rw.getY();
	rear_wing.transform.M[14] += offset_q_rw.getZ();
	rear_wing.color = Black; 
	rear_wing.Render(); 

	// rear wing support

	Cube rear_wing_support(info.rear_wing_support_size.x, info.rear_wing_support_size.y, info.rear_wing_support_size.z);

	vehicle->getChassisWorldTransform().getOpenGLMatrix(&rear_wing_support.transform);

	btVector3 offset_q_rw_sup(info.rear_wing_support_offset.x, info.rear_wing_support_offset.y, info.rear_wing_support_offset.z);
	offset_q_rw_sup = offset_q_rw_sup.rotate(q.getAxis(), q.getAngle());

	rear_wing_support.transform.M[12] += offset_q_rw_sup.getX();
	rear_wing_support.transform.M[13] += offset_q_rw_sup.getY();
	rear_wing_support.transform.M[14] += offset_q_rw_sup.getZ(); 
	rear_wing_support.color = Black;
	
	
	rear_wing_support.Render();



	// rear wing flag right

	Cube rear_wing_flag_right(info.rear_wing_flag_size.x, info.rear_wing_flag_size.y, info.rear_wing_flag_size.z);

	vehicle->getChassisWorldTransform().getOpenGLMatrix(&rear_wing_flag_right.transform);
	btVector3 offset_q_rw_flag_r(info.rear_wing_flag_offset.x, info.rear_wing_flag_offset.y, info.rear_wing_flag_offset.z);
	offset_q_rw_flag_r = offset_q_rw_flag_r.rotate(q.getAxis(), q.getAngle());

	rear_wing_flag_right.transform.M[12] += offset_q_rw_flag_r.getX();
	rear_wing_flag_right.transform.M[13] += offset_q_rw_flag_r.getY();
	rear_wing_flag_right.transform.M[14] += offset_q_rw_flag_r.getZ();
	rear_wing_flag_right.color = Red;
	rear_wing_flag_right.Render();


	// rear wing flag left

	Cube rear_wing_flag_left(info.rear_wing_flag_size.x, info.rear_wing_flag_size.y, info.rear_wing_flag_size.z);

	vehicle->getChassisWorldTransform().getOpenGLMatrix(&rear_wing_flag_left.transform);
	btVector3 offset_q_rw_flag_l(-info.rear_wing_flag_offset.x, info.rear_wing_flag_offset.y, info.rear_wing_flag_offset.z);
	offset_q_rw_flag_l = offset_q_rw_flag_l.rotate(q.getAxis(), q.getAngle());

	rear_wing_flag_left.transform.M[12] += offset_q_rw_flag_l.getX();
	rear_wing_flag_left.transform.M[13] += offset_q_rw_flag_l.getY();
	rear_wing_flag_left.transform.M[14] += offset_q_rw_flag_l.getZ();
	rear_wing_flag_left.color = Red;
	rear_wing_flag_left.Render();


	/*// front light right
	Cylinder front_light_right(info.front_light_radius_height.x, info.front_light_radius_height.y);


	vehicle->getChassisWorldTransform().getOpenGLMatrix(&front_light_right.transform);
	btVector3 offset_f_lig_r(-info.front_light_offset.x, info.front_light_offset.y, info.front_light_offset.z);
	offset_f_lig_r = offset_f_lig_r.rotate(q.getAxis(), q.getAngle());
	
	

	front_light_right.transform.M[12] += offset_f_lig_r.getX();
	front_light_right.transform.M[13] += offset_f_lig_r.getY();
	front_light_right.transform.M[14] += offset_f_lig_r.getZ();
	front_light_right.color = Red;
	front_light_right.Render();*/


	// ground

    Cube ground(5000, 0.05, 5000); 
	ground.color = { 0.9f, 0.9f, 0.9f, 1.0f }; 
	ground.Render(); 

}

// ----------------------------------------------------------------------------
void PhysVehicle3D::ApplyEngineForce(float force)
{
	for(int i = 0; i < vehicle->getNumWheels(); ++i)
	{
		if(info.wheels[i].drive == true)
		{
			vehicle->applyEngineForce(force, i);
		}
	}
}

// ----------------------------------------------------------------------------
void PhysVehicle3D::Brake(float force)
{
	for(int i = 0; i < vehicle->getNumWheels(); ++i)
	{
		if(info.wheels[i].brake == true)
		{
			vehicle->setBrake(force, i);
		}
	}
}

// ----------------------------------------------------------------------------
void PhysVehicle3D::Turn(float degrees)
{
	for(int i = 0; i < vehicle->getNumWheels(); ++i)
	{
		if(info.wheels[i].steering == true)
		{
			vehicle->setSteeringValue(degrees, i);
		}
	}
}

// ----------------------------------------------------------------------------
float PhysVehicle3D::GetKmh() const
{
	return vehicle->getCurrentSpeedKmHour();
}

vec3 PhysVehicle3D::GetForwardVector() const
{
	return { vehicle->getForwardVector().getX(), vehicle->getForwardVector().getY(),vehicle->getForwardVector().getZ() };
}