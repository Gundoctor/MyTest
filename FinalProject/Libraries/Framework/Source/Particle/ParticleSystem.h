#ifndef __PARTICLESYSTEM_H__
#define __PARTICLESYSTEM_H__

#define MAX_NUM_PARTICLE		1000
#define MAX_SPAWN_FREQUENCY		10		// 10 per second
#define MIN_SPAWN_FREQUENCY		0.1f	// 1 per 600 milliseconds (
#define PARTICLE_SIZE			25.0f	
#define PARTICLE_LIFE			5.0f

#define MAX_RAND_X 150
#define MAX_RAND_Y 150

class ParticleEmitter;
class ParticleRenderer;

class ParticleSystem
{
	friend class ParticleEmitter;

	struct ParticleFormat
	{
		Vector2 Position;
		Vector2 Velocity;
		Vector2 Size;
		float TimeToLive;
	};

public:
	ParticleSystem(ParticleRenderer* rend);
	~ParticleSystem();

	void Update(float deltatime);
	void CreateBurstAtLocation(Vector2 position, float partLifeSpan, unsigned int numToSpawn);

	// position for the spawn point of the emitter, frequency is 0.0 - 1.0, speed is 0.0 - 1.0. 0 for min, 1 for max
	ParticleEmitter* CreateEmitter(Vector2 position, float frequency, Vector2 velocity, float partLifeSpan);
	void QueueEmitterToBeDestroyed(ParticleEmitter* emitterToBeDestroyed);

private:
	// Private Member Variables
	ParticleRenderer* m_pRenderer;

	ParticleFormat *m_pParticles;
	unsigned int m_pActivePartIndices[MAX_NUM_PARTICLE];
	std::vector<unsigned int> m_InactiveParticleIndices;

	int m_NumActiveParticles;

	std::vector<ParticleEmitter*> m_pEmitters;
	std::vector<ParticleEmitter*> m_pEmittersToBeDestroyed;


	// Private Methods
	void CreateParticle(Vector2 position, Vector2 velocity, Vector2 size, float timeToLive);
	void DeactivateParticle(unsigned int activeListIndex);
	void DestroyEmitter(ParticleEmitter* emitterToDestroy);

	Vector2 GetRandomVelocity();
	
};

#endif