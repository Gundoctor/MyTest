#ifndef __SCOREDISPLAY_H__
#define __SCOREDISPLAY_H__

#define NUM_SCORE_DIGIT 6
#define NUM_INDICES_NEEDED NUM_SCORE_DIGIT * 6

class ShaderProgram;

class ScoreDisplay
{
	struct ScoreVertexFormat
	{
		// position 1, 2, 3, 4, 5, 6
		float unitPosition;
		float x, y;
		float u, v;
	};


public:
	ScoreDisplay(ShaderProgram* pShader, GLuint spriteStripID);
	~ScoreDisplay();


	void Draw(Vector2 scorePos, Vector2 camProj, Vector2 scoreScale);
	void UpdateScore(int score);

	int GetScore();

private:
	// Private member variables
	ShaderProgram* m_pShader;
	GLuint m_SpriteStripTexID;
	GLuint m_VBO;
	GLuint m_IBO;

	int m_Score;

	
	// Private Methods
	void Initialize();

};

#endif