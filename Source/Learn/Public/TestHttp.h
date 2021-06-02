// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "HttpModule.h"
#include "GameFramework/Actor.h"
#include "Interfaces/IHttpResponse.h"

#include "TestHttp.generated.h"

DECLARE_DYNAMIC_DELEGATE_OneParam(FMyHttpRespone, FString, Text);

UCLASS()
class LEARN_API ATestHttp : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ATestHttp();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void HttpGet(FString Url, FMyHttpRespone MyRespone)
	{
		auto RequestRef = FHttpModule::Get().CreateRequest();
		RequestRef->SetURL(Url);
		RequestRef->SetVerb("GET");
		RequestRef->OnProcessRequestComplete().BindLambda(
			[MyRespone](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
			{
				MyRespone.ExecuteIfBound(FString::FromInt(Response->GetResponseCode()));
			}
		);
		RequestRef->ProcessRequest();
	}
};
