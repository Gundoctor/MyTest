#include "FrameworkPCH.h"


ParticleEmitter::ParticleEmitter(ParticleSystem* sys) :
	m_pSystem(sys),
	m_position(0.0f, 0.0f),
	m_frequency(0.0f),
	m_period(0.0f),
	m_velocity(0.0f, 0.0f),
	m_timer(0.0f),
    m_partLifeSpan(PARTICLE_LIFE)
{
	
}

ParticleEmitter::~ParticleEmitter()
{

}

void ParticleEmitter::Update(float deltatime)
{
	m_timer += deltatime;
	if (m_timer > m_period)
	{
		m_timer -= m_period;

		// Spawn a particle
		SpawnParticle();
	}
}

void ParticleEmitter::SetUpEmitter(Vector2 pos, float freq, Vector2 velo, float partLifeSpan)
{
	SetPosition(pos);
	SetFrequency(freq);
	SetVelocity(velo);
    SetParticleLife(partLifeSpan);
}

void ParticleEmitter::SetPosition(Vector2 position)
{
	m_position = position;
}

void ParticleEmitter::SetFrequency(float frequency)
{
	m_frequency = frequency;
	CalculatePeriod();
}

void ParticleEmitter::SetVelocity(Vector2 velocity)
{
	m_velocity = velocity;
}

void ParticleEmitter::SetParticleLife(float partLifeSpan)
{
    m_partLifeSpan = partLifeSpan;
}

void ParticleEmitter::CalculatePeriod()
{
	m_period = 1 / (MIN_SPAWN_FREQUENCY + (m_frequency * (MAX_SPAWN_FREQUENCY - MIN_SPAWN_FREQUENCY)));
}

void ParticleEmitter::SpawnParticle()
{
	// Apply randomized offset to velocity
    Vector2 velo(0.0f, 0.0f);
    if (m_velocity.LengthSquared() > 0)
    {
        // Randomize velocity
        // +/- 10 degrees -> PI / (2 * 9)
        float angleOffset = ((rand() % 101) - 50) / 100.0f;
        angleOffset *= PI / 9;


        float angle = atanf(m_velocity.y / m_velocity.x);
        if (m_velocity.x < 0.0f && m_velocity.y < 0)
            angle += PI;
        angle += angleOffset;

        float mag = m_velocity.Length();
        velo.Set(mag * cosf(angle), mag * sinf(angle));
    }
    else
    {
		velo = m_pSystem->GetRandomVelocity();
    }

	m_pSystem->CreateParticle(m_position, velo, Vector2(PARTICLE_SIZE, PARTICLE_SIZE), m_partLifeSpan);
}