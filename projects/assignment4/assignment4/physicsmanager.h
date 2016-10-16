#pragma once

//#include "core/app.h"
//#include "render/window.h"
//#include "root.h"
//#include "rigidbody.h"
//#include "camera.h"
//#include "mesh.h"
//#include "texture.h"
//#include "light.h"
//#include "skybox.h"
//#include "shadermanager.h"
//#include "debugrenderer.h"
//#include "fbo.h"
#include "mypersonalmathlib.h"
//#include "textrenderer.h"
//#include <unordered_set>
//#include <algorithm>
//#include <limits>

class PhysicsManager
{
public:
	/// destructor
	~PhysicsManager();

	///Static function to get the static instance
	static PhysicsManager* Instance();

	Vector3 gravity;
	///// open app
	//bool Open();
	///// run app
	//void Run();

private:

	////physics stuff
	////sorted xyz lists
	//std::vector<SweepValue*> x_axis;
	//std::vector<SweepValue*> y_axis;
	//std::vector<SweepValue*> z_axis;

	//std::unordered_set<std::pair<RigidBody*, RigidBody*>> x_axis_collision;
	//std::unordered_set<std::pair<RigidBody*, RigidBody*>> y_axis_collision;
	//std::unordered_set<std::pair<RigidBody*, RigidBody*>> z_axis_collision;

	//std::unordered_set<std::pair<RigidBody*, RigidBody*>> broadphase_collision_list;
	//std::unordered_set<std::pair<RigidBody*, RigidBody*>> narrowphase_collision_list;

	//bool is_open = false;
	//bool is_picking_mode = true;
	//bool is_paused = true;
	//bool sort_swap = false;
	//int debug_rendering_toggle = 0;

	////view, projection matrices
	//Matrix44 view;
	//Matrix44 projection;

	////Nodes
	//Root* root;
	//Camera* camera;
	//Mesh* skybox_mesh;
	//Mesh* rigidbody_mesh;
	//Mesh* plane_mesh;
	//Texture* rigidbody_texture;
	//Light* light;
	//Skybox* skybox;
	//Texture* skybox_texture;
	//
	////map this
	//std::map<int, RigidBody*> object_list;
	//RigidBody* selected_object = nullptr;

	//float delta_time;
	//double last_time;
	//double fps_timer;
	//int window_width, window_height;

	//
	//FBO fbo;
	//Display::Window* window;

	/// Constructor
	PhysicsManager();
	///Private copy constructor to prevent creating an instance other than calling the Instance() function
	PhysicsManager(const PhysicsManager&);
	///Private assignment constructor to prevent creating an instance other than calling the Instance() function
	PhysicsManager& operator=(const PhysicsManager&);

	//void AddCubeToScene();
	//void AddPlaneToScene();
	//void PickingPass();
	//void RenderPass();
	//void CameraControls();

	////physics stuff
	//void BroadPhase();
	//void NarrowPhase();
	//bool SAT_TestBoxBox(RigidBody* a, RigidBody* b, Vector3 &mtv, float &overlap, std::vector<Vector3> &contact_points_out);
	//bool OverlapOnAxis(RigidBody* a, RigidBody* b, Vector3 axis, Vector3 center_vector, Vector3 &mtv, float &overlap, int &type_of_collision_out, int axis_index);
	//float ProjectOBBToAxis(OBB* box, Vector3 axis);

	//void SortAndSweep(std::unordered_set<std::pair<RigidBody*, RigidBody*>> &out_axis_collision, std::vector<SweepValue*> &axis_list, int axis);
	//void Intersect();
	//int TestAABB_AABB(AABB *a, AABB *b);
	//void GetSupportEdge(RigidBody* a, Vector3 &mtv, Vector3* support_out);
	//void GetContactPoint_EdgeEdge(std::vector<Vector3> &contact_points_out, const Vector3 &PA, const Vector3 &QA, const Vector3 &PB, const Vector3 &QB);
	//void ResolveCollision(RigidBody* a, RigidBody* b, std::vector<Vector3> &contact_points, const Vector3 &mtv);

	//// Compares two floats to see if a is greater than b by a slight margin of error. Small
	//// samples from both a and b are used in the comparison to bias the result in one direction
	//// in order to stray away from mixed results due to floating point error.
	//inline bool BiasGreaterThan(float a, float b)
	//{
	//	const float k_biasRelative = 0.95f;
	//	const float k_biasAbsolute = 0.01f;

	//	// >= instead of > for NaN comparison safety
	//	return a >= b * k_biasRelative + a * k_biasAbsolute;
	//}

	//void ClipFaceToSidePlane(std::vector<Vector3> &Out, const Vector3* face, const Vector3& normal, float plane_offset);
	//void CalcFaceVertices(Vector3* vertices, Vector3 axis, RigidBody *incident_body);
};