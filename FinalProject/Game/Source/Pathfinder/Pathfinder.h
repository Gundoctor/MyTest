#ifndef __PATHFINDER_H__
#define __PATHFINDER_H__


class Pathfinder
{
protected:
    int m_MapWidth;
    int m_MapHeight;

    Tilemap* m_pTilemap;
	Graph*	m_pGraph;

    // PathNode* m_Nodes;
    int m_NumNodes;

    int* m_OpenNodes;
    int m_NumOpen;

    // Internal methods
    void AddToOpen(int nodeindex);
    void RemoveFromOpen(int nodeindex);
    int FindNodeWithLowestFInOpen();

    int CalculateNodeIndex(int tx, int ty);
    int CheckIfNodeIsClearAndReturnNodeIndex(Node* node);
    void AddNeighboursToOpenList(Node* node, int endNodeIndex);

    float CalculateH(int nodeIndex1, int nodeIndex2);

public:
    Pathfinder(Tilemap* pTilemap, int width, int height);
    virtual ~Pathfinder();

    void SetAndPrimeTilemap(Tilemap* tilemap, int width, int height);
	void DrawGraph();
    void Reset();
	void SeedGraph();
	void PreAllocateGridGraph(int maxWidth, int maxHeight);

    // Start a search, supply the starting tile x,y and ending tile x,y
    // returns true if path found, false otherwise
    bool FindPath(int sx, int sy, int ex, int ey);

    // Retrieve the final path, pass a nullptr for "path" to just get length of the path
    int GetPath(int* path, int maxdistance, int ex, int ey);
};

#endif //__PathFinder_H__
