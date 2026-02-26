// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PathGraphData.h"


class BURNWITHPRIDE_API OBJParser
{
public:
	OBJParser() = delete;
	~OBJParser() = delete;

	static bool ParseOBJ(const FString& obj, TArray<FVertex>& OutputData);

};
