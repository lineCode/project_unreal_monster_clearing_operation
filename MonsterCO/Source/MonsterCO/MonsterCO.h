// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once


// --- Header
#include "CoreMinimal.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Engine/Private/KismetTraceUtils.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"


// How to decide this number...?
const float LARGE_DAMAGE_AMOUNT = 30.0f;
const float MAX_INITIAL_ATTRIBUTE_VALUE = 100.0f;
const int32 SLOT_MAX = 5;
const float WIDGET_RATE = 0.05f;
const int32 NEXT_STAGE_TIME = 5;
const int32 MAX_STAGE = 2;

#define ITEMDATA_NAME "MCOItemData"


// --- Check
#define ISTRUE(Expr)   { if(!(Expr)) return; }
#define ISTRUE_F(Expr) { if(!(Expr)) return false; }
#define ISTRUE_N(Expr) { if(!(Expr)) return nullptr; }
#define ISTRUE_Z(Expr) { if(!(Expr)) return 0; }


// --- Log
DECLARE_LOG_CATEGORY_EXTERN(MCOLog, Display, All);
DECLARE_LOG_CATEGORY_EXTERN(MCOAbility, Display, All);
DECLARE_LOG_CATEGORY_EXTERN(MCOCollision, Display, All);
DECLARE_LOG_CATEGORY_EXTERN(MCOTimer, Display, All);
DECLARE_LOG_CATEGORY_EXTERN(MCOMonsterAI, Display, All);


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
	NONE,
	
	TITLE,
	
	RESTART_STAGE_AFTER_LOSE,
	MOVE_TO_NEXT_STAGE,
	FIGHT,
	MONSTER_DIED,
	REWARD,
	
	NEXT,
	
	RESULT_WIN,
	RESULT_LOSE,
};

UENUM(BlueprintType)
enum class EMCOWidgetState : uint8
{
	NONE,	
	TITLE,
	INGAME,
	NEXT,
	RESULT,
	OPTION
};

UENUM(BlueprintType)
enum class EMCOMonsterMode : uint8
{
	Normal         UMETA(DisplayName = "Normal"),
	Max            UMETA(DisplayName = "Max")
};

UENUM(BlueprintType)
enum class EMCOPlayerMode : uint8
{
	TwoHand        UMETA(DisplayName = "TwoHand"),
	Bow            UMETA(DisplayName = "Bow"),
	Max            UMETA(DisplayName = "Max")
};

UENUM(BlueprintType)
enum class EMCOMonsterFlyMode : uint8
{
	Down = 0       UMETA(DisplayName = "Down"),
	Hold           UMETA(DisplayName = "Hold"),
	Up             UMETA(DisplayName = "Up"),
	None           UMETA(DisplayName = "None")
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
	Max
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
enum class EMCOEffectPolicy : uint8
{
	None,
	Instant        UMETA(DisplayName = "Instant"),
	Duration       UMETA(DisplayName = "Duration"),
	Infinite       UMETA(DisplayName = "Infinite"),
	Max            UMETA(Hidden)
};
ENUM_RANGE_BY_FIRST_AND_LAST(EMCOEffectPolicy, EMCOEffectPolicy::Instant, EMCOEffectPolicy::Infinite)


UENUM(BlueprintType)
enum class EMCONiagaraEffectType : uint8
{
	None           UMETA(DisplayName = "None"),
	Melee          UMETA(DisplayName = "Melee"),
	Flame          UMETA(DisplayName = "Flame")
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

