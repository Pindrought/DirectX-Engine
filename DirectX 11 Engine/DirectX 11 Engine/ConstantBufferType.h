#pragma once
#include "Light.h"

using namespace DirectX;

struct CB_VS_DEFAULT
{
	XMMATRIX  wvp; //world view projection matrix
	XMMATRIX world;
};

struct CB_PS_LIGHT
{
	Light light;
};

struct CB_PS_POINTLIGHT
{
	PointLight light;
};