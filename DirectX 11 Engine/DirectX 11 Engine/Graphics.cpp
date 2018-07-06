#include "Graphics.h"
#include "ShaderLoader.h"
#include <fstream>

void GenerateP3d()
{
	float x = 5.0f;
	Vertex v[] =
	{
		// Back Face
		Vertex(-x, -x, -x, 1.0f, 1.99f / 3.0f,-1.0f, -1.0f, -1.0f), //bottom right
		Vertex(-x,  x, -x, 1.0f, 1.01f / 3.0f,-1.0f,  1.0f, -1.0f), //top right
		Vertex(x,  x, -x, 0.75f, 1.01f / 3.0f, 1.0f,  1.0f, -1.0f), //top left
		Vertex(x, -x, -x, 0.75f, 1.99f / 3.0f, 1.0f, -1.0f, -1.0f), //bottom left

		// Front Face
		Vertex(-x, -x, x, 0.25f, 1.99f / 3.0f,-1.0f, -1.0f, 1.0f), //bottom left
		Vertex(x, -x, x, 0.5f, 1.99f / 3.0f, 1.0f, -1.0f, 1.0f), //bottom right
		Vertex(x,  x, x, 0.5f, 1.01f / 3.0f, 1.0f,  1.0f, 1.0f), //top right
		Vertex(-x,  x, x, 0.25f, 1.01f / 3.0f,-1.0f,  1.0f, 1.0f), //top left

		// Top Face - check?
		Vertex(-x, x, -x, 0.251f,0,-1.0f, 1.0f, -1.0f), //top left
		Vertex(-x, x,  x, 0.251f,1.0f/3.0f,-1.0f, 1.0f,  1.0f), //bottom left
		Vertex(x, x,  x, 0.499f,1.0f/3.0f, 1.0f, 1.0f,  1.0f), //bottom right
		Vertex(x, x, -x, 0.499f,0, 1.0f, 1.0f, -1.0f), //top right

		// Bottom Face
		Vertex(-x, -x, -x, 0.251f, 1.00f,-1.0f, -1.0f, -1.0f), //bottom left
		Vertex(x, -x, -x, 0.499f, 1.00f, 1.0f, -1.0f, -1.0f), //bottom right
		Vertex(x, -x,  x, 0.499f, 1.99f/3.0f, 1.0f, -1.0f,  1.0f), //top right
		Vertex(-x, -x,  x, 0.251f, 1.99f/3.0f,-1.0f, -1.0f,  1.0f), //top left

		// Left Face
		Vertex(-x, -x,  x, 0.249f, 1.999f / 3.0f,-1.0f, -1.0f,  1.0f), //bottom right
		Vertex(-x,  x,  x, 0.249f, 1.01f / 3.0f,-1.0f,  1.0f,  1.0f), //top right
		Vertex(-x,  x, -x, 0.0f, 1.01f / 3.0f,-1.0f,  1.0f, -1.0f), //top left
		Vertex(-x, -x, -x, 0.0f, 1.999f / 3.0f,-1.0f, -1.0f, -1.0f), //bottom left

		// Right Face
		Vertex(x, -x, -x, 0.75f, 1.99f/3.0f, 1.0f, -1.0f, -1.0f), //bottom right
		Vertex(x,  x, -x, 0.75f, 1.01f / 3.0f, 1.0f,  1.0f, -1.0f), //top right
		Vertex(x,  x,  x, 0.5f, 1.01f / 3.0f, 1.0f,  1.0f,  1.0f), //top left
		Vertex(x, -x,  x, 0.5f, 1.99f/3.0f, 1.0f, -1.0f,  1.0f), //bottom left
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

bool Graphics::Initialize(HWND hwnd, int width, int height)
{
	m_fps.Initialize();

	m_hwnd = hwnd;
	m_width = width;
	m_height = height;

	HRESULT hr;
	//Describe our Back Buffer for the swap chain
	DXGI_MODE_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(DXGI_MODE_DESC));

	bufferDesc.Width = width;
	bufferDesc.Height = height;
	bufferDesc.RefreshRate.Numerator = 60; //60/1 = 60 hz
	bufferDesc.RefreshRate.Denominator = 1;
	bufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	bufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	bufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	//Describe our SwapChain
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	ZeroMemory(&swapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));

	swapChainDesc.BufferDesc = bufferDesc;
	swapChainDesc.SampleDesc.Count = 8; //# of multisamples per pixel
	swapChainDesc.SampleDesc.Quality = 0; //Image Quality Level
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount = 1; //# of buffers in swap chain
	swapChainDesc.OutputWindow = m_hwnd;
	swapChainDesc.Windowed = TRUE;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	//Create our SwapChain
	hr = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, NULL, NULL, NULL,
		D3D11_SDK_VERSION, &swapChainDesc, &m_swapChain, &m_d3d11Device, NULL, &m_d3d11DevCon);
	if (hr != S_OK) //If error occurred
	{
		LogError("D3D11CreateDeviceAndSwapChain Failed. HRESULT = " + std::to_string(hr));
		return false;
	}

	//Create our BackBuffer
	ID3D11Texture2D* backBuffer;
	hr = m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer));
	if (hr != S_OK) //If error occurred
	{
		LogError("IDXGISwapChain::GetBuffer Failed. HRESULT = " + std::to_string(hr));
		return false;
	}

	//Describe our Depth/Stencil Buffer
	D3D11_TEXTURE2D_DESC depthStencilDesc;
	depthStencilDesc.Width = m_width;
	depthStencilDesc.Height = m_height;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilDesc.SampleDesc.Count = 8;
	depthStencilDesc.SampleDesc.Quality = 0;
	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.CPUAccessFlags = 0;
	depthStencilDesc.MiscFlags = 0;

	hr = m_d3d11Device->CreateTexture2D(&depthStencilDesc, NULL, &m_depthStencilBuffer);
	if (hr != S_OK) //If error occurred
	{
		LogError("ID3D11Device::CreateTexture2D Failed when used for depth stencil buffer. HRESULT = " + std::to_string(hr));
		return false;
	}

	hr = m_d3d11Device->CreateDepthStencilView(m_depthStencilBuffer, NULL, &m_depthStencilView);
	if (hr != S_OK) //If error occurred
	{
		LogError("ID3D11Device::CreateDepthStencilView Failed when used for depth stencil view. HRESULT = " + std::to_string(hr));
		return false;
	}

	//Create our Render Target
	hr = m_d3d11Device->CreateRenderTargetView(backBuffer, NULL, &m_renderTargetView);
	if (hr != S_OK) //If error occurred
	{
		LogError("ID3D11Device::CreateRenderTargetView Failed. HRESULT = " + std::to_string(hr));
		return false;
	}
	backBuffer->Release();

	//Set our Render Target
	m_d3d11DevCon->OMSetRenderTargets(1, &m_renderTargetView, m_depthStencilView);

	if (InitializeScene() == false)
		return false;

	return true;

}

