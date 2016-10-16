#pragma once

//Math Library
#include "mypersonalmathlib.h"

#include <vector>

/**
@class Node

Abstract Base Class for Nodes
*/
class Node
{
public:
	/// Constructor
	Node();
	/// Pure virtual destructor to make this class abstract
	virtual ~Node(void) = 0;

	/// Call the childrens' update. Reimplement in subclass for dynamic binding
	virtual void Update(Matrix44& view, const Matrix44& model = Matrix44());
	/// Call the childrens' draw. Reimplement in subclass for dynamic binding
	virtual void Draw(const Matrix44& projection, const Matrix44& view);
	virtual void Integrate(float timestep);
	/// Adds a child node
	void AddChildNode(Node* childNode);
	/// Removes a child node
	void RemoveChildNode(Node* childNode);

protected:
	//Holds children nodes
	std::vector<Node*> children;
};