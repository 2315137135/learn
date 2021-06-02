#include "LoaderBPFunctionLibrary.h"


#include <assimp/Importer.hpp>  // C++ importer interface
#include <assimp/postprocess.h> // Post processing flags

// FTransform GetNodeTransform(aiNode* parent_node)
// {
// 	aiVector3t<float> s, r, p;
// 	parent_node->mTransformation.Decompose(s, r, p);
// 	return  FTransform(
// 		FRotator(r.x, r.y, r.z),
// 		FVector(p.x, p.y, p.z),
// 		FVector(s.x, s.y, s.z));
// }

FTransform GetNodeTransform(aiNode* node)
{
	FMatrix tempMatrix;
	aiMatrix4x4 tempTrans = node->mTransformation;
	tempMatrix.M[0][0] = tempTrans.a1;
	tempMatrix.M[0][1] = tempTrans.b1;
	tempMatrix.M[0][2] = tempTrans.c1;
	tempMatrix.M[0][3] = tempTrans.d1;
	tempMatrix.M[1][0] = tempTrans.a2;
	tempMatrix.M[1][1] = tempTrans.b2;
	tempMatrix.M[1][2] = tempTrans.c2;
	tempMatrix.M[1][3] = tempTrans.d2;
	tempMatrix.M[2][0] = tempTrans.a3;
	tempMatrix.M[2][1] = tempTrans.b3;
	tempMatrix.M[2][2] = tempTrans.c3;
	tempMatrix.M[2][3] = tempTrans.d3;
	tempMatrix.M[3][0] = tempTrans.a4;
	tempMatrix.M[3][1] = tempTrans.b4;
	tempMatrix.M[3][2] = tempTrans.c4;
	tempMatrix.M[3][3] = tempTrans.d4;
	return FTransform(tempMatrix);
}

void FindMeshInfo(const aiScene* scene, aiNode* node, FReturnedData& result)
{
	//轴向适应
	const TArray<FVector> AxisArray
	{
		FVector(1, 0, 0),
		FVector(0, -1, 0),
		FVector(0, 0, 1),
	};
	int UpAxis = 2;
	int FrontAxis = 1;
	int CoordAxis = 0;

	if (scene->mMetaData && scene->mMetaData->HasKey("UpAxis") && scene->mMetaData->HasKey("FrontAxis") && scene->
		mMetaData->
		HasKey("CoordAxis"))
	{
		scene->mMetaData->Get("UpAxis", UpAxis);
		scene->mMetaData->Get("FrontAxis", FrontAxis);
		scene->mMetaData->Get("CoordAxis", CoordAxis);
	}
	const FTransform AxisTransForm(AxisArray[CoordAxis], AxisArray[FrontAxis], AxisArray[UpAxis], FVector::ZeroVector);


	double v = 1.;
	if (scene->mMetaData && scene->mMetaData->HasKey("UnitScaleFactor"))
	{
		scene->mMetaData->Get("UnitScaleFactor", v);
		UE_LOG(LogTemp, Warning, TEXT("%s"), *FString::SanitizeFloat(v));
	}

	const FTransform ScaleTransform = FTransform(FRotator::ZeroRotator, FVector::ZeroVector, FVector(v));

	for (uint32 i = 0; i < node->mNumMeshes; i++)
	{
		UE_LOG(LogTemp, Warning, TEXT("FindMeshInfo. %s\n"), UTF8_TO_TCHAR(node->mName.data));
		int MeshIndex = *node->mMeshes;
		aiMesh* mesh = scene->mMeshes[MeshIndex];
		FimportSection& mi = result.meshInfo[MeshIndex];

		//meta
		aiString meta;
		if (node->mMetaData && node->mMetaData->Get("UDP3DSMAX", meta))
		{
			mi.MetaData = UTF8_TO_TCHAR(meta.data);
		}

		//transform.
		mi.RelativeTransform = FTransform(GetNodeTransform(node));
		auto parent_node = node->mParent;
		while (parent_node)
		{
			mi.RelativeTransform *= GetNodeTransform(parent_node);
			parent_node = parent_node->mParent;
		}
		mi.RelativeTransform *= AxisTransForm * ScaleTransform;

		//name
		mi.Name = UTF8_TO_TCHAR(node->mName.data);

		//color
		aiColor4D aiColor;
		aiGetMaterialColor(scene->mMaterials[mesh->mMaterialIndex],AI_MATKEY_COLOR_DIFFUSE, &aiColor);
		mi.Color = FLinearColor(aiColor.r, aiColor.g, aiColor.b, aiColor.a);

		UE_LOG(LogTemp, Warning, TEXT("----------"));

		//vet
		for (uint32 j = 0; j < mesh->mNumVertices; ++j)
		{
			FVector vertex = FVector(
				mesh->mVertices[j].x,
				mesh->mVertices[j].y,
				mesh->mVertices[j].z);

			// vertex = mi.RelativeTransform.TransformFVector4(vertex);
			mi.Vertices.Push(vertex);

			//Normal
			if (mesh->HasNormals())
			{
				FVector normal = FVector(
					mesh->mNormals[j].x,
					mesh->mNormals[j].y,
					mesh->mNormals[j].z);
				// normal = mi.RelativeTransform.TransformVector(normal);
				mi.Normals.Push(normal);
			}
			else
			{
				mi.Normals.Push(FVector::ZeroVector);
			}

			//UV Coordinates - inconsistent coordinates
			if (mesh->HasTextureCoords(0))
			{
				FVector2D uv = FVector2D(mesh->mTextureCoords[0][j].x, 1 - mesh->mTextureCoords[0][j].y);
				mi.UV0.Add(uv);
			}

			//Tangent
			if (mesh->HasTangentsAndBitangents())
			{
				FProcMeshTangent meshTangent = FProcMeshTangent(
					mesh->mTangents[j].x,
					mesh->mTangents[j].y,
					mesh->mTangents[j].z
				);
				mi.Tangents.Push(meshTangent);
			}

			//Vertex color
			if (mesh->HasVertexColors(0))
			{
				FLinearColor color = FLinearColor(
					mesh->mColors[0][j].r,
					mesh->mColors[0][j].g,
					mesh->mColors[0][j].b,
					mesh->mColors[0][j].a
				);
				mi.VertexColors.Push(color);
			}
		}
	}
}


