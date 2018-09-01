#pragma once
#include "Vertex.h"
#include <string>
#include <vector>
#include <memory>

class ModelData
{
public:
	friend class ModelManager;
	std::string name;
	Microsoft::WRL::ComPtr<ID3D11Buffer> vertex_buffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> index_buffer;
	std::vector<Vertex> vertices;
	std::vector<DWORD> indices;
protected:
private:
	int refCount = 0;
};