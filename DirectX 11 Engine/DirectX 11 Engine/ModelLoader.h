#pragma once
#include "Vertex.h"
#include <string>
class ModelLoader
{
public:
	static HRESULT LoadModel(ID3D11Device * &device, ID3D11DeviceContext * &deviceContext, std::string &fileName, ID3D11Buffer * &vertexBuffer, ID3D11Buffer * &indexBuffer, UINT &vertCount);
private:
	static HRESULT Loadp3d(ID3D11Device * &device, ID3D11DeviceContext * &deviceContext, std::string &fileName, ID3D11Buffer * &vertexBuffer, ID3D11Buffer * &indexBuffer, UINT &vertCount);
	static HRESULT Loadobj(ID3D11Device * &device, ID3D11DeviceContext * &deviceContext, std::string &fileName, ID3D11Buffer * &vertexBuffer, ID3D11Buffer * &indexBuffer, UINT &vertCount);
};