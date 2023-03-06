#include "Importer.h"
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "nodeEntity.h"
#include "mesh3D.h"
#include "SoftTypes.h"

nodeEntity* Importer::importEntity(std::string path) {

	nodeEntity* ent = new nodeEntity;



	auto scene = aiImportFile(path.c_str(), aiProcess_OptimizeGraph | aiProcess_CalcTangentSpace | aiProcess_Triangulate | aiProcess_ValidateDataStructure);

	if (scene == nullptr)
	{
		printf("Failed to open 3d model.\n");
		exit(1);

	}
	else {

		printf("Opened 3d model successfully.\n");
	}

	for (int m = 0; m < scene->mNumMeshes; m++) {

		auto amesh = scene->mMeshes[m];

		auto mesh = new mesh3D;

		ent->AddMesh(mesh);

		for (int index = 0; index < amesh->mNumVertices; index++) {

			auto pos = amesh->mVertices[index];
			auto norm = amesh->mNormals[index];
			auto tex = amesh->mTextureCoords[0][index];

			vertex v;
			v.pos = v3d(pos.x, pos.y, pos.z);
			v.normal = v3d(norm.x, norm.y, norm.z);
			v.tex_coord = v3d(tex.x, tex.y, tex.z);

			mesh->AddVertex(v);



		}

		for (int tri = 0; tri < amesh->mNumFaces; tri++) {

			auto face = amesh->mFaces[tri];

			if (face.mNumIndices == 3) {

				triangle tri;

				tri.v0 = face.mIndices[0];
				tri.v1 = face.mIndices[1];
				tri.v2 = face.mIndices[2];

				mesh->AddTriangle(tri);

			}

		}

	}

	return ent;

}