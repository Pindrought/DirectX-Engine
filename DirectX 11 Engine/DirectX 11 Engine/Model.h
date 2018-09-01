#pragma once
#include "ModelLoader.h"
#include <string>

class Model
{
public:
	HRESULT Initialize(Microsoft::WRL::ComPtr<ID3D11Device> &device, std::string fileName);
	void Draw(ConstantBuffer<CB_VS_DEFAULT> & vertexBuffer, Microsoft::WRL::ComPtr<ID3D11DeviceContext> &deviceContext, const XMMATRIX & viewMat, const XMMATRIX & projectionMat);
	void SetPos(float x, float y, float z);
	void SetPos(XMVECTOR pos);
	void SetRotation(float xRot, float yRot, float zRot);
	void SetScaling(float xScale, float yScale, float zScale);
	void AdjustRotation(float xRotOffset, float yRotOffset, float zRotOffset);
	XMVECTOR GetPos();
	~Model();
private:
	void UpdateWorldMatrix();
	//Microsoft::WRL::ComPtr<ID3D11Buffer> indexBuffer;
	//Microsoft::WRL::ComPtr<ID3D11Buffer> vertBuffer;
	std::shared_ptr<ModelData> modeldata = nullptr;
	XMMATRIX world;
	float x, y, z;
	float xRot, yRot, zRot;
	float xScale, yScale, zScale;
	UINT vertCount;
};