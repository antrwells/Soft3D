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

	for (int m = 0; m < scene->mNumMaterials; m++) {

		auto mat = scene->mMaterials[m];


	}

	for (int m = 0; m < scene->mNumMeshes; m++) {

		auto amesh = scene->mMeshes[m];

		auto mesh = new mesh3D;

		ent->AddMesh(mesh);

		auto mat = scene->mMaterials[amesh->mMaterialIndex];

		//mesh->setColor(mat->Get)


		aiColor3D diffuseColor(0.f, 0.f, 0.f);
		aiColor3D specularColor(1.f, 1.f, 1.f);
		aiColor3D ambientColor(0.f, 0.f, 0.f);

		mat->Get(AI_MATKEY_COLOR_DIFFUSE, diffuseColor);

		mesh->setColor(color(diffuseColor.r, diffuseColor.g, diffuseColor.b));

//		mat->Get(AI_MATKEY_COLOR_DIFFUSE, diffuseColor);


		int a = 5;

		for (int index = 0; index < amesh->mNumVertices; index++) {

			auto pos = amesh->mVertices[index];
			auto norm = amesh->mNormals[index];
			auto tex = amesh->mTextureCoords[0][index];
//			auto col = amesh->mColors[0][index];

			vertex v;
			//v.color = v3d(col.r, col.g, col.b);
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