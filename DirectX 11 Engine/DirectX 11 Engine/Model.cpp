#include "Model.h"
#include "LogError.h"
#include <fstream>
#include <memory>

HRESULT Model::Initialize(ID3D11Device * device, ID3D11DeviceContext * deviceContext)
{
	//Create the vertex buffer
	Vertex v[] =
	{
		// Front Face
		Vertex(-1.0f, -1.0f, -1.0f, 0.0f, 1.0f,-1.0f, -1.0f, -1.0f),
		Vertex(-1.0f,  1.0f, -1.0f, 0.0f, 0.0f,-1.0f,  1.0f, -1.0f),
		Vertex(1.0f,  1.0f, -1.0f, 1.0f, 0.0f, 1.0f,  1.0f, -1.0f),
		Vertex(1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f, -1.0f, -1.0f),

		// Back Face
		Vertex(-1.0f, -1.0f, 1.0f, 1.0f, 1.0f,-1.0f, -1.0f, 1.0f),
		Vertex(1.0f, -1.0f, 1.0f, 0.0f, 1.0f, 1.0f, -1.0f, 1.0f),
		Vertex(1.0f,  1.0f, 1.0f, 0.0f, 0.0f, 1.0f,  1.0f, 1.0f),
		Vertex(-1.0f,  1.0f, 1.0f, 1.0f, 0.0f,-1.0f,  1.0f, 1.0f),

		// Top Face
		Vertex(-1.0f, 1.0f, -1.0f, 0.0f, 1.0f,-1.0f, 1.0f, -1.0f),
		Vertex(-1.0f, 1.0f,  1.0f, 0.0f, 0.0f,-1.0f, 1.0f,  1.0f),
		Vertex(1.0f, 1.0f,  1.0f, 1.0f, 0.0f, 1.0f, 1.0f,  1.0f),
		Vertex(1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, -1.0f),

		// Bottom Face
		Vertex(-1.0f, -1.0f, -1.0f, 1.0f, 1.0f,-1.0f, -1.0f, -1.0f),
		Vertex(1.0f, -1.0f, -1.0f, 0.0f, 1.0f, 1.0f, -1.0f, -1.0f),
		Vertex(1.0f, -1.0f,  1.0f, 0.0f, 0.0f, 1.0f, -1.0f,  1.0f),
		Vertex(-1.0f, -1.0f,  1.0f, 1.0f, 0.0f,-1.0f, -1.0f,  1.0f),

		// Left Face
		Vertex(-1.0f, -1.0f,  1.0f, 0.0f, 1.0f,-1.0f, -1.0f,  1.0f),
		Vertex(-1.0f,  1.0f,  1.0f, 0.0f, 0.0f,-1.0f,  1.0f,  1.0f),
		Vertex(-1.0f,  1.0f, -1.0f, 1.0f, 0.0f,-1.0f,  1.0f, -1.0f),
		Vertex(-1.0f, -1.0f, -1.0f, 1.0f, 1.0f,-1.0f, -1.0f, -1.0f),

		// Right Face
		Vertex(1.0f, -1.0f, -1.0f, 0.0f, 1.0f, 1.0f, -1.0f, -1.0f),
		Vertex(1.0f,  1.0f, -1.0f, 0.0f, 0.0f, 1.0f,  1.0f, -1.0f),
		Vertex(1.0f,  1.0f,  1.0f, 1.0f, 0.0f, 1.0f,  1.0f,  1.0f),
		Vertex(1.0f, -1.0f,  1.0f, 1.0f, 1.0f, 1.0f, -1.0f,  1.0f),
	};


	D3D11_BUFFER_DESC vertexBufferDesc;
	ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));

	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(v);
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA vertexBufferData;

	ZeroMemory(&vertexBufferData, sizeof(vertexBufferData));
	vertexBufferData.pSysMem = v;
	HRESULT hr;
	hr = device->CreateBuffer(&vertexBufferDesc, &vertexBufferData, &m_vertBuffer);
	if (hr != S_OK)
	{
		LogError("Failed to call ID3D11Device::CreateBuffer for the vertex buffer. Error Code: " + std::to_string(hr));
		return hr;
	}



	//Create the index buffer
	DWORD indices[] = {
		// Front Face
		0,  1,  2,
		0,  2,  3,

		// Back Face
		4,  5,  6,
		4,  6,  7,

		// Top Face
		8,  9, 10,
		8, 10, 11,

		// Bottom Face
		12, 13, 14,
		12, 14, 15,

		// Left Face
		16, 17, 18,
		16, 18, 19,

		// Right Face
		20, 21, 22,
		20, 22, 23
	};

	m_vertCount = ARRAYSIZE(indices);

	{
		float x = 5.0f;
		Vertex v[] =
		{
			// Back Face
			Vertex(-x, -x, -x, 0.25f, 2.0f/3.0f,-1.0f, -1.0f, -1.0f), //bottom right
			Vertex(-x,  x, -x, 0.25f, 1.0f / 3.0f,-1.0f,  1.0f, -1.0f), //top right
			Vertex(x,  x, -x, 0.0f, 1.0f / 3.0f, 1.0f,  1.0f, -1.0f), //top left
			Vertex(x, -x, -x, 0.0f, 2.0f / 3.0f, 1.0f, -1.0f, -1.0f), //bottom left

			// Front Face
			Vertex(-x, -x, x, 0.5f, 2.0f / 3.0f,-1.0f, -1.0f, 1.0f), //bottom left
			Vertex(x, -x, x, 0.75f, 2.0f / 3.0f, 1.0f, -1.0f, 1.0f), //bottom right
			Vertex(x,  x, x, 0.75f, 1.0f / 3.0f, 1.0f,  1.0f, 1.0f), //top right
			Vertex(-x,  x, x, 0.5f, 1.0f / 3.0f,-1.0f,  1.0f, 1.0f), //top left

			// Top Face
			Vertex(-x, x, -x, 0.0f, 1.0f,-1.0f, 1.0f, -1.0f),
			Vertex(-x, x,  x, 0.0f, 0.0f,-1.0f, 1.0f,  1.0f),
			Vertex(x, x,  x, 1.0f, 0.0f, 1.0f, 1.0f,  1.0f),
			Vertex(x, x, -x, 1.0f, 1.0f, 1.0f, 1.0f, -1.0f),

			// Bottom Face
			Vertex(-x, -x, -x, 1.0f, 1.0f,-1.0f, -1.0f, -1.0f),
			Vertex(x, -x, -x, 0.0f, 1.0f, 1.0f, -1.0f, -1.0f),
			Vertex(x, -x,  x, 0.0f, 0.0f, 1.0f, -1.0f,  1.0f),
			Vertex(-x, -x,  x, 1.0f, 0.0f,-1.0f, -1.0f,  1.0f),

			// Left Face
			Vertex(-x, -x,  x, 0.25f, 1.0f/3.0f,-1.0f, -1.0f,  1.0f), //top left
			Vertex(-x,  x,  x, 0.50f, 1.0f / 3.0f,-1.0f,  1.0f,  1.0f), //top right
			Vertex(-x,  x, -x, 0.50f, 2.0f / 3.0f,-1.0f,  1.0f, -1.0f), //bottom right
			Vertex(-x, -x, -x, 0.25f, 2.0f / 3.0f,-1.0f, -1.0f, -1.0f), //bottom left

			// Right Face
			Vertex(x, -x, -x, 0.0f, 1.0f, 1.0f, -1.0f, -1.0f),
			Vertex(x,  x, -x, 0.0f, 0.0f, 1.0f,  1.0f, -1.0f),
			Vertex(x,  x,  x, 1.0f, 0.0f, 1.0f,  1.0f,  1.0f),
			Vertex(x, -x,  x, 1.0f, 1.0f, 1.0f, -1.0f,  1.0f),
		};



		//Create the index buffer
		DWORD indices[] = {
			// Front Face
			2, 1, 0,
			3, 2, 0,

			// Back Face
			6, 5, 4,
			7, 6, 4,

			// Top Face
			10, 9, 8,
			11, 10, 8,

			// Bottom Face
			14, 13, 12,
			15, 14, 12,

			// Left Face
			18, 17, 16,
			19, 18, 16,

			// Right Face
			22, 21, 20,
			23, 22, 20
		};
		std::ofstream outfile("skybox.p3d", std::ios::binary);
		uint32_t vertexCount = ARRAYSIZE(v);
		uint32_t indexCount = ARRAYSIZE(indices);
		outfile.write((char*)&vertexCount, sizeof(uint32_t));
		outfile.write((char*)&indexCount, sizeof(uint32_t));
		outfile.write((char*)&v, sizeof(v));
		outfile.write((char*)&indices, sizeof(indices));
		outfile.close();
	}


	D3D11_BUFFER_DESC indexBufferDesc;
	ZeroMemory(&indexBufferDesc, sizeof(indexBufferDesc));
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(indices);
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA indexBufferData;
	indexBufferData.pSysMem = indices;
	hr = device->CreateBuffer(&indexBufferDesc, &indexBufferData, &m_indexBuffer);
	if (hr != S_OK)
	{
		LogError("Failed to call ID3D11Device::CreateBuffer for the index buffer. Error Code: " + std::to_string(hr));
		return hr;
	}
	SetPos(0, 0, 0);
	SetRotation(0, 0, 0);
	SetScaling(1, 1, 1);
	return S_OK;
}


