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
#include "UI\\Grid.h"

using namespace DirectX;

class Graphics
{
public:
	bool Initialize(HWND hwnd, int width, int height);
	void Release();
	void RenderFrame(float dt);
	Camera camera;
	Model cube;
	Model cube2;
	Model grassModel;
	Model skybox;
	Model spaceCompound;
	UI::Grid grid_test[3];
	std::string drawText = "TEST";
private:
	bool InitializeShaders();
	bool InitializeScene();

	int width = 0;
	int height = 0;
	HWND hwnd;
	Microsoft::WRL::ComPtr<IDXGISwapChain> swapChain;
	Microsoft::WRL::ComPtr<ID3D11Device> d3d11Device;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> d3d11DevCon;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> renderTargetView;

	Microsoft::WRL::ComPtr<ID3D11VertexShader> vs;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> ps;

	Microsoft::WRL::ComPtr<ID3D11PixelShader> ps_skymap;
	Microsoft::WRL::ComPtr<ID3D11VertexShader> vs_skymap;

	Microsoft::WRL::ComPtr<ID3D11PixelShader> ps_ui;
	Microsoft::WRL::ComPtr<ID3D11VertexShader> vs_ui;

	Microsoft::WRL::ComPtr<ID3D10Blob> vs_buffer;
	Microsoft::WRL::ComPtr<ID3D10Blob> ps_buffer;

	Microsoft::WRL::ComPtr<ID3D10Blob> vs_skymap_buffer;
	Microsoft::WRL::ComPtr<ID3D10Blob> ps_skymap_buffer;

	Microsoft::WRL::ComPtr<ID3D10Blob> vs_ui_buffer;
	Microsoft::WRL::ComPtr<ID3D10Blob> ps_ui_buffer;

	Microsoft::WRL::ComPtr<ID3D11InputLayout> default_vertLayout;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> ui_vertLayout;
	
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> depthStencilView;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> depthStencilBuffer;

	ConstantBuffer<CB_VS_UI> cb_vs_ui;
	ConstantBuffer<CB_PS_UI> cb_ps_ui;
	ConstantBuffer<CB_VS_DEFAULT> cb_vs_default;
	ConstantBuffer<CB_PS_LIGHT> cb_ps_light;
	ConstantBuffer<CB_PS_POINTLIGHT> cb_ps_pointlight;

	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> depthStencilState;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> depthStencilStateDisabled;

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> testTexture;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> grassTexture;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> skyboxTexture;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> borderTexture;

	Microsoft::WRL::ComPtr<ID3D11RasterizerState> rasterizerState;


	Microsoft::WRL::ComPtr<ID3D11BlendState> transparentBlendState;

	Microsoft::WRL::ComPtr<ID3D11SamplerState> samplerState;

	std::unique_ptr<SpriteBatch> spriteBatch;
	std::unique_ptr<SpriteFont> spriteFont;

	FpsClass fps;

	bool vSync = true;
};