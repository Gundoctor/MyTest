#ifndef __EDGE_H__
#define __EDGE_H__

class Node;

class Edge
{
public:
	Edge();
	~Edge();

	// Implies bidirectional graph
	Node* GetOtherNode(Node* node);
	void ResetEdge();
	void SetEdgeEndPoints(Node* node1, Node* node2);
    void CalculateEScore();

	// Getters
	float GetEScore();
	Node* GetNode1();
	Node* GetNode2();

	// Setters
	void SetEScore(float eScore);

private:
	// Member variables
    float m_EScore; // Cost to get from one node to the other

	Node *m_pNode1;
	Node *m_pNode2;
	
};


#endif