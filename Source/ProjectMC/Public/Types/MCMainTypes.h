#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "MCMainTypes.generated.h"

class UTexture2D;
class USkeletalMesh;
class UAnimInstance;
class UMCGameplayAbilitySet;
class UAttributeSet;

UENUM(BlueprintType)
enum EPlayerState
{
	Ready,
	NotReady,
	Loading,
	Waiting
};

UENUM(BlueprintType)
enum class EClassType : uint8
{
	Support,
	Tank,
	Damage
};

UENUM(BlueprintType)
enum class EClassRange : uint8
{
	Long,
	Mid,
	Melee
};

USTRUCT(BlueprintType)
struct FCharacterData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText ClassName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* ClassAvatar;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USkeletalMesh* CharacterMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UAnimInstance> AnimBP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EClassType ClassType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EClassRange Range;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMCGameplayAbilitySet* AbilitySet;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UAttributeSet> AttributeSet;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UDataTable* AttributeTable;
};