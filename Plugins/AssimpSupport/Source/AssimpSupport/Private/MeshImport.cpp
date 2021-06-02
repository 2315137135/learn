// Fill out your copyright notice in the Description page of Project Settings.


#include "MeshImport.h"


bool AMeshImport::LoadMeshData(FString& ErrorTex, TArray<FSection>& Sections, TArray<uint8> buffer)
{
	Assimp::Importer Importer;

	const aiScene* Scene = Importer.ReadFileFromMemory(
		buffer.GetData(),
		buffer.Num(),
		aiProcess_MakeLeftHanded |
		aiProcess_CalcTangentSpace |
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices |
		aiProcess_GenSmoothNormals |
		// aiProcess_SortByPType|
		0
	);
	
	// If the import failed, report it
	if (!Scene)
	{
		ErrorTex = UTF8_TO_TCHAR(Importer.GetErrorString());
		return false;
	}

	
	
	for (uint32 m = 0; m < Scene->mNumMeshes; m++)
	{
		Sections.Add(FSection());
		
		FSection& Section = Sections[m];
		aiMesh* mesh = Scene->mMeshes[m];


		
		

		
		
		for (uint32 i = 0; i < mesh->mNumVertices; i++)
		{
			Section.Vertices.Add(FVector
                (
                    mesh->mVertices[i].x,
                    mesh->mVertices[i].y,
                    mesh->mVertices[i].z
                ));
			//Normals
			if (mesh->HasNormals())
			{
				Section.Normals.Add(FVector
                    (
                        mesh->mNormals[i].x,
                        mesh->mNormals[i].y,
                        mesh->mNormals[i].z
                    ));
			}
			else
			{
				Section.Normals.Add(FVector::ZeroVector);
			}
			//VertexColors
			if (mesh->HasVertexColors(0))
			{
				Section.VertexColors.Add(FLinearColor
                    (
                        mesh->mColors[i]->r,
                        mesh->mColors[i]->g,
                        mesh->mColors[i]->b,
                        mesh->mColors[i]->a
                    )
                );
			}
			//UV
			if (mesh->HasTextureCoords(0))
			{
				Section.UV0.Add(FVector2D
                    (
                        mesh->mTextureCoords[0][i].x,
                        1 - mesh->mTextureCoords[0][i].y
                    ));
			}
			//tangents
			if (mesh->HasTangentsAndBitangents())
			{
				Section.Tangents.Add(FProcMeshTangent
                    (
                        mesh->mTangents->x,
                        mesh->mTangents->y,
                        mesh->mTangents->z
                    ));
			}
		}

		for (uint32 i = 0; i < mesh->mNumFaces; i++)
		{
			for (uint32 j = 0; j < mesh->mFaces[i].mNumIndices; j++)
			{
				Section.Triangles.Add(mesh->mFaces[i].mIndices[j]);
			}
		}
	}

	return true;
}


// Sets default values
AMeshImport::AMeshImport()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AMeshImport::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMeshImport::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

