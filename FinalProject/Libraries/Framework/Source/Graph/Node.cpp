#include "FrameworkPCH.h"

Node::Node() :
	m_Status(PathNodeStatus::Unchecked),
	m_ParentPath(nullptr),
    m_GScore(FLT_MAX),
	m_FScore(0),
	m_HScore(-1),
	m_Index(-1)
{
	m_Children.reserve(NODE_CHILDREN_NUM);
}

Node::~Node()
{

}

void Node::ResetNode()
{
	m_Status = PathNodeStatus::Unchecked;
	m_ParentPath = nullptr;
    m_GScore = FLT_MAX;
	m_FScore = 0;
	m_HScore = -1;
}

void Node::ClearNode()
{
    ResetNode();

    m_Children.clear();
}

void Node::AddEdge(Edge* child)
{
	m_Children.push_back(child);
}

void Node::UpdateGScore(float gScore)
{
    m_GScore = gScore;

    // Update all attached edge's scores
    UpdateAllChildren();
}

void Node::UpdateAllChildren()
{
    for (unsigned int i = 0; i < m_Children.size(); ++i)
    {
        // Update all the edges escore
        m_Children[i]->CalculateEScore();
    }
}

Node::PathNodeStatus Node::GetStatus()
{
	return m_Status;
}

Edge* Node::GetParent()
{
	return m_ParentPath;
}

float Node::GetGScore()
{
	return m_GScore;
}

float Node::GetFScore()
{
	return m_FScore;
}

float Node::GetHScore()
{
	return m_HScore;
}

int Node::GetIndex()
{
	return m_Index;
}

unsigned int Node::GetEdgeCount()
{
	return m_Children.size();
}

Edge* Node::GetEdgeAtIndex(unsigned int childIndex)
{
	assert(childIndex < m_Children.size());

	return m_Children[childIndex];
}

void Node::SetStatus(PathNodeStatus status)
{
	m_Status = status;
}

void Node::SetParent(Edge* parent)
{
	m_ParentPath = parent;
}

void Node::SetGScore(float gScore)
{
	m_GScore = gScore;
}

void Node::SetFScore(float fScore)
{
	m_FScore = fScore;
}

void Node::SetHScore(float hScore)
{
	m_HScore = hScore;
}

void Node::SetIndex(int index)
{
	m_Index = index;
}