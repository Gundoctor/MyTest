#include "FrameworkPCH.h"


Edge::Edge() :
	m_EScore(FLT_MAX),
	m_pNode1(nullptr),
	m_pNode2(nullptr)
{
}

Edge::~Edge()
{
}

Node* Edge::GetOtherNode(Node* node)
{
	if (m_pNode1 == node)
		return m_pNode2;
	else if (m_pNode2 == node)
		return m_pNode1;
	else
		return nullptr;
}

void Edge::ResetEdge()
{
	m_EScore = FLT_MAX;

	m_pNode1 = nullptr;
	m_pNode2 = nullptr;
}

void Edge::SetEdgeEndPoints(Node* node1, Node* node2)
{
	m_pNode1 = node1;
	m_pNode2 = node2;
}

void Edge::CalculateEScore()
{
    m_EScore = m_pNode1->GetGScore() + m_pNode2->GetGScore();
}

float Edge::GetEScore()
{
	return m_EScore;
}

Node* Edge::GetNode1()
{
	return m_pNode1;
}

Node* Edge::GetNode2()
{
	return m_pNode2;
}

void Edge::SetEScore(float eScore)
{
	m_EScore = eScore;
}