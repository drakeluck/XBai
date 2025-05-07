﻿#ifndef SANDBOX_PARTICLE_SYSTEM_H
#define SANDBOX_PARTICLE_SYSTEM_H

#include "XBai.h"

// 发射点属性
struct ParticleProps
{
	glm::vec2 Position;
	glm::vec2 Velocity, VelocityVariation;
	glm::vec4 ColorBegin, ColorEnd;
	float SizeBegin, SizeEnd, SizeVariation;
	float LifeTime = 1.0f;
};

class ParticleSystem
{
public:
	ParticleSystem(uint32_t maxParticles = 10000);

	void OnUpdate(XBai::TimeStep ts);
	void OnRender(XBai::OrthographicCamera& camera);

	void Emit(const ParticleProps& particleProps);
private:
	// 一个个粒子属性
	struct Particle
	{
		glm::vec2 Position;
		glm::vec2 Velocity;
		glm::vec4 ColorBegin, ColorEnd;
		float Rotation = 0.0f;
		float SizeBegin, SizeEnd;

		float LifeTime = 1.0f;
		float LifeRemaining = 0.0f;

		bool Active = false;
	};
	std::vector<Particle> m_ParticlePool;// 粒子池
	uint32_t m_PoolIndex;
};

#endif