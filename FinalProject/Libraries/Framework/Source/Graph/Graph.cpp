#include "FrameworkPCH.h"


Graph::Graph() :
	m_pNodes(nullptr),
	m_pEdges(nullptr),
	m_MaxNodeCount(0),
	m_MaxEdgeCount(0),
	m_Width(0),
	m_Height(0)
{

}

Graph::~Graph()
{
	delete[] m_pNodes;
	delete[] m_pEdges;
}

/******************************************************
 * To be used once to allocate enough space for the largest size graph
 * 
 ******************************************************/
void Graph::PreallocateGridGraph(unsigned int width, unsigned int height)
{
	assert(m_pNodes == nullptr && m_pEdges == nullptr);
	assert(m_MaxNodeCount <= 0 && m_MaxEdgeCount <= 0);

	m_Width = width;
	m_Height = height;
	m_MaxNodeCount = GetNodeCount(m_Width, m_Height);
	m_MaxEdgeCount = GetEdgeCount(m_Width, m_Height);

	m_pNodes = new Node[m_MaxNodeCount];
	m_pEdges = new Edge[m_MaxEdgeCount];
}

void Graph::SetUpGridGraph(unsigned int width, unsigned int height)
{
	unsigned int nodeCount = GetNodeCount(width, height);
	assert(nodeCount <= m_MaxNodeCount);

	unsigned int edgeCount = GetEdgeCount(width, height);
	assert(edgeCount <= m_MaxEdgeCount);

	m_Width = width;
	m_Height = height;

	unsigned int currEdgeCount = 0;
	for (unsigned int col = 0; col < m_Width; ++col)
	{
		for (unsigned int row = 0; row < m_Height; ++row)
		{
			// Add Node to the right and below
			// But only if there are nodes there
			Node* currNode = GetNodeAt(row, col);
			currNode->SetIndex(GetIndexAt(row, col));

			// If col = m_Width - 1, don't do the right edge
			if (col < m_Width - 1)
			{
				// Attach currNode with right node
				Node* rightNode = GetNodeAt(row, col + 1);
				Edge* rightEdge = &(m_pEdges[currEdgeCount++]);
				AttachEdgeToNodes(rightEdge, currNode, rightNode);
			}

			// if row = m_Height - 1, don't do the bottom edge
			if (row < m_Height - 1)
			{
				// Attach currNode with bottomNode
				Node* downNode = GetNodeAt(row + 1, col);
				Edge* downEdge = &(m_pEdges[currEdgeCount++]);
				AttachEdgeToNodes(downEdge, currNode, downNode);
			}
		}
	}
}

void Graph::ResetGraph()
{
	// Reset each node
	unsigned int nodeCount = GetNodeCount(m_Width, m_Height);
	for (unsigned int i = 0; i < nodeCount; ++i)
	{
		m_pNodes[i].ResetNode();
	}

    // No need to do anything to edges since they'll be recalculated when seeding the graph
}

void Graph::ReconstructGraph()
{
    // Clear each node of children edges
    unsigned int nodeCount = GetNodeCount(m_Width, m_Height);
    for (unsigned int i = 0; i < nodeCount; ++i)
    {
        m_pNodes[i].ClearNode();
    }

    // Reset Each Edge
    //unsigned int edgeCount = (m_Width * (m_Height - 1)) + (m_Height * (m_Width - 1)); // Long form
    unsigned int edgeCount = GetEdgeCount(m_Width, m_Height); // Factored
    for (unsigned int i = 0; i < edgeCount; ++i)
    {
        m_pEdges[i].ResetEdge();
    }
}

void Graph::SetGScoreAt(unsigned int row, unsigned int col, float gScore)
{
	unsigned int index = GetIndexAt(row, col);
	m_pNodes[index].SetGScore(gScore);
}

void Graph::SetUpEdgeScores()
{
	unsigned int edgeCount = GetEdgeCount(m_Width, m_Height);

	for (unsigned int i = 0; i < edgeCount; ++i)
	{
		Edge* curEdge = GetEdgeAtIndex(i);

		// Calculate EScore
		curEdge->CalculateEScore();
	}
}

void Graph::DrawGridGraph()
{
	for (unsigned int row = 0; row < m_Height; ++row)
	{
		for (unsigned int col = 0; col < m_Width; ++col)
		{
			unsigned int index = GetIndexAt(row, col);

			// OutputMessage("%.1f ", m_pNodes[index].GetGScore());
			OutputMessage("%d\t\t", index);
		}
		OutputMessage("\n");
	}
}

unsigned int Graph::GetIndexAt(unsigned int row, unsigned int col)
{
	return row * m_Width + col;
}

Node* Graph::GetNodeAt(unsigned int row, unsigned int col)
{
	assert(m_pNodes != nullptr);

    unsigned int index = GetIndexAt(row, col);
	return GetNodeAtIndex(index);
}

Node* Graph::GetNodeAtIndex(unsigned int index)
{
    return &(m_pNodes[index]);
}

Edge* Graph::GetEdgeAtIndex(unsigned int index)
{
	assert(m_pEdges != nullptr);

	return &(m_pEdges[index]);
}

void Graph::AttachEdgeToNodes(Edge* edge, Node* node1, Node* node2)
{
	assert(edge != nullptr);
	assert(node1 != nullptr);
	assert(node2 != nullptr);

	node1->AddEdge(edge);
	node2->AddEdge(edge);
	edge->SetEdgeEndPoints(node1, node2);
}

unsigned int Graph::GetEdgeCount(unsigned int width, unsigned int height)
{
	return 2 * width * height - (width + height);
}

unsigned int Graph::GetNodeCount(unsigned int width, unsigned int height)
{
	return width * height;
}