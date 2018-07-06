#pragma once
#ifndef ShaderHelper_h__
#define ShaderHelper_h__
#include <d3d11.h>

class ShaderLoader
{
public:
	static HRESULT LoadCompiledShader(const char *filename, ID3DBlob **blob);
};

#endif // ShaderHelper_h__