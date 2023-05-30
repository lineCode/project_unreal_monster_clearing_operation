// Copyright Epic Games, Inc. All Rights Reserved.

#include "MonsterCO.h"
#include "Modules/ModuleManager.h"

IMPLEMENT_PRIMARY_GAME_MODULE( FDefaultGameModuleImpl, MonsterCO, "MonsterCO" );
 
DEFINE_LOG_CATEGORY(MCOLog);

void FLog::Print(int32 InValue, int32 InKey, float Duration, FColor InColor)
{
	GEngine->AddOnScreenDebugMessage(InKey, Duration, InColor, FString::FromInt(InValue));
}

void FLog::Print(float& InValue, int32 InKey, float Duration, FColor InColor)
{
	GEngine->AddOnScreenDebugMessage(InKey, Duration, InColor, FString::SanitizeFloat(InValue));
}

void FLog::Print(const FString& InValue, int32 InKey, float Duration, FColor InColor)
{
	GEngine->AddOnScreenDebugMessage(InKey, Duration, InColor, InValue);
}

void FLog::Print(const FVector& InValue, int32 InKey, float Duration, FColor InColor)
{
	GEngine->AddOnScreenDebugMessage(InKey, Duration, InColor, InValue.ToString());
}

void FLog::Print(const FRotator& InValue, int32 InKey, float Duration, FColor InColor)
{
	GEngine->AddOnScreenDebugMessage(InKey, Duration, InColor, InValue.ToString());
}

void FLog::Log(const UObject* InObject)
{
	FString str;
	if (InObject != nullptr)
	{
		str.Append(InObject->GetName());
	}
	str.Append((InObject != nullptr) ? " is Not Null" : " is Null");

	UE_LOG(MCOLog, Warning, TEXT("%s"), *str);
}

