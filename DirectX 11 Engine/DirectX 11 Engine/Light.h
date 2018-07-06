#pragma once
#include <DirectXMath.h>
#include <Windows.h>

using namespace DirectX;

struct Light
{
	Light()
	{
		ZeroMemory(this, sizeof(Light));
	}
	XMFLOAT3 dir;
	float pad;
	XMFLOAT4 ambient;
	XMFLOAT4 diffuse;
};

struct PointLight
{
	PointLight()
	{
		ZeroMemory(this, sizeof(PointLight));
	}
	XMFLOAT4 color;
	XMVECTOR pos;
	XMFLOAT3 attenuation;
	float range;
};