// Fill out your copyright notice in the Description page of Project Settings.


#include "HttpFuncLib.h"


#include "GenericPlatform/GenericPlatformHttp.h"
#include "Interfaces/IHttpResponse.h"


void UHttpFuncLib::ExecuteHttpRequest(FString Url, FHhttpReturn HttpReturn, TMap<FString, FString> Headers,
                                      TMap<FString, FString> Params, TArray<uint8> Data, FString Verb)
{
	auto Request = FHttpModule::Get().CreateRequest();
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/x-www-form-urlencoded"));

	Url += "?";
	for (auto i : Params)
	{
		Url += ("&" + i.Key + "=" + i.Value);
	}
	Request->SetURL(Url);
	Request->SetContent(Data);
	Request->SetVerb(Verb);
	for (auto i : Headers)
		Request->SetHeader(i.Key, i.Value);

	Request->OnProcessRequestComplete().BindLambda
	([HttpReturn](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
	{
		FHttpHeaders Headers;
		int Code = Response->GetResponseCode();
		auto temp =  Response->GetAllHeaders();
		for (auto head: temp)
		{
			FString left;
			FString right;
			head.Split(":",&left,&right);
			Headers.Headers.Add(left,right);
		}
		HttpReturn.ExecuteIfBound(Code, bConnectedSuccessfully, Response->GetContent(), Headers);
	});
	Request->ProcessRequest();
}

bool UHttpFuncLib::loadFileToArray(TArray<uint8>& Data, FString Filename)
{
	return FFileHelper::LoadFileToArray(Data, *Filename);
}

FString UHttpFuncLib::Base64Encode(TArray<uint8> Data)
{
	return FBase64::Encode(Data);
}

void UHttpFuncLib::Base64Decode(FString SourceStr, TArray<uint8>& OutData)
{
	FBase64::Decode(SourceStr, OutData);
}

void UHttpFuncLib::MapKeySort(UPARAM(ref)TMap<FString, FString>& TargetMap, bool desc, bool UseKey)
{
	if (UseKey)
	{
		TargetMap.KeySort([desc](FString A, FString B) { return desc ? A > B : A < B; });
	}
	else
	{
		TargetMap.ValueSort([desc](FString A, FString B) { return desc ? A > B : A < B; });
	}
}

FString UHttpFuncLib::MD5(FString Source)
{
	return FMD5::HashAnsiString(*Source);
}

FString UHttpFuncLib::DataAsString(TArray<uint8> Data)
{
	Data.Add(0);
	return UTF8_TO_TCHAR(Data.GetData());
}

void UHttpFuncLib::GetStringData(FString Source, TArray<uint8>& Data)
{
	FTCHARToUTF8 s = FTCHARToUTF8(*Source);
	Data.Append((uint8*)s.Get(), s.Length() + 1);
	UE_LOG(LogTemp, Warning, TEXT("%i"), s.Length());
}

FString UHttpFuncLib::UrlEncode(FString Source)
{
	return FGenericPlatformHttp::UrlEncode(Source);
}

FString UHttpFuncLib::UrlDncode(FString Source)
{
	return FGenericPlatformHttp::UrlDecode(Source);
}
