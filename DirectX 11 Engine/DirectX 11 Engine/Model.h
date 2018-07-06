#pragma once
#include "Vertex.h"
#include <string>

class Model
{
public:
	HRESULT Initialize(ID3D11Device * device, ID3D11DeviceContext * deviceContext);
	HRESULT Initialize(ID3D11Device * device, ID3D11DeviceContext * deviceContext, std::string fileName);
	void Draw(ConstantBuffer<CB_VS_DEFAULT> & vertexBuffer, ID3D11DeviceContext * deviceContext, const XMMATRIX & viewMat, const XMMATRIX & projectionMat);
	void SetPos(float x, float y, float z);
	void SetPos(XMVECTOR pos);
	void SetRotation(float xRot, float yRot, float zRot);
	void SetScaling(float xScale, float yScale, float zScale);
	void AdjustRotation(float xRotOffset, float yRotOffset, float zRotOffset);
	XMVECTOR GetPos();
private:
	void UpdateWorldMatrix();
	ID3D11Buffer* indexBuffer;
	ID3D11Buffer* vertBuffer;
	XMMATRIX world;
	float x, y, z;
	float xRot, yRot, zRot;
	float xScale, yScale, zScale;
	UINT vertCount;
};