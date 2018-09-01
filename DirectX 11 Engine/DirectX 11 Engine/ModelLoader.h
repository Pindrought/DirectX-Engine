#pragma once
#include "ModelManager.h"

class ModelLoader
{
public:
	static HRESULT LoadModel(Microsoft::WRL::ComPtr<ID3D11Device> &device, std::string &fileName, std::shared_ptr<ModelData> &modeldata);
private:
	static HRESULT Loadp3d(Microsoft::WRL::ComPtr<ID3D11Device> &device, std::string &fileName, std::shared_ptr<ModelData> &modeldata);
	static HRESULT Loadobj(Microsoft::WRL::ComPtr<ID3D11Device> &device, std::string &fileName, std::shared_ptr<ModelData> &modeldata);
};