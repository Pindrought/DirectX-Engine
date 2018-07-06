#include "ShaderLoader.h"
#include <fstream>
#include <memory>
#include <stdint.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#pragma comment(lib,"D3DCompiler.lib")
using namespace std;

HRESULT ShaderLoader::LoadCompiledShader(const char *filename, ID3DBlob **blob)
{
	ifstream ifs(filename, ios::binary);
	if (ifs.bad() || ifs.fail())
	{
		string failmsg = "Failed to load shader from ";
		failmsg.append(filename);
		return E_ACCESSDENIED;
	}

	ifs.seekg(0, ios::end);
	size_t size = (size_t)ifs.tellg();
	auto buf = unique_ptr<char>(new char[size]);

	ifs.seekg(0, ios::beg);
	ifs.read(buf.get(), size);
	ifs.close();

	if (FAILED(D3DCreateBlob(size, blob)))
		return E_OUTOFMEMORY;

	memcpy((*blob)->GetBufferPointer(), buf.get(), size);
	return S_OK;
}