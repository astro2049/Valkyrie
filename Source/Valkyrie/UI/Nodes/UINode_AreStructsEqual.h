// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/KismetSystemLibrary.h"
#include "UINode_AreStructsEqual.generated.h"

UCLASS()
class VALKYRIE_API UUINode_AreStructsEqual : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(
		BlueprintPure,
		CustomThunk,
		Category="Valkyrie",
		meta=(
			DisplayName="Equal (Struct)",
			CompactNodeTitle="==",
			CustomStructureParam="aLeft,aRight"
		)
	)
	static bool AreStructsEqual(
		const FGenericStruct& aLeft,
		const FGenericStruct& aRight
	);

	DECLARE_FUNCTION(execAreStructsEqual);
};
