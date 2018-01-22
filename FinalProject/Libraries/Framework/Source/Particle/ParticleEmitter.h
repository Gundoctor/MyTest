#ifndef __PARTICLEEMITTER_H__
#define __PARTICLEEMITTER_H__


class ParticleSystem;

class ParticleEmitter
{
	friend class ParticleSystem;

public:
	ParticleEmitter(ParticleSystem* sys);
	~ParticleEmitter();

	void Update(float deltatime);
    void SetUpEmitter(Vector2 pos, float freq, Vector2 velo, float partLifeSpan);

	// Setters
	void SetPosition(Vector2 position);
	void SetFrequency(float frequency);
	void SetVelocity(Vector2 velocity);
    void SetParticleLife(float partLifeSpan);

private:
	// Private Member Variables
	ParticleSystem * m_pSystem;

	Vector2 m_position;
	float m_frequency;
	float m_period;
	Vector2 m_velocity;
    float m_partLifeSpan;

	float m_timer;

	// Private Methods
	void CalculatePeriod();
	void SpawnParticle();
};


#endif