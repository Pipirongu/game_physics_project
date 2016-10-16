//------------------------------------------------------------------------------
/**
	Application class used for example application.
	
	(C) 2015 Individual contributors, see AUTHORS file
*/
//------------------------------------------------------------------------------
#pragma once

#include "core/app.h"
#include "render/window.h"
#include "root.h"
#include "rigidbody.h"
#include "camera.h"
#include "mesh.h"
#include "texture.h"
#include "light.h"
#include "skybox.h"
#include "shadermanager.h"
#include "debugrenderer.h"
#include "fbo.h"
#include "mypersonalmathlib.h"
#include "textrenderer.h"
#include <unordered_set>
#include <algorithm>
#include <limits>
#include "contact.h"

namespace Graphics
{
class RenderScene : public Core::App
{
public:
	/// destructor
	~RenderScene();

	///Static function to get the static instance
	static RenderScene* Instance();

	/// open app
	bool Open();
	/// run app
	void Run();

private:
	std::string fps;
	std::string pos;
	std::string objects;

	//Physics stuff begins
	//sorted xyz lists for broadphase sort and sweep
	std::vector<SweepValue*> x_axis;
	std::vector<SweepValue*> y_axis;
	std::vector<SweepValue*> z_axis;

	//Collision pairs in each axis for broadphase
	std::unordered_set<std::pair<RigidBody*, RigidBody*>> x_axis_collision;
	std::unordered_set<std::pair<RigidBody*, RigidBody*>> y_axis_collision;
	std::unordered_set<std::pair<RigidBody*, RigidBody*>> z_axis_collision;

	std::unordered_set<std::pair<RigidBody*, RigidBody*>> broadphase_collision_list;
	Vector3 gravity = Vector3(0.f, 9.f, 0.f);
	//Physics stuff ends

	bool is_open = false;
	bool is_picking_mode = true;
	bool is_paused = true;
	bool sort_swap = false;
	int debug_rendering_toggle = 2;
	int wireframe_rendering_toggle = -1;

	//view, projection matrices
	Matrix44 view;
	Matrix44 projection;

	//Nodes
	Root* root;
	Camera* camera;
	Mesh* skybox_mesh;
	Mesh* rigidbody_mesh;
	Mesh* plane_mesh;
	Mesh* plank_mesh;
	Texture* rigidbody_texture;
	Light* light;
	Skybox* skybox;
	Texture* skybox_texture;
	
	//map this
	std::map<int, RigidBody*> object_list;
	RigidBody* selected_object = nullptr;

	float delta_time;
	float physics_timestep = 1.f / 60.f;
	float physics_time_accumulator = 0.0f;

	double current_time;
	double fps_timer;
	int window_width, window_height;

	
	FBO fbo;
	Display::Window* window;

	/// Constructor
	RenderScene();
	///Private copy constructor to prevent creating an instance other than calling the Instance() function
	RenderScene(const RenderScene&);
	///Private assignment constructor to prevent creating an instance other than calling the Instance() function
	RenderScene& operator=(const RenderScene&);

	void AddCubeInfrontOfCamera(bool is_kinematic);
	void AddCubeToScene(bool is_kinematic, Vector3 position, float degrees = 0, Vector3 axis = Vector3(1, 1, 1));
	void AddPlankToScene(bool is_kinematic, Vector3 position, float degrees = 0, Vector3 axis = Vector3(1, 1, 1));
	void AddPlaneToScene(bool is_kinematic, Vector3 position, float degrees = 0, Vector3 axis = Vector3(1, 1, 1));
	void PickingPass();
	void RenderPass();
	void CameraControls();

	//physics stuff begins
	//Broadphase functions
	void BroadPhase();
	void SortAndSweep(std::unordered_set<std::pair<RigidBody*, RigidBody*>> &out_axis_collision, std::vector<SweepValue*> &axis_list, int axis);
	//Checks if broadphase collision occurs in all axes and updates the broadphase collision list
	void Intersect();

