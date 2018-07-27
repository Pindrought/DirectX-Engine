#include "Graphics.h"
#include "ShaderLoader.h"
#include <fstream>

void GenerateP3d() //was just using this to generate my skybox 3d object, keeping for documentation in case I messed something up (probably did)
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
	this->fps.Initialize();

	this->hwnd = hwnd;
	this->width = width;
	this->height = height;

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
	swapChainDesc.BufferCount = 2; //# of buffers in swap chain
	swapChainDesc.OutputWindow = this->hwnd;
	swapChainDesc.Windowed = TRUE;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	//Create our SwapChain
	hr = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, NULL, NULL, NULL,
		D3D11_SDK_VERSION, &swapChainDesc, &this->swapChain, &this->d3d11Device, NULL, &this->d3d11DevCon);
	if (hr != S_OK) //If error occurred
	{
		LogError("D3D11CreateDeviceAndSwapChain Failed. HRESULT = " + std::to_string(hr));
		return false;
	}

	//Create our BackBuffer
	ID3D11Texture2D* backBuffer;
	hr = this->swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer));
	if (hr != S_OK) //If error occurred
	{
		LogError("IDXGISwapChain::GetBuffer Failed. HRESULT = " + std::to_string(hr));
		return false;
	}

	//Describe our Depth/Stencil Buffer
	D3D11_TEXTURE2D_DESC depthStencilDesc;
	depthStencilDesc.Width = this->width;
	depthStencilDesc.Height = this->height;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilDesc.SampleDesc.Count = 8;
	depthStencilDesc.SampleDesc.Quality = 0;
	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.CPUAccessFlags = 0;
	depthStencilDesc.MiscFlags = 0;

	hr = this->d3d11Device->CreateTexture2D(&depthStencilDesc, NULL, &this->depthStencilBuffer);
	if (hr != S_OK) //If error occurred
	{
		LogError("ID3D11Device::CreateTexture2D Failed when used for depth stencil buffer. HRESULT = " + std::to_string(hr));
		return false;
	}

	hr = this->d3d11Device->CreateDepthStencilView(this->depthStencilBuffer, NULL, &this->depthStencilView);
	if (hr != S_OK) //If error occurred
	{
		LogError("ID3D11Device::CreateDepthStencilView Failed when used for depth stencil view. HRESULT = " + std::to_string(hr));
		return false;
	}

	//Create our Render Target
	hr = this->d3d11Device->CreateRenderTargetView(backBuffer, NULL, &this->renderTargetView);
	if (hr != S_OK) //If error occurred
	{
		LogError("ID3D11Device::CreateRenderTargetView Failed. HRESULT = " + std::to_string(hr));
		return false;
	}
	backBuffer->Release();

	//Set our Render Target
	this->d3d11DevCon->OMSetRenderTargets(1, &this->renderTargetView, this->depthStencilView);

	if (InitializeScene() == false)
		return false;

	return true;

}

