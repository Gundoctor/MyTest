#ifndef __NODE_H__
#define __NODE_H__

#define NODE_CHILDREN_NUM 4

class Edge;

class Node
{
public:
	enum PathNodeStatus
	{
		Unchecked,
		Open,
		Closed,
	};

	Node();
	~Node();

    // Reset for pathfinding
	void ResetNode();

    // Resets the node and clears all children
    void ClearNode();

	void AddEdge(Edge* child);

    // Used for updating the GScore of the tile, will update all attached edges scores
    void UpdateGScore(float gScore);

	// Getters
	Node::PathNodeStatus GetStatus();
	Edge* GetParent();
    float GetGScore();
    float GetFScore();
	float GetHScore();

	int GetIndex();

	unsigned int GetEdgeCount();
	Edge* GetEdgeAtIndex(unsigned int childIndex);

	// Setters
	void SetStatus(Node::PathNodeStatus status);
	void SetParent(Edge* parent);
	void SetGScore(float gScore);
	void SetFScore(float fScore);
	void SetHScore(float hScore);

	void SetIndex(int index);


private:
    // Private Member Variables
	PathNodeStatus m_Status;

    float m_GScore; // Hidden value of the tile itself, used for calculating G score of edges
    float m_FScore; // current sum.
    float m_HScore; // heuristic: manhatten distance to destination.

	Edge *m_ParentPath;
	std::vector<Edge*> m_Children;

	int m_Index;

    // Private Methods
    void UpdateAllChildren();
};


#endif