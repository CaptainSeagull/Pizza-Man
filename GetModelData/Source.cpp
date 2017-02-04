#include "Header.h"
#include "assimp\Importer.hpp"
#include "assimp\postprocess.h"
#include "assimp\scene.h"
#include <string>
#include <iostream>

namespace ModelData
{
	void GetModelData(const char* file, float &sizeX, float &sizeY, float &sizeZ, float &positionX, float &positionY, float &positionZ)
	{
		Assimp::Importer *importer = new Assimp::Importer();
		const aiScene *scene = importer->ReadFile(file,
			aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_GenUVCoords | aiProcess_PreTransformVertices);

		int numMeshes = scene->mNumMeshes;

		float minx, miny, minz, maxx, maxy, maxz;
		bool firstDone = false;

		for (unsigned int meshIndex = 0; meshIndex < scene->mNumMeshes; meshIndex++)
		{
			//std::vector<GLuint> indices;
			const aiMesh* currentMesh = scene->mMeshes[meshIndex];


			for (unsigned int i = 0; i < currentMesh->mNumVertices; i++)
			{
				aiVector3D vertex = currentMesh->mVertices[i];
				if (firstDone)
				{
					if (vertex.x < minx) minx = vertex.x;
					else if (vertex.x > maxx) maxx = vertex.x;

					if (vertex.y < miny) miny = vertex.y;
					else if (vertex.y > maxy) maxy = vertex.y;

					if (vertex.z < minz) minz = vertex.z;
					else if (vertex.z > maxz) maxz = vertex.z;
				}
				else
				{
					minx = vertex.x; miny = vertex.y; minz = vertex.z;
					maxx = vertex.x; maxy = vertex.y; maxz = vertex.z;
					firstDone = true;
				}
			}
		}

		sizeX = maxx - minx; sizeY = maxy - miny; sizeZ = maxz - minz;
		positionX = minx + sizeX / 2.0f; positionY = miny + sizeY / 2.0f; positionZ = minz + sizeZ / 2.0f;
	}
}