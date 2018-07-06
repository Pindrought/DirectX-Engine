#pragma once
#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"D3DCompiler.lib")
#pragma comment(lib,"DirectXTK.lib")
#pragma comment(lib,"FW1FontWrapper.lib")

#include "LogError.h"
#include "Model.h"
#include <d3d11.h>
#include <D3Dcompiler.h>
#include <DDSTextureLoader.h>
#include <SpriteFont.h>
#include <SpriteBatch.h>
#include <memory>
#include "ConstantBuffer.h"
#include "IncludeUtilities.h"
#include "Camera.h"

using namespace DirectX;

class Graphics
{
public:
	bool Initialize(HWND hwnd, int width, int height);
	void Release();
	void RenderFrame(float dt);
	Camera camera;
	Model m_model;
	Model m_grassModel;
	Model m_skybox;
	std::string drawText = "TEST";
private:
	bool InitializeScene();

	int m_width = 0;
	int m_height = 0;
	HWND m_hwnd;
	IDXGISwapChain* m_swapChain;
	ID3D11Device* m_d3d11Device;
	ID3D11DeviceContext* m_d3d11DevCon;
	ID3D11RenderTargetView* m_renderTargetView;

	ID3D11VertexShader* m_vs;
	ID3D11PixelShader* m_ps;

	ID3D11PixelShader * m_ps_skymap;
	ID3D11VertexShader * m_vs_skymap;


	ID3D10Blob* m_vs_buffer;
	ID3D10Blob* m_ps_buffer;

	ID3D10Blob* m_vs_skymap_buffer;
	ID3D10Blob* m_ps_skymap_buffer;

	ID3D11InputLayout* m_vertLayout;

	ID3D11Buffer* m_indexBuffer;
	ID3D11Buffer* m_vertBuffer;

	ID3D11DepthStencilView* m_depthStencilView;
	ID3D11Texture2D* m_depthStencilBuffer;

	ConstantBuffer<CB_VS_DEFAULT> cb_vs_default;
	ConstantBuffer<CB_PS_LIGHT> cb_ps_light;
	ConstantBuffer<CB_PS_POINTLIGHT> cb_ps_pointlight;


	ID3D11RasterizerState* m_rasterizerState;

	ID3D11ShaderResourceView* m_testTexture;
	ID3D11ShaderResourceView* m_grassTexture;
	ID3D11ShaderResourceView* m_skyboxTexture;

	ID3D11SamplerState* m_samplerState;

	std::unique_ptr<SpriteBatch> spriteBatch;
	std::unique_ptr<SpriteFont> spriteFont;

	FpsClass m_fps;

};