HRESULT Model::Initialize(ID3D11Device * device, ID3D11DeviceContext * deviceContext, std::string fileName)
{
	std::ifstream infile(fileName, std::ios::binary);

	if (infile.is_open() == false)
		return E_ACCESSDENIED;
	uint32_t vertexCount = 0;
	uint32_t indexCount = 0;
	infile.read((char*)&vertexCount, sizeof(uint32_t)); //vertex count
	infile.read((char*)&indexCount, sizeof(uint32_t)); //index count
	std::unique_ptr<Vertex[]> v(new Vertex[vertexCount]); //vertices data
	infile.read((char*)v.get(), sizeof(Vertex) * vertexCount);
	std::unique_ptr<DWORD[]> indices(new DWORD[indexCount]); //indices data
	infile.read((char*)indices.get(), sizeof(DWORD) * indexCount);
	m_vertCount = indexCount;

	Vertex * check = v.get();
	DWORD * check2 = indices.get();

	uint32_t val = sizeof(v);

	D3D11_BUFFER_DESC vertexBufferDesc;
	ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));

	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(Vertex)*vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA vertexBufferData;

	ZeroMemory(&vertexBufferData, sizeof(vertexBufferData));
	vertexBufferData.pSysMem = v.get();
	HRESULT hr;
	hr = device->CreateBuffer(&vertexBufferDesc, &vertexBufferData, &m_vertBuffer);
	if (hr != S_OK)
	{
		LogError("Failed to call ID3D11Device::CreateBuffer for the vertex buffer. Error Code: " + std::to_string(hr));
		return hr;
	}


	D3D11_BUFFER_DESC indexBufferDesc;
	ZeroMemory(&indexBufferDesc, sizeof(indexBufferDesc));
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(DWORD)*indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA indexBufferData;
	indexBufferData.pSysMem = indices.get();
	hr = device->CreateBuffer(&indexBufferDesc, &indexBufferData, &m_indexBuffer);
	if (hr != S_OK)
	{
		LogError("Failed to call ID3D11Device::CreateBuffer for the index buffer. Error Code: " + std::to_string(hr));
		return hr;
	}
	SetPos(0, 0, 0);
	SetRotation(0, 0, 0);
	SetScaling(1, 1, 1);
	return S_OK;
}

