#pragma once
#include "..\\Vertex.h"

namespace UI
{
	class Grid
	{
	public:
		HRESULT Initialize(Microsoft::WRL::ComPtr<ID3D11Device> &device, Microsoft::WRL::ComPtr<ID3D11DeviceContext> &deviceContext, ConstantBuffer<CB_VS_UI> & cb_vs_ui, ConstantBuffer<CB_PS_UI> &cb_ps_ui, float window_width, float window_height, float x, float y, float width, float height);
		void Draw();
		XMFLOAT2 WindowToSpace(XMFLOAT2 coords);
		void SetColor(float r, float g, float b, float a);
		void SetColor(XMFLOAT4 maincolor);
		void SetBackgroundColor(float r, float g, float b, float a);
		void SetBackgroundColor(XMFLOAT4 color);
		void SetPosition(float x, float y);
		void SetPosition(XMFLOAT2 pos);
		void AdjustPosition(float offsetx, float offsety);
		void AdjustPosition(XMFLOAT2 offset);
	private:
		ConstantBuffer<CB_VS_UI> * cb_vs_ui;
		ConstantBuffer<CB_PS_UI> * cb_ps_ui;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> deviceContext;
		Microsoft::WRL::ComPtr<ID3D11Buffer> indexBuffer;
		Microsoft::WRL::ComPtr<ID3D11Buffer> vertBuffer;
		XMFLOAT2 pos;
		UINT vertCount;
		XMFLOAT4 maincolor = { 0.0f,0.0f,0.0f,0.5f };
		XMFLOAT4 bgcolor = { 1.0f,1.0f,1.0f,0.5f };
		float window_width;
		float window_height;
	};
}