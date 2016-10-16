//------------------------------------------------------------------------------
// exampleapp.cc
// (C) 2015 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "config.h"
#include "renderscene.h"

#include <cstring>

using namespace Display;
namespace Graphics
{

//------------------------------------------------------------------------------
/**
*/
RenderScene::RenderScene()
{
	//Sets radix character to . and not ,
	setlocale(LC_ALL, "POSIX");

}

//------------------------------------------------------------------------------
/**
*/
RenderScene::~RenderScene()
{
	delete this->root;
	delete this->camera;
	delete this->light;
	delete this->rigidbody_mesh;
	delete this->rigidbody_texture;
	delete this->skybox;
	delete this->skybox_mesh;
	delete this->skybox_texture;
	
	for (std::map<int, RigidBody*>::iterator it = this->object_list.begin(); it != this->object_list.end(); it++){
		delete it->second;
	}
}

//------------------------------------------------------------------------------
/**
*/
RenderScene* RenderScene::Instance()
{
	static RenderScene instance;

	return &instance;
}

//------------------------------------------------------------------------------
/**
*/
bool
RenderScene::Open()
{
	App::Open();
	this->window = new Display::Window;

	// key callback
	this->window->SetKeyPressFunction([this](int32 key, int32 scancode, int32 action, int32 mods)
	{
		if (key == GLFW_KEY_1 && action == GLFW_PRESS){
			this->SetScene("Plank");
		}
		if (key == GLFW_KEY_2 && action == GLFW_PRESS){
			this->SetScene("StackOfBoxes");
		}
		if (key == GLFW_KEY_3 && action == GLFW_PRESS){
			this->SetScene("SlidingBoxes");
		}

		if (key == GLFW_KEY_4 && action == GLFW_PRESS){
			this->wireframe_rendering_toggle++;
			if (this->wireframe_rendering_toggle == 2){
				this->wireframe_rendering_toggle = 0;
			}
			if (this->wireframe_rendering_toggle == 0){
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			}
			else if (this->wireframe_rendering_toggle == 1){
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			}
		}
		if (key == GLFW_KEY_E && action == GLFW_PRESS){
			this->AddCubeInfrontOfCamera(false);
		}
		if (key == GLFW_KEY_Q && action == GLFW_PRESS){
			if (this->debug_rendering_toggle == 3){
				this->debug_rendering_toggle = 0;
			}
			else{
				this->debug_rendering_toggle++;
			}
		}
		if (key == GLFW_KEY_SPACE && action == GLFW_PRESS){
			if (this->is_paused){
				this->is_paused = false;
			}
			else{
				this->is_paused = true;
			}
		}
		if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS){
			this->is_open = false;
		}
	});

	// close callback
	this->window->SetCloseFunction([this]()
	{
		this->is_open = false;
	});

	// window resize callback
	this->window->SetWindowSizeFunction([this](int width, int height)
	{
		if (width != 0 || height != 0){
			this->window->GetWindowSize(&this->window_width, &this->window_height);
			this->window->SetSize(this->window_width, this->window_height);
			float aspect = (float)this->window_width / (float)this->window_height;
			this->projection = Matrix44::Perspective(60, aspect, 0.1, 1000);
			TextRenderer::Instance()->SetProjection(Matrix44::Ortho(0.0f, this->window_width, 0.0f, this->window_height, -1, 1));
			this->fbo.UpdateTextureDimensions(this->window_width, this->window_height);
		}
	});

	
	//init time
	this->current_time = glfwGetTime();
	this->fps_timer = 0;

	if (this->window->Open())
	{
		glfwSwapInterval(0);

		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);
		glEnable(GL_CULL_FACE);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	
		ShaderManager::Instance()->AddShaderProgram("shaders/simple_vs.glsl", "shaders/simple_fs.glsl", "main");
		ShaderManager::Instance()->AddShaderProgram("shaders/picking_vs.glsl", "shaders/picking_fs.glsl", "picking");
		ShaderManager::Instance()->AddShaderProgram("shaders/skybox_vs.glsl", "shaders/skybox_fs.glsl", "skybox");
		ShaderManager::Instance()->AddShaderProgram("shaders/debug_vs.glsl", "shaders/debug_fs.glsl", "debug");
		ShaderManager::Instance()->AddShaderProgram("shaders/text_vs.glsl", "shaders/text_fs.glsl", "text");

		//Objects
		this->root = new Root;
		this->camera = new Camera;
		
		this->rigidbody_mesh = new Mesh;
		this->rigidbody_mesh->LoadOBJ("models/cube.obj");
		this->rigidbody_texture = new Texture;
		this->rigidbody_texture->LoadTexture("textures/cube.png");

		//plane
		this->plane_mesh = new Mesh;
		this->plane_mesh->LoadOBJ("models/plane.obj");

		//plank
		this->plank_mesh = new Mesh;
		this->plank_mesh->LoadOBJ("models/plank.obj");

		this->skybox_mesh = new Mesh;
		this->skybox_mesh->LoadOBJ("models/cube.obj");
		this->skybox = new Skybox;
		this->skybox_texture = new Texture;
		this->skybox_texture->LoadSkyboxTexture
			("textures/skybox/skybox_back.png",
			"textures/skybox/skybox_down.png",
			"textures/skybox/skybox_front.png",
			"textures/skybox/skybox_left.png",
			"textures/skybox/skybox_right.png",
			"textures/skybox/skybox_up.png");

		this->skybox->SetMesh(this->skybox_mesh);
		this->skybox->SetTexture(this->skybox_texture);
		this->skybox->Scale(150);
		
		this->light = new Light;

		this->root->AddChildNode(this->camera);
		this->root->AddChildNode(this->light);
		this->camera->AddChildNode(this->skybox);

		//Init the camera vectors just so the objects are rendered in the scene
		camera->UpdateCamera();

		// load projection matrix first time because resize callback won't be called until user resizes window.
		this->window->GetWindowSize(&this->window_width, &this->window_height);
		
		float aspect = (float)this->window_width / (float)this->window_height;
		this->projection = Matrix44::Perspective(60, aspect, 0.1, 1000);

		this->fbo.Init(this->window_width, this->window_height);
		TextRenderer::Instance()->Init("fonts/font.ttf", 18);
		TextRenderer::Instance()->SetProjection(Matrix44::Ortho(0.0f, this->window_width, 0.0f, this->window_height, -1, 1));

		this->is_open = true;
		return true;
	}
	return false;
}

//------------------------------------------------------------------------------
/**
*/
void
RenderScene::Run()
{
	while(this->is_open)
	{		
		//deltatime
		double new_time = glfwGetTime();
		this->delta_time = float(new_time - this->current_time);
		float frame_time = this->delta_time;
		this->current_time = new_time;
		this->physics_time_accumulator += frame_time;

		//Poll for key input
		this->window->Update();
		//Moves the player
		this->CameraControls();
		this->SelectedObjectControls();
		this->skybox->Rotate(1*this->delta_time, 0.f, 1.f, 0.f);
		

		/*
		Fixed timestep; updates physics only when time accumulator is timestep. 
		Handles low fps by integrating multiple times.
		If we get uneven fps so that there is some time left in the accumulator. It is then saved for the next frame where the accumulator fills up
		1. Integrate
		2. Test for Collision
		3. Respond to Collision
		*/
		while (this->physics_time_accumulator >= this->physics_timestep)
		{
			this->physics_time_accumulator -= this->physics_timestep;

			this->PickingPass();
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			this->BroadPhase();
			this->NarrowPhase();


			if (this->is_paused){
				//Integrate
				for (std::map<int, RigidBody*>::iterator it = this->object_list.begin(); it != this->object_list.end(); it++){
					it->second->Integrate(this->physics_timestep);
				}
			}

			//Update matrices
			this->root->Update(this->view);

			for (std::map<int, RigidBody*>::iterator it = this->object_list.begin(); it != this->object_list.end(); it++){
				Matrix33 rot = it->second->rotation_around_own_axis.ConvertToMatrix33();
				Matrix33 rotT = rot.Transposed();

				it->second->inverse_inertia_tensor_world_space = rot * it->second->inverse_inertia_tensor * rotT;
			}

			//Update AABB
			for (std::map<int, RigidBody*>::iterator it = this->object_list.begin(); it != this->object_list.end(); it++){
				it->second->UpdateOBB();
				it->second->UpdateAABB();
			}

		}

		this->RenderPass();

		ShaderManager::Instance()->ChangeShader("text");

		if ((current_time - this->fps_timer) > 0.2f){
			this->fps = std::to_string((int)(1 / this->delta_time)) + " FPS";
			this->pos = "[" + std::to_string((int)camera->position[0]) + "," + std::to_string((int)camera->position[1]) + "," + std::to_string((int)camera->position[2]) + "]";
			this->objects = "Objects: " + std::to_string(this->object_list.size());
			this->fps_timer = current_time;
		}

		TextRenderer::Instance()->SetColor(1, 1, 1);
		TextRenderer::Instance()->RenderText(fps, 20.f, this->window_height - 25.f, 1);
		TextRenderer::Instance()->SetColor(1, 1, 0);
		TextRenderer::Instance()->RenderText(objects, 20.f, this->window_height - 50.f, 1);
		TextRenderer::Instance()->RenderText("Toggle Scenes: 1-3", 20.f, this->window_height - 100.f, 1);
		TextRenderer::Instance()->RenderText("Toggle Wireframe: 4", 20.f, this->window_height - 125.f, 1);
		TextRenderer::Instance()->RenderText("Pause: Spacebar", 20.f, this->window_height - 150.f, 1);
		TextRenderer::Instance()->RenderText("Toggle Bounding Box: Q", 20.f, this->window_height - 175.f, 1);
		TextRenderer::Instance()->RenderText("Spawn Cube: E", 20.f, this->window_height - 200.f, 1);

		this->window->SwapBuffers();

	}
	this->window->Close();
}

