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

	wheel.color = Red; //  Blue;

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

	Cube chassis(info.chassis_size.x, info.chassis_size.y, info.chassis_size.z);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&chassis.transform);
	btQuaternion q = vehicle->getChassisWorldTransform().getRotation();
	btVector3 offset(info.chassis_offset.x, info.chassis_offset.y, info.chassis_offset.z);
	offset = offset.rotate(q.getAxis(), q.getAngle());

	chassis.transform.M[12] += offset.getX();
	chassis.transform.M[13] += offset.getY();
	chassis.transform.M[14] += offset.getZ();


	chassis.Render();


	// rear wing 

	Cube rear_wing(info.rear_wing_size.x, info.rear_wing_size.y, info.rear_wing_size.z);

	vehicle->getChassisWorldTransform().getOpenGLMatrix(&rear_wing.transform);
	btVector3 offset_q_rw(info.rear_wing_offset.x, info.rear_wing_offset.y, info.rear_wing_offset.z);
	offset_q_rw = offset_q_rw.rotate(q.getAxis(), q.getAngle());

	rear_wing.transform.M[12] += offset_q_rw.getX();
	rear_wing.transform.M[13] += offset_q_rw.getY();
	rear_wing.transform.M[14] += offset_q_rw.getZ();
	rear_wing.color = Green; 
	rear_wing.Render(); 

	// rear wing support

	/*Cube rear_wing_support(info.rear_wing_support_size.x, info.rear_wing_support_size.y, info.rear_wing_support_size.z);

	vehicle->getChassisWorldTransform().getOpenGLMatrix(&rear_wing_support.transform);
	rear_wing_support.transform.rotate(-200, (0, 1, 0));

	btVector3 offset_q_rw_sup(info.rear_wing_support_offset.x, info.rear_wing_support_offset.y, info.rear_wing_support_offset.z);
	offset_q_rw_sup = offset_q_rw_sup.rotate(q.getAxis(), q.getAngle());

	rear_wing_support.transform.M[12] += offset_q_rw_sup.getX();
	rear_wing_support.transform.M[13] += offset_q_rw_sup.getY();
	rear_wing_support.transform.M[14] += offset_q_rw_sup.getZ();
	rear_wing_support.color = Green;
	rear_wing_support.Render();*/






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