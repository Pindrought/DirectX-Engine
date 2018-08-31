#pragma once
#include "Vertex.h"
#include <string>
class ModelLoader
{
public:
	static HRESULT LoadModel(Microsoft::WRL::ComPtr<ID3D11Device> &device,  std::string &fileName, Microsoft::WRL::ComPtr<ID3D11Buffer> &vertexBuffer, Microsoft::WRL::ComPtr<ID3D11Buffer> &indexBuffer, UINT &vertCount);
private:
	static HRESULT Loadp3d(Microsoft::WRL::ComPtr<ID3D11Device> &device,  std::string &fileName, Microsoft::WRL::ComPtr<ID3D11Buffer> &vertexBuffer, Microsoft::WRL::ComPtr<ID3D11Buffer> &indexBuffer, UINT &vertCount);
	static HRESULT Loadobj(Microsoft::WRL::ComPtr<ID3D11Device> &device,  std::string &fileName, Microsoft::WRL::ComPtr<ID3D11Buffer> &vertexBuffer, Microsoft::WRL::ComPtr<ID3D11Buffer> &indexBuffer, UINT &vertCount);
};