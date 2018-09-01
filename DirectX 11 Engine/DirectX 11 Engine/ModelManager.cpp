#include "ModelManager.h"

std::vector<std::shared_ptr<ModelData>> ModelManager::loaded_models;

std::shared_ptr<ModelData> ModelManager::GetModelData(std::string filename)
{
	for (int i = 0; i < loaded_models.size(); i++)
	{
		if (strcmp(loaded_models[i]->name.c_str(), filename.c_str()) == 0)
		{
			loaded_models[i]->refCount += 1;
			return loaded_models[i];
		}
	}
	return nullptr;
}

void ModelManager::StoreModelData(std::shared_ptr<ModelData> modeldata)
{
	modeldata->refCount = 1;
	loaded_models.push_back(modeldata);
}

void ModelManager::ReleaseModelInstance(std::shared_ptr<ModelData> modeldata)
{
	if (modeldata == nullptr)
		return;
	modeldata->refCount -= 1;
	if (modeldata->refCount == 0)
	{
		for (int i = 0; i < loaded_models.size(); i++)
		{
			if (loaded_models[i] == modeldata)
			{
				loaded_models.erase(loaded_models.begin() + i);
				break;
			}
		}
	}
}