void RenderScene::AddCubeInfrontOfCamera(bool is_kinematic)
{
	RigidBody* rigidbody = new RigidBody;
	rigidbody->SetPosition(camera->position + camera->direction.Normalized() * 5);

	rigidbody->is_kinematic = is_kinematic;
	if (is_kinematic){
		rigidbody->SetMass(0.f);
	}


	rigidbody->SetMesh(this->rigidbody_mesh);
	rigidbody->SetTexture(this->rigidbody_texture);
	Vector3 dimensions = rigidbody->mesh->CalculateDimensions();
	rigidbody->SetInertiaTensor(Matrix33::InertiaTensor(rigidbody->mass, dimensions).Inverse());
	rigidbody->SetCenterOfMass(rigidbody->mesh->CalculateCenterOfMass());

	rigidbody->aabb = rigidbody->mesh->InitAABB();
	rigidbody->aabb->object = rigidbody;

	/************************************************************************/
	/* obb                                                                  */
	/************************************************************************/
	rigidbody->obb = new OBB;
	rigidbody->obb->half_extent = (rigidbody->aabb->max.value - rigidbody->aabb->min.value) / 2.f;
	rigidbody->obb->object = rigidbody;

	rigidbody->obb->InitVertexList(rigidbody->aabb->min.value, rigidbody->aabb->max.value);


	//worldspace init
	rigidbody->aabb->min.value = rigidbody->aabb->min.value + rigidbody->position;
	rigidbody->aabb->max.value = rigidbody->aabb->max.value + rigidbody->position;

	this->x_axis.push_back(&rigidbody->aabb->min);
	this->x_axis.push_back(&rigidbody->aabb->max);
	this->y_axis.push_back(&rigidbody->aabb->min);
	this->y_axis.push_back(&rigidbody->aabb->max);
	this->z_axis.push_back(&rigidbody->aabb->min);
	this->z_axis.push_back(&rigidbody->aabb->max);

	this->root->AddChildNode(rigidbody);
	this->object_list[rigidbody->ID()] = rigidbody;
}

void RenderScene::AddCubeToScene(bool is_kinematic, Vector3 position, float degrees, Vector3 axis)
{
	RigidBody* rigidbody = new RigidBody;
	rigidbody->SetPosition(position);
	rigidbody->is_kinematic = is_kinematic;
	if (is_kinematic){
		rigidbody->SetMass(0.f);
	}


	rigidbody->SetMesh(this->rigidbody_mesh);
	rigidbody->SetTexture(this->rigidbody_texture);
	Vector3 dimensions = rigidbody->mesh->CalculateDimensions();
	rigidbody->SetInertiaTensor(Matrix33::InertiaTensor(rigidbody->mass, dimensions).Inverse());
	rigidbody->SetCenterOfMass(rigidbody->mesh->CalculateCenterOfMass());

	rigidbody->aabb = rigidbody->mesh->InitAABB();
	rigidbody->aabb->object = rigidbody;

	/************************************************************************/
	/* obb                                                                  */
	/************************************************************************/
	rigidbody->obb = new OBB;
	rigidbody->obb->half_extent = (rigidbody->aabb->max.value - rigidbody->aabb->min.value) / 2.f;
	rigidbody->obb->object = rigidbody;

	rigidbody->obb->InitVertexList(rigidbody->aabb->min.value, rigidbody->aabb->max.value);


	//worldspace init
	rigidbody->aabb->min.value = rigidbody->aabb->min.value + rigidbody->position;
	rigidbody->aabb->max.value = rigidbody->aabb->max.value + rigidbody->position;

	this->x_axis.push_back(&rigidbody->aabb->min);
	this->x_axis.push_back(&rigidbody->aabb->max);
	this->y_axis.push_back(&rigidbody->aabb->min);
	this->y_axis.push_back(&rigidbody->aabb->max);
	this->z_axis.push_back(&rigidbody->aabb->min);
	this->z_axis.push_back(&rigidbody->aabb->max);

	this->root->AddChildNode(rigidbody);
	this->object_list[rigidbody->ID()] = rigidbody;
}

void RenderScene::AddPlaneToScene(bool is_kinematic, Vector3 position, float degrees, Vector3 axis)
{
	RigidBody* rigidbody = new RigidBody;
	rigidbody->SetPosition(position);
	rigidbody->Rotate(degrees, axis);
	rigidbody->is_kinematic = is_kinematic;
	if (is_kinematic){
		rigidbody->SetMass(0.f);
	}


	rigidbody->SetMesh(this->plane_mesh);
	rigidbody->SetTexture(this->rigidbody_texture);
	Vector3 dimensions = rigidbody->mesh->CalculateDimensions();
	rigidbody->SetInertiaTensor(Matrix33::InertiaTensor(rigidbody->mass, dimensions).Inverse());
	rigidbody->SetCenterOfMass(rigidbody->mesh->CalculateCenterOfMass());

	rigidbody->aabb = rigidbody->mesh->InitAABB();
	rigidbody->aabb->object = rigidbody;

	/************************************************************************/
	/* obb                                                                  */
	/************************************************************************/
	rigidbody->obb = new OBB;
	rigidbody->obb->half_extent = (rigidbody->aabb->max.value - rigidbody->aabb->min.value) / 2.f;
	rigidbody->obb->object = rigidbody;

	rigidbody->obb->InitVertexList(rigidbody->aabb->min.value, rigidbody->aabb->max.value);


	//worldspace init
	rigidbody->aabb->min.value = rigidbody->aabb->min.value + rigidbody->position;
	rigidbody->aabb->max.value = rigidbody->aabb->max.value + rigidbody->position;

	this->x_axis.push_back(&rigidbody->aabb->min);
	this->x_axis.push_back(&rigidbody->aabb->max);
	this->y_axis.push_back(&rigidbody->aabb->min);
	this->y_axis.push_back(&rigidbody->aabb->max);
	this->z_axis.push_back(&rigidbody->aabb->min);
	this->z_axis.push_back(&rigidbody->aabb->max);

	this->root->AddChildNode(rigidbody);
	this->object_list[rigidbody->ID()] = rigidbody;
}

void RenderScene::AddPlankToScene(bool is_kinematic, Vector3 position, float degrees /*= 0*/, Vector3 axis /*= Vector3(1, 1, 1)*/)
{
	RigidBody* rigidbody = new RigidBody;
	rigidbody->SetPosition(position);
	rigidbody->is_kinematic = is_kinematic;
	if (is_kinematic){
		rigidbody->SetMass(0.f);
	}


	rigidbody->SetMesh(this->plank_mesh);
	rigidbody->SetTexture(this->rigidbody_texture);
	Vector3 dimensions = rigidbody->mesh->CalculateDimensions();
	rigidbody->SetInertiaTensor(Matrix33::InertiaTensor(rigidbody->mass, dimensions).Inverse());
	rigidbody->SetCenterOfMass(rigidbody->mesh->CalculateCenterOfMass());

	rigidbody->aabb = rigidbody->mesh->InitAABB();
	rigidbody->aabb->object = rigidbody;

	/************************************************************************/
	/* obb                                                                  */
	/************************************************************************/
	rigidbody->obb = new OBB;
	rigidbody->obb->half_extent = (rigidbody->aabb->max.value - rigidbody->aabb->min.value) / 2.f;
	rigidbody->obb->object = rigidbody;

	rigidbody->obb->InitVertexList(rigidbody->aabb->min.value, rigidbody->aabb->max.value);


	//worldspace init
	rigidbody->aabb->min.value = rigidbody->aabb->min.value + rigidbody->position;
	rigidbody->aabb->max.value = rigidbody->aabb->max.value + rigidbody->position;

	this->x_axis.push_back(&rigidbody->aabb->min);
	this->x_axis.push_back(&rigidbody->aabb->max);
	this->y_axis.push_back(&rigidbody->aabb->min);
	this->y_axis.push_back(&rigidbody->aabb->max);
	this->z_axis.push_back(&rigidbody->aabb->min);
	this->z_axis.push_back(&rigidbody->aabb->max);

	this->root->AddChildNode(rigidbody);
	this->object_list[rigidbody->ID()] = rigidbody;
}

