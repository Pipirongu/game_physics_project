//#include "physicsmanager.h"
//
////------------------------------------------------------------------------------
///**
//*/
//PhysicsManager::PhysicsManager()
//{
//	//Sets radix character to . and not ,
//	setlocale(LC_ALL, "POSIX");
//
//}
//
////------------------------------------------------------------------------------
///**
//*/
//PhysicsManager::~PhysicsManager()
//{
//	delete this->root;
//	delete this->camera;
//	delete this->light;
//	delete this->rigidbody_mesh;
//	delete this->rigidbody_texture;
//	delete this->skybox;
//	delete this->skybox_mesh;
//	delete this->skybox_texture;
//	
//	for (std::map<int, RigidBody*>::iterator it = this->object_list.begin(); it != this->object_list.end(); it++){
//		delete it->second;
//	}
//}
//
////------------------------------------------------------------------------------
///**
//*/
//PhysicsManager* PhysicsManager::Instance()
//{
//	static PhysicsManager instance;
//
//	return &instance;
//}
//
////------------------------------------------------------------------------------
///**
//*/
//bool
//PhysicsManager::Open()
//{
//	App::Open();
//	this->window = new Display::Window;
//
//	// key callback
//	this->window->SetKeyPressFunction([this](int32 key, int32 scancode, int32 action, int32 mods)
//	{
//		if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS){
//			this->is_open = false;
//
//		}
//		if(this->window->GetKey(GLFW_KEY_E) == GLFW_PRESS){
//			this->AddCubeToScene();
//		}
//		if (this->window->GetKey(GLFW_KEY_Q) == GLFW_PRESS){
//			if (this->debug_rendering_toggle == 3){
//				this->debug_rendering_toggle = 0;
//			}
//			else{
//				this->debug_rendering_toggle++;
//			}
//		}
//		if (this->window->GetKey(GLFW_KEY_SPACE) == GLFW_PRESS){
//			if (this->is_paused){
//				this->is_paused = false;
//			}
//			else{
//				this->is_paused = true;
//			}
//		}
//	});
//
//	// close callback
//	this->window->SetCloseFunction([this]()
//	{
//		this->is_open = false;
//	});
//
//	// window resize callback
//	this->window->SetWindowSizeFunction([this](int width, int height)
//	{
//		if (width != 0 || height != 0){
//			this->window->GetWindowSize(&this->window_width, &this->window_height);
//			this->window->SetSize(this->window_width, this->window_height);
//			float aspect = (float)this->window_width / (float)this->window_height;
//			this->projection = Matrix44::Perspective(60, aspect, 0.1, 1000);
//			TextRenderer::Instance()->SetProjection(Matrix44::Ortho(0.0f, this->window_width, 0.0f, this->window_height, -1, 1));
//			this->fbo.UpdateTextureDimensions(this->window_width, this->window_height);
//		}
//	});
//
//	
//	//init last_time
//	this->last_time = glfwGetTime();
//	this->fps_timer = 0;
//
//	if (this->window->Open())
//	{
//		glfwSwapInterval(0);
//
//		glEnable(GL_DEPTH_TEST);
//		glDepthFunc(GL_LEQUAL);
//		glEnable(GL_CULL_FACE);
//		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
//	
//		ShaderManager::Instance()->AddShaderProgram("shaders/simple_vs.glsl", "shaders/simple_fs.glsl", "main");
//		ShaderManager::Instance()->AddShaderProgram("shaders/picking_vs.glsl", "shaders/picking_fs.glsl", "picking");
//		ShaderManager::Instance()->AddShaderProgram("shaders/skybox_vs.glsl", "shaders/skybox_fs.glsl", "skybox");
//		ShaderManager::Instance()->AddShaderProgram("shaders/debug_vs.glsl", "shaders/debug_fs.glsl", "debug");
//		ShaderManager::Instance()->AddShaderProgram("shaders/text_vs.glsl", "shaders/text_fs.glsl", "text");
//
//		//Objects
//		this->root = new Root;
//		this->camera = new Camera;
//		
//		this->rigidbody_mesh = new Mesh;
//		this->rigidbody_mesh->LoadOBJ("models/cube.obj");
//		this->rigidbody_texture = new Texture;
//		this->rigidbody_texture->LoadTexture("textures/cube.png");
//
//		//plane
//		this->plane_mesh = new Mesh;
//		this->plane_mesh->LoadOBJ("models/plane.obj");
//
//		this->skybox_mesh = new Mesh;
//		this->skybox_mesh->LoadOBJ("models/cube.obj");
//		this->skybox = new Skybox;
//		this->skybox_texture = new Texture;
//		this->skybox_texture->LoadSkyboxTexture
//			("textures/skybox/skybox_back.png",
//			"textures/skybox/skybox_down.png",
//			"textures/skybox/skybox_front.png",
//			"textures/skybox/skybox_left.png",
//			"textures/skybox/skybox_right.png",
//			"textures/skybox/skybox_up.png");
//
//		this->skybox->SetMesh(this->skybox_mesh);
//		this->skybox->SetTexture(this->skybox_texture);
//		this->skybox->Scale(150);
//		
//		this->light = new Light;
//
//		this->root->AddChildNode(this->camera);
//		this->root->AddChildNode(this->light);
//		this->camera->AddChildNode(this->skybox);
//
//		//Init the camera vectors just so the objects are rendered in the scene
//		camera->UpdateCamera();
//
//		// load projection matrix first time because resize callback won't be called until user resizes window.
//		this->window->GetWindowSize(&this->window_width, &this->window_height);
//		
//		float aspect = (float)this->window_width / (float)this->window_height;
//		this->projection = Matrix44::Perspective(60, aspect, 0.1, 1000);
//
//		this->fbo.Init(this->window_width, this->window_height);
//		TextRenderer::Instance()->Init("fonts/font.ttf", 18);
//		TextRenderer::Instance()->SetProjection(Matrix44::Ortho(0.0f, this->window_width, 0.0f, this->window_height, -1, 1));
//
//		this->AddPlaneToScene();
//
//		this->is_open = true;
//		return true;
//	}
//	return false;
//}
//
////------------------------------------------------------------------------------
///**
//*/
//void
//PhysicsManager::Run()
//{
//	while(this->is_open)
//	{		
//		//Poll for key input
//		this->window->Update();
//
//		//deltatime
//		double current_time = glfwGetTime();
//		this->delta_time = float(current_time - this->last_time);
//		
//		//Moves the player
//		this->CameraControls();		
//		this->skybox->Rotate(1*this->delta_time, 0.f, 1.f, 0.f);
//		
//		if (this->is_paused){
//			//Integrate
//			for (std::map<int, RigidBody*>::iterator it = this->object_list.begin(); it != this->object_list.end(); it++){
//				//it->second->ApplyImpulse(Vector3(0, -100, 0), it->second->position);
//				it->second->Integrate(this->delta_time);
//			}
//		}
//		//Update matrices
//		this->root->Update(this->view);
//
//		//Update AABB
//		for (std::map<int, RigidBody*>::iterator it = this->object_list.begin(); it != this->object_list.end(); it++){
//			it->second->UpdateOBB();
//			it->second->UpdateAABB();
//		}
//
//
//		/************************************************************************/
//		/*                                                                       */
//		/************************************************************************/
//
//		this->PickingPass();
//
//		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//		this->BroadPhase();
//		this->NarrowPhase();
//
//		this->RenderPass();
//
//		ShaderManager::Instance()->ChangeShader("text");
//
//		if ((current_time - this->fps_timer) > 0.2f){
//			this->fps = std::to_string((int)(1 / this->delta_time)) + " FPS";
//			this->pos = "[" + std::to_string((int)camera->position[0]) + "," + std::to_string((int)camera->position[1]) + "," + std::to_string((int)camera->position[2]) + "]";
//			this->objects = "Objects: " + std::to_string(this->object_list.size());
//			this->fps_timer = current_time;
//		}
//
//		TextRenderer::Instance()->SetColor(1, 1, 1);
//		TextRenderer::Instance()->RenderText(fps, 20.f, this->window_height - 25.f, 1);
//		TextRenderer::Instance()->SetColor(1, 1, 0);
//		TextRenderer::Instance()->RenderText(pos, 20.f, this->window_height - 50.f, 1);
//		TextRenderer::Instance()->RenderText(objects, 20.f, this->window_height - 75.f, 1);
//		TextRenderer::Instance()->RenderText(std::to_string(this->delta_time), 20.f, this->window_height - 100.f, 1);
//
//		this->window->SwapBuffers();
//		this->last_time = current_time;
//	}
//	this->window->Close();
//}
//
//void PhysicsManager::AddCubeToScene()
//{
//	RigidBody* cube = new RigidBody;
//	cube->SetPosition(camera->position + camera->direction.Normalized() * 5);
//	cube->SetMass(1.f);
//
//	cube->SetMesh(this->rigidbody_mesh);
//	cube->SetTexture(this->rigidbody_texture);
//	Vector3 dimensions = cube->mesh->CalculateDimensions();
//	cube->SetInertiaTensor(Matrix33::InertiaTensor(cube->mass, dimensions).Inverse());
//	cube->SetCenterOfMass(cube->mesh->CalculateCenterOfMass());
//
//	cube->aabb = cube->mesh->InitAABB();
//	cube->aabb->object = cube;
//
//	/************************************************************************/
//	/* obb                                                                  */
//	/************************************************************************/
//	cube->obb = new OBB;
//	cube->obb->half_extent = (cube->aabb->max.value - cube->aabb->min.value) / 2.f;
//	cube->obb->object = cube;
//
//	cube->obb->InitVertexList(cube->aabb->min.value, cube->aabb->max.value);
//
//
//	//worldspace init
//	cube->aabb->min.value = cube->aabb->min.value + cube->position;
//	cube->aabb->max.value = cube->aabb->max.value + cube->position;
//
//	this->x_axis.push_back(&cube->aabb->min);
//	this->x_axis.push_back(&cube->aabb->max);
//	this->y_axis.push_back(&cube->aabb->min);
//	this->y_axis.push_back(&cube->aabb->max);
//	this->z_axis.push_back(&cube->aabb->min);
//	this->z_axis.push_back(&cube->aabb->max);
//
//	this->root->AddChildNode(cube);
//	this->object_list[cube->ID()] = cube;
//}
//
//void RenderScene::AddPlaneToScene()
//{
//	RigidBody* plane = new RigidBody;
//	plane->SetPosition(0,-50,0);
//	plane->SetMass(0.f);
//
//	plane->SetMesh(this->plane_mesh);
//	plane->SetTexture(this->rigidbody_texture);
//	Vector3 dimensions = plane->mesh->CalculateDimensions();
//	plane->SetInertiaTensor(Matrix33::InertiaTensor(plane->mass, dimensions));
//	plane->SetCenterOfMass(plane->mesh->CalculateCenterOfMass());
//
//	plane->aabb = plane->mesh->InitAABB();
//	plane->aabb->object = plane;
//
//	/************************************************************************/
//	/* obb                                                                  */
//	/************************************************************************/
//	plane->obb = new OBB;
//	plane->obb->half_extent = (plane->aabb->max.value - plane->aabb->min.value) / 2.f;
//	plane->obb->object = plane;
//
//	plane->obb->InitVertexList(plane->aabb->min.value, plane->aabb->max.value);
//
//
//	//worldspace init
//	plane->aabb->min.value = plane->aabb->min.value + plane->position;
//	plane->aabb->max.value = plane->aabb->max.value + plane->position;
//
//	this->x_axis.push_back(&plane->aabb->min);
//	this->x_axis.push_back(&plane->aabb->max);
//	this->y_axis.push_back(&plane->aabb->min);
//	this->y_axis.push_back(&plane->aabb->max);
//	this->z_axis.push_back(&plane->aabb->min);
//	this->z_axis.push_back(&plane->aabb->max);
//
//	this->root->AddChildNode(plane);
//	this->object_list[plane->ID()] = plane;
//}
//
//void RenderScene::PickingPass()
//{
//	ShaderManager::Instance()->ChangeShader("picking");
//	this->fbo.Bind();
//	
//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//	for (std::map<int, RigidBody*>::iterator it = this->object_list.begin(); it != this->object_list.end(); it++){
//		it->second->DrawPass1(this->projection, this->view);
//	}
//	
//	this->fbo.Unbind();
//	
//	//read a pixel and set color of object	
//	if(this->is_picking_mode && this->window->GetMouseButton(GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS){
//		//get pixel here and color selected
//		this->fbo.Bind();
//		glReadBuffer(GL_COLOR_ATTACHMENT0);
//
//		Vector3 pixel;
//		double x, y;
//		this->window->GetCursorPos(&x, &y);
//		//inverted y coordinate because glfw 0,0 starts at topleft while opengl texture 0,0 starts at bottomleft
//		glReadPixels(x, this->window_height - y, 1, 1, GL_RGB, GL_FLOAT, &pixel);
//		//reset color of previous selected object
//		if(this->selected_object){
//			this->selected_object->SetPickingColor(Vector3(0,0,0));	
//		}
//		//if the picked object is a pickable object make it as selected and set its color
//		if(this->object_list.find((int)pixel[0]) != this->object_list.end()){
//			this->selected_object = this->object_list[pixel[0]];
//			this->selected_object->SetPickingColor(Vector3(0.2,0.2,0));
//			
//			glReadBuffer(GL_COLOR_ATTACHMENT1);
//			Vector3 world_position;
//			//inverted y coordinate because glfw 0,0 starts at topleft while opengl texture 0,0 starts at bottomleft
//			glReadPixels(x, this->window_height - y, 1, 1, GL_RGB, GL_FLOAT, &world_position);
//		
//
//			Vector3 impulse = (world_position - (camera->position)).Normalized();
//			this->selected_object->ApplyImpulse(impulse* 40.f, world_position);
//		}
//		//if it's an unpickable object or the background. Reset selected to nullptr.
//		else{			
//			this->selected_object = nullptr;
//		}
//
//		glReadBuffer(GL_NONE);
//		this->fbo.Unbind();
//	}
//
//}
//
//void RenderScene::RenderPass()
//{
//	ShaderManager::Instance()->ChangeShader("main");
//
//	//Send light properties to shader
//	this->light->Draw(this->projection, this->view);
//
//	//Draw objects
//	for (std::map<int, RigidBody*>::iterator it = this->object_list.begin(); it != this->object_list.end(); it++){
//		it->second->Draw(this->projection, this->view);
//	}
//
//	ShaderManager::Instance()->ChangeShader("debug");
//	//Draw light
//	this->light->DrawDebug(this->projection, this->view);
//
//	if (this->debug_rendering_toggle <= 2){
//		//Draw bounding boxes
//		for (std::map<int, RigidBody*>::iterator it = this->object_list.begin(); it != this->object_list.end(); it++){
//			if (this->debug_rendering_toggle == 0 || this->debug_rendering_toggle == 1){
//				it->second->DrawAABB(this->projection, this->view);
//			}
//			if (this->debug_rendering_toggle == 0 || this->debug_rendering_toggle == 2){
//				it->second->DrawOBB(this->projection, this->view);
//			}
//		}
//	}
//
//	//Draw skybox
//	ShaderManager::Instance()->ChangeShader("skybox");
//	this->skybox->Draw(this->projection, this->view);
//}
//
////------------------------------------------------------------------------------
///**
//*/
//void RenderScene::CameraControls()
//{
//	if (this->window->GetMouseButton(GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS){
//		this->is_picking_mode = false;
//
//		double mouse_pos_x, mouse_pos_y;
//		int width, height;
//		this->window->GetCursorPos(&mouse_pos_x, &mouse_pos_y);
//		this->window->GetWindowSize(&width, &height);
//		double mid_x = width / 2;
//		double mid_y = height / 2;
//
//		bool skip = false;
//		//Prevents camera from jumping
//		if (abs(mid_x - mouse_pos_x) > 10 || abs(mid_y - mouse_pos_y) > 10){
//			this->window->SetCursorPos(mid_x, mid_y);
//			skip = true;
//		}
//		if (!skip){
//			camera->MouseInput(mouse_pos_x, mouse_pos_y, width, height, this->delta_time);
//			//Hide the cursor
//			this->window->SetCursorMode(GLFW_CURSOR_DISABLED);
//			//Set the mouse pointer to the middle of the widget
//			this->window->SetCursorPos(mid_x, mid_y);
//		}
//	}
//	else if (this->window->GetMouseButton(GLFW_MOUSE_BUTTON_RIGHT) == GLFW_RELEASE){
//		this->is_picking_mode = true;
//		this->window->SetCursorMode(GLFW_CURSOR_NORMAL);
//	}
//
//	camera->KeyboardInput(this->window->GetKey(GLFW_KEY_W), this->window->GetKey(GLFW_KEY_S), this->window->GetKey(GLFW_KEY_A), this->window->GetKey(GLFW_KEY_D), this->delta_time);
//
//	//updates the player
//	this->camera->UpdateCameraMatrix(); //update camera matrix
//}
//
//void RenderScene::SortAndSweep(std::unordered_set<std::pair<RigidBody*, RigidBody*>> &out_axis_collision, std::vector<SweepValue*> &axis_list, int axis)
//{
//	int j;
//	SweepValue* temp;
//
//	for (int i = 0; i < axis_list.size(); i++){
//		j = i;
//
//		while (j > 0 && axis_list[j]->value[axis] < axis_list[j - 1]->value[axis]){
//			temp = axis_list[j];
//
//			SweepValue* left_value = axis_list[j - 1];
//			SweepValue* right_value = axis_list[j];
//
//			//if the value to the left in list(which is bigger than right value) is a max point, an overlap occurs
//			if (right_value->is_min && !left_value->is_min){
//				//overlap
//				//add pair to axislist
//				out_axis_collision.insert(std::make_pair(right_value->bounding_box->object, left_value->bounding_box->object));
//				this->sort_swap = true;
//				/*
//				Instead of saving every overlap, we can do a full aabb/aabb check here and directly add to collision list.
//				*/
//			}
//			else if (!right_value->is_min && left_value->is_min){
//				this->sort_swap = true;
//				//separating
//				//remove pair from axislist
//				right_value->bounding_box->object->aabb_color.Insert(0, 0, 1);
//				left_value->bounding_box->object->aabb_color.Insert(0, 0, 1);
//
//				right_value->bounding_box->object->obb_color.Insert(1, 1, 0);
//				left_value->bounding_box->object->obb_color.Insert(1, 1, 0);
//
//				out_axis_collision.erase(std::make_pair(right_value->bounding_box->object, left_value->bounding_box->object));
//				this->broadphase_collision_list.erase(std::make_pair(right_value->bounding_box->object, left_value->bounding_box->object));
//				//maybe remove from narrowphase
//				this->narrowphase_collision_list.erase(std::make_pair(right_value->bounding_box->object, left_value->bounding_box->object));
//			}
//			axis_list[j] = axis_list[j - 1];
//			axis_list[j - 1] = temp;
//			j--;
//		}
//	}
//}
//
//void RenderScene::Intersect()
//{
//	//Only check if a swap occurred
//	if (this->sort_swap){
//		for (std::unordered_set<std::pair<RigidBody*, RigidBody*>>::iterator it = this->x_axis_collision.begin(); it != this->x_axis_collision.end(); it++){
//			if (this->y_axis_collision.count((*it)) && this->z_axis_collision.count((*it))){
//				(*it).first->aabb_color.Insert(1, 0, 0);
//				(*it).second->aabb_color.Insert(1, 0, 0);
//				this->broadphase_collision_list.insert((*it));
//			}
//		}
//		this->sort_swap = false;
//	}
//}
//
//int RenderScene::TestAABB_AABB(AABB *a, AABB *b)
//{
//	// Exit with no intersection if separated along an axis
//	if (a->max.value[0] < b->min.value[0] || a->min.value[0] > b->max.value[0]) return 0;
//	if (a->max.value[1] < b->min.value[1] || a->min.value[1] > b->max.value[1]) return 0;
//	if (a->max.value[2] < b->min.value[2] || a->min.value[2] > b->max.value[2]) return 0;
//
//	// Overlapping on all axes means AABBs are intersecting
//	return 1;
//}
//
//void RenderScene::BroadPhase()
//{
//	//Sort and Sweep
//	this->SortAndSweep(this->x_axis_collision, this->x_axis, 0);
//	this->SortAndSweep(this->y_axis_collision, this->y_axis, 1);
//	this->SortAndSweep(this->z_axis_collision, this->z_axis, 2);
//
//	//Find colliding pairs in all axes
//	this->Intersect();
//}
//
//void RenderScene::NarrowPhase()
//{
//	for (std::unordered_set<std::pair<RigidBody*, RigidBody*>>::iterator it = this->broadphase_collision_list.begin(); it != this->broadphase_collision_list.end(); it++){
//		Vector3 mtv;
//		float overlap;
//		std::vector<Vector3> contact_points;
//		if (this->SAT_TestBoxBox((*it).first, (*it).second, mtv, overlap, contact_points)){
//			//actual collision
//			(*it).first->obb_color.Insert(0, 1, 1);
//			(*it).second->obb_color.Insert(0, 1, 1);
//
//
//			ShaderManager::Instance()->ChangeShader("debug");
//			for (int i = 0; i < contact_points.size(); i++)
//			{
//				Matrix44 translation = Matrix44::Translation(contact_points[i]);
//				Matrix44 mvp = projection * view * translation;
//
//				//DebugRenderer::Instance()->DrawPoint(mvp, 10, Vector3(1.f, 1.f, 0.f));
//			}
//
//			this->ResolveCollision((*it).first, (*it).second, contact_points, mtv);
//
//			this->narrowphase_collision_list.insert((*it));
//		}
//		else{
//			//no collision
//			(*it).first->obb_color.Insert(1, 1, 0);
//			(*it).second->obb_color.Insert(1, 1, 0);
//			this->narrowphase_collision_list.erase((*it));
//		}
//	}
//}
//
//bool RenderScene::SAT_TestBoxBox(RigidBody* a, RigidBody* b, Vector3 &mtv, float &overlap, std::vector<Vector3> &contact_points_out)
//{
//	//center vector
//	Vector3 center_vector = b->obb->pos - a->obb->pos;
//
//	Vector3 best_axis;
//	float best = FLT_MAX;
//	int type_of_collision;
//
//	//call overlap on axis 15 times. Return true if all tests are true
//	//if statements instead and generate contact points depending on the mtv also invert the mtv if it's from box two.
//	//point-face
//	//Box one axes
//	if (!this->OverlapOnAxis(a, b, a->obb->rot[0], center_vector, best_axis, best, type_of_collision, 0)){
//		return false;
//	}
//	if (!this->OverlapOnAxis(a, b, a->obb->rot[1], center_vector, best_axis, best, type_of_collision, 1)){
//		return false;
//	}
//	if (!this->OverlapOnAxis(a, b, a->obb->rot[2], center_vector, best_axis, best, type_of_collision, 2)){
//		return false;
//	}
//
//	//Box two axes
//	if (!this->OverlapOnAxis(a, b, b->obb->rot[0], center_vector, best_axis, best, type_of_collision, 3)){
//		return false;
//	}
//	if (!this->OverlapOnAxis(a, b, b->obb->rot[1], center_vector, best_axis, best, type_of_collision, 4)){
//		return false;
//	}
//	if (!this->OverlapOnAxis(a, b, b->obb->rot[2], center_vector, best_axis, best, type_of_collision, 5)){
//		return false;
//	}
//
//	//edge-edge
//	//9 cross products
//	if (!this->OverlapOnAxis(a, b, Vector3::Cross(a->obb->rot[0], b->obb->rot[0]), center_vector, best_axis, best, type_of_collision, 6)){
//		return false;
//	}
//	if (!this->OverlapOnAxis(a, b, Vector3::Cross(a->obb->rot[0], b->obb->rot[1]), center_vector, best_axis, best, type_of_collision, 7)){
//		return false;
//	}
//	if (!this->OverlapOnAxis(a, b, Vector3::Cross(a->obb->rot[0], b->obb->rot[2]), center_vector, best_axis, best, type_of_collision, 8)){
//		return false;
//	}
//	if (!this->OverlapOnAxis(a, b, Vector3::Cross(a->obb->rot[1], b->obb->rot[0]), center_vector, best_axis, best, type_of_collision, 9)){
//		return false;
//	}
//	if (!this->OverlapOnAxis(a, b, Vector3::Cross(a->obb->rot[1], b->obb->rot[1]), center_vector, best_axis, best, type_of_collision, 10)){
//		return false;
//	}
//	if (!this->OverlapOnAxis(a, b, Vector3::Cross(a->obb->rot[1], b->obb->rot[2]), center_vector, best_axis, best, type_of_collision, 11)){
//		return false;
//	}
//	if (!this->OverlapOnAxis(a, b, Vector3::Cross(a->obb->rot[2], b->obb->rot[0]), center_vector, best_axis, best, type_of_collision, 12)){
//		return false;
//	}
//	if (!this->OverlapOnAxis(a, b, Vector3::Cross(a->obb->rot[2], b->obb->rot[1]), center_vector, best_axis, best, type_of_collision, 13)){
//		return false;
//	}
//	if (!this->OverlapOnAxis(a, b, Vector3::Cross(a->obb->rot[2], b->obb->rot[2]), center_vector, best_axis, best, type_of_collision, 14)){
//		return false;
//	}
//
//	//////////////////////////////////////////////////////////////////////////
//	if (type_of_collision < 3){
//		//clip and get points
//
//		Vector3 normal1, normal2;
//		float neg_offset1, pos_offset1, neg_offset2, pos_offset2;
//		//get normal for all face and it's offset
//		switch (type_of_collision)
//		{
//		case 0: //skip x
//		{
//			normal1 = a->obb->rot[1].Normalized();
//			normal2 = a->obb->rot[2].Normalized();
//			float y = Vector3::Dot(a->obb->pos, normal1);
//			float z = Vector3::Dot(a->obb->pos, normal2);
//			neg_offset1 = -y + a->obb->half_extent[1];
//			pos_offset1 = y + a->obb->half_extent[1];
//			neg_offset2 = -z + a->obb->half_extent[2];
//			pos_offset2 = z + a->obb->half_extent[2];
//		}
//			break;
//
//		case 1: //skip y
//		{
//			normal1 = a->obb->rot[0].Normalized();
//			normal2 = a->obb->rot[2].Normalized();
//			float x = Vector3::Dot(a->obb->pos, normal1);
//			float z = Vector3::Dot(a->obb->pos, normal2);
//			neg_offset1 = -x + a->obb->half_extent[0];
//			pos_offset1 = x + a->obb->half_extent[0];
//			neg_offset2 = -z + a->obb->half_extent[2];
//			pos_offset2 = z + a->obb->half_extent[2];
//		}
//			break;
//
//		case 2: //skip z
//		{
//			normal1 = a->obb->rot[0].Normalized();
//			normal2 = a->obb->rot[1].Normalized();
//			float x = Vector3::Dot(a->obb->pos, normal1);
//			float y = Vector3::Dot(a->obb->pos, normal2);
//			neg_offset1 = -x + a->obb->half_extent[0];
//			pos_offset1 = x + a->obb->half_extent[0];
//			neg_offset2 = -y + a->obb->half_extent[1];
//			pos_offset2 = y + a->obb->half_extent[1];
//		}
//			break;
//		}
//		Vector3 incident_axis;
//		float incident_tracker = FLT_MAX;
//		//get incident face vertices by dotting all axis with mtv
//		for (int i = 0; i < 3; i++)
//		{
//			float most_neg = Vector3::Dot(b->obb->rot[i], best_axis);
//			if (most_neg < incident_tracker){
//				incident_tracker = most_neg;
//				incident_axis = b->obb->rot[i];
//			}
//			most_neg = Vector3::Dot(-1 * b->obb->rot[i], best_axis);
//			if (most_neg < incident_tracker){
//				incident_tracker = most_neg;
//				incident_axis = -1 * b->obb->rot[i];
//			}
//		}
//		//for (int i = 0; i < 3; i++)
//		//{
//
//		//}
//		incident_axis.Normalize();
//
//		Vector3 incident_face[4];
//		this->CalcFaceVertices(incident_face, incident_axis, b);
//
//		for (int i = 0; i < 4; i++)
//		{
//			Matrix44 debugmvp = this->projection * this->view * Matrix44::Translation(incident_face[i]);
//			DebugRenderer::Instance()->DrawPoint(debugmvp, 10.f, Vector3(1, 1, 1));
//		}
//
//
//		std::vector<Vector3> contacts;
//		this->ClipFaceToSidePlane(contacts, incident_face, -1 * normal1, neg_offset1);
//		this->ClipFaceToSidePlane(contacts, incident_face, normal1, pos_offset1);
//		this->ClipFaceToSidePlane(contacts, incident_face, -1 * normal2, neg_offset2);
//		this->ClipFaceToSidePlane(contacts, incident_face, normal2, pos_offset2);
//
//		Vector3 refNormal = best_axis;
//		float x = Vector3::Dot(a->obb->pos, refNormal);
//		float neg_offsett = -x + a->obb->half_extent[type_of_collision];
//		float pos_offsett = x + a->obb->half_extent[type_of_collision];
//
//		for (int i = 0; i < contacts.size(); i++)
//		{
//			//float Distance1 = Vector3::Dot(contacts[i], -1 * refNormal) - neg_offsett;
//			//if (Distance1 <= 0){
//			//	//keep point
//			//	contact_points_out.push_back(contacts[i]);
//			//}
//
//			float Distance2 = Vector3::Dot(contacts[i], refNormal) - pos_offsett;
//			if (Distance2 <= 0){
//				//keep point
//				contact_points_out.push_back(contacts[i]);
//			}
//		}
//	}
//	else if (type_of_collision < 6){
//		// invert best_axis
//		best_axis = -1 * best_axis;
//		//clip and get points
//
//		Vector3 normal1, normal2;
//		float neg_offset1, pos_offset1, neg_offset2, pos_offset2;
//		//get normal for all face and it's offset
//		switch (type_of_collision)
//		{
//		case 3: //skip x
//		{
//			normal1 = a->obb->rot[1].Normalized();
//			normal2 = a->obb->rot[2].Normalized();
//			float y = Vector3::Dot(a->obb->pos, normal1);
//			float z = Vector3::Dot(a->obb->pos, normal2);
//			neg_offset1 = -y + a->obb->half_extent[1];
//			pos_offset1 = y + a->obb->half_extent[1];
//			neg_offset2 = -z + a->obb->half_extent[2];
//			pos_offset2 = z + a->obb->half_extent[2];
//		}
//			break;
//
//		case 4: //skip y
//		{
//			normal1 = a->obb->rot[0].Normalized();
//			normal2 = a->obb->rot[2].Normalized();
//			float x = Vector3::Dot(a->obb->pos, normal1);
//			float z = Vector3::Dot(a->obb->pos, normal2);
//			neg_offset1 = -x + a->obb->half_extent[0];
//			pos_offset1 = x + a->obb->half_extent[0];
//			neg_offset2 = -z + a->obb->half_extent[2];
//			pos_offset2 = z + a->obb->half_extent[2];
//		}
//			break;
//
//		case 5: //skip z
//		{
//			normal1 = a->obb->rot[0].Normalized();
//			normal2 = a->obb->rot[1].Normalized();
//			float x = Vector3::Dot(a->obb->pos, normal1);
//			float y = Vector3::Dot(a->obb->pos, normal2);
//			neg_offset1 = -x + a->obb->half_extent[0];
//			pos_offset1 = x + a->obb->half_extent[0];
//			neg_offset2 = -y + a->obb->half_extent[1];
//			pos_offset2 = y + a->obb->half_extent[1];
//		}
//			break;
//		}
//		Vector3 incident_axis;
//		float incident_tracker = FLT_MAX;
//		//get incident face vertices by dotting all axis with mtv
//		for (int i = 0; i < 3; i++)
//		{
//			float most_neg = Vector3::Dot(b->obb->rot[i], best_axis);
//			if (most_neg < incident_tracker){
//				incident_tracker = most_neg;
//				incident_axis = b->obb->rot[i];
//			}
//			most_neg = Vector3::Dot(-1 * b->obb->rot[i], best_axis);
//			if (most_neg < incident_tracker){
//				incident_tracker = most_neg;
//				incident_axis = -1 * b->obb->rot[i];
//			}
//		}
//		//for (int i = 0; i < 3; i++)
//		//{
//		//	float most_neg = Vector3::Dot(-1 * b->obb->rot[i], best_axis);
//		//	if (most_neg < incident_tracker){
//		//		incident_tracker = most_neg;
//		//		incident_axis = -1 * b->obb->rot[i];
//		//	}
//		//}
//		incident_axis.Normalize();
//
//		Vector3 incident_face[4];
//		this->CalcFaceVertices(incident_face, incident_axis, b);
//		std::reverse(&incident_face[0], &incident_face[4]);
//
//		for (int i = 0; i < 4; i++)
//		{
//			Matrix44 debugmvp = this->projection * this->view * Matrix44::Translation(incident_face[i]);
//			DebugRenderer::Instance()->DrawPoint(debugmvp,10.f,Vector3(1,1,1));
//		}
//
//
//		std::vector<Vector3> contacts;
//		this->ClipFaceToSidePlane(contacts, incident_face, -1 * normal1, neg_offset1);
//		this->ClipFaceToSidePlane(contacts, incident_face, normal1, pos_offset1);
//		this->ClipFaceToSidePlane(contacts, incident_face, -1 * normal2, neg_offset2);
//		this->ClipFaceToSidePlane(contacts, incident_face, normal2, pos_offset2);
//
//
//		Vector3 refNormal = best_axis;
//		float x = Vector3::Dot(a->obb->pos, refNormal);
//		float neg_offsett = -x + a->obb->half_extent[type_of_collision-3];
//		float pos_offsett = x + a->obb->half_extent[type_of_collision-3];
//
//		for (int i = 0; i < contacts.size(); i++)
//		{
//			//float Distance1 = Vector3::Dot(contacts[i], -1 * refNormal) - neg_offsett;
//			//if (Distance1 <= 0){
//			//	//keep point
//			//	contact_points_out.push_back(contacts[i]);
//			//}
//
//			float Distance2 = Vector3::Dot(contacts[i], refNormal) - pos_offsett;
//			if (Distance2 <= 0){
//				//keep point
//				contact_points_out.push_back(contacts[i]);
//			}
//		}
//	}
//	else{
//		//edge-edge case
//		Vector3 support_edgeA[2];
//		Vector3 support_edgeB[2];
//		this->GetSupportEdge(a, mtv, support_edgeA);
//		this->GetSupportEdge(b, -1*mtv, support_edgeB);
//		this->GetContactPoint_EdgeEdge(contact_points_out, support_edgeA[0], support_edgeA[1], support_edgeB[0], support_edgeB[1]);
//	}
//
//	//return array with contact points and return mtv.
//	//contact_points = []
//	mtv = best_axis;
//	overlap = best;
//	//resolve collision outside
//	return true;
//}
//
//void RenderScene::ClipFaceToSidePlane(std::vector<Vector3> &Out, const Vector3* face, const Vector3& normal, float plane_offset)
//{
//	Vector3 Vertex1 = face[0];
//	float Distance1 = Vector3::Dot(Vertex1, normal) - plane_offset;//rnDistance(Plane, Vertex1.Position);
//
//	for (int i = 1; i < 4; i++)
//	{
//		Vector3 Vertex2 = face[i];
//		float Distance2 = Vector3::Dot(Vertex2, normal) - plane_offset;//rnDistance(Plane, Vertex2.Position);
//
//		if (Distance1 <= 0.0f && Distance2 <= 0.0f)
//		{
//			// Both vertices are behind the plane - keep vertex2
//			Out.push_back(Vertex2);
//		}
//		else if (Distance1 <= 0.0f && Distance2 > 0.0f)
//		{
//			// Vertex1 is behind the plane, vertex2 is in front -> intersection point
//			float Fraction = Distance1 / (Distance1 - Distance2);
//			Vector3 Position = Vertex1 + Fraction * (Vertex2 - Vertex1);
//
//			// Keep intersection point 
//			Vector3 Vertex = Position;
//			Out.push_back(Vertex);
//		}
//		else if (Distance2 <= 0.0f && Distance1 > 0)
//		{
//			// Vertex2 is behind the plane, vertex1 is in front -> intersection point
//			float Fraction = Distance1 / (Distance1 - Distance2);
//			Vector3 Position = Vertex1 + Fraction * (Vertex2 - Vertex1);
//
//			// Keep intersection point 
//			Vector3 Vertex = Position;
//			Out.push_back(Vertex);
//
//			// And also keep vertex2
//			Out.push_back(Vertex2);
//		}
//
//		// Keep vertex2 as starting vertex for next edge
//		Vertex1 = Vertex2;
//		Distance1 = Distance2;
//	}
//}
//
//bool RenderScene::OverlapOnAxis(RigidBody* a, RigidBody* b, Vector3 axis, Vector3 center_vector, Vector3 &mtv, float &overlap, int &type_of_collision_out, int axis_index)
//{
//	//Handle if axis generated by cross product is 0 by reporting that it's a collision. (Happens when crossing two axes with same direction)
//	if (axis == Vector3(0, 0, 0)){
//		return false;
//	}
//
//	Vector3 axis_normalized = axis.Normalized();
//	//project obb one and obb two on axis - oneProject and twoProject
//	float oneProject = this->ProjectOBBToAxis(a->obb, axis_normalized);
//	float twoProject = this->ProjectOBBToAxis(b->obb, axis_normalized);
//
//	//project the center vector to the axis as well - distance
//	float distance = fabs(Vector3::Dot(center_vector, axis_normalized));
//
//	float o = (oneProject + twoProject) - distance;
//
//	//if (o < overlap){
//	//	type_of_collision_out = axis_index;
//	//	overlap = o;
//	//	mtv = axis_normalized;
//	//}
//
//	if (this->BiasGreaterThan(overlap, o)){
//		type_of_collision_out = axis_index;
//		overlap = o;
//		mtv = axis_normalized;
//
//	}
//
//	//projections - distance = overlap
//	return (distance <= oneProject + twoProject);
//}
//
//float RenderScene::ProjectOBBToAxis(OBB* box, Vector3 axis)
//{
//	return (fabs(Vector3::Dot(box->half_extent[0] * box->rot[0].Normalized(), axis)) +
//			fabs(Vector3::Dot(box->half_extent[1] * box->rot[1].Normalized(), axis)) +
//			fabs(Vector3::Dot(box->half_extent[2] * box->rot[2].Normalized(), axis)));
//}
//
//void RenderScene::ResolveCollision(RigidBody* a, RigidBody* b, std::vector<Vector3> &contact_points, const Vector3 &mtv)
//{
//	for (int i = 0; i < contact_points.size(); i++)
//	{
//
//		const float e = std::min(a->restitution, b->restitution);
//		//Vector4 center_of_mass_world_space_a = a->GetModel() * Vector4(a->center_of_mass);
//		//Vector4 center_of_mass_world_space_b = b->GetModel() * Vector4(b->center_of_mass);
//		Vector3 rA = contact_points[i] - a->position;// Vector3(center_of_mass_world_space_a);
//		Vector3 rB = contact_points[i] - b->position;// Vector3(center_of_mass_world_space_b);
//		Vector3 kA = Vector3::Cross(rA, mtv);
//		Vector3 kB = Vector3::Cross(rB, mtv);
//		Matrix33 rot_a = a->GetModel().ConvertToMatrix33();
//		Matrix33 rotT_a = rot_a.Transposed();
//		Matrix33 inertia_tensor_a = rot_a * a->inverse_inertia_tensor * rotT_a;
//
//		Matrix33 rot_b = b->GetModel().ConvertToMatrix33();
//		Matrix33 rotT_b = rot_b.Transposed();
//		Matrix33 inertia_tensor_b = rot_b * b->inverse_inertia_tensor * rotT_b;
//		Vector3 uA = inertia_tensor_a * kA;
//		Vector3 uB = inertia_tensor_b * kB;
//
//		float fNumer = -(1 + e)*(Vector3::Dot(mtv, a->velocity - b->velocity) + Vector3::Dot(a->angular_velocity, kA) - Vector3::Dot(b->angular_velocity, kA));
//		float fDenom = a->inverse_mass + b->inverse_mass + Vector3::Dot(kA, uA) + Vector3::Dot(kB, uB);
//		float f = fNumer / fDenom;
//		//a->ApplyImpulse(mtv * f, contact_points[i]);
//		//b->ApplyImpulse(mtv * -f, contact_points[i]);
//
//
//		a->accum_force += f*mtv;
//		b->accum_force -= f*mtv;
//		a->accum_torque += (f*kA);
//		b->accum_torque -= (f*kB);
//
//		//compute derived quantities, linear/angular velocity
//		a->velocity = a->accum_force*a->inverse_mass*this->delta_time;
//		b->velocity = b->accum_force*b->inverse_mass*this->delta_time;
//		a->angular_velocity += (f*uA)*this->delta_time;
//		b->angular_velocity += (f*uB)*this->delta_time;
//
//	}
//}
//
//void RenderScene::GetSupportEdge(RigidBody* a, Vector3 &mtv, Vector3* support_out)
//{
//	//q3Vec3 absN = q3Abs(n);
//	Matrix33 tx = a->GetModel().ConvertToMatrix33();
//	Vector3 n = tx.Transposed() * mtv;
//	Vector3 absN;
//	if (n[0] < 0.0){
//		absN[0] = -n[0];
//	}
//	else{
//		absN[0] = n[0];
//	}
//	if (n[1] < 0.0){
//		absN[1] = -n[1];
//	}
//	else{
//		absN[1] = n[1];
//	}
//	if (n[2] < 0.0){
//		absN[2] = -n[2];
//	}
//	else{
//		absN[2] = n[2];
//	}
//	Vector3 aa, b;
//
//	// x > y
//	if (absN[0] > absN[1])
//	{
//		// x > y > z
//		if (absN[1] > absN[2])
//		{
//			aa.Insert(a->obb->half_extent[0], a->obb->half_extent[1], a->obb->half_extent[2]);
//			b.Insert(a->obb->half_extent[0], a->obb->half_extent[1], -a->obb->half_extent[2]);
//		}
//
//		// x > z > y || z > x > y
//		else
//		{
//			aa.Insert(a->obb->half_extent[0], a->obb->half_extent[1], a->obb->half_extent[2]);
//			b.Insert(a->obb->half_extent[0], -a->obb->half_extent[1], a->obb->half_extent[2]);
//		}
//	}
//
//	// y > x
//	else
//	{
//		// y > x > z
//		if (absN[0] > absN[2])
//		{
//			aa.Insert(a->obb->half_extent[0], a->obb->half_extent[1], a->obb->half_extent[2]);
//			b.Insert(a->obb->half_extent[0], a->obb->half_extent[1], -a->obb->half_extent[2]);
//		}
//
//		// z > y > x || y > z > x
//		else
//		{
//			aa.Insert(a->obb->half_extent[0], a->obb->half_extent[1], a->obb->half_extent[2]);
//			b.Insert(-a->obb->half_extent[0], a->obb->half_extent[1], a->obb->half_extent[2]);
//		}
//	}
//
//	float signx, signy, signz;
//	if (n[0] >= 0.0)
//	{
//		signx = 1.0;
//	}
//	else
//	{
//		signx = -1.0;
//	}
//	if (n[1] >= 0.0)
//	{
//		signy = 1.0;
//	}
//	else
//	{
//		signy = -1.0;
//	}
//	if (n[2] >= 0.0)
//	{
//		signz = 1.0;
//	}
//	else
//	{
//		signz = -1.0;
//	}
//
//	aa[0] *= signx;
//	aa[1] *= signy;
//	aa[2] *= signz;
//	b[0] *= signx;
//	b[1] *= signy;
//	b[2] *= signz;
//
//
//	support_out[0] = tx * aa + a->position;
//	support_out[1] = tx * b + a->position;
//
//	//if (Vector3::Dot(b->position - a->position, mtv) < 0.f){
//	//	mtv = -1*mtv;
//	//}
//
//	//float dmin = Vector3::Dot(a->obb->vertices[0], mtv);
//	//const float threshold = 1.0E-3f;
//	////support points of a and b. need a way to so associate the points with owner rigidbody, maybe just two arrays
//	////Vector3 support_points[4];
//	//int sI = 0;
//	//Matrix33 m = a->GetModel().ConvertToMatrix33();
//
//	//for (int i = 1; i < 8; i++){
//	//	float d = Vector3::Dot(a->obb->vertices[i], mtv);
//	//	if (d < dmin){
//	//		dmin = d;
//	//	}
//	//}
//
//	//for (int i = 0; i < 8; i++){
//	//	float d = Vector3::Dot(a->obb->vertices[i], mtv);
//
//	//	if (d < dmin + threshold){
//	//		support_out[sI] = Vector3(a->GetModel() * Vector4(a->obb->vertices[i])) + a->position;
//	//		if (sI == 1){
//	//			break;
//	//			//return num of support
//	//		}
//	//		sI++;
//	//	}
//	//}
//
//	////maybe no need just call this func twice with -mtv for rB
//	//dmin = Vector3::Dot(b->obb->vertices[0], mtv);
//	//for (int i = 1; i < 8; i++){
//	//	float d = Vector3::Dot(b->obb->vertices[i], mtv);
//	//	if (d > dmin){
//	//		dmin = d;
//	//	}
//	//}
//
//	//for (int i = 0; i < 8; i++){
//	//	float d = Vector3::Dot(b->obb->vertices[i], mtv);
//
//	//	if (d > dmin - threshold){
//	//		support_points[sI] = Vector3(b->GetModel() * Vector4(b->obb->vertices[i])) + b->position;
//	//		if (sI == 3){
//	//			sI++;
//	//			break;
//	//		}
//	//		sI++;
//	//	}
//	//}
//	//After obtaining the support points. Project them on axis perpendicular to mtv (-normal.y,normal.x, normal.z)?
//	//Sort the projected support points and pick the two in the middle. Project them on the other rigidbody edge(edge created from support points, rB is inverse)
//	//We now have the two contact points add them together and divide by 2
//
//
//	//out_contact = Vector3(a->GetModel() * Vector4(a->obb->vertices[i])) + a->position;
//}
//
//void RenderScene::GetContactPoint_EdgeEdge(std::vector<Vector3> &contact_points_out, const Vector3 &PA, const Vector3 &QA, const Vector3 &PB, const Vector3 &QB)
//{
//	Vector3 DA = QA - PA;
//	Vector3 DB = QB - PB;
//	Vector3 r = PA - PB;
//	float a = Vector3::Dot(DA, DA);
//	float e = Vector3::Dot(DB, DB);
//	float f = Vector3::Dot(DB, r);
//	float c = Vector3::Dot(DA, r);
//
//	float b = Vector3::Dot(DA, DB);
//	float denom = a * e - b * b;
//
//	float TA = (b * f - c * e) / denom;
//	float TB = (b * TA + f) / e;
//
//	Vector3 CA = PA + DA * TA;
//	Vector3 CB = PB + DB * TB;
//
//	contact_points_out.push_back((CA + CB) * 0.5f);
//}
//
//void RenderScene::CalcFaceVertices(Vector3* vertices, Vector3 axis, RigidBody *incident_body)
//{
//	Vector3 xAxis = incident_body->obb->rot[0];
//	Vector3 yAxis = incident_body->obb->rot[1];
//	Vector3 zAxis = incident_body->obb->rot[2];
//	Vector3 referencePos;
//	Vector3 u;
//	Vector3 v;
//	if (abs(Vector3::Dot(axis,xAxis)) > 0.9f)
//	{
//		referencePos = incident_body->obb->pos + incident_body->obb->half_extent[0]*xAxis;
//		u = incident_body->obb->half_extent[1] * yAxis;
//		v = incident_body->obb->half_extent[2] * zAxis;
//	}
//	else if (abs(Vector3::Dot(axis, yAxis)) > 0.9f)
//	{
//		referencePos = incident_body->obb->pos + incident_body->obb->half_extent[1] * yAxis;
//		u = incident_body->obb->half_extent[0] * xAxis;
//		v = incident_body->obb->half_extent[2] * zAxis;
//	}
//	else if (abs(Vector3::Dot(axis, zAxis)) > 0.9f)
//	{
//		referencePos = incident_body->obb->pos + incident_body->obb->half_extent[2] * zAxis;
//		u = incident_body->obb->half_extent[0] * xAxis;
//		v = incident_body->obb->half_extent[1] * yAxis;
//	}
//	vertices[0] = referencePos + u + v;
//	vertices[1] = referencePos - u + v;
//	vertices[2] = referencePos - u - v;
//	vertices[3] = referencePos + u - v;
//}