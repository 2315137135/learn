#pragma once
#include "ProceduralMeshComponent.h"
#include <assimp/scene.h>       // Output data structure 
#include "Kismet/BlueprintFunctionLibrary.h"
#include "LoaderBPFunctionLibrary.generated.h"

UENUM(BlueprintType)
enum class EPathType : uint8
{
	Absolute,
	Relative
};


USTRUCT(BlueprintType)
struct FimportSection
{
	GENERATED_USTRUCT_BODY()
	aiMesh* MeshPtr;
	aiNode* NodePtr;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ReturnedData")
	TArray<FVector> Vertices;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ReturnedData")
	TArray<int32> Triangles;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ReturnedData")
	TArray<FVector> Normals;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ReturnedData")
	TArray<FVector2D> UV0;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ReturnedData")
	TArray<FLinearColor> VertexColors;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ReturnedData")
	TArray<FProcMeshTangent> Tangents;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ReturnedData")
	FTransform RelativeTransform;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ReturnedData")
	FString Name;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ReturnedData")
	FLinearColor Color;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ReturnedData")
	FString MetaData;
	
};

USTRUCT(BlueprintType)
struct FReturnedData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ReturnedData")
	bool bSuccess;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ReturnedData")
	int32 NumMeshes;


	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ReturnedData")
	TArray<FimportSection> meshInfo;
};


/**
 * 
 */
UCLASS()
class ULoaderBPFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category="MeshLoader")
	static FReturnedData LoadMesh(TArray<uint8> Data);
	


	
};