	//Narrowphase functions
	void NarrowPhase();
	bool SAT_TestBoxBox(RigidBody* a, RigidBody* b, Vector3 &smallest_axis, float &smallest_penetration, int &axis_number, int& bestSingleAxis);
	bool OverlapOnAxis(RigidBody* a, RigidBody* b, Vector3 axis, Vector3 center_vector, Vector3 &smallest_axis, float &smallest_penetration, int &axis_number, int axis_index);
	float ProjectOBBToAxis(OBB* box, Vector3 axis);
	void GenerateContactPoints(RigidBody* a, RigidBody* b, Vector3 &smallest_axis, float &smallest_penetration, int axis_number, std::unordered_set<Contact*>& contact_points, int& bestSingleAxis);
	void CalculateContacts_Face(Vector3 center_vector, RigidBody* a, RigidBody* b, Vector3 &smallest_axis, float &smallest_penetration, int axis_number, Vector3* reference_face, Vector3* incident_face, std::unordered_set<Contact*>& contact_points);
	//Creates side planes(normals and offsets) for a reference body. Every plane except the reference plane aka face the collision occurs at. A normal can represent two planes by inverting it.
	void CreateReferenceSidePlanes(RigidBody* reference_body, int &axis_number, Vector3& normal1, Vector3& normal2, float& neg_offset1, float& pos_offset1, float& neg_offset2, float& pos_offset2);
	void FlipSmallestAxis(Vector3& smallest_axis, Vector3 center_vector);
	//Dot all incident body axes with reference normal. Most negative is the incident_normal
	void CalculateIncidentAxis(RigidBody* incident_body, Vector3& reference_normal, Vector3& incident_normal);
	void CalcFaceVertices(Vector3* vertices, Vector3 axis, RigidBody *incident_body);
	void ClipFaceToSidePlane(std::vector<Contact> contacts_to_clip, const Vector3& normal, float plane_offset, std::vector<Contact> &contacts);
	//Only keep contact points that is behind or at a side plane
	void FilterContactPoints(RigidBody* one, RigidBody* two,
							std::vector<Contact>& contacts,
							const Vector3& ref_plane, float ref_offset, 
							const Vector3& side_plane1, float side_offset1, 
							const Vector3& side_plane2, float side_offset2,
							const Vector3& side_plane3, float side_offset3,
							const Vector3& side_plane4, float side_offset4,
							std::unordered_set<Contact*>& contacts_points);
	void CalculateContacts_EdgeEdge(Vector3 center_vector, RigidBody* a, RigidBody* b, Vector3& smallest_axis, float smallest_penetration, int axis_number, std::unordered_set<Contact*>& contacts_points, int& bestSingleAxis);
	Vector3 contactPoint(const Vector3 &pOne, const Vector3 &dOne, float oneSize, const Vector3 &pTwo, const Vector3 &dTwo, float twoSize, bool useOne);


	void ProcessContact(Contact* contact, float& Pn, Vector3&vel1, Vector3& ang_vel1, Vector3&vel2, Vector3& ang_vel2);
	void GetSupportEdge(RigidBody* a, Vector3 &mtv, Vector3* support_out);
	void GetContactPoint_EdgeEdge(std::vector<Vector3> &contact_points_out, const Vector3 &PA, const Vector3 &QA, const Vector3 &PB, const Vector3 &QB);

	void Solver(Contact* contact);

	int TestAABB_AABB(AABB *a, AABB *b);
	void DrawSidePlanes(const Vector3& normal1, const Vector3& normal2, const Vector3& onePosition, int index1, int index2, const Vector3& oneHalfSize);
	
	// Compares two floats to see if a is greater than b by a slight margin of error. Small
	// samples from both a and b are used in the comparison to bias the result in one direction
	// in order to stray away from mixed results due to floating point error.
	inline bool BiasGreaterThan(float a, float b)
	{
		const float k_biasRelative = 0.95f;
		const float k_biasAbsolute = 0.01f;

		// >= instead of > for NaN comparison safety
		return a >= b * k_biasRelative + a * k_biasAbsolute;
	}
	void PositionalCorrection(RigidBody* one, RigidBody* two, float smallest_penetration, Vector3 smallest_axis);
	void SelectedObjectControls();
	//physics stuff ends

	void SetScene(std::string scene);
	void ClearScene();
};

} // namespace Example
