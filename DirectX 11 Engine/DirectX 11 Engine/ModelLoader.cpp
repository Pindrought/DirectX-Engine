#include "ModelLoader.h"
#include <fstream>
#include <memory>
#include "LogError.h"
#include <vector>
#include <sstream>

HRESULT ModelLoader::LoadModel(ID3D11Device * &device, ID3D11DeviceContext * &deviceContext, std::string &fileName, ID3D11Buffer * &vertexBuffer, ID3D11Buffer * &indexBuffer, UINT &vertCount)
{
	std::string p3dextension = ".p3d";
	std::string objextension = ".obj";
	size_t pos = fileName.find(p3dextension);
	size_t val = (fileName.size() - 5);
	if (fileName.find(p3dextension) == (fileName.size()-4)) 
	{
		return Loadp3d(device, deviceContext, fileName, vertexBuffer, indexBuffer, vertCount);
	}
	if (fileName.find(objextension) == (fileName.size() - 4))
	{
		return Loadobj(device, deviceContext, fileName, vertexBuffer, indexBuffer, vertCount);
	}
	LogError("3d Object File Extension not supported in file: " + fileName);
	return E_NOTIMPL;
}



HRESULT ModelLoader::Loadp3d(ID3D11Device * &device, ID3D11DeviceContext * &deviceContext, std::string &fileName, ID3D11Buffer * &vertexBuffer, ID3D11Buffer * &indexBuffer, UINT &vertCount)
{
	std::ifstream infile(fileName, std::ios::binary);

	if (infile.is_open() == false)
		return E_ACCESSDENIED;
	uint32_t vertexCount = 0;
	uint32_t indexCount = 0;
	infile.read((char*)&vertexCount, sizeof(uint32_t)); //vertex count
	infile.read((char*)&indexCount, sizeof(uint32_t)); //index count
	std::unique_ptr<Vertex[]> v(new Vertex[vertexCount]); //vertices data
	infile.read((char*)v.get(), sizeof(Vertex) * vertexCount);
	std::unique_ptr<DWORD[]> indices(new DWORD[indexCount]); //indices data
	infile.read((char*)indices.get(), sizeof(DWORD) * indexCount);
	vertCount = indexCount;

	Vertex * check = v.get();
	DWORD * check2 = indices.get();


	/*{
		std::vector<Vertex> vertdata;
		vertdata.resize(vertexCount);
		memcpy(vertdata.data(), v.get(), sizeof(Vertex)*vertCount);

		std::vector<DWORD> indexdata;
		indexdata.resize(indexCount);
		memcpy(indexdata.data(), indices.get(), sizeof(DWORD)*indexCount);
		Sleep(5);
	}*/

	D3D11_BUFFER_DESC vertexBufferDesc;
	ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));

	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(Vertex)*vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA vertexBufferData;

	ZeroMemory(&vertexBufferData, sizeof(vertexBufferData));
	vertexBufferData.pSysMem = v.get();
	HRESULT hr;
	hr = device->CreateBuffer(&vertexBufferDesc, &vertexBufferData, &vertexBuffer);
	if (hr != S_OK)
	{
		LogError("Failed to call ID3D11Device::CreateBuffer for the vertex buffer. Error Code: " + std::to_string(hr));
		return hr;
	}


	D3D11_BUFFER_DESC indexBufferDesc;
	ZeroMemory(&indexBufferDesc, sizeof(indexBufferDesc));
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(DWORD)*indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA indexBufferData;
	indexBufferData.pSysMem = indices.get();
	hr = device->CreateBuffer(&indexBufferDesc, &indexBufferData, &indexBuffer);
	if (hr != S_OK)
	{
		LogError("Failed to call ID3D11Device::CreateBuffer for the index buffer. Error Code: " + std::to_string(hr));
		return hr;
	}

	return S_OK;
}

bool LoadFaceData(std::string mystring, int * facedata)
{
	std::vector<std::string> values;
	std::string line = "";
	for (int i = 0; i < mystring.size(); i++)
	{
		switch (mystring[i])
		{
		case ' ':
		case '/':
		case '\r':
		case '\n':
			if (line.size() != 0)
			{
				values.push_back(line);
				line = "";
			}
			break;
		default:
			line += mystring[i];
		}
	}
	if (line.size() != 0)
	{
		values.push_back(line);
		line = "";
	}

	if (values.size() != 9)
		return false;

	for (int i = 0; i < 9; i++)
	{
		std::stringstream ss(values[i]);
		int value = 0;
		ss >> value;
		facedata[i] = value;
	}
	return true;
}

