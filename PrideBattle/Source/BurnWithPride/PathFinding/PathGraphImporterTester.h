// ...existing code...
#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PathGraphImporterTester.generated.h"

UCLASS()
class BURNWITHPRIDE_API APathGraphImporterTester : public AActor
{
    GENERATED_BODY()
public:
    APathGraphImporterTester();

#if WITH_EDITORONLY_DATA
    UPROPERTY(EditAnywhere, Category = "Import")
    FString ObjFilePath;

    UPROPERTY(EditAnywhere, Category = "Import")
    FString DestinationFolder = TEXT("/Game/Test");
#endif

#if WITH_EDITOR
    UFUNCTION(CallInEditor, Category = "Import")
    void NewRunImport();

#endif
};