void FindMesh(const aiScene* scene, aiNode* node, FReturnedData& retdata)
{
	FindMeshInfo(scene, node, retdata);

	for (uint32 m = 0; m < node->mNumChildren; ++m)
	{
		FindMesh(scene, node->mChildren[m], retdata);
	}
}


FReturnedData ULoaderBPFunctionLibrary::LoadMesh(TArray<uint8> Data)
{
	FReturnedData result;
	result.bSuccess = false;
	result.meshInfo.Empty();
	result.NumMeshes = 0;

	Assimp::Importer mImporter;

	const aiScene* mScenePtr = mImporter.ReadFileFromMemory(Data.GetData(), Data.Num(),
	                                                        aiProcess_Triangulate |
	                                                        aiProcess_MakeLeftHanded |
	                                                        aiProcess_CalcTangentSpace |
	                                                        aiProcess_GenSmoothNormals |
	                                                        aiProcess_OptimizeMeshes |
	                                                        aiProcess_SortByPType);

	if (mScenePtr == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("读取文件失败，请检查格式或者数据是否损坏\n"));
		return result;
	}

	if (mScenePtr->HasMeshes())
	{
		result.meshInfo.SetNum(mScenePtr->mNumMeshes, false);
		FindMesh(mScenePtr, mScenePtr->mRootNode, result);
		for (uint32 i = 0; i < mScenePtr->mNumMeshes; ++i)
		{
			//Triangle number
			for (uint32 l = 0; l < mScenePtr->mMeshes[i]->mNumFaces; ++l)
			{
				for (uint32 m = 0; m < mScenePtr->mMeshes[i]->mFaces[l].mNumIndices; ++m)
				{
					result.meshInfo[i].Triangles.Push(mScenePtr->mMeshes[i]->mFaces[l].mIndices[m]);
				}
			}
		}
		result.bSuccess = true;
	}
	return result;
}