void Model::Draw(ConstantBuffer<CB_VS_DEFAULT> & vertexBuffer, ID3D11DeviceContext * deviceContext, const XMMATRIX & viewMat, const XMMATRIX & projectionMat)
{
	//Set the vertex buffer
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	deviceContext->IASetVertexBuffers(0, 1, &m_vertBuffer, &stride, &offset);
	deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	//constant buffer for vertex shader
	vertexBuffer.Data.wvp = XMMatrixTranspose(m_world * viewMat * projectionMat);
	vertexBuffer.Data.world = XMMatrixTranspose(m_world);
	auto vsbuffer = vertexBuffer.Buffer();
	deviceContext->VSSetConstantBuffers(0, 1, &vsbuffer); //set the constant buffer for the vertex shader
	vertexBuffer.ApplyChanges(deviceContext);
	deviceContext->DrawIndexed(m_vertCount, 0, 0);
}

void Model::SetPos(float x, float y, float z)
{
	m_x = x;
	m_y = y;
	m_z = z;
	UpdateWorldMatrix();
}

void Model::SetPos(XMVECTOR pos)
{
	m_x = pos.m128_f32[0];
	m_y = pos.m128_f32[1];
	m_z = pos.m128_f32[2];
	UpdateWorldMatrix();
}

void Model::SetRotation(float xRot, float yRot, float zRot)
{
	m_xRot = xRot / 180 * 3.14159;
	m_yRot = yRot / 180 * 3.14159;
	m_zRot = zRot / 180 * 3.14159;
	UpdateWorldMatrix();
}

void Model::SetScaling(float xScale, float yScale, float zScale)
{
	m_xScale = xScale;
	m_yScale = yScale;
	m_zScale = zScale;
	UpdateWorldMatrix();
}

void Model::AdjustRotation(float xRotOffset, float yRotOffset, float zRotOffset)
{
	m_xRot += xRotOffset;
	m_yRot += yRotOffset;
	m_zRot += zRotOffset;
	if (m_xRot > XM_2PI)
		m_xRot -= XM_2PI;
	else if (m_xRot < 0)
		m_xRot += XM_2PI;
	if (m_yRot > XM_2PI)
		m_yRot -= XM_2PI;
	else if (m_yRot < 0)
		m_yRot += XM_2PI;
	if (m_zRot > XM_2PI)
		m_zRot -= XM_2PI;
	else if (m_zRot < 0)
		m_zRot += XM_2PI;
	UpdateWorldMatrix();
}

XMVECTOR Model::GetPos()
{
	return XMVECTOR{ m_x,m_y,m_z, 0 };
}

void Model::UpdateWorldMatrix()
{
	m_world = XMMatrixRotationRollPitchYaw(m_xRot, m_yRot, m_zRot) * XMMatrixScaling(m_xScale, m_yScale, m_zScale) * XMMatrixTranslation(m_x,m_y,m_z);
}