bool Graphics::InitializeScene()
{
	//load default shaders
	HRESULT hr;
	hr = ShaderLoader::LoadCompiledShader("Debug\\defaultvertex.cso", &this->vs_buffer);
	if (hr != S_OK)
	{
		LogError("Failed to load vertex shader. Error Code: " + HRToString(hr));
		return false;
	}
	hr = ShaderLoader::LoadCompiledShader("Debug\\defaultpixel.cso", &this->ps_buffer);
	if (hr != S_OK)
	{
		LogError("Failed to load pixel shader. Error Code: " + HRToString(hr));
		return false;
	}

	//load compiled shaders
	hr = ShaderLoader::LoadCompiledShader("Debug\\skymapvertex.cso", &this->vs_skymap_buffer);
	if (hr != S_OK)
	{
		LogError("Failed to load vertex shader. Error Code: " + HRToString(hr));
		return false;
	}
	hr = ShaderLoader::LoadCompiledShader("Debug\\skymappixel.cso", &this->ps_skymap_buffer);
	if (hr != S_OK)
	{
		LogError("Failed to load pixel shader. Error Code: " + HRToString(hr));
		return false;
	}

	//Create the Shader Objects
	hr = this->d3d11Device->CreateVertexShader(this->vs_buffer->GetBufferPointer(), this->vs_buffer->GetBufferSize(), NULL, &this->vs);
	if (hr != S_OK)
	{
		LogError("Failed to create Vertex Shader. Error Code: " + HRToString(hr));
		return false;
	}

	hr = this->d3d11Device->CreatePixelShader(this->ps_buffer->GetBufferPointer(), this->ps_buffer->GetBufferSize(), NULL, &this->ps);
	if (hr != S_OK)
	{
		LogError("Failed to create Pixel Shader. Error Code: " + HRToString(hr));
		return false;
	}

	//create skymap shaders
	hr = this->d3d11Device->CreateVertexShader(this->vs_skymap_buffer->GetBufferPointer(), this->vs_skymap_buffer->GetBufferSize(), NULL, &this->vs_skymap);
	if (hr != S_OK)
	{
		LogError("Failed to create Vertex Shader. Error Code: " + HRToString(hr));
		return false;
	}

	hr = this->d3d11Device->CreatePixelShader(this->ps_skymap_buffer->GetBufferPointer(), this->ps_skymap_buffer->GetBufferSize(), NULL, &this->ps_skymap);
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

	this->d3d11Device->CreateInputLayout(layout, numElements, this->vs_buffer->GetBufferPointer(),
		this->vs_buffer->GetBufferSize(), &this->vertLayout);
	if (hr != S_OK)
	{
		LogError("Failed to create Input Layout. Error Code: " + std::to_string(hr));
		return false;
	}

	//Set the Input Layout
	this->d3d11DevCon->IASetInputLayout(this->vertLayout);

	//Set Primitive Topology
	this->d3d11DevCon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//Create the Viewport
	D3D11_VIEWPORT viewport;
	ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = this->width;
	viewport.Height = this->height;

	//Set the Viewport for Rasterizer Stage
	this->d3d11DevCon->RSSetViewports(1, &viewport);

	//Setup camera and world view projection matrix
	camera.SetPosition(0, 0, -1);
	camera.SetProjectionValues(90, this->width, this->height, 1, 1000);
	
	//create constant buffer for vertex shader
	hr = cb_vs_default.Initialize(this->d3d11Device);
	if (hr != S_OK)
	{
		LogError("Failed Initialize default Vertex Shader Constant Buffer. Error Code: " + std::to_string(hr));
		return false;
	}

	if (hr = this->skybox.Initialize(this->d3d11Device, this->d3d11DevCon, "skybox.p3d") != S_OK)
	{
		LogError("Failed Initialize model. Error Code: " + std::to_string(hr));
		return false;
	}
	this->skybox.SetPos(0, 0, 0);

	//if (hr = this->cube.Initialize(this->d3d11Device, this->d3d11DevCon, "Data\\Objects\\cube.obj") != S_OK)//


	if (hr = this->cube.Initialize(this->d3d11Device, this->d3d11DevCon, "Data\\Objects\\cube.obj") != S_OK)
	{
		LogError("Failed Initialize model. Error Code: " + std::to_string(hr));
		return false;
	}

	this->cube.SetPos(0, 2, 3);

	if (hr = this->grassModel.Initialize(this->d3d11Device, this->d3d11DevCon, "grass.p3d") != S_OK)
	{
		LogError("Failed Initialize model. Error Code: " + std::to_string(hr));
		return false;
	}

	this->grassModel.SetPos(0, -2, 0);
	this->grassModel.SetRotation(0, 0, 0);
	this->grassModel.SetScaling(1000, 1000, 1000);

	camera.SetRotation(0,0,0);


	//create constant buffer for pixel shader ambient/directional light
	hr = cb_ps_light.Initialize(this->d3d11Device);
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
	this->d3d11DevCon->PSSetConstantBuffers(0, 1, &psbuffer); //set the constant buffer for the vertex shader

	cb_ps_light.ApplyChanges(this->d3d11DevCon);

	//Create point light constant buffer in pixel shader
	hr = cb_ps_pointlight.Initialize(this->d3d11Device);
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
	this->d3d11DevCon->PSSetConstantBuffers(1, 1, &psbuffer2); //set the constant buffer for the vertex shader

	cb_ps_pointlight.ApplyChanges(this->d3d11DevCon);


	D3D11_DEPTH_STENCIL_DESC depthstencildesc;
	ZeroMemory(&depthstencildesc, sizeof(D3D11_DEPTH_STENCIL_DESC));

	depthstencildesc.DepthEnable = true;
	depthstencildesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthstencildesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

	hr = this->d3d11Device->CreateDepthStencilState(&depthstencildesc, &this->depthStencilState);
	if (hr != S_OK)
	{
		LogError("Failed CreateRasterizerState. Error Code: " + std::to_string(hr));
		return false;
	}
	this->d3d11DevCon->OMSetDepthStencilState(this->depthStencilState, 0);

	//create transparent blend state for ui elements
	D3D11_BLEND_DESC blendDesc;
	ZeroMemory(&blendDesc, sizeof(blendDesc));

	D3D11_RENDER_TARGET_BLEND_DESC rtbd;
	ZeroMemory(&rtbd, sizeof(rtbd));

	rtbd.BlendEnable = true;
	rtbd.SrcBlend = D3D11_BLEND_SRC_COLOR;
	rtbd.DestBlend = D3D11_BLEND_BLEND_FACTOR;
	rtbd.BlendOp = D3D11_BLEND_OP_ADD;
	rtbd.SrcBlendAlpha = D3D11_BLEND_SRC_ALPHA;
	rtbd.DestBlendAlpha = D3D11_BLEND_SRC_ALPHA;
	rtbd.BlendOpAlpha = D3D11_BLEND_OP_ADD;
	rtbd.RenderTargetWriteMask = D3D10_COLOR_WRITE_ENABLE_ALL;

	blendDesc.AlphaToCoverageEnable = false;
	blendDesc.RenderTarget[0] = rtbd;

	hr = this->d3d11Device->CreateBlendState(&blendDesc, &this->transparentBlendState);
	if (FAILED(hr))
	{
		LogError("Failed CreateBlendState. Error Code: " + std::to_string(hr));
		return false;
	}

	//Create rasterizer desc
	D3D11_RASTERIZER_DESC rasterizerdesc;

	ZeroMemory(&rasterizerdesc, sizeof(D3D11_RASTERIZER_DESC));
	rasterizerdesc.FillMode = D3D11_FILL_SOLID;
	rasterizerdesc.CullMode = D3D11_CULL_BACK;
	hr = this->d3d11Device->CreateRasterizerState(&rasterizerdesc, &this->rasterizerState);
	if (hr != S_OK)
	{
		LogError("Failed CreateRasterizerState. Error Code: " + std::to_string(hr));
		return false;
	}
	this->d3d11DevCon->RSSetState(this->rasterizerState);

	//Load texture into our shader resource
	hr = CreateDDSTextureFromFile(this->d3d11Device, L"Data\\hqskymap2.dds", NULL, &this->skyboxTexture);
	if (hr != S_OK)
	{
		LogError("Failed CreateDDSTextureFromFile. Error Code: " + std::to_string(hr));
		return false;
	}

	hr = CreateDDSTextureFromFile(this->d3d11Device, L"Data\\braynzar.dds", NULL, &this->testTexture);
	if (hr != S_OK)
	{
		LogError("Failed CreateDDSTextureFromFile. Error Code: " + std::to_string(hr));
		return false;
	}

	hr = CreateDDSTextureFromFile(this->d3d11Device, L"Data\\grass.dds", NULL, &this->grassTexture);
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
	hr = this->d3d11Device->CreateSamplerState(&sampDesc, &this->samplerState); //Create sampler state
	if (hr != S_OK)
	{
		LogError("Failed CreateSamplerState. Error Code: " + std::to_string(hr));
		return false;
	}

	spriteBatch = std::make_unique<SpriteBatch>(this->d3d11DevCon);
	spriteFont = std::make_unique<SpriteFont>(this->d3d11Device, L"Data\\Fonts\\consolas.spritefont");

	return true;
}

