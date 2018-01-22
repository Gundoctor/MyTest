#include "FrameworkPCH.h"


ParticleSystem::ParticleSystem(ParticleRenderer* rend) :
	m_pRenderer(rend),
	m_NumActiveParticles(0)
{
	// Allocate space for particles
	m_pParticles = new ParticleFormat[MAX_NUM_PARTICLE];

	// All particles are inactive
	// Not super necessary to do this backwards, but since we pop from the back
	// Might as well have the order in reverse
	m_InactiveParticleIndices.reserve(MAX_NUM_PARTICLE);
	for (int i = MAX_NUM_PARTICLE - 1; i >= 0 ; --i)
	{
		m_InactiveParticleIndices.push_back(i);
	}
}

ParticleSystem::~ParticleSystem()
{
	// Put all active emitters to be deleted
	for (ParticleEmitter* em : m_pEmitters)
	{
		QueueEmitterToBeDestroyed(em);
	}

	// Delete all emitters queued up to be deleted
	for (ParticleEmitter* em : m_pEmittersToBeDestroyed)
	{
		DestroyEmitter(em);
	}
	m_pEmittersToBeDestroyed.clear();
    m_pEmitters.clear();


	if (m_pParticles != nullptr)
		delete[] m_pParticles;
}

void ParticleSystem::Update(float deltatime)
{
	// Delete emitters that need to be destroyed
	if (m_pEmittersToBeDestroyed.size() > 0)
	{
		for (ParticleEmitter* em : m_pEmittersToBeDestroyed)
		{
			DestroyEmitter(em);
		}
		m_pEmittersToBeDestroyed.clear();
	}

	// Deactivate
	for (int i = m_NumActiveParticles - 1; i >= 0; --i)
	{
		unsigned int particleIndex = m_pActivePartIndices[i];
		if (m_pParticles[particleIndex].TimeToLive <= 0.0f)
		{
			DeactivateParticle(i);
		}
	}

	// Update particles
	for (int i = 0; i < m_NumActiveParticles; ++i)
	{
		unsigned int particleIndex = m_pActivePartIndices[i];

		m_pParticles[particleIndex].Position += (m_pParticles[particleIndex].Velocity * deltatime);
		m_pParticles[particleIndex].TimeToLive -= deltatime;
	}

	// Update Valid Emitters
	// Updating emitters will only spawn new particles
	for (ParticleEmitter* em : m_pEmitters)
	{
		em->Update(deltatime);
	}

	// Add all active particles to the renderer
	m_pRenderer->ResetActiveParticleCount();
	for (int i = 0; i < m_NumActiveParticles; ++i)
	{
		unsigned int particleIndex = m_pActivePartIndices[i];
		m_pRenderer->AddParticle(m_pParticles[particleIndex].Position, m_pParticles[particleIndex].Size);
	}
}

void ParticleSystem::CreateBurstAtLocation(Vector2 position, float partLifeSpan, unsigned int numToSpawn)
{
	for (unsigned int i = 0; i < numToSpawn; ++i)
	{
		Vector2 velo = GetRandomVelocity();
		CreateParticle(position, velo, Vector2(PARTICLE_SIZE, PARTICLE_SIZE), partLifeSpan);
	}
}

void ParticleSystem::CreateParticle(Vector2 position, Vector2 velocity, Vector2 size, float timeToLive)
{
	if (m_NumActiveParticles >= MAX_NUM_PARTICLE)
		return;

	// Get the last element from the inactive list
	unsigned int newPartIndex = m_InactiveParticleIndices.back();
	m_InactiveParticleIndices.pop_back();

	// "Add" the new particle's index to the end of the active list
	m_pActivePartIndices[m_NumActiveParticles++] = newPartIndex;

	m_pParticles[newPartIndex].Position = position;
	m_pParticles[newPartIndex].Velocity = velocity;
	m_pParticles[newPartIndex].Size = size;
	m_pParticles[newPartIndex].TimeToLive = timeToLive;
}

void ParticleSystem::DeactivateParticle(unsigned int activeListIndex)
{
	// Add particle to be deactivated to the inactive list
	unsigned int deactivateIndex = m_pActivePartIndices[activeListIndex];
	m_InactiveParticleIndices.push_back(deactivateIndex);

	// Decrement number of active particles and copy the last active element to the
	// element that is being deactivated
	m_NumActiveParticles--;
	m_pActivePartIndices[activeListIndex] = m_pActivePartIndices[m_NumActiveParticles];
}

ParticleEmitter* ParticleSystem::CreateEmitter(Vector2 position, float frequency, Vector2 velocity, float partLifeSpan)
{
	ParticleEmitter* emitter = new ParticleEmitter(this);
	emitter->SetUpEmitter(position, frequency, velocity, partLifeSpan);
	m_pEmitters.push_back(emitter);

	return emitter;
}

void ParticleSystem::QueueEmitterToBeDestroyed(ParticleEmitter* emitterToBeDestroyed)
{
	// Add to destroy list
	m_pEmittersToBeDestroyed.push_back(emitterToBeDestroyed);
}

void ParticleSystem::DestroyEmitter(ParticleEmitter* emitterToDestroy)
{
	// Find emitter in list
	std::vector<ParticleEmitter*>::iterator it = std::find(m_pEmitters.begin(), m_pEmitters.end(), emitterToDestroy);

	assert(it != m_pEmitters.end());
	if (it != m_pEmitters.end())
		return;

	// Remove from active list
	m_pEmitters.erase(it);

	// Delete
	delete emitterToDestroy;
}

Vector2 ParticleSystem::GetRandomVelocity()
{
	// make completely random
	int randX = rand() % (MAX_RAND_X * 2) - MAX_RAND_X;
	int randY = rand() % (MAX_RAND_Y * 2) - MAX_RAND_Y;

	return Vector2((float)randX, (float)randY);
}