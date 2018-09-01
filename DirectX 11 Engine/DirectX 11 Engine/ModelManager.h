#pragma once
#include "ModelData.h"

class ModelManager
{
public:
	static std::shared_ptr<ModelData> GetModelData(std::string filename);
	static void StoreModelData(std::shared_ptr<ModelData> modeldata);
	static void ReleaseModelInstance(std::shared_ptr<ModelData> modeldata);
private:
	static std::vector<std::shared_ptr<ModelData>> loaded_models;
};