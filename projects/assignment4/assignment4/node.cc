#include "node.h"

Node::Node()
{
}

Node::~Node(void)
{
	//Clear children list
	this->children.clear();
}

void Node::Update(Matrix44& view, const Matrix44& model)
{
	//Calls the childrens' update
	if (!this->children.empty())
	{
		for (int i = 0; i < this->children.size(); i++)
		{
			if (this->children[i] != nullptr)
			{
				this->children[i]->Update(view, model);
			}
		}
	}
}

//------------------------------------------------------------------------------
/**
	mvp matrices as arguments
*/
void Node::Draw(const Matrix44& projection, const Matrix44& view)
{
	//Calls the childrens' draw
	if (!this->children.empty())
	{
		for (int i = 0; i < this->children.size(); i++)
		{
			if (this->children[i] != nullptr)
			{
				this->children[i]->Draw(projection, view);
			}
		}
	}
}

void Node::Integrate(float timestep)
{
	//Calls the childrens' draw
	if (!this->children.empty())
	{
		for (int i = 0; i < this->children.size(); i++)
		{
			if (this->children[i] != nullptr)
			{
				this->children[i]->Integrate(timestep);
			}
		}
	}
}

void Node::AddChildNode(Node* childNode)
{
	//Sets the child node's parent to this and adds the child to this node's children list
	if(childNode != nullptr)
	{
		this->children.push_back(childNode);
	}
}

void Node::RemoveChildNode(Node* childNode)
{
	//Iterate the children list and find the the child. Then remove it from the list
	if (childNode != nullptr && !this->children.empty())
	{
		for (int i = 0; i < this->children.size(); i++)
		{
			if (this->children[i] == childNode)
			{
				this->children.erase(this->children.begin() + i);
				break; //break the for loop
			}
		}
	}
}