HRESULT ModelLoader::Loadobj(ID3D11Device *& device, ID3D11DeviceContext *& deviceContext, std::string & fileName, ID3D11Buffer *& vertexBuffer, ID3D11Buffer *& indexBuffer, UINT & vertCount)
{
	std::vector<Vertex> vertices;

	std::ifstream infile(fileName, std::ios::binary);

	if (infile.is_open() == false)
		return E_ACCESSDENIED;

	HRESULT hr = 0;
	std::string meshMatLib;					//String to hold our obj material library filename

	bool is_righthanded_coordinate_system = false;
												//Arrays to store our model's information
	std::vector<DWORD> indices;
	std::vector<XMFLOAT3> vertPos;
	std::vector<XMFLOAT3> vertNorm;
	std::vector<XMFLOAT2> vertTexCoord;
	std::vector<std::string> meshMaterials;

	//Vertex definition indices
	std::vector<int> vertPosIndex;
	std::vector<int> vertNormIndex;
	std::vector<int> vertTCIndex;

	//Make sure we have a default if no tex coords or normals are defined
	bool hasTexCoord = false;
	bool hasNorm = false;

	//Temp variables to store into vectors
	std::string meshMaterialsTemp;
	int vertPosIndexTemp;
	int vertNormIndexTemp;
	int vertTCIndexTemp;

	wchar_t checkChar;		//The variable we will use to store one char from file at a time
	std::string face;		//Holds the string containing our face vertices
	int vIndex = 0;			//Keep track of our vertex index count
	int triangleCount = 0;	//Total Triangles
	int totalVerts = 0;
	int meshTriangles = 0;


	while (!infile.eof()) //while not at end of file
	{
		std::streampos pos = infile.tellg();
		std::string word;
		infile >> word;
		if (infile.eof())
			break;

		if (word[0] == '#') //if line is a comment
		{
			infile.seekg(pos); //seek back to beginning of this line
			std::string line;
			std::getline(infile, line); //skip this line
			continue;
		}

		if (strcmp(word.c_str(), "v") == 0) //if vertices
		{
			float vx, vy, vz;
			infile >> vx >> vy >> vz;
			if (is_righthanded_coordinate_system)	//If model is from an RH Coord System
				vertPos.push_back(XMFLOAT3(vx, vy, vz * -1.0f));	//Invert the Z axis
			else
				vertPos.push_back(XMFLOAT3(vx, vy, vz));
			continue;
		}

		if (strcmp(word.c_str(), "vt") == 0) //if vert tex coords
		{
			float vtcu, vtcv;
			infile >> vtcu >> vtcv;		//Store next two types

			if (is_righthanded_coordinate_system)	//If model is from an RH Coord System
				vertTexCoord.push_back(XMFLOAT2(vtcu, 1.0f - vtcv));	//Reverse the "v" axis
			else
				vertTexCoord.push_back(XMFLOAT2(vtcu, vtcv));

			hasTexCoord = true;	//We know the model uses texture coords
			continue;
		}

		if (strcmp(word.c_str(), "vn") == 0) //if vert normals
		{
			float vnx, vny, vnz;
			infile >> vnx >> vny >> vnz;	//Store next three types

			if (is_righthanded_coordinate_system)	//If model is from an RH Coord System
				vertNorm.push_back(XMFLOAT3(vnx, vny, vnz * -1.0f));	//Invert the Z axis
			else
				vertNorm.push_back(XMFLOAT3(vnx, vny, vnz));

			hasNorm = true;	//We know the model defines normals
			continue;
		}

		//if (strcmp(word.c_str(), "g") == 0) //if new group
		//{
		//	subsetIndexStart.push_back(vIndex);		//Start index for this subset
		//	subsetCount++;
		//	infile.seekg(pos); //seek back to beginning of this line
		//	std::string line;
		//	std::getline(infile, line); //skip this line
		//}

		if (strcmp(word.c_str(), "s") == 0) //if smooth shading (we are skipping this for now)
		{
			infile.seekg(pos); //seek back to beginning of this line
			std::string line;
			std::getline(infile, line); //skip this line
			continue;
		}

		if (strcmp(word.c_str(), "mtllib") == 0) //if material lib
		{
			infile >> meshMatLib; //get material library name
			infile.seekg(pos); //seek back to beginning of this line
			std::string line;
			std::getline(infile, line); //skip this line
			continue;
		}

		if (strcmp(word.c_str(), "usemtl") == 0) //if use material lib
		{
			meshMaterialsTemp = "";	//Make sure this is cleared
			infile >> meshMaterialsTemp; //Get next type (string)
			meshMaterials.push_back(meshMaterialsTemp);
			continue;
		}

		if (strcmp(word.c_str(), "f") == 0) //if use material lib
		{
			std::string line;
			std::getline(infile, line); //get face data
			int facedata[9];
			if (!LoadFaceData(line, &facedata[0]))
			{
				LogError("Failed to load face data for line: " + line + ". OBJ file needs to have only 3 vertices per face and position/texture coord/normal for each vertex.");
			}
			for (int i = 0; i < 9; i++)
			{
				facedata[i] -= 1; //element indexes start at 1 in obj file, so adjust for c++ arrays
			}
			for (int i = 0; i < 3; i++)
			{
				Vertex newvert;
				newvert.pos = vertPos[facedata[(i*3)+0]];
				newvert.texCoord = vertTexCoord[facedata[(i * 3) +1]];
				newvert.normal = vertNorm[facedata[(i * 3) +2]];
				vertices.push_back(newvert);
			}
			continue;
		}
	}

	for (int i = 0; i < vertices.size(); i++)
	{
		indices.push_back(i); //we'll fix this later but for now reusing vertices and just assigning a seperate index for each vert
	}

	vertCount = indices.size();

	D3D11_BUFFER_DESC vertexBufferDesc;
	ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));

	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(Vertex)*vertices.size();
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA vertexBufferData;

	ZeroMemory(&vertexBufferData, sizeof(vertexBufferData));
	vertexBufferData.pSysMem = vertices.data();
	hr = device->CreateBuffer(&vertexBufferDesc, &vertexBufferData, &vertexBuffer);
	if (hr != S_OK)
	{
		LogError("Failed to call ID3D11Device::CreateBuffer for the vertex buffer. Error Code: " + std::to_string(hr));
		return hr;
	}


	D3D11_BUFFER_DESC indexBufferDesc;
	ZeroMemory(&indexBufferDesc, sizeof(indexBufferDesc));
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(DWORD)*indices.size();
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA indexBufferData;
	indexBufferData.pSysMem = indices.data();
	hr = device->CreateBuffer(&indexBufferDesc, &indexBufferData, &indexBuffer);
	if (hr != S_OK)
	{
		LogError("Failed to call ID3D11Device::CreateBuffer for the index buffer. Error Code: " + std::to_string(hr));
		return hr;
	}

	//	switch (checkChar)
	//	{

	//		//Get Face Index
	//	case 'f':	//f - defines the faces
	//		checkChar = fileIn.get();
	//		if (checkChar == ' ')
	//		{
	//			face = L"";
	//			std::wstring VertDef;	//Holds one vertex definition at a time
	//			triangleCount = 0;

	//			checkChar = fileIn.get();
	//			while (checkChar != '\n')
	//			{
	//				face += checkChar;			//Add the char to our face string
	//				checkChar = fileIn.get();	//Get the next Character
	//				if (checkChar == ' ')		//If its a space...
	//					triangleCount++;		//Increase our triangle count
	//			}

	//			//Check for space at the end of our face string
	//			if (face[face.length() - 1] == ' ')
	//				triangleCount--;	//Each space adds to our triangle count

	//			triangleCount -= 1;		//Ever vertex in the face AFTER the first two are new faces

	//			std::wstringstream ss(face);

	//			if (face.length() > 0)
	//			{
	//				int firstVIndex, lastVIndex;	//Holds the first and last vertice's index

	//				for (int i = 0; i < 3; ++i)		//First three vertices (first triangle)
	//				{
	//					ss >> VertDef;	//Get vertex definition (vPos/vTexCoord/vNorm)

	//					std::wstring vertPart;
	//					int whichPart = 0;		//(vPos, vTexCoord, or vNorm)

	//											//Parse this string
	//					for (int j = 0; j < VertDef.length(); ++j)
	//					{
	//						if (VertDef[j] != '/')	//If there is no divider "/", add a char to our vertPart
	//							vertPart += VertDef[j];

	//						//If the current char is a divider "/", or its the last character in the string
	//						if (VertDef[j] == '/' || j == VertDef.length() - 1)
	//						{
	//							std::wistringstream wstringToInt(vertPart);	//Used to convert wstring to int

	//							if (whichPart == 0)	//If vPos
	//							{
	//								wstringToInt >> vertPosIndexTemp;
	//								vertPosIndexTemp -= 1;		//subtract one since c++ arrays start with 0, and obj start with 1

	//															//Check to see if the vert pos was the only thing specified
	//								if (j == VertDef.length() - 1)
	//								{
	//									vertNormIndexTemp = 0;
	//									vertTCIndexTemp = 0;
	//								}
	//							}

	//							else if (whichPart == 1)	//If vTexCoord
	//							{
	//								if (vertPart != L"")	//Check to see if there even is a tex coord
	//								{
	//									wstringToInt >> vertTCIndexTemp;
	//									vertTCIndexTemp -= 1;	//subtract one since c++ arrays start with 0, and obj start with 1
	//								}
	//								else	//If there is no tex coord, make a default
	//									vertTCIndexTemp = 0;

	//								//If the cur. char is the second to last in the string, then
	//								//there must be no normal, so set a default normal
	//								if (j == VertDef.length() - 1)
	//									vertNormIndexTemp = 0;

	//							}
	//							else if (whichPart == 2)	//If vNorm
	//							{
	//								std::wistringstream wstringToInt(vertPart);

	//								wstringToInt >> vertNormIndexTemp;
	//								vertNormIndexTemp -= 1;		//subtract one since c++ arrays start with 0, and obj start with 1
	//							}

	//							vertPart = L"";	//Get ready for next vertex part
	//							whichPart++;	//Move on to next vertex part					
	//						}
	//					}

	//					//Check to make sure there is at least one subset
	//					if (subsetCount == 0)
	//					{
	//						subsetIndexStart.push_back(vIndex);		//Start index for this subset
	//						subsetCount++;
	//					}

	//					//Avoid duplicate vertices
	//					bool vertAlreadyExists = false;
	//					if (totalVerts >= 3)	//Make sure we at least have one triangle to check
	//					{
	//						//Loop through all the vertices
	//						for (int iCheck = 0; iCheck < totalVerts; ++iCheck)
	//						{
	//							//If the vertex position and texture coordinate in memory are the same
	//							//As the vertex position and texture coordinate we just now got out
	//							//of the obj file, we will set this faces vertex index to the vertex's
	//							//index value in memory. This makes sure we don't create duplicate vertices
	//							if (vertPosIndexTemp == vertPosIndex[iCheck] && !vertAlreadyExists)
	//							{
	//								if (vertTCIndexTemp == vertTCIndex[iCheck])
	//								{
	//									indices.push_back(iCheck);		//Set index for this vertex
	//									vertAlreadyExists = true;		//If we've made it here, the vertex already exists
	//								}
	//							}
	//						}
	//					}

	//					//If this vertex is not already in our vertex arrays, put it there
	//					if (!vertAlreadyExists)
	//					{
	//						vertPosIndex.push_back(vertPosIndexTemp);
	//						vertTCIndex.push_back(vertTCIndexTemp);
	//						vertNormIndex.push_back(vertNormIndexTemp);
	//						totalVerts++;	//We created a new vertex
	//						indices.push_back(totalVerts - 1);	//Set index for this vertex
	//					}

	//					//If this is the very first vertex in the face, we need to
	//					//make sure the rest of the triangles use this vertex
	//					if (i == 0)
	//					{
	//						firstVIndex = indices[vIndex];	//The first vertex index of this FACE
	//					}

	//					//If this was the last vertex in the first triangle, we will make sure
	//					//the next triangle uses this one (eg. tri1(1,2,3) tri2(1,3,4) tri3(1,4,5))
	//					if (i == 2)
	//					{
	//						lastVIndex = indices[vIndex];	//The last vertex index of this TRIANGLE
	//					}
	//					vIndex++;	//Increment index count
	//				}

	//				meshTriangles++;	//One triangle down

	//									//If there are more than three vertices in the face definition, we need to make sure
	//									//we convert the face to triangles. We created our first triangle above, now we will
	//									//create a new triangle for every new vertex in the face, using the very first vertex
	//									//of the face, and the last vertex from the triangle before the current triangle
	//				for (int l = 0; l < triangleCount - 1; ++l)	//Loop through the next vertices to create new triangles
	//				{
	//					//First vertex of this triangle (the very first vertex of the face too)
	//					indices.push_back(firstVIndex);			//Set index for this vertex
	//					vIndex++;

	//					//Second Vertex of this triangle (the last vertex used in the tri before this one)
	//					indices.push_back(lastVIndex);			//Set index for this vertex
	//					vIndex++;

	//					//Get the third vertex for this triangle
	//					ss >> VertDef;

	//					std::wstring vertPart;
	//					int whichPart = 0;

	//					//Parse this string (same as above)
	//					for (int j = 0; j < VertDef.length(); ++j)
	//					{
	//						if (VertDef[j] != '/')
	//							vertPart += VertDef[j];
	//						if (VertDef[j] == '/' || j == VertDef.length() - 1)
	//						{
	//							std::wistringstream wstringToInt(vertPart);

	//							if (whichPart == 0)
	//							{
	//								wstringToInt >> vertPosIndexTemp;
	//								vertPosIndexTemp -= 1;

	//								//Check to see if the vert pos was the only thing specified
	//								if (j == VertDef.length() - 1)
	//								{
	//									vertTCIndexTemp = 0;
	//									vertNormIndexTemp = 0;
	//								}
	//							}
	//							else if (whichPart == 1)
	//							{
	//								if (vertPart != L"")
	//								{
	//									wstringToInt >> vertTCIndexTemp;
	//									vertTCIndexTemp -= 1;
	//								}
	//								else
	//									vertTCIndexTemp = 0;
	//								if (j == VertDef.length() - 1)
	//									vertNormIndexTemp = 0;

	//							}
	//							else if (whichPart == 2)
	//							{
	//								std::wistringstream wstringToInt(vertPart);

	//								wstringToInt >> vertNormIndexTemp;
	//								vertNormIndexTemp -= 1;
	//							}

	//							vertPart = L"";
	//							whichPart++;
	//						}
	//					}

	//					//Check for duplicate vertices
	//					bool vertAlreadyExists = false;
	//					if (totalVerts >= 3)	//Make sure we at least have one triangle to check
	//					{
	//						for (int iCheck = 0; iCheck < totalVerts; ++iCheck)
	//						{
	//							if (vertPosIndexTemp == vertPosIndex[iCheck] && !vertAlreadyExists)
	//							{
	//								if (vertTCIndexTemp == vertTCIndex[iCheck])
	//								{
	//									indices.push_back(iCheck);			//Set index for this vertex
	//									vertAlreadyExists = true;		//If we've made it here, the vertex already exists
	//								}
	//							}
	//						}
	//					}

	//					if (!vertAlreadyExists)
	//					{
	//						vertPosIndex.push_back(vertPosIndexTemp);
	//						vertTCIndex.push_back(vertTCIndexTemp);
	//						vertNormIndex.push_back(vertNormIndexTemp);
	//						totalVerts++;					//New vertex created, add to total verts
	//						indices.push_back(totalVerts - 1);		//Set index for this vertex
	//					}

	//					//Set the second vertex for the next triangle to the last vertex we got		
	//					lastVIndex = indices[vIndex];	//The last vertex index of this TRIANGLE

	//					meshTriangles++;	//New triangle defined
	//					vIndex++;
	//				}
	//			}
	//		}
	//		break;

	//	

	//	default:
	//		break;
	//	}
	//}

	//subsetIndexStart.push_back(vIndex); //There won't be another index start after our last subset, so set it here

	//									//sometimes "g" is defined at the very top of the file, then again before the first group of faces.
	//									//This makes sure the first subset does not conatain "0" indices.
	//if (subsetIndexStart[1] == 0)
	//{
	//	subsetIndexStart.erase(subsetIndexStart.begin() + 1);
	//	meshSubsets--;
	//}

	////Make sure we have a default for the tex coord and normal
	////if one or both are not specified
	//if (!hasNorm)
	//	vertNorm.push_back(XMFLOAT3(0.0f, 0.0f, 0.0f));
	//if (!hasTexCoord)
	//	vertTexCoord.push_back(XMFLOAT2(0.0f, 0.0f));

	////Close the obj file, and open the mtl file
	//fileIn.close();
	//fileIn.open(meshMatLib.c_str());

	//std::wstring lastStringRead;
	//int matCount = material.size();	//total materials

	//								//kdset - If our diffuse color was not set, we can use the ambient color (which is usually the same)
	//								//If the diffuse color WAS set, then we don't need to set our diffuse color to ambient
	//bool kdset = false;

	//if (fileIn)
	//{
	//	while (fileIn)
	//	{
	//		checkChar = fileIn.get();	//Get next char

	//		switch (checkChar)
	//		{
	//			//Check for comment
	//		case '#':
	//			checkChar = fileIn.get();
	//			while (checkChar != '\n')
	//				checkChar = fileIn.get();
	//			break;

	//			//Set diffuse color
	//		case 'K':
	//			checkChar = fileIn.get();
	//			if (checkChar == 'd')	//Diffuse Color
	//			{
	//				checkChar = fileIn.get();	//remove space

	//				fileIn >> material[matCount - 1].difColor.x;
	//				fileIn >> material[matCount - 1].difColor.y;
	//				fileIn >> material[matCount - 1].difColor.z;

	//				kdset = true;
	//			}

	//			//Ambient Color (We'll store it in diffuse if there isn't a diffuse already)
	//			if (checkChar == 'a')
	//			{
	//				checkChar = fileIn.get();	//remove space
	//				if (!kdset)
	//				{
	//					fileIn >> material[matCount - 1].difColor.x;
	//					fileIn >> material[matCount - 1].difColor.y;
	//					fileIn >> material[matCount - 1].difColor.z;
	//				}
	//			}
	//			break;

	//			//Check for transparency
	//		case 'T':
	//			checkChar = fileIn.get();
	//			if (checkChar == 'r')
	//			{
	//				checkChar = fileIn.get();	//remove space
	//				float Transparency;
	//				fileIn >> Transparency;

	//				material[matCount - 1].difColor.w = Transparency;

	//				if (Transparency > 0.0f)
	//					material[matCount - 1].transparent = true;
	//			}
	//			break;

	//			//Some obj files specify d for transparency
	//		case 'd':
	//			checkChar = fileIn.get();
	//			if (checkChar == ' ')
	//			{
	//				float Transparency;
	//				fileIn >> Transparency;

	//				//'d' - 0 being most transparent, and 1 being opaque, opposite of Tr
	//				Transparency = 1.0f - Transparency;

	//				material[matCount - 1].difColor.w = Transparency;

	//				if (Transparency > 0.0f)
	//					material[matCount - 1].transparent = true;
	//			}
	//			break;

	//			//Get the diffuse map (texture)
	//		case 'm':
	//			checkChar = fileIn.get();
	//			if (checkChar == 'a')
	//			{
	//				checkChar = fileIn.get();
	//				if (checkChar == 'p')
	//				{
	//					checkChar = fileIn.get();
	//					if (checkChar == '_')
	//					{
	//						//map_Kd - Diffuse map
	//						checkChar = fileIn.get();
	//						if (checkChar == 'K')
	//						{
	//							checkChar = fileIn.get();
	//							if (checkChar == 'd')
	//							{
	//								std::wstring fileNamePath;

	//								fileIn.get();	//Remove whitespace between map_Kd and file

	//												//Get the file path - We read the pathname char by char since
	//												//pathnames can sometimes contain spaces, so we will read until
	//												//we find the file extension
	//								bool texFilePathEnd = false;
	//								while (!texFilePathEnd)
	//								{
	//									checkChar = fileIn.get();

	//									fileNamePath += checkChar;

	//									if (checkChar == '.')
	//									{
	//										for (int i = 0; i < 3; ++i)
	//											fileNamePath += fileIn.get();

	//										texFilePathEnd = true;
	//									}
	//								}

	//								//check if this texture has already been loaded
	//								bool alreadyLoaded = false;
	//								for (int i = 0; i < textureNameArray.size(); ++i)
	//								{
	//									if (fileNamePath == textureNameArray[i])
	//									{
	//										alreadyLoaded = true;
	//										material[matCount - 1].texArrayIndex = i;
	//										material[matCount - 1].hasTexture = true;
	//									}
	//								}

	//								//if the texture is not already loaded, load it now
	//								if (!alreadyLoaded)
	//								{
	//									ID3D11ShaderResourceView* tempMeshSRV;
	//									hr = D3DX11CreateShaderResourceViewFromFile(d3d11Device, fileNamePath.c_str(),
	//										NULL, NULL, &tempMeshSRV, NULL);
	//									if (SUCCEEDED(hr))
	//									{
	//										textureNameArray.push_back(fileNamePath.c_str());
	//										material[matCount - 1].texArrayIndex = meshSRV.size();
	//										meshSRV.push_back(tempMeshSRV);
	//										material[matCount - 1].hasTexture = true;
	//									}
	//								}
	//							}
	//						}
	//						//map_d - alpha map
	//						else if (checkChar == 'd')
	//						{
	//							//Alpha maps are usually the same as the diffuse map
	//							//So we will assume that for now by only enabling
	//							//transparency for this material, as we will already
	//							//be using the alpha channel in the diffuse map
	//							material[matCount - 1].transparent = true;
	//						}
	//					}
	//				}
	//			}
	//			break;

	//		case 'n':	//newmtl - Declare new material
	//			checkChar = fileIn.get();
	//			if (checkChar == 'e')
	//			{
	//				checkChar = fileIn.get();
	//				if (checkChar == 'w')
	//				{
	//					checkChar = fileIn.get();
	//					if (checkChar == 'm')
	//					{
	//						checkChar = fileIn.get();
	//						if (checkChar == 't')
	//						{
	//							checkChar = fileIn.get();
	//							if (checkChar == 'l')
	//							{
	//								checkChar = fileIn.get();
	//								if (checkChar == ' ')
	//								{
	//									//New material, set its defaults
	//									SurfaceMaterial tempMat;
	//									material.push_back(tempMat);
	//									fileIn >> material[matCount].matName;
	//									material[matCount].transparent = false;
	//									material[matCount].hasTexture = false;
	//									material[matCount].texArrayIndex = 0;
	//									matCount++;
	//									kdset = false;
	//								}
	//							}
	//						}
	//					}
	//				}
	//			}
	//			break;

	//		default:
	//			break;
	//		}
	//	}
	//}
	//else
	//{
	//	SwapChain->SetFullscreenState(false, NULL);	//Make sure we are out of fullscreen

	//	std::wstring message = L"Could not open: ";
	//	message += meshMatLib;

	//	MessageBox(0, message.c_str(),
	//		L"Error", MB_OK);

	//	return false;
	//}

	////Set the subsets material to the index value
	////of the its material in our material array
	//for (int i = 0; i < meshSubsets; ++i)
	//{
	//	bool hasMat = false;
	//	for (int j = 0; j < material.size(); ++j)
	//	{
	//		if (meshMaterials[i] == material[j].matName)
	//		{
	//			subsetMaterialArray.push_back(j);
	//			hasMat = true;
	//		}
	//	}
	//	if (!hasMat)
	//		subsetMaterialArray.push_back(0); //Use first material in array
	//}

	//std::vector<Vertex> vertices;
	//Vertex tempVert;

	////Create our vertices using the information we got 
	////from the file and store them in a vector
	//for (int j = 0; j < totalVerts; ++j)
	//{
	//	tempVert.pos = vertPos[vertPosIndex[j]];
	//	tempVert.normal = vertNorm[vertNormIndex[j]];
	//	tempVert.texCoord = vertTexCoord[vertTCIndex[j]];

	//	vertices.push_back(tempVert);
	//}

	////////////////////////Compute Normals///////////////////////////
	////If computeNormals was set to true then we will create our own
	////normals, if it was set to false we will use the obj files normals
	//if (computeNormals)
	//{
	//	std::vector<XMFLOAT3> tempNormal;

	//	//normalized and unnormalized normals
	//	XMFLOAT3 unnormalized = XMFLOAT3(0.0f, 0.0f, 0.0f);

	//	//Used to get vectors (sides) from the position of the verts
	//	float vecX, vecY, vecZ;

	//	//Two edges of our triangle
	//	XMVECTOR edge1 = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	//	XMVECTOR edge2 = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);

	//	//Compute face normals
	//	for (int i = 0; i < meshTriangles; ++i)
	//	{
	//		//Get the vector describing one edge of our triangle (edge 0,2)
	//		vecX = vertices[indices[(i * 3)]].pos.x - vertices[indices[(i * 3) + 2]].pos.x;
	//		vecY = vertices[indices[(i * 3)]].pos.y - vertices[indices[(i * 3) + 2]].pos.y;
	//		vecZ = vertices[indices[(i * 3)]].pos.z - vertices[indices[(i * 3) + 2]].pos.z;
	//		edge1 = XMVectorSet(vecX, vecY, vecZ, 0.0f);	//Create our first edge

	//														//Get the vector describing another edge of our triangle (edge 2,1)
	//		vecX = vertices[indices[(i * 3) + 2]].pos.x - vertices[indices[(i * 3) + 1]].pos.x;
	//		vecY = vertices[indices[(i * 3) + 2]].pos.y - vertices[indices[(i * 3) + 1]].pos.y;
	//		vecZ = vertices[indices[(i * 3) + 2]].pos.z - vertices[indices[(i * 3) + 1]].pos.z;
	//		edge2 = XMVectorSet(vecX, vecY, vecZ, 0.0f);	//Create our second edge

	//														//Cross multiply the two edge vectors to get the un-normalized face normal
	//		XMStoreFloat3(&unnormalized, XMVector3Cross(edge1, edge2));
	//		tempNormal.push_back(unnormalized);			//Save unormalized normal (for normal averaging)
	//	}

	//	//Compute vertex normals (normal Averaging)
	//	XMVECTOR normalSum = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	//	int facesUsing = 0;
	//	float tX;
	//	float tY;
	//	float tZ;

	//	//Go through each vertex
	//	for (int i = 0; i < totalVerts; ++i)
	//	{
	//		//Check which triangles use this vertex
	//		for (int j = 0; j < meshTriangles; ++j)
	//		{
	//			if (indices[j * 3] == i ||
	//				indices[(j * 3) + 1] == i ||
	//				indices[(j * 3) + 2] == i)
	//			{
	//				tX = XMVectorGetX(normalSum) + tempNormal[j].x;
	//				tY = XMVectorGetY(normalSum) + tempNormal[j].y;
	//				tZ = XMVectorGetZ(normalSum) + tempNormal[j].z;

	//				normalSum = XMVectorSet(tX, tY, tZ, 0.0f);	//If a face is using the vertex, add the unormalized face normal to the normalSum
	//				facesUsing++;
	//			}
	//		}

	//		//Get the actual normal by dividing the normalSum by the number of faces sharing the vertex
	//		normalSum = normalSum / facesUsing;

	//		//Normalize the normalSum vector
	//		normalSum = XMVector3Normalize(normalSum);

	//		//Store the normal in our current vertex
	//		vertices[i].normal.x = XMVectorGetX(normalSum);
	//		vertices[i].normal.y = XMVectorGetY(normalSum);
	//		vertices[i].normal.z = XMVectorGetZ(normalSum);

	//		//Clear normalSum and facesUsing for next vertex
	//		normalSum = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	//		facesUsing = 0;

	//	}
	//}

	////Create index buffer
	//D3D11_BUFFER_DESC indexBufferDesc;
	//ZeroMemory(&indexBufferDesc, sizeof(indexBufferDesc));

	//indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	//indexBufferDesc.ByteWidth = sizeof(DWORD) * meshTriangles * 3;
	//indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	//indexBufferDesc.CPUAccessFlags = 0;
	//indexBufferDesc.MiscFlags = 0;

	//D3D11_SUBRESOURCE_DATA iinitData;

	//iinitData.pSysMem = &indices[0];
	//hr = device->CreateBuffer(&indexBufferDesc, &iinitData, &indexBuffer);

	////Create Vertex Buffer
	//D3D11_BUFFER_DESC vertexBufferDesc;
	//ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));

	//vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	//vertexBufferDesc.ByteWidth = sizeof(Vertex) * totalVerts;
	//vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	//vertexBufferDesc.CPUAccessFlags = 0;
	//vertexBufferDesc.MiscFlags = 0;

	//D3D11_SUBRESOURCE_DATA vertexBufferData;

	//ZeroMemory(&vertexBufferData, sizeof(vertexBufferData));
	//vertexBufferData.pSysMem = &vertices[0];
	//hr = device->CreateBuffer(&vertexBufferDesc, &vertexBufferData, &vertexBuffer);
	//
	infile.close();
	return S_OK;
}