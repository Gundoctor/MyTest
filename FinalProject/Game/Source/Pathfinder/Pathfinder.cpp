#include "GamePCH.h"

Pathfinder::Pathfinder(Tilemap* pTilemap, int width, int height)
{
    m_pTilemap = pTilemap;

    m_NumNodes = width * height;

    m_OpenNodes = new int[m_NumNodes];

    m_MapWidth = width;
    m_MapHeight = height;

	m_pGraph = new Graph();
}
    
Pathfinder::~Pathfinder()
{
	delete m_pGraph;
    delete[] m_OpenNodes;
}

void Pathfinder::SetAndPrimeTilemap(Tilemap* tilemap, int width, int height)
{
    m_pTilemap = tilemap;

    m_MapWidth = width;
    m_MapHeight = height;

	m_pGraph->ReconstructGraph();
    Reset();
    SeedGraph();
}

void Pathfinder::DrawGraph()
{
	m_pGraph->DrawGridGraph();
}

void Pathfinder::Reset()
{
    m_NumOpen = 0;

    //for( int i=0; i<m_NumNodes; i++ )
    //{
    //    m_Nodes[i].parentNodeIndex = -1;
    //    m_Nodes[i].status = PathNode::Unchecked;

    //    m_Nodes[i].f = 0;
    //    m_Nodes[i].g = FLT_MAX; // set g to be highest cost possible, so any comparison will be better.
    //    m_Nodes[i].h = -1; // -1 indicates the heuristic hasn't been calculated yet.
    //} 

	m_pGraph->ResetGraph();
	m_pGraph->SetUpGridGraph(m_MapWidth, m_MapHeight);
}

void Pathfinder::SeedGraph()
{
	// I decided that the values will be decided here
	for (int col = 0; col < m_MapWidth; ++col)
	{
		for (int row = 0; row < m_MapHeight; ++row)
		{
			// If Walkable, then Set G score
			float gScore = m_pTilemap->GetCostForTile(row, col);

			m_pGraph->SetGScoreAt(row, col, gScore);
		}
	}

    // Sets up Edge scores based on each tile's G score
	m_pGraph->SetUpEdgeScores();

    // No need to reset the G score since it will be reset when we look for a path
}

void Pathfinder::PreAllocateGridGraph(int maxWidth, int maxHeight)
{
	m_pGraph->PreallocateGridGraph((unsigned int)maxWidth, (unsigned int)maxHeight);
}
        
bool Pathfinder::FindPath(int sx, int sy, int ex, int ey)
{
    // reset the pathfinder
    Reset();

    // get the starting tile index and the destination tile index 
    unsigned int startIndex = CalculateNodeIndex(sx, sy);
    unsigned int endIndex = CalculateNodeIndex(ex, ey);;

    // set starting node cost to zero, then add it to the open list to start the process.
    Node* startingNode = m_pGraph->GetNodeAtIndex(startIndex);
    startingNode->SetGScore(0);
    m_OpenNodes[m_NumOpen++] = startIndex;

    while( true )
    {
        // find the lowest F and remove it from the open list.
        int lowestFIndex = FindNodeWithLowestFInOpen();
        RemoveFromOpen(lowestFIndex);

        // if we found the end node, we're done.
        if (endIndex == lowestFIndex)
            return true;

        // mark it as closed
        Node* lowestFNode = m_pGraph->GetNodeAtIndex(lowestFIndex);
        lowestFNode->SetStatus(Node::Closed);

        // add neighbours to open list
		AddNeighboursToOpenList(lowestFNode, endIndex);

        // if we're out of nodes to check, then we're done without finding the end node.
        if( m_NumOpen == 0 )
            return false;
    }

    // this shouldn't happen, we should either find the destination or run out of nodes in the open list.
    assert( false );
    return false;
}

/******************************************
 * Return whole path instead of just the next node
 ******************************************/
int Pathfinder::GetPath(int* path, int maxdistance, int ex, int ey)
{
    int nodeIndex = CalculateNodeIndex( ex, ey );

    int length = 0;
    while( true )
    {
        if( path != nullptr && length >= maxdistance )
            return -1; // path didn't fit in size.

        if( path != nullptr ) // if no path array is passed in, just get the length
            path[length] = nodeIndex;
        length++;

		Node* node = m_pGraph->GetNodeAtIndex(nodeIndex);
		Edge* edge = node->GetParent();

        if( edge == nullptr )
            return length;

		nodeIndex = edge->GetOtherNode(node)->GetIndex();
    }

    // there should be no way to get here.
    assert( false );
    return -1;
}

