// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "DustCounter.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class BURNWITHPRIDE_API UDustCounter : public UObject
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "DustCounter")
	float GetDustCount()const { return m_DustCount; }
	UFUNCTION(BlueprintCallable, Category = "DustCounter")
	void Add();
	UFUNCTION(BlueprintCallable, Category = "DustCounter")
	void Reset() { m_DustCount = 0.0f; }
private:
	float m_DustCount = 0.0f;
};