bool Graphics::InitializeScene()
{
	//load default shaders
	HRESULT hr;
	hr = ShaderLoader::LoadCompiledShader("Debug\\defaultvertex.cso", &m_vs_buffer);
	if (hr != S_OK)
	{
		LogError("Failed to load vertex shader. Error Code: " + HRToString(hr));
		return false;
	}
	hr = ShaderLoader::LoadCompiledShader("Debug\\defaultpixel.cso", &m_ps_buffer);
	if (hr != S_OK)
	{
		LogError("Failed to load pixel shader. Error Code: " + HRToString(hr));
		return false;
	}

	//load compiled shaders
	hr = ShaderLoader::LoadCompiledShader("Debug\\skymapvertex.cso", &m_vs_skymap_buffer);
	if (hr != S_OK)
	{
		LogError("Failed to load vertex shader. Error Code: " + HRToString(hr));
		return false;
	}
	hr = ShaderLoader::LoadCompiledShader("Debug\\skymappixel.cso", &m_ps_skymap_buffer);
	if (hr != S_OK)
	{
		LogError("Failed to load pixel shader. Error Code: " + HRToString(hr));
		return false;
	}

	//Create the Shader Objects
	hr = m_d3d11Device->CreateVertexShader(m_vs_buffer->GetBufferPointer(), m_vs_buffer->GetBufferSize(), NULL, &m_vs);
	if (hr != S_OK)
	{
		LogError("Failed to create Vertex Shader. Error Code: " + HRToString(hr));
		return false;
	}

	hr = m_d3d11Device->CreatePixelShader(m_ps_buffer->GetBufferPointer(), m_ps_buffer->GetBufferSize(), NULL, &m_ps);
	if (hr != S_OK)
	{
		LogError("Failed to create Pixel Shader. Error Code: " + HRToString(hr));
		return false;
	}

	//create skymap shaders
	hr = m_d3d11Device->CreateVertexShader(m_vs_skymap_buffer->GetBufferPointer(), m_vs_skymap_buffer->GetBufferSize(), NULL, &m_vs_skymap);
	if (hr != S_OK)
	{
		LogError("Failed to create Vertex Shader. Error Code: " + HRToString(hr));
		return false;
	}

	hr = m_d3d11Device->CreatePixelShader(m_ps_skymap_buffer->GetBufferPointer(), m_ps_skymap_buffer->GetBufferSize(), NULL, &m_ps_skymap);
	if (hr != S_OK)
	{
		LogError("Failed to create Pixel Shader. Error Code: " + HRToString(hr));
		return false;
	}

	//Create the Input Layout
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",     0, DXGI_FORMAT_R32G32B32_FLOAT,    0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	UINT numElements = ARRAYSIZE(layout);

	m_d3d11Device->CreateInputLayout(layout, numElements, m_vs_buffer->GetBufferPointer(),
		m_vs_buffer->GetBufferSize(), &m_vertLayout);
	if (hr != S_OK)
	{
		LogError("Failed to create Input Layout. Error Code: " + std::to_string(hr));
		return false;
	}

	//Set the Input Layout
	m_d3d11DevCon->IASetInputLayout(m_vertLayout);

	//Set Primitive Topology
	m_d3d11DevCon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//Create the Viewport
	D3D11_VIEWPORT viewport;
	ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = m_width;
	viewport.Height = m_height;

	//Set the Viewport for Rasterizer Stage
	m_d3d11DevCon->RSSetViewports(1, &viewport);

	//Setup camera and world view projection matrix
	camera.SetPosition(0, 0, -1);
	camera.SetProjectionValues(90, m_width, m_height, 1, 1000);
	
	//create constant buffer for vertex shader
	cb_vs_default.Initialize(m_d3d11Device);
	if (hr != S_OK)
	{
		LogError("Failed Initialize default Vertex Shader Constant Buffer. Error Code: " + std::to_string(hr));
		return false;
	}

	//GenerateP3d();

	if (m_skybox.Initialize(m_d3d11Device, m_d3d11DevCon, "skybox.p3d") != S_OK)
	{
		LogError("Failed Initialize model. Error Code: " + std::to_string(hr));
		return false;
	}
	m_skybox.SetPos(0, 0, 0);

	if (m_model.Initialize(m_d3d11Device, m_d3d11DevCon, "cube.p3d") != S_OK)
	{
		LogError("Failed Initialize model. Error Code: " + std::to_string(hr));
		return false;
	}

	m_model.SetPos(0, 2, 3);

	if (m_grassModel.Initialize(m_d3d11Device, m_d3d11DevCon, "grass.p3d") != S_OK)
	{
		LogError("Failed Initialize model. Error Code: " + std::to_string(hr));
		return false;
	}

	m_grassModel.SetPos(0, -2, 0);
	m_grassModel.SetRotation(0, 0, 0);
	m_grassModel.SetScaling(1000, 1000, 1000);

	camera.SetRotation(0,0,0);


	//create constant buffer for pixel shader ambient/directional light
	cb_ps_light.Initialize(m_d3d11Device);
	if (hr != S_OK)
	{
		LogError("Failed Initialize Constant Buffer for Lighted Pixel Shader. Error Code: " + std::to_string(hr));
		return false;
	}

	float lightxDir(1), lightyDir(0), lightzDir(0);
	cb_ps_light.Data.light.dir = XMFLOAT3(-lightxDir, -lightyDir, -lightzDir);
	cb_ps_light.Data.light.ambient = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	cb_ps_light.Data.light.diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	auto psbuffer = cb_ps_light.Buffer();
	m_d3d11DevCon->PSSetConstantBuffers(0, 1, &psbuffer); //set the constant buffer for the vertex shader

	cb_ps_light.ApplyChanges(m_d3d11DevCon);

	//Create point light constant buffer in pixel shader
	cb_ps_pointlight.Initialize(m_d3d11Device);
	if (hr != S_OK)
	{
		LogError("Failed Initialize Constant Buffer for PointLight Pixel Shader. Error Code: " + std::to_string(hr));
		return false;
	}

	PointLight temp;
	temp.attenuation = { 0.0f, 0.2f, 0.0f };
	temp.pos = { 0,0,0 };
	temp.color = { 1.0f,-0.2f,-0.2f ,1 };
	temp.range = 50;

	cb_ps_pointlight.Data.light = temp;
	auto psbuffer2 = cb_ps_pointlight.Buffer();
	m_d3d11DevCon->PSSetConstantBuffers(1, 1, &psbuffer2); //set the constant buffer for the vertex shader

	cb_ps_pointlight.ApplyChanges(m_d3d11DevCon);


	//Create rasterizer desc
	D3D11_RASTERIZER_DESC rasterizerdesc;

	ZeroMemory(&rasterizerdesc, sizeof(D3D11_RASTERIZER_DESC));
	rasterizerdesc.FillMode = D3D11_FILL_SOLID;
	rasterizerdesc.CullMode = D3D11_CULL_BACK;
	hr = m_d3d11Device->CreateRasterizerState(&rasterizerdesc, &m_rasterizerState);
	if (hr != S_OK)
	{
		LogError("Failed CreateRasterizerState. Error Code: " + std::to_string(hr));
		return false;
	}
	m_d3d11DevCon->RSSetState(m_rasterizerState);

	//Load texture into our shader resource
	hr = CreateDDSTextureFromFile(m_d3d11Device, L"Data\\hqskymap2.dds", NULL, &m_skyboxTexture);
	if (hr != S_OK)
	{
		LogError("Failed CreateDDSTextureFromFile. Error Code: " + std::to_string(hr));
		return false;
	}

	hr = CreateDDSTextureFromFile(m_d3d11Device, L"Data\\braynzar.dds", NULL, &m_testTexture);
	if (hr != S_OK)
	{
		LogError("Failed CreateDDSTextureFromFile. Error Code: " + std::to_string(hr));
		return false;
	}

	hr = CreateDDSTextureFromFile(m_d3d11Device, L"Data\\grass.dds", NULL, &m_grassTexture);
	if (hr != S_OK)
	{
		LogError("Failed CreateDDSTextureFromFile. Error Code: " + std::to_string(hr));
		return false;
	}

	//Create sampler description for sampler state
	D3D11_SAMPLER_DESC sampDesc;
	ZeroMemory(&sampDesc, sizeof(sampDesc));
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
	hr = m_d3d11Device->CreateSamplerState(&sampDesc, &m_samplerState); //Create sampler state
	if (hr != S_OK)
	{
		LogError("Failed CreateSamplerState. Error Code: " + std::to_string(hr));
		return false;
	}

	spriteBatch = std::make_unique<SpriteBatch>(m_d3d11DevCon);
	spriteFont = std::make_unique<SpriteFont>(m_d3d11Device, L"Data\\Fonts\\consolas.spritefont");

	return true;
}

