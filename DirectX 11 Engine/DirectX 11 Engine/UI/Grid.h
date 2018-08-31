#pragma once
#include "..\\Vertex.h"

namespace UI
{
	class Grid
	{
	public:
		HRESULT Initialize(Microsoft::WRL::ComPtr<ID3D11Device> &device, Microsoft::WRL::ComPtr<ID3D11DeviceContext> &deviceContext, ConstantBuffer<CB_UI> & cb_ui, float window_width, float window_height, float x, float y, float width, float height);
		void Draw();
		XMFLOAT2 WindowToSpace(XMFLOAT2 coords);

	private:
		ConstantBuffer<CB_UI> * cb_ui;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> deviceContext;
		Microsoft::WRL::ComPtr<ID3D11Buffer> indexBuffer;
		Microsoft::WRL::ComPtr<ID3D11Buffer> vertBuffer;
		XMFLOAT2 pos;
		UINT vertCount;
		float window_height, window_width;
	};
}