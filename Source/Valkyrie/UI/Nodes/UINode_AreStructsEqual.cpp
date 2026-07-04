// Fill out your copyright notice in the Description page of Project Settings.

#include "UINode_AreStructsEqual.h"

#include "UObject/UnrealType.h"

bool UUINode_AreStructsEqual::AreStructsEqual(
	const FGenericStruct&,
	const FGenericStruct&
)
{
	checkNoEntry();
	return false;
}

DEFINE_FUNCTION(UUINode_AreStructsEqual::execAreStructsEqual)
{
	Stack.StepCompiledIn<FStructProperty>(nullptr);
	const FStructProperty* const leftProperty = CastField<FStructProperty>(Stack.MostRecentProperty);
	const void* const leftData = Stack.MostRecentPropertyAddress;

	Stack.StepCompiledIn<FStructProperty>(nullptr);
	const FStructProperty* const rightProperty = CastField<FStructProperty>(Stack.MostRecentProperty);
	const void* const rightData = Stack.MostRecentPropertyAddress;

	P_FINISH;
	P_NATIVE_BEGIN;

		const bool areEqual = leftProperty &&
			rightProperty &&
			leftData &&
			rightData &&
			leftProperty->Struct == rightProperty->Struct &&
			leftProperty->Struct->CompareScriptStruct(leftData, rightData, PPF_None);
		*static_cast<bool*>(RESULT_PARAM) = areEqual;

	P_NATIVE_END;
}