void RenderScene::PickingPass()
{
	ShaderManager::Instance()->ChangeShader("picking");
	this->fbo.Bind();
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	for (std::map<int, RigidBody*>::iterator it = this->object_list.begin(); it != this->object_list.end(); it++){
		it->second->DrawPass1(this->projection, this->view);
	}
	
	this->fbo.Unbind();
	
	//read a pixel and set color of object	
	if(this->is_picking_mode && this->window->GetMouseButton(GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS){
		//get pixel here and color selected
		this->fbo.Bind();
		glReadBuffer(GL_COLOR_ATTACHMENT0);

		Vector3 pixel;
		double x, y;
		this->window->GetCursorPos(&x, &y);
		//inverted y coordinate because glfw 0,0 starts at topleft while opengl texture 0,0 starts at bottomleft
		glReadPixels(x, this->window_height - y, 1, 1, GL_RGB, GL_FLOAT, &pixel);
		//reset color of previous selected object
		if(this->selected_object){
			this->selected_object->SetPickingColor(Vector3(0,0,0));	
		}
		//if the picked object is a pickable object make it as selected and set its color
		if(this->object_list.find((int)pixel[0]) != this->object_list.end()){
			this->selected_object = this->object_list[pixel[0]];
			this->selected_object->SetPickingColor(Vector3(0.2,0.2,0));
			
			glReadBuffer(GL_COLOR_ATTACHMENT1);
			Vector3 world_position;
			//inverted y coordinate because glfw 0,0 starts at topleft while opengl texture 0,0 starts at bottomleft
			glReadPixels(x, this->window_height - y, 1, 1, GL_RGB, GL_FLOAT, &world_position);
		

			Vector3 impulse = (world_position - (camera->position)).Normalized();
			this->selected_object->ApplyImpulse(impulse * 20.f, world_position);
		}
		//if it's an unpickable object or the background. Reset selected to nullptr.
		else{			
			this->selected_object = nullptr;
		}

		glReadBuffer(GL_NONE);
		this->fbo.Unbind();
	}

}

void RenderScene::RenderPass()
{

	ShaderManager::Instance()->ChangeShader("main");

	//Send light properties to shader
	this->light->Draw(this->projection, this->view);

	//Draw objects
	for (std::map<int, RigidBody*>::iterator it = this->object_list.begin(); it != this->object_list.end(); it++){
		it->second->Draw(this->projection, this->view);
	}

	ShaderManager::Instance()->ChangeShader("debug");
	//Draw light
	this->light->DrawDebug(this->projection, this->view);

	if (this->debug_rendering_toggle <= 2){
		//Draw bounding boxes
		for (std::map<int, RigidBody*>::iterator it = this->object_list.begin(); it != this->object_list.end(); it++){
			if (this->debug_rendering_toggle == 0 || this->debug_rendering_toggle == 1){
				it->second->DrawAABB(this->projection, this->view);
			}
			if (this->debug_rendering_toggle == 0 || this->debug_rendering_toggle == 2){
				it->second->DrawOBB(this->projection, this->view);
			}
		}
	}

	//Draw skybox
	ShaderManager::Instance()->ChangeShader("skybox");
	this->skybox->Draw(this->projection, this->view);
}

//------------------------------------------------------------------------------
/**
*/
void RenderScene::CameraControls()
{
	if (this->window->GetMouseButton(GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS){
		this->is_picking_mode = false;

		double mouse_pos_x, mouse_pos_y;
		int width, height;
		this->window->GetCursorPos(&mouse_pos_x, &mouse_pos_y);
		this->window->GetWindowSize(&width, &height);
		double mid_x = width / 2;
		double mid_y = height / 2;

		bool skip = false;
		//Prevents camera from jumping
		if (abs(mid_x - mouse_pos_x) > 50 || abs(mid_y - mouse_pos_y) > 50){
			this->window->SetCursorPos(mid_x, mid_y);
			skip = true;
		}
		if (!skip){
			camera->MouseInput(mouse_pos_x, mouse_pos_y, width, height, this->delta_time);
			//Hide the cursor
			this->window->SetCursorMode(GLFW_CURSOR_DISABLED);
			//Set the mouse pointer to the middle of the widget
			this->window->SetCursorPos(mid_x, mid_y);
		}
	}
	else if (this->window->GetMouseButton(GLFW_MOUSE_BUTTON_RIGHT) == GLFW_RELEASE){
		this->is_picking_mode = true;
		this->window->SetCursorMode(GLFW_CURSOR_NORMAL);
	}

	camera->KeyboardInput(this->window->GetKey(GLFW_KEY_W), this->window->GetKey(GLFW_KEY_S), this->window->GetKey(GLFW_KEY_A), this->window->GetKey(GLFW_KEY_D), this->delta_time);

	//updates the player
	this->camera->UpdateCameraMatrix(); //update camera matrix
}

void RenderScene::SortAndSweep(std::unordered_set<std::pair<RigidBody*, RigidBody*>> &out_axis_collision, std::vector<SweepValue*> &axis_list, int axis)
{
	int j;
	SweepValue* temp;

	for (int i = 0; i < axis_list.size(); i++){
		j = i;

		while (j > 0 && axis_list[j]->value[axis] < axis_list[j - 1]->value[axis]){
			temp = axis_list[j];

			SweepValue* left_value = axis_list[j - 1];
			SweepValue* right_value = axis_list[j];

			//if the value to the left in list(which is bigger than right value) is a max point, an overlap occurs
			if (right_value->is_min && !left_value->is_min){
				//overlap
				//add pair to axislist
				out_axis_collision.insert(std::make_pair(right_value->bounding_box->object, left_value->bounding_box->object));
				this->sort_swap = true;
				/*
				Instead of saving every overlap, we can do a full aabb/aabb check here and directly add to collision list.
				*/
			}
			else if (!right_value->is_min && left_value->is_min){
				this->sort_swap = true;
				//separating
				//remove pair from axislist
				right_value->bounding_box->object->aabb_color.Insert(0, 0, 1);
				left_value->bounding_box->object->aabb_color.Insert(0, 0, 1);

				right_value->bounding_box->object->obb_color.Insert(1, 1, 0);
				left_value->bounding_box->object->obb_color.Insert(1, 1, 0);

				out_axis_collision.erase(std::make_pair(right_value->bounding_box->object, left_value->bounding_box->object));
				this->broadphase_collision_list.erase(std::make_pair(right_value->bounding_box->object, left_value->bounding_box->object));
			}
			axis_list[j] = axis_list[j - 1];
			axis_list[j - 1] = temp;
			j--;
		}
	}
}

void RenderScene::Intersect()
{
	//Only check if a swap occurred
	if (this->sort_swap){
		for (std::unordered_set<std::pair<RigidBody*, RigidBody*>>::iterator it = this->x_axis_collision.begin(); it != this->x_axis_collision.end(); it++){
			if (this->y_axis_collision.count((*it)) && this->z_axis_collision.count((*it))){
				(*it).first->aabb_color.Insert(1, 0, 0);
				(*it).second->aabb_color.Insert(1, 0, 0);
				this->broadphase_collision_list.insert((*it));
			}
		}
		this->sort_swap = false;
	}
}

int RenderScene::TestAABB_AABB(AABB *a, AABB *b)
{
	// Exit with no intersection if separated along an axis
	if (a->max.value[0] < b->min.value[0] || a->min.value[0] > b->max.value[0]) return 0;
	if (a->max.value[1] < b->min.value[1] || a->min.value[1] > b->max.value[1]) return 0;
	if (a->max.value[2] < b->min.value[2] || a->min.value[2] > b->max.value[2]) return 0;

	// Overlapping on all axes means AABBs are intersecting
	return 1;
}

void RenderScene::BroadPhase()
{
	//Sort and Sweep
	this->SortAndSweep(this->x_axis_collision, this->x_axis, 0);
	this->SortAndSweep(this->y_axis_collision, this->y_axis, 1);
	this->SortAndSweep(this->z_axis_collision, this->z_axis, 2);

	//Find colliding pairs in all axes
	this->Intersect();
}

void RenderScene::NarrowPhase()
{
	for (std::unordered_set<std::pair<RigidBody*, RigidBody*>>::iterator it = this->broadphase_collision_list.begin(); it != this->broadphase_collision_list.end(); it++){
		Vector3 smallest_axis; //minimum translation vector from SAT.(needs to flip it depending on which side the collision happens)
		float smallest_penetration = FLT_MAX; //the smallest penetration to be used if the collision is edge-edge
		int axis_number = -1; //keep track what kind of axis is the smallest
		int bestSingleAxis;
		std::unordered_set<Contact*> contact_points; //filtered contacts

		if (this->SAT_TestBoxBox((*it).first, (*it).second, smallest_axis, smallest_penetration, axis_number, bestSingleAxis)){
			//generates a container with contacts(depends if it's from clipping then penetration is from there. Edge-edge then smallest_penetration)
			this->GenerateContactPoints((*it).first, (*it).second, smallest_axis, smallest_penetration, axis_number, contact_points, bestSingleAxis);
			//using a container with filtered contacts we resolve the pair

			float Pn;
			Vector3 changeInVel1;
			Vector3 changeInAng_Vel1;
			Vector3 changeInVel2;
			Vector3 changeInAng_Vel2;

			for (int i = 0; i < 10; i++)
			{
				for (auto& contact : contact_points)
				{
					//this->Solver(contact);
					this->ProcessContact(contact, Pn, changeInVel1, changeInAng_Vel1, changeInVel2, changeInAng_Vel2);
				}
			}

			for (auto& contact : contact_points)
			{
				Matrix44 debug_mvp = this->projection * this->view * Matrix44::Translation(contact->position);
				DebugRenderer::Instance()->DrawPoint(debug_mvp, 20, Vector3(0, 1, 0));
			}

			if (contact_points.size() > 0)
			{
				Contact* contact = *contact_points.begin();
				DebugRenderer::Instance()->DrawLine(this->view, this->projection, contact->normal, contact->two->position);
				//contact->one->velocity += changeInVel1;
				//contact->two->velocity += changeInVel2;
				//contact->one->angular_velocity += changeInAng_Vel1;
				//contact->two->angular_velocity += changeInAng_Vel2;
				//this->PositionalCorrection(contact->one, contact->two, contact->penetration, contact->normal);
			}

			(*it).first->obb_color.Insert(0, 1, 1);
			(*it).second->obb_color.Insert(0, 1, 1);

		}
		else{
			//no collision
			(*it).first->obb_color.Insert(1, 1, 0);
			(*it).second->obb_color.Insert(1, 1, 0);
		}
	}
}

bool RenderScene::SAT_TestBoxBox(RigidBody* a, RigidBody* b, Vector3 &smallest_axis, float &smallest_penetration, int &axis_number, int& bestSingleAxis)
{
	axis_number = 255;
	//center vector
	Vector3 center_vector = b->obb->pos - a->obb->pos;

	//call smallest_penetration on axis 15 times. Return true if all tests are true
	//if statements instead and generate contact points depending on the mtv also invert the mtv if it's from box two.
	//point-face
	//Box one axes
	if (this->OverlapOnAxis(a, b, a->obb->rot[0], center_vector, smallest_axis, smallest_penetration, axis_number, 0) &&
		this->OverlapOnAxis(a, b, a->obb->rot[1], center_vector, smallest_axis, smallest_penetration, axis_number, 1) &&
		this->OverlapOnAxis(a, b, a->obb->rot[2], center_vector, smallest_axis, smallest_penetration, axis_number, 2) &&
		//Box two
		this->OverlapOnAxis(a, b, b->obb->rot[0], center_vector, smallest_axis, smallest_penetration, axis_number, 3) &&
		this->OverlapOnAxis(a, b, b->obb->rot[1], center_vector, smallest_axis, smallest_penetration, axis_number, 4) &&
		this->OverlapOnAxis(a, b, b->obb->rot[2], center_vector, smallest_axis, smallest_penetration, axis_number, 5))
	{
		bestSingleAxis = axis_number;
		//edge-edge
		//9 cross products
		if (this->OverlapOnAxis(a, b, Vector3::Cross(a->obb->rot[0], b->obb->rot[0]), center_vector, smallest_axis, smallest_penetration, axis_number, 6) &&
			this->OverlapOnAxis(a, b, Vector3::Cross(a->obb->rot[0], b->obb->rot[1]), center_vector, smallest_axis, smallest_penetration, axis_number, 7) &&
			this->OverlapOnAxis(a, b, Vector3::Cross(a->obb->rot[0], b->obb->rot[2]), center_vector, smallest_axis, smallest_penetration, axis_number, 8) &&
			this->OverlapOnAxis(a, b, Vector3::Cross(a->obb->rot[1], b->obb->rot[0]), center_vector, smallest_axis, smallest_penetration, axis_number, 9) &&
			this->OverlapOnAxis(a, b, Vector3::Cross(a->obb->rot[1], b->obb->rot[1]), center_vector, smallest_axis, smallest_penetration, axis_number, 10) &&
			this->OverlapOnAxis(a, b, Vector3::Cross(a->obb->rot[1], b->obb->rot[2]), center_vector, smallest_axis, smallest_penetration, axis_number, 11) &&
			this->OverlapOnAxis(a, b, Vector3::Cross(a->obb->rot[2], b->obb->rot[0]), center_vector, smallest_axis, smallest_penetration, axis_number, 12) &&
			this->OverlapOnAxis(a, b, Vector3::Cross(a->obb->rot[2], b->obb->rot[1]), center_vector, smallest_axis, smallest_penetration, axis_number, 13) &&
			this->OverlapOnAxis(a, b, Vector3::Cross(a->obb->rot[2], b->obb->rot[2]), center_vector, smallest_axis, smallest_penetration, axis_number, 14))
		{
			return true;
		}
		else{
			return false;
		}
	}
	else{
		return false;
	}
}

void RenderScene::GenerateContactPoints(RigidBody* a, RigidBody* b, Vector3 &smallest_axis, float &smallest_penetration, int axis_number, std::unordered_set<Contact*>& contact_points, int& bestSingleAxis)
{
	Vector3 center_vector = b->position - a->position;
	//////////////////////////////////////////////////////////////////////////
	if (axis_number < 3){
		Vector3 reference_face[4]; //for debug rendering 
		Vector3 incident_face[4];

		this->CalculateContacts_Face(center_vector, a, b, smallest_axis, smallest_penetration, axis_number, reference_face, incident_face, contact_points);

	}
	else if (axis_number < 6){
		//invert the axis
		Vector3 reference_face[4]; //for debug rendering 
		Vector3 incident_face[4];

		this->CalculateContacts_Face(-1.f*center_vector, b, a, smallest_axis, smallest_penetration, axis_number - 3, reference_face, incident_face, contact_points);

	}
	else{
		this->CalculateContacts_EdgeEdge(center_vector, a, b, smallest_axis, smallest_penetration, axis_number, contact_points, bestSingleAxis);

		////edge-edge case
		//Vector3 support_edgeA[2];
		//Vector3 support_edgeB[2];
		//this->GetSupportEdge(a, smallest_axis, support_edgeA);
		//this->GetSupportEdge(b, -1 * smallest_axis, support_edgeB);
		//this->GetContactPoint_EdgeEdge(contact_points, support_edgeA[0], support_edgeA[1], support_edgeB[0], support_edgeB[1]);
	}

}

void RenderScene::CalculateContacts_Face(Vector3 center_vector, RigidBody* a, RigidBody* b, Vector3 &smallest_axis, float &smallest_penetration, int axis_number, Vector3* reference_face, Vector3* incident_face, std::unordered_set<Contact*>& contact_points)
{
	Vector3 normal1, normal2;
	float neg_offset1, pos_offset1, neg_offset2, pos_offset2;

	// wrong here maybe
	this->CreateReferenceSidePlanes(a, axis_number, normal1, normal2, neg_offset1, pos_offset1, neg_offset2, pos_offset2);
	//flip mtv
	this->FlipSmallestAxis(smallest_axis, center_vector); //center vector
	//calc incident axis
	Vector3 incident_normal;
	this->CalculateIncidentAxis(b, smallest_axis, incident_normal);
	DebugRenderer::Instance()->DrawLine(view, projection, incident_normal, b->position, 5, Vector3(1, 1, 0));

	//calc ref and inci face
	this->CalcFaceVertices(reference_face, smallest_axis, a);
	this->CalcFaceVertices(incident_face, incident_normal, b);


	//clip incident face to sideplanes
	std::vector<Contact> contacts;
	Contact contact1, contact2, contact3, contact4;
	contact1.position = incident_face[0];
	contact2.position = incident_face[1];
	contact3.position = incident_face[2];
	contact4.position = incident_face[3];

	contacts.push_back(contact1);
	contacts.push_back(contact2);
	contacts.push_back(contact3);
	contacts.push_back(contact4);

	this->ClipFaceToSidePlane(contacts, -1 * normal1, neg_offset1, contacts);
	for (auto& contact : contacts){
		Matrix44 debug_mvp = this->projection * this->view * Matrix44::Translation(contact.position);
		DebugRenderer::Instance()->DrawPoint(debug_mvp, 5, Vector3(0, 1, 1));
	}
	this->ClipFaceToSidePlane(contacts, normal1, pos_offset1, contacts);
	for (auto& contact : contacts){
		Matrix44 debug_mvp = this->projection * this->view * Matrix44::Translation(contact.position);
		DebugRenderer::Instance()->DrawPoint(debug_mvp, 5, Vector3(0, 1, 1));
	}
	this->ClipFaceToSidePlane(contacts, -1 * normal2, neg_offset2, contacts);
	for (auto& contact : contacts){
		Matrix44 debug_mvp = this->projection * this->view * Matrix44::Translation(contact.position);
		DebugRenderer::Instance()->DrawPoint(debug_mvp, 5, Vector3(0, 1, 1));
	}
	this->ClipFaceToSidePlane(contacts, normal2, pos_offset2, contacts);
	for (auto& contact : contacts){
		Matrix44 debug_mvp = this->projection * this->view * Matrix44::Translation(contact.position);
		DebugRenderer::Instance()->DrawPoint(debug_mvp, 5, Vector3(0, 1, 1));
	}

	//ref plane is the positive
	float refplane_offset = Vector3::Dot(a->obb->pos, smallest_axis);
	refplane_offset = refplane_offset + a->obb->half_extent[axis_number];

	////slower more logical way
	//Vector3 centerPointOfRefFace = oneObj->node.position + mtv*oneObj->obb.halfExtent[typeOfCollision];
	//float pos_offsettT = centerPointOfRefFace.dotAKAscalar(mtv); 

	//filtering
	this->FilterContactPoints(a, b, contacts, smallest_axis, refplane_offset, -1*normal1, neg_offset1, normal1, pos_offset1, -1*normal2, neg_offset2, normal2, pos_offset2, contact_points);

}

void RenderScene::CreateReferenceSidePlanes(RigidBody* reference_body, int &axis_number, Vector3& normal1, Vector3& normal2, float& neg_offset1, float& pos_offset1, float& neg_offset2, float& pos_offset2)
{
	//Information from Box2D Lite
	switch (axis_number)
	{
		case 0: //Reference face is on the x axis so we create side planes for the other axes
		{
			normal1 = reference_body->obb->rot[1]; //Normalize?
			normal2 = reference_body->obb->rot[2];
			float y = Vector3::Dot(reference_body->obb->pos, normal1);
			float z = Vector3::Dot(reference_body->obb->pos, normal2);
			neg_offset1 = -y + reference_body->obb->half_extent[1];
			pos_offset1 = y + reference_body->obb->half_extent[1];
			neg_offset2 = -z + reference_body->obb->half_extent[2];
			pos_offset2 = z + reference_body->obb->half_extent[2];
			/*
			//so that hax method above works below is more logical way
			neg_offset1 = (onePosition - normal1*oneHalfSize[1]).dotAKAscalar(-1.f*normal1);
			pos_offset1 = (onePosition + normal1*oneHalfSize[1]).dotAKAscalar(normal1);
			neg_offset2 = (onePosition - normal2*oneHalfSize[2]).dotAKAscalar(-1.f*normal2);
			pos_offset2 = (onePosition + normal2*oneHalfSize[2]).dotAKAscalar(normal2);
			*/

			//Draw Line here
			this->DrawSidePlanes(normal1, normal2, reference_body->obb->pos, 1, 2, reference_body->obb->half_extent);
		}
		break;

		case 1: //Reference face is on the y axis so we create side planes for the other axes
		{
			normal1 = reference_body->obb->rot[0];
			normal2 = reference_body->obb->rot[2];
			float x = Vector3::Dot(reference_body->obb->pos, normal1);
			float z = Vector3::Dot(reference_body->obb->pos, normal2);
			neg_offset1 = -x + reference_body->obb->half_extent[0];
			pos_offset1 = x + reference_body->obb->half_extent[0];
			neg_offset2 = -z + reference_body->obb->half_extent[2];
			pos_offset2 = z + reference_body->obb->half_extent[2];

			this->DrawSidePlanes(normal1, normal2, reference_body->obb->pos, 0, 2, reference_body->obb->half_extent);
		}
		break;

		case 2: //Reference face is on the z axis so we create side planes for the other axes
		{
			normal1 = reference_body->obb->rot[0];
			normal2 = reference_body->obb->rot[1];
			float x = Vector3::Dot(reference_body->obb->pos, normal1);
			float y = Vector3::Dot(reference_body->obb->pos, normal2);
			neg_offset1 = -x + reference_body->obb->half_extent[0];
			pos_offset1 = x + reference_body->obb->half_extent[0];
			neg_offset2 = -y + reference_body->obb->half_extent[1];
			pos_offset2 = y + reference_body->obb->half_extent[1];

			this->DrawSidePlanes(normal1, normal2, reference_body->obb->pos, 0, 1, reference_body->obb->half_extent);
		}
		break;
	}
}

void RenderScene::FlipSmallestAxis(Vector3& smallest_axis, Vector3 center_vector)
{
	//We got the minimum translation vector from SAT test(smallest_axis)
	//But it has one direction only because we take it as an axis of the box
	//What happens if the incident body collides on the opposite face? We need to detect that and invert(flip) the axis so it points right.
	float toCentreDist = Vector3::Dot(center_vector, smallest_axis);
	if (toCentreDist < 0) //Negative then we know it's the other direction
	{
		smallest_axis = smallest_axis * -1.0f;
	}
}

void RenderScene::CalculateIncidentAxis(RigidBody* incident_body, Vector3& reference_normal, Vector3& incident_normal)
{
	float incident_tracker = FLT_MAX;
	//get incident face vertices by dotting all axes with mtv
	for (int i = 0; i < 3; i++)
	{
		float most_neg = Vector3::Dot(incident_body->obb->rot[i], reference_normal);
		if (most_neg < incident_tracker){
			incident_tracker = most_neg;
			incident_normal = incident_body->obb->rot[i];
		}
		//Test the opposite axis as well
		most_neg = Vector3::Dot(-1 * incident_body->obb->rot[i], reference_normal);
		if (most_neg < incident_tracker){
			incident_tracker = most_neg;
			incident_normal = -1 * incident_body->obb->rot[i];
		}
	}
}

void RenderScene::ClipFaceToSidePlane(std::vector<Contact> contacts_to_clip, const Vector3& normal, float plane_offset, std::vector<Contact> &contacts)
{
	contacts.clear();

	Vector3 Vertex1 = contacts_to_clip.back().position; //start edge forms from endvertex->startvertex
	float Distance1 = Vector3::Dot(Vertex1, normal) - plane_offset;

	for (int i = 0; i < contacts_to_clip.size(); i++)
	{
		Vector3 Vertex2 = contacts_to_clip[i].position;
		float Distance2 = Vector3::Dot(Vertex2, normal) - plane_offset;

		if (Distance1 <= 0.0f && Distance2 <= 0.0f)
		{
			// Both vertices are behind the plane - keep vertex2
			Contact contact1;
			contact1.position = Vertex2;
			contacts.push_back(contact1);
		}
		else if (Distance1 <= 0.0f && Distance2 > 0.0f)
		{
			// Vertex1 is behind the plane, vertex2 is in front -> intersection point
			float Fraction = Distance1 / (Distance1 - Distance2);
			Vector3 Position = Vertex1 + Fraction * (Vertex2 - Vertex1);

			// Keep intersection point
			Contact contact1;
			contact1.position = Position;
			contacts.push_back(contact1);
		}
		else if (Distance2 <= 0.0f && Distance1 > 0)
		{
			// Vertex2 is behind the plane, vertex1 is in front -> intersection point
			float Fraction = Distance1 / (Distance1 - Distance2);
			Vector3 Position = Vertex1 + Fraction * (Vertex2 - Vertex1);

			// Keep intersection point 
			Contact contact1;
			contact1.position = Position;
			contacts.push_back(contact1);

			// And also keep vertex2
			Contact contact2;
			contact2.position = Vertex2;
			contacts.push_back(contact2);
		}

		// Keep vertex2 as starting vertex for next edge
		Vertex1 = Vertex2;
		Distance1 = Distance2;
	}
}

void RenderScene::FilterContactPoints(RigidBody* one, RigidBody* two, std::vector<Contact>& contacts,
	const Vector3& ref_plane, float ref_offset,
	const Vector3& side_plane1, float side_offset1,
	const Vector3& side_plane2, float side_offset2,
	const Vector3& side_plane3, float side_offset3,
	const Vector3& side_plane4, float side_offset4,
	std::unordered_set<Contact*>& contacts_points)
{
	//keep all points behind or on the planes tested on
	for (int i = 0; i < contacts.size(); i++)
	{
		float epsilon = 0.0001f;
		float seperation = Vector3::Dot(contacts[i].position, ref_plane) - (ref_offset);
		//behind or on plane, keep point
		if (seperation <= 0)
		{ 
			Contact* new_contact = new Contact;
			new_contact->position = contacts[i].position;
			new_contact->penetration = -1*seperation;
			new_contact->normal = ref_plane;
			new_contact->one = one;
			new_contact->two = two;
			contacts_points.insert(new_contact);
		}
	}
}
bool RenderScene::OverlapOnAxis(RigidBody* a, RigidBody* b, Vector3 axis, Vector3 center_vector, Vector3 &smallest_axis, float &smallest_penetration, int &axis_number, int axis_index)
{
	//Handle if axis generated by cross product is 0 by reporting that it's a collision. (Happens when crossing two axes with same direction)
	if (axis.SquareMagnitude() < 0.0001){
		return true;
	}

	Vector3 axis_normalized = axis.Normalized();
	//project obb one and obb two on axis - oneProject and twoProject
	float oneProject = this->ProjectOBBToAxis(a->obb, axis_normalized);
	float twoProject = this->ProjectOBBToAxis(b->obb, axis_normalized);

	//project the center vector to the axis as well - distance
	float distance = fabs(Vector3::Dot(center_vector, axis_normalized));
	float penetration = (oneProject + twoProject) - distance;

	//Distance turns out greater than the two boxes halfextents together. No collision!
	if (penetration < 0){
		return false;
	}
	//Updates the smallest penetration for minimum translation vector 
	if (penetration < smallest_penetration){
		smallest_penetration = penetration;
		smallest_axis = axis_normalized;
		axis_number = axis_index;

	}

	//if (this->BiasGreaterThan(smallest_penetration, penetration)){
	//	smallest_penetration = penetration;
	//	smallest_axis = axis_normalized;
	//	axis_number = axis_index;

	//}
	return true; //collision


}

float RenderScene::ProjectOBBToAxis(OBB* box, Vector3 axis)
{
	return (fabs(Vector3::Dot(box->half_extent[0] * box->rot[0], axis)) +
			fabs(Vector3::Dot(box->half_extent[1] * box->rot[1], axis)) +
			fabs(Vector3::Dot(box->half_extent[2] * box->rot[2], axis)));

	//return
	//	box->half_extent[0] * abs(Vector3::Dot(axis, box->rot[0])) + //get not normalized axes otherwise you will get floating errors!
	//	box->half_extent[1] * abs(Vector3::Dot(axis, box->rot[1])) +
	//	box->half_extent[2] * abs(Vector3::Dot(axis,box->rot[2]));
}

void RenderScene::Solver( Contact* contact)
{
	RigidBody* ent1 = contact->one;
	RigidBody* ent2 = contact->two;

	const float e = std::min(ent1->restitution, ent2->restitution);
	Vector3 rA = contact->position - ent1->position;// Vector3(center_of_mass_world_space_a);
	Vector3 rB = contact->position - ent2->position;// Vector3(center_of_mass_world_space_b);
	Vector3 kA = Vector3::Cross(rA, contact->normal);
	Vector3 kB = Vector3::Cross(rB, contact->normal);
	Matrix33 rot_a = ent1->GetModel().ConvertToMatrix33();
	Matrix33 rotT_a = rot_a.Transposed();
	Matrix33 inertia_tensor_a = rot_a * ent1->inverse_inertia_tensor * rotT_a;

	Matrix33 rot_b = ent2->GetModel().ConvertToMatrix33();
	Matrix33 rotT_b = rot_b.Transposed();
	Matrix33 inertia_tensor_b = rot_b * ent2->inverse_inertia_tensor * rotT_b;
	Vector3 uA = inertia_tensor_a * kA;
	Vector3 uB = inertia_tensor_b * kB;

	float fNumer = -(1 + e)*(Vector3::Dot(contact->normal, ent1->velocity - ent2->velocity) + Vector3::Dot(ent1->angular_velocity, kA) - Vector3::Dot(ent2->angular_velocity, kA));
	float fDenom = ent1->inverse_mass + ent2->inverse_mass + Vector3::Dot(kA, uA) + Vector3::Dot(kB, uB);
	float f = fNumer / fDenom;


	Vector3 impulse = f*contact->normal;

	//compute derived quantities, linear/angular velocity
	ent1->velocity += impulse*ent1->inverse_mass;
	ent2->velocity -= impulse*ent2->inverse_mass;
	ent1->angular_velocity += (f*uA);
	ent2->angular_velocity -= (f*uB);
}

void RenderScene::GetSupportEdge(RigidBody* a, Vector3 &mtv, Vector3* support_out)
{
	//q3Vec3 absN = q3Abs(n);
	Matrix33 tx = a->GetModel().ConvertToMatrix33();
	Vector3 n = tx.Transposed() * mtv;
	Vector3 absN;
	if (n[0] < 0.0){
		absN[0] = -n[0];
	}
	else{
		absN[0] = n[0];
	}
	if (n[1] < 0.0){
		absN[1] = -n[1];
	}
	else{
		absN[1] = n[1];
	}
	if (n[2] < 0.0){
		absN[2] = -n[2];
	}
	else{
		absN[2] = n[2];
	}
	Vector3 aa, b;

	// x > y
	if (absN[0] > absN[1])
	{
		// x > y > z
		if (absN[1] > absN[2])
		{
			aa.Insert(a->obb->half_extent[0], a->obb->half_extent[1], a->obb->half_extent[2]);
			b.Insert(a->obb->half_extent[0], a->obb->half_extent[1], -a->obb->half_extent[2]);
		}

		// x > z > y || z > x > y
		else
		{
			aa.Insert(a->obb->half_extent[0], a->obb->half_extent[1], a->obb->half_extent[2]);
			b.Insert(a->obb->half_extent[0], -a->obb->half_extent[1], a->obb->half_extent[2]);
		}
	}

	// y > x
	else
	{
		// y > x > z
		if (absN[0] > absN[2])
		{
			aa.Insert(a->obb->half_extent[0], a->obb->half_extent[1], a->obb->half_extent[2]);
			b.Insert(a->obb->half_extent[0], a->obb->half_extent[1], -a->obb->half_extent[2]);
		}

		// z > y > x || y > z > x
		else
		{
			aa.Insert(a->obb->half_extent[0], a->obb->half_extent[1], a->obb->half_extent[2]);
			b.Insert(-a->obb->half_extent[0], a->obb->half_extent[1], a->obb->half_extent[2]);
		}
	}

	float signx, signy, signz;
	if (n[0] >= 0.0)
	{
		signx = 1.0;
	}
	else
	{
		signx = -1.0;
	}
	if (n[1] >= 0.0)
	{
		signy = 1.0;
	}
	else
	{
		signy = -1.0;
	}
	if (n[2] >= 0.0)
	{
		signz = 1.0;
	}
	else
	{
		signz = -1.0;
	}

	aa[0] *= signx;
	aa[1] *= signy;
	aa[2] *= signz;
	b[0] *= signx;
	b[1] *= signy;
	b[2] *= signz;


	support_out[0] = tx * aa + a->position;
	support_out[1] = tx * b + a->position;

	//if (Vector3::Dot(b->position - a->position, mtv) < 0.f){
	//	mtv = -1*mtv;
	//}

	//float dmin = Vector3::Dot(a->obb->vertices[0], mtv);
	//const float threshold = 1.0E-3f;
	////support points of a and b. need a way to so associate the points with owner rigidbody, maybe just two arrays
	////Vector3 support_points[4];
	//int sI = 0;
	//Matrix33 m = a->GetModel().ConvertToMatrix33();

	//for (int i = 1; i < 8; i++){
	//	float d = Vector3::Dot(a->obb->vertices[i], mtv);
	//	if (d < dmin){
	//		dmin = d;
	//	}
	//}

	//for (int i = 0; i < 8; i++){
	//	float d = Vector3::Dot(a->obb->vertices[i], mtv);

	//	if (d < dmin + threshold){
	//		support_out[sI] = Vector3(a->GetModel() * Vector4(a->obb->vertices[i])) + a->position;
	//		if (sI == 1){
	//			break;
	//			//return num of support
	//		}
	//		sI++;
	//	}
	//}

	////maybe no need just call this func twice with -mtv for rB
	//dmin = Vector3::Dot(b->obb->vertices[0], mtv);
	//for (int i = 1; i < 8; i++){
	//	float d = Vector3::Dot(b->obb->vertices[i], mtv);
	//	if (d > dmin){
	//		dmin = d;
	//	}
	//}

	//for (int i = 0; i < 8; i++){
	//	float d = Vector3::Dot(b->obb->vertices[i], mtv);

	//	if (d > dmin - threshold){
	//		support_points[sI] = Vector3(b->GetModel() * Vector4(b->obb->vertices[i])) + b->position;
	//		if (sI == 3){
	//			sI++;
	//			break;
	//		}
	//		sI++;
	//	}
	//}
	//After obtaining the support points. Project them on axis perpendicular to mtv (-normal.y,normal.x, normal.z)?
	//Sort the projected support points and pick the two in the middle. Project them on the other rigidbody edge(edge created from support points, rB is inverse)
	//We now have the two contact points add them together and divide by 2


	//out_contact = Vector3(a->GetModel() * Vector4(a->obb->vertices[i])) + a->position;
}

void RenderScene::GetContactPoint_EdgeEdge(std::vector<Vector3> &contact_points_out, const Vector3 &PA, const Vector3 &QA, const Vector3 &PB, const Vector3 &QB)
{
	Vector3 DA = QA - PA;
	Vector3 DB = QB - PB;
	Vector3 r = PA - PB;
	float a = Vector3::Dot(DA, DA);
	float e = Vector3::Dot(DB, DB);
	float f = Vector3::Dot(DB, r);
	float c = Vector3::Dot(DA, r);

	float b = Vector3::Dot(DA, DB);
	float denom = a * e - b * b;

	float TA = (b * f - c * e) / denom;
	float TB = (b * TA + f) / e;

	Vector3 CA = PA + DA * TA;
	Vector3 CB = PB + DB * TB;

	contact_points_out.push_back((CA + CB) * 0.5f);
}

void RenderScene::CalcFaceVertices(Vector3* vertices, Vector3 axis, RigidBody *incident_body)
{
	Vector3 xAxis = incident_body->obb->rot[0];
	Vector3 yAxis = incident_body->obb->rot[1];
	Vector3 zAxis = incident_body->obb->rot[2];
	Vector3 referencePos;
	Vector3 u;
	Vector3 v;

	float sign = 0.f;
	if (abs(sign = Vector3::Dot(axis,xAxis)) > 0.9f)
	{
		if (sign > 0.f){
			referencePos = incident_body->obb->pos + incident_body->obb->half_extent[0] * xAxis;
		}
		else{
			referencePos = incident_body->obb->pos - incident_body->obb->half_extent[0] * xAxis;
		}
		u = incident_body->obb->half_extent[1] * yAxis;
		v = incident_body->obb->half_extent[2] * zAxis;
	}
	else if (abs(sign = Vector3::Dot(axis, yAxis)) > 0.9f)
	{
		if (sign > 0.f){
			referencePos = incident_body->obb->pos + incident_body->obb->half_extent[1] * yAxis;
		}
		else{
			referencePos = incident_body->obb->pos - incident_body->obb->half_extent[1] * yAxis;
		}
		u = incident_body->obb->half_extent[0] * xAxis;
		v = incident_body->obb->half_extent[2] * zAxis;
	}
	else if (abs(sign = Vector3::Dot(axis, zAxis)) > 0.9f)
	{
		if (sign > 0.f){
			referencePos = incident_body->obb->pos + incident_body->obb->half_extent[2] * zAxis;
		}
		else{
			referencePos = incident_body->obb->pos - incident_body->obb->half_extent[2] * zAxis;
		}
		u = incident_body->obb->half_extent[0] * xAxis;
		v = incident_body->obb->half_extent[1] * yAxis;
	}
	vertices[0] = referencePos + u + v;
	vertices[1] = referencePos - u + v;
	vertices[2] = referencePos - u - v;
	vertices[3] = referencePos + u - v;
}

void RenderScene::CalculateContacts_EdgeEdge(Vector3 center_vector, RigidBody* a, RigidBody* b, Vector3& smallest_axis, float smallest_penetration, int axis_number, std::unordered_set<Contact*>& contacts_points, int& bestSingleAxis)
{
	// We've got an edge-edge contact. Find out which axes

	axis_number -= 6;
	unsigned oneAxisIndex = axis_number / 3;
	unsigned twoAxisIndex = axis_number % 3;
	Vector3 oneAxis = a->obb->rot[oneAxisIndex];
	Vector3 twoAxis = b->obb->rot[twoAxisIndex];

	// The axis should point from box one to box two.
	float toCentreDist = Vector3::Dot(center_vector, smallest_axis); //tocenter
	if (toCentreDist > 0) //have turned sign!
	{
		smallest_axis = smallest_axis * -1.0f;
	}

	// We have the axes, but not the edges: each axis has 4 edges parallel
	// to it, we need to find which of the 4 for each object. We do
	// that by finding the point in the centre of the edge. We know
	// its component in the direction of the box's collision axis is zero
	// (its a mid-point) and we determine which of the extremes in each
	// of the other axes is closest.
	Vector3 ptOnOneEdge = a->obb->half_extent;
	Vector3 ptOnTwoEdge = b->obb->half_extent;
	for (unsigned i = 0; i < 3; i++)
	{
		if (i == oneAxisIndex) ptOnOneEdge[i] = 0;
		else if (Vector3::Dot(a->obb->rot[i], smallest_axis) > 0) ptOnOneEdge[i] = -ptOnOneEdge[i];

		if (i == twoAxisIndex) ptOnTwoEdge[i] = 0;
		else if (Vector3::Dot(b->obb->rot[i], smallest_axis) < 0) ptOnTwoEdge[i] = -ptOnTwoEdge[i];
	}

	// Move them into world coordinates (they are already oriented
	// correctly, since they have been derived from the axes).
	ptOnOneEdge = a->GetModel().ConvertToMatrix33() * ptOnOneEdge + a->position;
	ptOnTwoEdge = b->GetModel().ConvertToMatrix33() * ptOnTwoEdge + b->position;

	// So we have a point and a direction for the colliding edges.
	// We need to find out point of closest approach of the two
	// line-segments.

	Vector3 vertex = this->contactPoint(
		ptOnOneEdge, oneAxis, a->obb->half_extent[oneAxisIndex],
		ptOnTwoEdge, twoAxis, b->obb->half_extent[twoAxisIndex],
		bestSingleAxis > 2
		);

	// We can fill the contact.
	Contact* contact = new Contact();
	contact->penetration = smallest_penetration;
	contact->normal = smallest_axis;
	contact->position = vertex;
	contact->one = b;
	contact->two = a;

	contacts_points.insert(contact);
}

Vector3 RenderScene::contactPoint(const Vector3 &pOne, const Vector3 &dOne, float oneSize, const Vector3 &pTwo, const Vector3 &dTwo, float twoSize, bool useOne)
{
	// If useOne is true, and the contact point is outside
	// the edge (in the case of an edge-face contact) then
	// we use one's midpoint, otherwise we use two's.

	Vector3 toSt, cOne, cTwo;
	float dpStaOne, dpStaTwo, dpOneTwo, smOne, smTwo;
	float denom, mua, mub;

	smOne = dOne.SquareMagnitude();
	smTwo = dTwo.SquareMagnitude();
	dpOneTwo = Vector3::Dot(dTwo, dOne);

	toSt = pOne - pTwo;
	dpStaOne = Vector3::Dot(dOne,toSt);
	dpStaTwo = Vector3::Dot(dTwo,toSt);

	denom = smOne * smTwo - dpOneTwo * dpOneTwo;

	// Zero denominator indicates parrallel lines
	if (abs(denom) < 0.0001f) {
		return useOne ? pOne : pTwo;
	}

	mua = (dpOneTwo * dpStaTwo - smTwo * dpStaOne) / denom;
	mub = (smOne * dpStaTwo - dpOneTwo * dpStaOne) / denom;

	// If either of the edges has the nearest point out
	// of bounds, then the edges aren't crossed, we have
	// an edge-face contact. Our point is on the edge, which
	// we know from the useOne parameter.
	if (mua > oneSize ||
		mua < -oneSize ||
		mub > twoSize ||
		mub < -twoSize)
	{
		return useOne ? pOne : pTwo;
	}
	else
	{
		cOne = pOne + mua * dOne;
		cTwo = pTwo + mub * dTwo;

		return cOne * 0.5 + cTwo * 0.5;
	}
}

void RenderScene::ProcessContact(Contact* contact, float& Pn, Vector3&vel1, Vector3& ang_vel1, Vector3&vel2, Vector3& ang_vel2)
{
	const float k_allowedPenetration = 0.01f;
	float BAUMGARTE = 0.2f;
	//BAUMGARTE
	float bias = -BAUMGARTE * 1.f/this->physics_timestep * std::min(0.0f, -contact->penetration + k_allowedPenetration);

	RigidBody* ent1 = contact->one;
	RigidBody* ent2 = contact->two;

	const float e = std::min(ent1->restitution, ent2->restitution);//0.0f; //restitution, if set to 0 then the object we collide with will absorb most of the impact and won't move much, if set to 1 both objects will fly their way
	Vector3 contactPoint = contact->position;
	Vector3 contactNormal = contact->normal;

	Vector3 rA = contactPoint - ent1->position;
	Vector3 rB = contactPoint - ent2->position;
	Vector3 kA = Vector3::Cross(rA, contactNormal);
	Vector3 kB = Vector3::Cross(rB, contactNormal);

	Vector3 uA = ent1->inverse_inertia_tensor_world_space*kA;
	Vector3 uB = ent2->inverse_inertia_tensor_world_space*kB;

	// Precompute normal mass, tangent mass, and bias.

	// Normal Mass //is it force of normal pushing away from the surface?
	float normalMass = ent1->inverse_mass + ent2->inverse_mass;

	if (!ent1->is_kinematic){
		normalMass += Vector3::Dot(kA, uA);
	}
	if (!ent2->is_kinematic){
		normalMass += Vector3::Dot(kB, uB);
	}

	// Relative velocity at contact
	Vector3 relativeVel = (ent2->velocity + Vector3::Cross(ent2->angular_velocity, rB)) - (ent1->velocity + Vector3::Cross(ent1->angular_velocity, rA));
	float numer = -(1 + e)*Vector3::Dot(relativeVel, contactNormal) + bias;
	float denom = ent1->inverse_mass + ent2->inverse_mass + Vector3::Dot(ent1->inverse_inertia_tensor_world_space*(Vector3::Cross(kA, rA) + ent2->inverse_inertia_tensor_world_space*Vector3::Cross(kB, rB)), contactNormal);
	//double f = numer / denom;
	float f = numer / normalMass;

	float Pn0 = contact->accumulated_impulse;
	contact->accumulated_impulse = std::fmax(Pn0 + f, 0.0f);
	f = contact->accumulated_impulse - Pn0;


	//f = std::fmax(f, 0.0);

	Vector3 impulse = f*contactNormal;


	contact->one->velocity -= impulse*ent1->inverse_mass;
	contact->two->velocity += impulse*ent2->inverse_mass;
	//contact->one->angular_velocity -= ent1->inverse_inertia_tensor_world_space*Vector3::Cross(rA, impulse);
	//contact->two->angular_velocity += ent2->inverse_inertia_tensor_world_space*Vector3::Cross(rB, impulse);
	contact->one->angular_velocity -= f*uA;
	contact->two->angular_velocity += f*uB;

	//vel1 -= impulse*ent1->inverse_mass;
	//vel2 += impulse*ent2->inverse_mass;
	//ang_vel1 -= f*uA;
	//ang_vel2 += f*uB;
}

void RenderScene::PositionalCorrection(RigidBody* one, RigidBody* two, float smallest_penetration, Vector3 smallest_axis)
{
	const float percent = 0.2f; // usually 20% to 80%
	const float slop = 0.01; // usually 0.01 to 0.1
	//Vector3 correction = (std::max(penetration - slop, 0.0f) / (one->massInverse + two->massInverse)) * normal;
	Vector3 correction = (std::max(smallest_penetration - slop, 0.0f) / (one->inverse_mass + two->inverse_mass)) * percent * smallest_axis;
	one->Translate(-1*one->inverse_mass * correction);
	two->Translate(two->inverse_mass * correction);
}

void RenderScene::SelectedObjectControls()
{
	if (this->window->GetKey(GLFW_KEY_PAGE_UP) == GLFW_PRESS){
		if (this->selected_object){
			this->selected_object->Translate(0.f, 0.1, 0.f);
		}
	}
	if (this->window->GetKey(GLFW_KEY_PAGE_DOWN) == GLFW_PRESS){
		if (this->selected_object){
			this->selected_object->Translate(0.f, -0.1, 0.f);
		}
	}

	if (this->window->GetKey(GLFW_KEY_UP) == GLFW_PRESS){
		if (this->selected_object){
			this->selected_object->Translate(0.f, 0.f, -0.01f);
		}
	}
	if (this->window->GetKey(GLFW_KEY_DOWN) == GLFW_PRESS){
		if (this->selected_object){
			this->selected_object->Translate(0.f, 0.f, 0.01f);
		}
	}
	if (this->window->GetKey(GLFW_KEY_LEFT) == GLFW_PRESS){
		if (this->selected_object){
			this->selected_object->Translate(-0.01f, 0.f, 0.f);
		}
	}
	if (this->window->GetKey(GLFW_KEY_RIGHT) == GLFW_PRESS){
		if (this->selected_object){
			this->selected_object->Translate(0.01f, 0.f, 0.f);
		}
	}

}

void RenderScene::DrawSidePlanes(const Vector3& normal1, const Vector3& normal2, const Vector3& onePosition, int index1, int index2, const Vector3& oneHalfSize)
{
	Vector3 vertexOnPlane1 = onePosition - normal1*oneHalfSize[index1]; // -1*normal
	Vector3 vertexOnPlane2 = onePosition + normal1*oneHalfSize[index1];
	Vector3 vertexOnPlane3 = onePosition - normal2*oneHalfSize[index2]; // -1*normal
	Vector3 vertexOnPlane4 = onePosition + normal2*oneHalfSize[index2];
	//let's draw calculated planes with normals at plane points
	//DebugRenderer::Instance()->line.mat->SetColor(1, 0, 0); //red
	//DebugRenderer::Instance()->point.mat->SetColor(1, 0, 1);
	DebugRenderer::Instance()->DrawPlaneN(this->view, this->projection, -1.f*normal1, vertexOnPlane1, Vector3(1,0,0));

	//DebugRenderer::Instance()->line.mat->SetColor(0, 1, 0); //green
	//DebugRenderer::Instance()->point.mat->SetColor(1, 0, 1);
	DebugRenderer::Instance()->DrawPlaneN(this->view, this->projection, normal1, vertexOnPlane2, Vector3(0, 1, 0));

	//DebugRenderer::Instance()->line.mat->SetColor(0, 0, 1); //blue
	//DebugRenderer::Instance()->point.mat->SetColor(1, 0, 1);
	DebugRenderer::Instance()->DrawPlaneN(this->view, this->projection, -1.f*normal2, vertexOnPlane3, Vector3(0, 0,1));

	//DebugRenderer::Instance()->line.mat->SetColor(1, 1, 0); //yellow
	//DebugRenderer::Instance()->point.mat->SetColor(1, 0, 1);
	DebugRenderer::Instance()->DrawPlaneN(this->view, this->projection, normal2, vertexOnPlane4, Vector3(1, 1, 0));
}

void RenderScene::SetScene(std::string scene)
{
	if (scene == "Plank"){
		this->ClearScene();
		this->AddPlaneToScene(true,Vector3(0, -20, 0));
		this->AddCubeToScene(true, Vector3(0, -16.9, 0));
		this->AddPlankToScene(false, Vector3(0, -13, 0));
	}
	else if (scene == "StackOfBoxes"){
		this->ClearScene();
		this->AddPlaneToScene(true, Vector3(0, -20, 0));
		this->AddCubeToScene(false, Vector3(0, -15, 0));
		this->AddCubeToScene(false, Vector3(0, -12, 0));
		this->AddCubeToScene(false, Vector3(0, -9, 0));
		this->AddCubeToScene(false, Vector3(0, -6, 0));
	}
	else if (scene == "SlidingBoxes"){
		this->ClearScene();
		this->AddPlaneToScene(true, Vector3(0, -20, -100), 30.f, Vector3(0,0,1));
		this->AddCubeToScene(false, Vector3(0, 4, -50));
		this->AddCubeToScene(false, Vector3(0, 20, -45));
		this->AddCubeToScene(false, Vector3(0, 10, -30));
		this->AddCubeToScene(false, Vector3(0, 8, -20));
	}
}

void RenderScene::ClearScene()
{
	for (std::map<int, RigidBody*>::iterator it = this->object_list.begin(); it != this->object_list.end(); it++){
		this->root->RemoveChildNode(it->second);
		delete it->second;
	}

	this->object_list.clear();

	this->x_axis.clear();
	this->y_axis.clear();
	this->z_axis.clear();

	this->x_axis_collision.clear();
	this->y_axis_collision.clear();
	this->z_axis_collision.clear();

	this->broadphase_collision_list.clear();

}

} // namespace Example
