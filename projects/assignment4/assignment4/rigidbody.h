#pragma once

#include "node.h"
#include "mesh.h"
#include "texture.h"
#include "mypersonalmathlib.h"
#include "debugrenderer.h"
#include "physicsmanager.h"
#include "aabb.h"
#include "obb.h"
#include <vector>

/**
@class RigidBody

RigidBody class
*/
class RigidBody : public Node
{
public:
	/// Used by Rotate functions to decide how the object should rotate
	enum RotateAround
	{
		Self,
		World
	};
	
	/// Constructor
	RigidBody();
	/// Destructor
	~RigidBody(void);
	
	/// Updates the model/view matrix and then calls its childrens' update
	void Update(Matrix44& view, const Matrix44& model = Matrix44());
	void UpdateAABB();
	void UpdateOBB();

	void DrawPass1(const Matrix44& projection, const Matrix44& view);
	/// Does nothing with this class. Just calls its childrens' update and forwards the arguments
	void Draw(const Matrix44& projection, const Matrix44& view);
	void DrawAABB(const Matrix44& projection, const Matrix44& view);
	void DrawOBB(const Matrix44& projection, const Matrix44& view);

	void Integrate(float timestep);
	void ApplyImpulse(Vector3 force, Vector3 picking_point);
	void SetInertiaTensor(const Matrix33& inertia_tensor);
	void SetCenterOfMass(const Vector3 &center_of_mass);

	//set for mesh, texture
	void SetMesh(Mesh* mesh);
	void SetID();
	void SetTexture(Texture* texture);
	int ID();
	void SetPickingColor(Vector3 color);
	
	/// Returns the model
	Matrix44 GetModel();

	/// Multiply the transform's rotation matrix with a Rotation matrix around an arbitrary axis
	void Rotate(float angle, int x, int y, int z, RotateAround space = Self);
	/// Multiply the transform's rotation matrix with a Rotation matrix around an arbitrary axis
	void Rotate(float angle, Vector3 vector, RotateAround space = Self);

	/// Multiply the transform's translation matrix with a Translation matrix
	void Translate(float x, float y, float z);
	/// Multiply the transform's translation matrix with a Translation matrix
	void Translate(Vector3 vector);
	
	void SetPosition( float x, float y, float z );
	void SetPosition( Vector3 vector);
	
	/// Multiply the transform's scaling matrix with a Scaling matrix. Saves the scale factor so the bounding sphere can be scaled elsewhere in the program. Scales equally for xyz axis
	void Scale(float factor);
	/// Sets the scale values in the transform's matrix to factor
	/// Sets the scaling matrix to identity before multiplying it with a Scaling matrix. Saves the scale factor so the bounding sphere can be scaled elsewhere in the program. Scales equally for xyz axis
	void SetScale(float factor);
	/// Multiply the scaling matrix with a Scaling matrix. Doesn't have to scale uniformly
	void Scale(float x, float y, float z);
	/// Multiply the scaling matrix with a Scaling matrix. Doesn't have to scale uniformly
	void Scale (Vector3 vector);

	/// inverted camera lookat function for objects
	void LookAt(Vector3 eye_position, Vector3 eye_target, Vector3 eye_up);
	void SetMass(float mass);

	Mesh* mesh;
	Texture* texture;
	AABB* aabb;
	OBB* obb;
	Vector3 aabb_color;
	Vector3 obb_color;

	//My Physics Stuff
	float mass = 1.f;
	float inverse_mass = 1.f/1.f;
	float restitution = 0.f;
	Vector3 center_of_mass;
	Vector3 position;
	Vector3 velocity;
	Vector3 angular_velocity;
	float linear_damping = 0.85f;
	float angular_damping = 0.85f;
	bool is_kinematic = false;
	Vector3 accum_force;
	Vector3 accum_torque;
	Matrix33 inverse_inertia_tensor;
	Matrix33 inverse_inertia_tensor_world_space;
	Quaternion orientation;
	Vector3 acceleration;
	Vector3 angular_acceleration;


	Matrix44 model;
	//Holds the rotation values of this transform, applied after translation. So rotation speed around planets can be changed
	Matrix44 rotation;
	//Holds the scaling values of this transform
	Matrix44 scaling;
	//Rotation matrix applied first to get effect "rotate around own axis"
	Matrix44 rotation_around_own_axis;

	//A Combination of Translation/Rotation matrices
	Matrix44 look_at;

private:
	Vector3 picking_color;

	int id;
	
	//Material Properties
	Vector3 ambient_material;
	Vector3 specular_material;
	float shininess;


};

//std specialization
namespace std {
	template <>
	struct hash<std::pair<RigidBody*, RigidBody*>>
	{
		inline size_t operator()(const std::pair<RigidBody*, RigidBody*> &v) const
		{
			return (v.first->ID() + v.second->ID());
		}
	};

	template <>
	struct equal_to<std::pair<RigidBody*, RigidBody*>>
	{
		inline bool operator()(const std::pair<RigidBody*, RigidBody*>& lhs, const std::pair<RigidBody*, RigidBody*>& rhs) const
		{
			return (lhs.first == rhs.first && lhs.second == rhs.second ||
				lhs.first == rhs.second && lhs.second == rhs.first);
		}
	};
}