#ifndef __GRAPH_H__
#define __GRAPH_H__

class Node;
class Edge;

class Graph
{
public:
	Graph();
	~Graph();

	void PreallocateGridGraph(unsigned int width, unsigned int height);
	void SetUpGridGraph(unsigned int width, unsigned int height);

    // Resets the graph for pathfinding
	void ResetGraph();

    // Reconstructs the graph from scratch
    void ReconstructGraph();

	void SetGScoreAt(unsigned int row, unsigned int col, float gScore);
	void SetUpEdgeScores();

    // Visual display for sanity
	void DrawGridGraph();

    // Getters
    Node* GetNodeAt(unsigned int row, unsigned int col);
    Node* GetNodeAtIndex(unsigned int index);

private:
	// Member variables
	Node* m_pNodes;
	Edge* m_pEdges;

	unsigned int m_MaxNodeCount;
	unsigned int m_MaxEdgeCount;
	unsigned int m_Width;
	unsigned int m_Height;

	// Private methods
	Edge* GetEdgeAtIndex(unsigned int index);
	unsigned int GetIndexAt(unsigned int row, unsigned int col);

	void AttachEdgeToNodes(Edge* edge, Node* node1, Node* node2);

	unsigned int GetEdgeCount(unsigned int width, unsigned int height);
	unsigned int GetNodeCount(unsigned int width, unsigned int height);


};



#endif