void Graphics::Release()
{
	//Release the COM Objects we created
	if (this->swapChain != nullptr)
		this->swapChain->Release();
	if (this->d3d11Device != nullptr)
		this->d3d11Device->Release();
	if (this->d3d11DevCon != nullptr)
		this->d3d11DevCon->Release();
	if (this->vs != nullptr)
		this->vs->Release();
	if (this->ps != nullptr)
		this->ps->Release();
	if (this->vs_buffer != nullptr)
		this->vs_buffer->Release();
	if (this->ps_buffer != nullptr)
		this->ps_buffer->Release();
	if (this->vertLayout != nullptr)
		this->vertLayout->Release();
	if (this->vertBuffer != nullptr)
		this->vertBuffer->Release();
	if (this->vertLayout != nullptr)
		this->vertLayout->Release();
	if (this->depthStencilView != nullptr)
		this->depthStencilView->Release();
	if (this->depthStencilBuffer != nullptr)
		this->depthStencilBuffer->Release();
	if (this->rasterizerState != nullptr)
		this->rasterizerState->Release();
	if (this->testTexture != nullptr)
		this->testTexture->Release();
	if (this->samplerState != nullptr)
		this->samplerState->Release();
	if (this->transparentBlendState != nullptr)
		this->transparentBlendState->Release();
}