void Graphics::Release()
{
	//Release the COM Objects we created
	if (m_swapChain != nullptr)
		m_swapChain->Release();
	if (m_d3d11Device != nullptr)
		m_d3d11Device->Release();
	if (m_d3d11DevCon != nullptr)
		m_d3d11DevCon->Release();
	if (m_vs != nullptr)
		m_vs->Release();
	if (m_ps != nullptr)
		m_ps->Release();
	if (m_vs_buffer != nullptr)
		m_vs_buffer->Release();
	if (m_ps_buffer != nullptr)
		m_ps_buffer->Release();
	if (m_vertLayout != nullptr)
		m_vertLayout->Release();
	if (m_vertBuffer != nullptr)
		m_vertBuffer->Release();
	if (m_vertLayout != nullptr)
		m_vertLayout->Release();
	if (m_depthStencilView != nullptr)
		m_depthStencilView->Release();
	if (m_depthStencilBuffer != nullptr)
		m_depthStencilBuffer->Release();
	if (m_rasterizerState != nullptr)
		m_rasterizerState->Release();
	if (m_testTexture != nullptr)
		m_testTexture->Release();
	if (m_samplerState != nullptr)
		m_samplerState->Release();
}

void Graphics::RenderFrame(float dt)
{
	m_fps.Frame();
	//Clear our backbuffer to the updated color
	const float bgColor[4] = { 0, 0, 0.0f, 1.0f };

	//??BLEND STATE?? - come back to this later
	auto psbuffer = cb_ps_light.Buffer();
	m_d3d11DevCon->PSSetConstantBuffers(0, 1, &psbuffer); //set the constant buffer for the pixel shader for ambient/directional light 
	cb_ps_light.ApplyChanges(m_d3d11DevCon);

	auto psbuffer2 = cb_ps_pointlight.Buffer();
	m_d3d11DevCon->PSSetConstantBuffers(1, 1, &psbuffer2); //set the constant buffer for the pixel shader for point light
	cb_ps_pointlight.ApplyChanges(m_d3d11DevCon);

	//??Depth Stencil State?? - come back to this later
	m_d3d11DevCon->IASetInputLayout(m_vertLayout);
	m_d3d11DevCon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_d3d11DevCon->RSSetState(m_rasterizerState);
	m_d3d11DevCon->PSSetSamplers(0, 1, &m_samplerState); //set sampler state to use
	
	m_d3d11DevCon->ClearRenderTargetView(m_renderTargetView, bgColor);
	//Refresh the Depth/Stencil view
	m_d3d11DevCon->ClearDepthStencilView(m_depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	//draw skybox
	m_d3d11DevCon->VSSetShader(m_vs_skymap, 0, 0);
	m_d3d11DevCon->PSSetShader(m_ps_skymap, 0, 0);

	m_d3d11DevCon->PSSetShaderResources(0, 1, &m_skyboxTexture); //set texture to use for pixel shader
	m_skybox.Draw(cb_vs_default, m_d3d11DevCon, camera.GetViewMatrix(), camera.GetProjectionMatrix());

	m_d3d11DevCon->VSSetShader(m_vs, 0, 0);
	m_d3d11DevCon->PSSetShader(m_ps, 0, 0);
	//draw grass
	m_d3d11DevCon->PSSetShaderResources(0, 1, &m_grassTexture); //set texture to use for pixel shader
	m_grassModel.Draw(cb_vs_default, m_d3d11DevCon, camera.GetViewMatrix(), camera.GetProjectionMatrix());

	//draw model
	m_d3d11DevCon->PSSetShaderResources(0, 1, &m_testTexture); //set texture to use for pixel shader
	m_model.Draw(cb_vs_default, m_d3d11DevCon, camera.GetViewMatrix(), camera.GetProjectionMatrix());
	
	

	//draw font
	spriteBatch->Begin();
	std::string writeString = "FPS: " + std::to_string(m_fps.GetFps());
	std::wstring wstr(writeString.begin(), writeString.end());
	const wchar_t * cstr = wstr.c_str();
	spriteFont->DrawString(spriteBatch.get(), cstr, XMFLOAT2(10, 10), Colors::Red, 0.0f, { 0,0 }, { 0.5f,0.5f });

	std::wstring wstr2(drawText.begin(), drawText.end());
	const wchar_t * cstr2 = wstr2.c_str();
	spriteFont->DrawString(spriteBatch.get(), cstr2, XMFLOAT2(10, 27), Colors::White, 0.0f, { 0,0 }, { 0.5f,0.5f });

	spriteBatch->End();


	//Present the backbuffer to the screen
	m_swapChain->Present(0, 0);
}