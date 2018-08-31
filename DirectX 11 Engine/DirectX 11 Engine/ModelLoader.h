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

class ModelManager
{
public:
	static std::shared_ptr<ModelData> GetModelData(std::string filename);
	static void StoreModelData(std::shared_ptr<ModelData> modeldata);
	static void ReleaseModelInstance(std::shared_ptr<ModelData> modeldata);
private:
	static std::vector<std::shared_ptr<ModelData>> loaded_models;
};

class ModelLoader
{
public:
	static HRESULT LoadModel(Microsoft::WRL::ComPtr<ID3D11Device> &device,  std::string &fileName, Microsoft::WRL::ComPtr<ID3D11Buffer> &vertexBuffer, Microsoft::WRL::ComPtr<ID3D11Buffer> &indexBuffer, UINT &vertCount);

private:
	static HRESULT Loadp3d(Microsoft::WRL::ComPtr<ID3D11Device> &device,  std::string &fileName, Microsoft::WRL::ComPtr<ID3D11Buffer> &vertexBuffer, Microsoft::WRL::ComPtr<ID3D11Buffer> &indexBuffer, UINT &vertCount);
	static HRESULT Loadobj(Microsoft::WRL::ComPtr<ID3D11Device> &device,  std::string &fileName, Microsoft::WRL::ComPtr<ID3D11Buffer> &vertexBuffer, Microsoft::WRL::ComPtr<ID3D11Buffer> &indexBuffer, UINT &vertCount);
};