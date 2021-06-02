// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"
#include "Serialization/JsonSerializerMacros.h"
#include "GameFramework/GameModeBase.h"
#include "JsonUtilities/Public/JsonDomBuilder.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "PasteJsonBPLibrary.generated.h"


UCLASS()
class UPasteJsonBPLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_UCLASS_BODY()
public:

	UFUNCTION(BlueprintCallable)
	static FString GetJsonValue(const FString& SourceStr, FString FieldPath)
	{
		TArray<FString> FieldNames;
		FieldPath.ParseIntoArray(FieldNames,TEXT("."));
		if (FieldNames.Num() <= 0) return ""; //检查是否有路径

		TSharedPtr<FJsonValue> JsonValue;
		TSharedRef<TJsonReader<TCHAR>> JsonReader = TJsonReaderFactory<TCHAR>::Create(SourceStr);
		if (!FJsonSerializer::Deserialize(JsonReader, JsonValue)) return ""; //Paste并检查是否成功

		for (FString Field : FieldNames)
		{
			Field = Field.Replace(TEXT("\""),TEXT(""));
			if (Field.IsNumeric()) //先尝试作为数组读取
			{
				int Index = FCString::Atoi(*Field);
				auto JsonArray = JsonValue->AsArray();
				if (JsonArray.IsValidIndex(Index))
				{
					JsonValue = JsonArray[Index];
					continue;
				}
			}
			JsonValue = JsonValue->AsObject()->GetField<EJson::None>(Field);
		}

		EJson type = JsonValue->Type;
		if (type == EJson::Array || type == EJson::Object)
		{
			FString JsonString;
			auto JsonWriter = TJsonWriterFactory<>::Create(&JsonString);
			FJsonSerializer::Serialize(JsonValue, "", JsonWriter);
			return JsonString;
		}
		return JsonValue->AsString();
	}

	UFUNCTION(BlueprintCallable,BlueprintPure)
	static FString MapToJson(TMap<FString, FString> Map)
	{
		FJsonDomBuilder::FObject JsonObjectBuilder;
		for (auto Item : Map)
		{
			TSharedPtr<FJsonValue> JsonValue;
			TSharedRef<TJsonReader<TCHAR>> JsonReader = TJsonReaderFactory<TCHAR>::Create(Item.Value);
			FJsonSerializer::Deserialize(JsonReader, JsonValue);
			if (JsonValue)
			{
				JsonObjectBuilder.Set(Item.Key, JsonValue);
			}
			else
			{
				JsonObjectBuilder.Set(Item.Key, Item.Value);
			}
		}
		return JsonObjectBuilder.ToString<>();
	}

	UFUNCTION(BlueprintCallable,BlueprintPure)
	static FString ArrayToJson(TArray<FString> Array)
	{
		FJsonDomBuilder::FArray JsonArrayBuilder;
		for (auto Item : Array)
		{
			TSharedPtr<FJsonValue> JsonValue;
			TSharedRef<TJsonReader<TCHAR>> JsonReader = TJsonReaderFactory<TCHAR>::Create(Item);
			FJsonSerializer::Deserialize(JsonReader, JsonValue);
			if (JsonValue)
			{
				JsonArrayBuilder.Add(JsonValue);
			}
			else
			{
				JsonArrayBuilder.Add(Item);
			}
		}
		return JsonArrayBuilder.ToString();
	}
};
