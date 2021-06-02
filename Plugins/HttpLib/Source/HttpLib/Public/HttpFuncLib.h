// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "HttpModule.h"
#include "HttpFuncLib.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FHttpHeaders
{
	GENERATED_BODY()
	UPROPERTY(BlueprintReadWrite)
	TMap<FString,FString> Headers;
};

typedef TMap<FString,FString> Fhe;

DECLARE_DYNAMIC_DELEGATE_FourParams(FHhttpReturn,int, Code,bool, Successfully,UPARAM(ref)TArray<uint8>, data, FHttpHeaders,Headers);

UCLASS()
class  UHttpFuncLib : public UBlueprintFunctionLibrary
{
	
	GENERATED_BODY()

		UFUNCTION(BlueprintCallable, meta = (AutoCreateRefTerm ="Data,Params,Headers"))
		static void ExecuteHttpRequest(FString Url, FHhttpReturn HttpReturn, TMap<FString,FString> Headers, TMap<FString, FString> Params, TArray<uint8> Data, FString Verb = TEXT("POST"));
	UFUNCTION(BlueprintCallable)
		static bool loadFileToArray(TArray<uint8>& Data, FString Filename);

	UFUNCTION(BlueprintCallable)
static bool SaveArrayToFile(TArray<uint8>Array,FString Filename)
	{
		return FFileHelper::SaveArrayToFile(Array,*Filename);
	}

	UFUNCTION(BlueprintCallable, BlueprintPure)
		static FString Base64Encode(TArray<uint8>Data);

	UFUNCTION(BlueprintCallable, BlueprintPure)
		static void Base64Decode(FString SourceStr, TArray<uint8>& OutData);

	UFUNCTION(BlueprintCallable, Category = "Utilities|Map")
		static void MapKeySort(UPARAM(ref) TMap<FString, FString>& TargetMap, bool desc = false, bool UseKey = true);


	UFUNCTION(BlueprintCallable)
		static FString MD5(FString Source);

	UFUNCTION(BlueprintCallable, BlueprintPure)
		static FString DataAsString(TArray<uint8>Data);
	UFUNCTION(BlueprintCallable,BlueprintPure)
		static void GetStringData(FString Source, TArray<uint8>& Data);


	UFUNCTION(BlueprintCallable, BlueprintPure)
		static FString UrlEncode(FString Source);
	UFUNCTION(BlueprintCallable, BlueprintPure)
		static FString UrlDncode(FString Source);
};
