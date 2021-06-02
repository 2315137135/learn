// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags

#include "CoreMinimal.h"


#include "HttpModule.h"
#include "ProceduralMeshComponent.h"
#include "Engine/Classes/GameFramework/Actor.h"
#include "Interfaces/IHttpResponse.h"

#include "MeshImport.generated.h"

DECLARE_DYNAMIC_DELEGATE_TwoParams(FHttpResult, int, code, const TArray<uint8>&, data);


USTRUCT(BlueprintType)
struct FSection
{
	GENERATED_BODY()
	UPROPERTY(BlueprintReadOnly)
	TArray<FVector> Vertices;

	UPROPERTY(BlueprintReadOnly)
	TArray<int> Triangles;

	UPROPERTY(BlueprintReadOnly)
	TArray<FVector> Normals;

	UPROPERTY(BlueprintReadOnly)
	TArray<FVector2D> UV0;

	UPROPERTY(BlueprintReadOnly)
	TArray<FLinearColor> VertexColors;

	UPROPERTY(BlueprintReadOnly)
	TArray<FProcMeshTangent> Tangents;
};

UCLASS()
class ASSIMPSUPPORT_API AMeshImport : public AActor
{
	GENERATED_BODY()


public:

	UFUNCTION(BlueprintCallable)
	bool LoadMeshData(FString& ErrorTex, TArray<FSection>&  Section, TArray<uint8> buffer);

	UFUNCTION(BlueprintCallable)
	bool LoadMeshFile(FString file_name, FString& ErrorTex, TArray<FSection>&  Sections)
	{
		TArray<uint8> buffer;
		FFileHelper::LoadFileToArray(buffer, *file_name);
		return LoadMeshData(ErrorTex, Sections, buffer);
	}

	UFUNCTION(BlueprintCallable)
	bool SaveString(FString Content, FString FileName)
	{
		return FFileHelper::SaveStringToFile(Content, *FileName);
	}

	UFUNCTION(BlueprintCallable)
	void HttpRequest(FString url, FString Verb, FHttpResult result)
	{
		auto Request = FHttpModule::Get().CreateRequest();
		Request->SetVerb("GET");
		Request->SetURL(url);
		Request->OnProcessRequestComplete().BindLambda(
			[result](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
			{
				result.ExecuteIfBound(Response->GetResponseCode(), Response->GetContent());
			});
		Request->ProcessRequest();
	}

	// Sets default values for this actor's properties
	AMeshImport();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