void Pathfinder::AddToOpen(int nodeindex)
{
	Node::PathNodeStatus status = m_pGraph->GetNodeAtIndex(nodeindex)->GetStatus();
    assert( status != Node::Closed );

    // if the node isn't already open, then add it to the list.
    if( status != Node::Open )
    {
        m_OpenNodes[m_NumOpen] = nodeindex;
        m_NumOpen++;
        m_pGraph->GetNodeAtIndex(nodeindex)->SetStatus(Node::Open);
    }
}

void Pathfinder::RemoveFromOpen(int nodeindex)
{
    // remove the node from the open list, since we don't care about order, replace the node we're removing with the last node in list
    for( int i=0; i<m_NumOpen; i++ )
    {
        if( m_OpenNodes[i] == nodeindex )
        {
            m_NumOpen--;
            m_OpenNodes[i] = m_OpenNodes[m_NumOpen];
            return;
        }
    }
}

int Pathfinder::FindNodeWithLowestFInOpen()
{
    // loop through the nodes in the open list, then find and return the node with the lowest f score
    if (m_NumOpen <= 0)
        return -1;

    // Get the f score of the first open node
    float f = m_pGraph->GetNodeAtIndex(m_OpenNodes[0])->GetFScore();
    int lowestFIndex = m_OpenNodes[0];

    for (int i = 1; i < m_NumOpen; ++i)
    {
        // Get the Node at the index specified in the open node list
        int curIndex = m_OpenNodes[i];
        Node* curNode = m_pGraph->GetNodeAtIndex(curIndex);

        // Compare f score
        float curNodeFScore = curNode->GetFScore();
        if (curNodeFScore < f)
        {
            f = curNodeFScore;
            lowestFIndex = curIndex;
        }
    }

    return lowestFIndex;
}

int Pathfinder::CalculateNodeIndex(int tx, int ty)
{
    assert( tx >= 0 && tx < m_MapWidth &&
            ty >= 0 && ty < m_MapHeight );

    // calculate the node index based on the tiles x/y
	// need to invert y value
	ty = m_MapHeight - 1 - ty;

    return ty * m_MapWidth + tx;
}

int Pathfinder::CheckIfNodeIsClearAndReturnNodeIndex(Node* node)
{
	int nodeIndex = node->GetIndex();

    // if the node is out of bounds, return -1 (an invalid tile index)
	if (nodeIndex > m_MapHeight * m_MapWidth || nodeIndex < 0)
		return -1;

    // if the node is already closed, return -1 (an invalid tile index)
	if (node->GetStatus() == Node::Closed)
		return -1;
        
    // if the node can't be walked on, return -1 (an invalid tile index)
	bool isWalkable = m_pTilemap->GetTileInfo(nodeIndex)->m_Walkable;
	if (!isWalkable)
		return -1;

    // return a valid tile index
	return nodeIndex;
}

void Pathfinder::AddNeighboursToOpenList(Node* node, int endNodeIndex)
{
    // calculate the tile x/y based on the nodeIndex

    // create an array of the four neighbour tiles
	unsigned int edgeCount = node->GetEdgeCount();
	for (unsigned int i = 0; i < edgeCount; ++i)
	{
		Edge* curEdge = node->GetEdgeAtIndex(i);

		// check if the node to add has a valid node index
		Node* neighbourNode = curEdge->GetOtherNode(node);
		int nodeIndex = CheckIfNodeIsClearAndReturnNodeIndex(neighbourNode);
		if (nodeIndex >= 0)
		{
			// add the node to the open list
			AddToOpen(neighbourNode->GetIndex());

			// if the cost to get there from here is less than the previous cost to get there, then overwrite the values.
			float currentG = neighbourNode->GetGScore();
			float calculatedG = node->GetGScore() + curEdge->GetEScore();
			if (calculatedG <= currentG)
			{
				if (calculatedG == currentG)
				{
					int chanceToCont = rand() % 2;

					if (chanceToCont)
						continue;
				}

				// set the parent node.
				neighbourNode->SetParent(curEdge);

				// calculate the cost to travel to that node.
				neighbourNode->SetGScore(calculatedG);

				// if we haven't already calculated the heuristic, calculate it.
				float h = neighbourNode->GetHScore();
				if (h < 0)
				{
					h = CalculateH(neighbourNode->GetIndex(), endNodeIndex);
					neighbourNode->SetHScore(h);
				}

				// calculate the final value
				float f = h + calculatedG;
				neighbourNode->SetFScore(f);
			}
		}
	}
}

float Pathfinder::CalculateH(int nodeIndex1, int nodeIndex2)
{
    // calculate the h score using the manhatten distance (absolute diff in x + absolute diff in y)
	int node1x = nodeIndex1 % m_MapHeight;
	int node1y = nodeIndex1 - node1x;

	int node2x = nodeIndex2 % m_MapHeight;
	int node2y = nodeIndex2 - node2x;

	return (float)(abs(node2x - node1x) + abs(node2y - node1y));
}