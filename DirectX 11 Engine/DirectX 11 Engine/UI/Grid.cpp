#include "Grid.h"
#include <vector>
#include "..\\LogError.h"

namespace UI
{
	HRESULT Grid::Initialize(Microsoft::WRL::ComPtr<ID3D11Device> &device, Microsoft::WRL::ComPtr<ID3D11DeviceContext> &deviceContext, ConstantBuffer<CB_UI> & cb_ui, float window_width, float window_height, float x, float y, float width, float height)
	{
		this->deviceContext = deviceContext;
		this->cb_ui = &cb_ui;
		this->pos.x = (2*x)/window_width;
		this->pos.y = -(2*y)/window_height;
		this->window_width = window_width;
		this->window_height = window_height;

		std::vector<UIVertex> verts;
		
		//top left x coordinate = -1
		//top left y coordinate = -1
		//bottom right x coordinate = 1
		//bottom right y coordinate = 1


		/*float leftx = (x/window_width) - 0.5f;
		float topy = (-y / window_height) + 0.5f;
		float rightx = ((x + width) / window_width) -0.5f;
		float bottomy = (-(y+height) / window_height) +0.5f;*/
/*
		float leftx = 0.0f;
		float topy = 0.5f;
		float rightx = 0.5f;
		float bottomy = 0.0f;*/

		//coordinate need to be between -1 and 1

		if (width <= 24 || height <= 24)
		{
			LogError("Minimum width/height = 25px for UI::Grid");
			return E_FAIL;
		}
		
		XMFLOAT2 topleft, botright;


		//topleft corner
		topleft = WindowToSpace({ 0,0 });
		botright = WindowToSpace({ 12.0f,12.0f });

		verts.push_back(UIVertex(topleft.x, topleft.y, 1.0f, 0.0f, 0.0f)); //topleft
		verts.push_back(UIVertex(botright.x, topleft.y, 1.0f, 0.33f, 0.0f)); //topright
		verts.push_back(UIVertex(botright.x, botright.y, 1.0f, 0.33f, 0.33f)); //bottomright
		verts.push_back(UIVertex(topleft.x, botright.y, 1.0f, 0.0f, 0.33f)); //bottomleft

		//topright corner
		topleft = WindowToSpace({ width-12.0f,0.0f });
		botright = WindowToSpace({ width,12.0f });
		verts.push_back(UIVertex(topleft.x, topleft.y, 1.0f, 0.66f, 0.0f)); //topleft
		verts.push_back(UIVertex(botright.x, topleft.y, 1.0f, 1.0f, 0.0f)); //topright
		verts.push_back(UIVertex(botright.x, botright.y, 1.0f, 1.0f, 0.33f)); //bottomright
		verts.push_back(UIVertex(topleft.x, botright.y, 1.0f, 0.66f, 0.33f)); //bottomleft

		//topmid spacer
		topleft = WindowToSpace({ 12.0f ,0.0f });
		botright = WindowToSpace({ width - 12.0f,12.0f });
		verts.push_back(UIVertex(topleft.x, topleft.y, 1.0f, 0.33f, 0.0f)); //topleft
		verts.push_back(UIVertex(botright.x, topleft.y, 1.0f, 0.66f, 0.0f)); //topright
		verts.push_back(UIVertex(botright.x, botright.y, 1.0f, 0.66, 0.33f)); //bottomright
		verts.push_back(UIVertex(topleft.x, botright.y, 1.0f, 0.33f, 0.33f)); //bottomleft

		//midleft section
		topleft = WindowToSpace({ 0,12.0f });
		botright = WindowToSpace({ 12.0f,height-12.0f });

		verts.push_back(UIVertex(topleft.x, topleft.y, 1.0f, 0.0f, 0.33f)); //topleft
		verts.push_back(UIVertex(botright.x, topleft.y, 1.0f, 0.33f, 0.33f)); //topright
		verts.push_back(UIVertex(botright.x, botright.y, 1.0f, 0.33f, 0.66f)); //bottomright
		verts.push_back(UIVertex(topleft.x, botright.y, 1.0f, 0.0f, 0.66f)); //bottomleft

		//middle middle section
		topleft = WindowToSpace({ 12.0f,12.0f });
		botright = WindowToSpace({ width-12.0f,height - 12.0f });

		verts.push_back(UIVertex(topleft.x, topleft.y, 1.0f, 0.33f, 00.33f)); //topleft
		verts.push_back(UIVertex(botright.x, topleft.y, 1.0f, 0.66f, 0.33f)); //topright
		verts.push_back(UIVertex(botright.x, botright.y, 1.0f, 0.66f, 0.66f)); //bottomright
		verts.push_back(UIVertex(topleft.x, botright.y, 1.0f, 0.33f, 0.66f)); //bottomleft

		//middle right section
		topleft = WindowToSpace({ width-12,12.0f });
		botright = WindowToSpace({ width,height - 12.0f });

		verts.push_back(UIVertex(topleft.x, topleft.y, 1.0f, 0.66f, 0.33f)); //topleft
		verts.push_back(UIVertex(botright.x, topleft.y, 1.0f, 1.00f, 0.33f)); //topright
		verts.push_back(UIVertex(botright.x, botright.y, 1.0f, 1.00f, 0.66f)); //bottomright
		verts.push_back(UIVertex(topleft.x, botright.y, 1.0f, 0.66f, 0.66f)); //bottomleft

		//bottom left section
		topleft = WindowToSpace({ 0,height-12.0f });
		botright = WindowToSpace({ 12.0f,height });

		verts.push_back(UIVertex(topleft.x, topleft.y, 1.0f, 0.0f, 0.66f)); //topleft
		verts.push_back(UIVertex(botright.x, topleft.y, 1.0f, 0.33f, 0.66f)); //topright
		verts.push_back(UIVertex(botright.x, botright.y, 1.0f, 0.33f, 1.0f)); //bottomright
		verts.push_back(UIVertex(topleft.x, botright.y, 1.0f, 0.0f, 1.0f)); //bottomleft

		//bottom middle section
		topleft = WindowToSpace({ 12.0f,height - 12.0f });
		botright = WindowToSpace({ width-12.0f,height });

		verts.push_back(UIVertex(topleft.x, topleft.y, 1.0f, 0.33f, 0.66f)); //topleft
		verts.push_back(UIVertex(botright.x, topleft.y, 1.0f, 0.66f, 0.66f)); //topright
		verts.push_back(UIVertex(botright.x, botright.y, 1.0f, 0.66f, 1.0f)); //bottomright
		verts.push_back(UIVertex(topleft.x, botright.y, 1.0f, 0.33f, 1.0f)); //bottomleft

		//bottom right section
		topleft = WindowToSpace({ width-12.0f,height - 12.0f });
		botright = WindowToSpace({ width,height });

		verts.push_back(UIVertex(topleft.x, topleft.y, 1.0f, 0.66f, 0.66f)); //topleft
		verts.push_back(UIVertex(botright.x, topleft.y, 1.0f, 1.00f, 0.66f)); //topright
		verts.push_back(UIVertex(botright.x, botright.y, 1.0f, 1.00f, 1.0f)); //bottomright
		verts.push_back(UIVertex(topleft.x, botright.y, 1.0f, 0.66f, 1.0f)); //bottomleft


		std::vector<DWORD> indices = 
		{ 
			//top row
			0, 1, 2, 2,3,0, //topleft corner 
			4, 5, 6, 6,7,4, //topright corner
			8, 9, 10, 10, 11, 8, //topmid section
			//mid row
			12, 13, 14, 14, 15, 12, //midleft section
			16, 17, 18, 18, 19, 16, //middle middle section
			20, 21, 22, 22, 23, 20, //middle right section
			//bottom row
			24, 25, 26, 26, 27, 24, //bottomleft section
			28, 29, 30, 30, 31, 28, //bottom middle
			32, 33, 34, 34, 35, 32, //bottom right
		};

		uint32_t vertexCount = verts.size();
		uint32_t indexCount = indices.size();
		vertCount = indexCount;

		D3D11_BUFFER_DESC vertexBufferDesc;
		ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));

		vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		vertexBufferDesc.ByteWidth = sizeof(UIVertex)*vertexCount;
		vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vertexBufferDesc.CPUAccessFlags = 0;
		vertexBufferDesc.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA vertexBufferData;

		ZeroMemory(&vertexBufferData, sizeof(vertexBufferData));
		vertexBufferData.pSysMem = verts.data();
		HRESULT hr;
		hr = device->CreateBuffer(&vertexBufferDesc, &vertexBufferData, this->vertBuffer.GetAddressOf());
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
		indexBufferData.pSysMem = indices.data();
		hr = device->CreateBuffer(&indexBufferDesc, &indexBufferData, this->indexBuffer.GetAddressOf());
		if (hr != S_OK)
		{
			LogError("Failed to call ID3D11Device::CreateBuffer for the index buffer. Error Code: " + std::to_string(hr));
			return hr;
		}

		return S_OK;
	}
	void Grid::Draw()
	{
		UINT stride = sizeof(UIVertex);
		UINT offset = 0;
		deviceContext->IASetVertexBuffers(0, 1, this->vertBuffer.GetAddressOf(), &stride, &offset);
		deviceContext->IASetIndexBuffer(this->indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
		//constant buffer for ui shader
		cb_ui->Data.position.x = this->pos.x;
		cb_ui->Data.position.y = this->pos.y;
		cb_ui->Data.position.z = 0;
		cb_ui->Data.position.w = 0;
		auto vsbuffer = cb_ui->Buffer();
		deviceContext->VSSetConstantBuffers(0, 1, &vsbuffer); //set the constant buffer for the vertex shader
		cb_ui->ApplyChanges(this->deviceContext);
		this->deviceContext->DrawIndexed(this->vertCount, 0, 0);
	}
	XMFLOAT2 Grid::WindowToSpace(XMFLOAT2 coords)
	{
		XMFLOAT2 r; //return coords
		r.x = -1.0f + (coords.x / window_width) * 2;
		r.y = 1.0f - (coords.y / this->window_height) * 2;
		return r;
	}
}