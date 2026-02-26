
#include "PathGraphImporterTester.h"
#include "CoreMinimal.h"
#include "PathGraphDataImporter.h"
#include "Engine/Engine.h"

APathGraphImporterTester::APathGraphImporterTester() {}

#if WITH_EDITOR
void APathGraphImporterTester::NewRunImport()
{
    if (ObjFilePath.IsEmpty())
    {
        UE_LOG(LogTemp, Warning, TEXT("ObjFilePath empty"));
        return;
    }
    UPathGraphDataImporter::MakeDataAssetFromOBJ(ObjFilePath, DestinationFolder);
}

#endif