void Graphics::RenderFrame(float dt)
{
	this->fps.Frame();
	//Clear our backbuffer to the updated color
	const float bgColor[4] = { 1.0, 0, 0.0f, 1.0f };

	

	this->d3d11DevCon->OMSetBlendState(0, 0, 0xffffffff); //set blend state to opaque for solid objects
	auto psbuffer = cb_ps_light.Buffer();
	this->d3d11DevCon->PSSetConstantBuffers(0, 1, &psbuffer); //set the constant buffer for the pixel shader for ambient/directional light 
	cb_ps_light.ApplyChanges(this->d3d11DevCon);

	auto psbuffer2 = cb_ps_pointlight.Buffer();
	this->d3d11DevCon->PSSetConstantBuffers(1, 1, &psbuffer2); //set the constant buffer for the pixel shader for point light
	cb_ps_pointlight.ApplyChanges(this->d3d11DevCon);

	this->d3d11DevCon->OMSetDepthStencilState(this->depthStencilState, 0);
	this->d3d11DevCon->IASetInputLayout(this->vertLayout);
	this->d3d11DevCon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	this->d3d11DevCon->RSSetState(this->rasterizerState);
	this->d3d11DevCon->PSSetSamplers(0, 1, &this->samplerState); //set sampler state to use
	
	this->d3d11DevCon->ClearRenderTargetView(this->renderTargetView, bgColor);
	//Refresh the Depth/Stencil view
	this->d3d11DevCon->ClearDepthStencilView(this->depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	//draw skybox
	/*static float bval = 0.00f;
	static float bdt = 0.001f;
	bval += bdt*dt;
	if (bval >= 1.0f || bval < 0.0f)
	{
		bdt = -bdt;
		bval += bdt*dt;
	}
	float blendFactor[] = { bval, bval, bval, bval };
	d3d11DevCon->OMSetBlendState(this->transparentBlendState, blendFactor, 0xffffffff);
*/

	this->d3d11DevCon->VSSetShader(this->vs_skymap, 0, 0);
	this->d3d11DevCon->PSSetShader(this->ps_skymap, 0, 0);

	this->d3d11DevCon->PSSetShaderResources(0, 1, &this->skyboxTexture); //set texture to use for pixel shader
	this->skybox.Draw(cb_vs_default, this->d3d11DevCon, camera.GetViewMatrix(), camera.GetProjectionMatrix());

	this->d3d11DevCon->VSSetShader(this->vs, 0, 0);
	this->d3d11DevCon->PSSetShader(this->ps, 0, 0);

	this->d3d11DevCon->OMSetBlendState(0, 0, 0xffffffff); //set blend state to opaque for solid objects

	//draw grass
	this->d3d11DevCon->PSSetShaderResources(0, 1, &this->grassTexture); //set texture to use for pixel shader
	this->grassModel.Draw(cb_vs_default, this->d3d11DevCon, camera.GetViewMatrix(), camera.GetProjectionMatrix());

	//draw cube model
	this->d3d11DevCon->PSSetShaderResources(0, 1, &this->testTexture); //set texture to use for pixel shader
	this->cube.Draw(cb_vs_default, this->d3d11DevCon, camera.GetViewMatrix(), camera.GetProjectionMatrix());
	
	

	//draw font
	spriteBatch->Begin();
	std::string writeString = "FPS: " + std::to_string(this->fps.GetFps());
	std::wstring wstr(writeString.begin(), writeString.end());
	const wchar_t * cstr = wstr.c_str();
	spriteFont->DrawString(spriteBatch.get(), cstr, XMFLOAT2(10, 10), Colors::Red, 0.0f, { 0,0 }, { 0.5f,0.5f });

	std::wstring wstr2(drawText.begin(), drawText.end());
	const wchar_t * cstr2 = wstr2.c_str();
	spriteFont->DrawString(spriteBatch.get(), cstr2, XMFLOAT2(10, 27), Colors::White, 0.0f, { 0,0 }, { 0.5f,0.5f });
	spriteBatch->End();


	//Present the backbuffer to the screen
	this->swapChain->Present(0, 0);
}