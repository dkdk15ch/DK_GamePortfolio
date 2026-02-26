// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "PathGraphDataImporter.generated.h"

UCLASS()
class UPathGraphDataImporter : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
#if WITH_EDITOR

    UFUNCTION(BlueprintCallable, Category = "PathGraph|Import", meta = (ToolTip = "Import an OBJ file and create a UPathGraphData asset at /Game/PathGraphs/<name>"))
    static void MakeDataAssetFromOBJ(const FString& ObjFilePath, const FString& DestinationFolder = TEXT("/Game/PathGraphs"));


#endif

};
