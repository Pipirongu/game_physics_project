#pragma once

#include "mypersonalmathlib.h"
#include "rigidbody.h"

/**
@struct Contact

Stores a contact, normal and penetration
*/
struct Contact
{
	Vector3 position;
	Vector3 normal;
	float penetration = -1;
	float accumulated_impulse = 0.f;
	RigidBody* one = nullptr;
	RigidBody* two = nullptr;
};
//
////std specialization
//namespace std {
//
//	// hash key, makes sure contacts that is considered the same(same position) are put in the same bucket(collision)
//	template <>
//	class hash < Contact* >
//	{
//	public:
//		size_t operator()(const Contact* val) const {
//			return val->position.a ^ val->position.b ^ val->position.c;
//			//return val->position.SquareMagnitude() - 0.001f;//works
//
//		}
//	};
//
//	// used whenever there's a collision(a contact with same hash as another, i.e put in the same bucket)
//	// each object must have a unique key. this equality test check if the values are the same and not add to the unordered_set
//	template <>
//	struct equal_to<Contact*>
//	{
//		inline bool operator()(const Contact* lhs, const Contact* rhs) const
//		{
//			return (lhs->position == rhs->position);
//		}
//	};
//}