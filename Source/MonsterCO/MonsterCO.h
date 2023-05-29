// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once


// --- Header
#include "CoreMinimal.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"


// --- Check
#define ISTRUE(Expr)   { if(!(Expr)) return; }
#define ISTRUE_F(Expr) { if(!(Expr)) return false; }
#define ISTRUE_N(Expr) { if(!(Expr)) return nullptr; }


// --- Log
DECLARE_LOG_CATEGORY_EXTERN(MCOLog, Display, All);

class MONSTERCO_API FLog
{
public:
	static void Print(int32 InValue, int32 InKey = -1, float Duration = 5.0f, FColor InColor = FColor::Yellow);
	static void Print(float& InValue, int32 InKey = -1, float Duration = 5.0f, FColor InColor = FColor::Yellow);
	static void Print(const FString& InValue, int32 InKey = -1, float Duration = 5.0f, FColor InColor = FColor::Yellow);
	static void Print(const FVector& InValue, int32 InKey = -1, float Duration = 5.0f, FColor InColor = FColor::Yellow);
	static void Print(const FRotator& InValue, int32 InKey = -1, float Duration = 5.0f, FColor InColor = FColor::Yellow);

	static void Log(const UObject* InObject);
};

#define MCOPRINT(Format, ...)            FLog::Print(FString::Printf(Format, ##__VA_ARGS__))
#define MCOLOG_CALLINFO                  (FString(__FUNCTION__) + TEXT("(") + FString::FromInt(__LINE__) + TEXT(")"))
#define MCOLOG(Format, ...)              UE_LOG(MCOLog, Warning, TEXT("%s"), *FString::Printf(Format, ##__VA_ARGS__))
#define MCOLOG_V(Verbosity, Format, ...) UE_LOG(MCOLog, Verbosity, TEXT("%s"), *FString::Printf(Format, ##__VA_ARGS__))
#define MCOLOG_C(Format, ...)            UE_LOG(MCOLog, Warning, TEXT("%s%s"), MCOLOG_CALLINFO, *FString::Printf(Format, ##__VA_ARGS__))


// -- ConstructorHelpers
#define GETASSET(OutResult, ClassType, Path)\
static ConstructorHelpers::FObjectFinder<ClassType> __##OutResult(Path);\
if (__##OutResult.Succeeded())\
{OutResult = __##OutResult.Object;}\
else {OutResult = nullptr;}\
ensure(OutResult);
 
#define GETCLASS(OutResult, ClassType, Path)\
static ConstructorHelpers::FClassFinder<ClassType> __##OutResult(Path);\
if (__##OutResult.Succeeded())\
{OutResult = __##OutResult.Class;}\
else {OutResult = nullptr;}\
ensure(OutResult);

#define SETASSET(ClassType, Path) [this]()-> ClassType* {\
static ConstructorHelpers::FObjectFinder<ClassType> __##ClassType(Path);\
MCOCHECK(__##ClassType);\
if (__##ClassType.Succeeded())\
{return __##ClassType.Object;}\
else {return nullptr;}\
}();


// --- Enum

UENUM(BlueprintType)
enum class EMCOAbilityID : uint8
{
	None         UMETA(DisplayName = "None"),
	Confirm      UMETA(DisplayName = "Confirm"),
	Cancel       UMETA(DisplayName = "Cancel"),
    
	Jump         UMETA(DisplayName = "Jump"),
	Dodge        UMETA(DisplayName = "Roll"),
	Dash         UMETA(DisplayName = "Dash"),
	Equip        UMETA(DisplayName = "Equip"),
	NormalAttack UMETA(DisplayName = "NormalAttack"),
	Damaged      UMETA(DisplayName = "Damaged"),
	Dead         UMETA(DisplayName = "Dead"),

	// TO DO
	SkillA       UMETA(DisplayName = "SkillA"),
	SkillB       UMETA(DisplayName = "SkillB")
};

UENUM(BlueprintType)
enum class EMCOModeType : uint8
{
	TwoHand      UMETA(DisplayName = "TwoHand"),
	Max          UMETA(DisplayName = "Max")
};

UENUM(BlueprintType)
enum class EMCOCharacterSpeed : uint8
{
	Slow         UMETA(DisplayName = "Slow"),
	Normal       UMETA(DisplayName = "Normal"),
	Fast         UMETA(DisplayName = "Fast")
};

UENUM(BlueprintType)
enum class EMCOCharacterDirection : uint8
{
	Front          UMETA(DisplayName = "Front"),
	Front_Left     UMETA(DisplayName = "Front_Left"),
	Left           UMETA(DisplayName = "Left"),
	Back_Left      UMETA(DisplayName = "Back_Left"),
	Back           UMETA(DisplayName = "Back"),
	Back_Right     UMETA(DisplayName = "Back_Right"),
	Right          UMETA(DisplayName = "Right"),
	Front_Right    UMETA(DisplayName = "Front_Right"),
	Max,
	None
};

UENUM(BlueprintType)
enum class EMCOCharacterDirectionOption : uint8
{
	FrontBack      UMETA(DisplayName = "FrontBack"),
	LeftRight      UMETA(DisplayName = "RightLeft"),
	FourSide       UMETA(DisplayName = "FourSide"),
	SixSide        UMETA(DisplayName = "SixSide"),
	EightSide      UMETA(DisplayName = "EightSide"),
};
	
UENUM(BlueprintType)
enum class EMCOAbilityActivationPolicy : uint8
{
	// Try to activate the ability when the input is triggered.
	OnInputTriggered,

	// Continually try to activate the ability while the input is active.
	WhileInputActive,

	// Try to activate the ability when an avatar is assigned.
	OnSpawn
};


// --- Helper
class MONSTERCO_API FHelper
{
public:
	static FString GetEnumDisplayName(const FString& InEnumName, const int64 InEnumValue)
	{
		const UEnum* EnumType = FindObject<UEnum>(nullptr, *FString::Printf(TEXT("/Script/MonsterCO.%s"), *InEnumName), true);

		FString MetaData;
		if (EnumType)
		{
			MetaData = EnumType->GetDisplayNameTextByValue(InEnumValue).ToString();
		}

		return MetaData;
	}
};

