// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once


// --- Header
#include "CoreMinimal.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"


const int SLOT_MAX = 5;
const float WIDGET_RATE = 0.05f;
#define ITEMDATA_NAME "MCOItemData"


// --- Check
#define ISTRUE(Expr)   { if(!(Expr)) return; }
#define ISTRUE_F(Expr) { if(!(Expr)) return false; }
#define ISTRUE_N(Expr) { if(!(Expr)) return nullptr; }
#define ISTRUE_Z(Expr) { if(!(Expr)) return 0; }


// --- Log
DECLARE_LOG_CATEGORY_EXTERN(MCOLog, Display, All);
DECLARE_LOG_CATEGORY_EXTERN(MCOAbility, Display, All);


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
#define MCOLOG_I(Format, ...)            UE_LOG(MCOLog, Warning, TEXT("%s%s"), MCOLOG_CALLINFO, *FString::Printf(Format, ##__VA_ARGS__))
#define MCOLOG_C(Category, Format, ...)  UE_LOG(Category, Warning, TEXT("%s"), *FString::Printf(Format, ##__VA_ARGS__))


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


// --- Enum
UENUM(BlueprintType)
enum class EMCOGameState : uint8
{
	LOBBY,
	FIGHT,
	REWARD,
	RESULT
};


UENUM(BlueprintType)
enum class EMCOAbilityID : uint8
{
	None           UMETA(DisplayName = "None"),
	Confirm        UMETA(DisplayName = "Confirm"),
	Cancel         UMETA(DisplayName = "Cancel"),
      
	Idle           UMETA(DisplayName = "Idle"),
	Jump           UMETA(DisplayName = "Jump"),
	Dodge          UMETA(DisplayName = "Roll"),
	Dash           UMETA(DisplayName = "Dash"),
	Equip          UMETA(DisplayName = "Equip"),
	NormalAttack   UMETA(DisplayName = "NormalAttack"),
	Damaged        UMETA(DisplayName = "Damaged"),
	Dead           UMETA(DisplayName = "Dead"),
  
	// TO DO  
	SkillA         UMETA(DisplayName = "SkillA"),
	SkillB         UMETA(DisplayName = "SkillB")
};

UENUM(BlueprintType)
enum class EMCOMonsterAttack : uint8
{
	NormalAttack   UMETA(DisplayName = "NormalAttack"),
	SkillA         UMETA(DisplayName = "SkillA"),
	SkillB         UMETA(DisplayName = "SkillB")
};

UENUM(BlueprintType)
enum class EMCOModeType : uint8
{
	TwoHand        UMETA(DisplayName = "TwoHand"),
	Max            UMETA(DisplayName = "Max")
};

UENUM(BlueprintType)
enum class EMCOCharacterSpeed : uint8
{
	Slow           UMETA(DisplayName = "Slow"),
	Normal         UMETA(DisplayName = "Normal"),
	Fast           UMETA(DisplayName = "Fast")
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

UENUM(BlueprintType)
enum class EMCOItemType : uint8
{
	Weapon        UMETA(DisplayName = "Weapon"),
	Potion        UMETA(DisplayName = "Potion"),
	Scroll        UMETA(DisplayName = "Scroll")
};

UENUM(BlueprintType)
enum class EMCOStaminaConsumptionPolicy : uint8
{
	Infinite      UMETA(DisplayName = "Infinite"),
	Instant       UMETA(DisplayName = "Instant")
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

