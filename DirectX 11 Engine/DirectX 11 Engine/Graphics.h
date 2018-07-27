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
	Model cube;
	Model grassModel;
	Model skybox;
	Model spaceCompound;
	std::string drawText = "TEST";
private:
	bool InitializeScene();

	int width = 0;
	int height = 0;
	HWND hwnd;
	IDXGISwapChain* swapChain;
	ID3D11Device* d3d11Device;
	ID3D11DeviceContext* d3d11DevCon;
	ID3D11RenderTargetView* renderTargetView;

	ID3D11VertexShader* vs;
	ID3D11PixelShader* ps;

	ID3D11PixelShader * ps_skymap;
	ID3D11VertexShader * vs_skymap;


	ID3D10Blob* vs_buffer;
	ID3D10Blob* ps_buffer;

	ID3D10Blob* vs_skymap_buffer;
	ID3D10Blob* ps_skymap_buffer;

	ID3D11InputLayout* vertLayout;

	ID3D11Buffer* indexBuffer;
	ID3D11Buffer* vertBuffer;

	ID3D11DepthStencilView* depthStencilView;
	ID3D11Texture2D* depthStencilBuffer;

	ConstantBuffer<CB_VS_DEFAULT> cb_vs_default;
	ConstantBuffer<CB_PS_LIGHT> cb_ps_light;
	ConstantBuffer<CB_PS_POINTLIGHT> cb_ps_pointlight;

	ID3D11DepthStencilState * depthStencilState;
	ID3D11RasterizerState* rasterizerState;

	ID3D11ShaderResourceView* testTexture;
	ID3D11ShaderResourceView* grassTexture;
	ID3D11ShaderResourceView* skyboxTexture;

	ID3D11BlendState* transparentBlendState;

	ID3D11SamplerState* samplerState;

	std::unique_ptr<SpriteBatch> spriteBatch;
	std::unique_ptr<SpriteFont> spriteFont;

	FpsClass fps;

};