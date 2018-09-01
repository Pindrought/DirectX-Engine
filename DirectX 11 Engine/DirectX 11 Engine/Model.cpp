#include "Model.h"
#include "LogError.h"
#include <fstream>
#include <memory>
#include "ModelLoader.h"

HRESULT Model::Initialize(Microsoft::WRL::ComPtr<ID3D11Device> &device, Microsoft::WRL::ComPtr<ID3D11DeviceContext> &deviceContext, ConstantBuffer<CB_VS_DEFAULT> & vertexBuffer, std::string fileName)
{
	//HRESULT hr = ModelLoader::LoadModel(device, fileName, this->vertBuffer, this->indexBuffer, this->vertCount);
	this->deviceContext = deviceContext;
	this->cb_vs_buffer = &vertexBuffer;
	HRESULT hr = ModelLoader::LoadModel(device, fileName, this->modeldata);
	if (hr != S_OK)
	{
		return hr;
	}

	SetPos(0, 0, 0);
	SetRotation(0, 0, 0);
	SetScaling(1, 1, 1);

	return S_OK;
}

void Model::Draw(const XMMATRIX & viewMat, const XMMATRIX & projectionMat)
{
	//Set the vertex buffer
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	//deviceContext->IASetVertexBuffers(0, 1, this->vertBuffer.GetAddressOf(), &stride, &offset);
	//deviceContext->IASetIndexBuffer(this->indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	this->deviceContext->IASetVertexBuffers(0, 1, this->modeldata->vertex_buffer.GetAddressOf(), &stride, &offset);
	this->deviceContext->IASetIndexBuffer(this->modeldata->index_buffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	//constant buffer for vertex shader
	this->cb_vs_buffer->Data.wvp = XMMatrixTranspose(this->world * viewMat * projectionMat);
	this->cb_vs_buffer->Data.world = XMMatrixTranspose(this->world);
	auto vsbuffer = this->cb_vs_buffer->Buffer();
	deviceContext->VSSetConstantBuffers(0, 1, &vsbuffer); //set the constant buffer for the vertex shader
	this->cb_vs_buffer->ApplyChanges(this->deviceContext);
	deviceContext->DrawIndexed(this->modeldata->indices.size(), 0, 0);
}

void Model::SetPos(float x, float y, float z)
{
	this->x = x;
	this->y = y;
	this->z = z;
	UpdateWorldMatrix();
}

void Model::SetPos(XMVECTOR pos)
{
	this->x = pos.m128_f32[0];
	this->y = pos.m128_f32[1];
	this->z = pos.m128_f32[2];
	UpdateWorldMatrix();
}

void Model::SetRotation(float xRot, float yRot, float zRot)
{
	this->xRot = xRot / 180 * 3.14159;
	this->yRot = yRot / 180 * 3.14159;
	this->zRot = zRot / 180 * 3.14159;
	UpdateWorldMatrix();
}

void Model::SetScaling(float xScale, float yScale, float zScale)
{
	this->xScale = xScale;
	this->yScale = yScale;
	this->zScale = zScale;
	UpdateWorldMatrix();
}

void Model::AdjustRotation(float xRotOffset, float yRotOffset, float zRotOffset)
{
	this->xRot += xRotOffset;
	this->yRot += yRotOffset;
	this->zRot += zRotOffset;
	if (this->xRot > XM_2PI)
		this->xRot -= XM_2PI;
	else if (this->xRot < 0)
		this->xRot += XM_2PI;
	if (this->yRot > XM_2PI)
		this->yRot -= XM_2PI;
	else if (this->yRot < 0)
		this->yRot += XM_2PI;
	if (this->zRot > XM_2PI)
		this->zRot -= XM_2PI;
	else if (this->zRot < 0)
		this->zRot += XM_2PI;
	UpdateWorldMatrix();
}

XMVECTOR Model::GetPos()
{
	return XMVECTOR{ this->x,this->y,this->z, 0 };
}

Model::~Model()
{
	ModelManager::ReleaseModelInstance(modeldata);
}

void Model::UpdateWorldMatrix()
{
	this->world = XMMatrixRotationRollPitchYaw(this->xRot, this->yRot, this->zRot) * XMMatrixScaling(this->xScale, this->yScale, this->zScale) * XMMatrixTranslation(this->x,this->y,this->z);
}
