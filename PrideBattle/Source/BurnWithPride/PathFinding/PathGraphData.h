#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "PathGraphData.generated.h"

USTRUCT(BlueprintType)
struct FNeighbor
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PathGraph")
	int32 VertexIndex;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PathGraph")
	float Cost;

	FNeighbor()
		: VertexIndex(INDEX_NONE)
		, Cost(0.0f)
	{
	}

	FNeighbor(int32 InIndex, float InCost)
		: VertexIndex(InIndex)
		, Cost(InCost)
	{
	}

	static void AddNeighborIfNotExists(TArray<FVertex>& Vertices, int32 FromIndex, int32 ToIndex);
};

USTRUCT(BlueprintType)
struct FVertex
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PathGraph")
	FVector Position;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PathGraph")
	TArray<FNeighbor> Neighbors;

	bool bIsClosed;
	int32 Previous;

	FVertex()
		: Position(FVector::ZeroVector)
		, Neighbors()
		, bIsClosed(false)
		, Previous(INDEX_NONE)
	{
	}

	FVertex(const FVector& pos)
		: Position(pos)
		, Neighbors()
		, bIsClosed(false)
		, Previous(INDEX_NONE)
	{
	}
};

//	Data Asset
UCLASS(BlueprintType)
class BURNWITHPRIDE_API UPathGraphData : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PathGraph")
	TArray<FVertex> m_Vertices;
};