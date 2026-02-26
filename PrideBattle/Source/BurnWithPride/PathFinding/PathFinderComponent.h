// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PathGraphData.h"
#include "PathFinderComponent.generated.h"

/**
 * @brief	経路探索実行時の探索ノード
 *			探索ノード数分必要
 */
USTRUCT()
struct FPathNodeRuntimeData
{
	GENERATED_BODY();

	uint8 ClosedFlags;   // 0: 未、1: コスト計算済み
	uint8 InOpenFlags;   // 0: 未、1: 探索候補に追加済み
	int32 Parent;        // parent vertex index 
	float GCost;         // g cost
	float HCost;         // h cost (optional キャッシュ)
	float FCost;         // f cost (G + H)

	void Reset() {
		ClosedFlags = 0;
		InOpenFlags = 0;
		Parent = INDEX_NONE;
		GCost = FLT_MAX;
		HCost = 0.0f;
		FCost = FLT_MAX;
	}

	float CalcFCost() const{ return GCost + HCost; }
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BURNWITHPRIDE_API UPathFinderComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPathFinderComponent();

	// PathData
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pathfinding")
	UPathGraphData* m_GraphData = nullptr;

	// 探索結果
	UPROPERTY(BlueprintReadOnly, Category = "Pathfinding")
	TArray<FVector> m_ResultPath;

	// 次に返すインデックス（GetNextMove 用）
	UPROPERTY(BlueprintReadOnly, Category = "Pathfinding")
	int32 NextPathIndex = 0;


	UFUNCTION(BlueprintCallable, Category = "Pathfinding")
	bool FindPath(const FVector& StartPos, const FVector& GoalPos);


	// 次の移動先を返す（存在しなければ false）
	UFUNCTION(BlueprintCallable,BlueprintPure, Category = "Pathfinding")
	FVector GetNextMovePos();

	//	パスクリア
	UFUNCTION(BlueprintCallable, Category = "Pathfinding")
	void ClearResultPath();

	//	ゴール手前の経路かどうかをチェック
	UFUNCTION(BlueprintCallable, Category = "Pathfinding")
	bool CheckLastPath()const { return m_bLastPath; }

	//	ランダムな探索ノードの位置を取得
	UFUNCTION(BlueprintCallable, Category = "Pathfinding")
	FVector GetRandomLocation();


	//	最大探索数
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pathfinding")
	int32 MaxIterations = 7000;


	

protected:
	// ランタイム配列（探索ごとに初期化）
	TArray<uint8> ClosedFlags;   // 0: 未、1: Closed
	TArray<uint8> InOpenFlags;   // 0: 未、1: In open set
	TArray<int32> Parent;        // parent vertex index (INDEX_NONE = none)
	TArray<float> GCost;         // g cost
	TArray<float> HCost;         // h cost (optional キャッシュ)
	TArray<float> FCost;         // f cost (G + H)

	TArray<FPathNodeRuntimeData> Nodes;

	//	現在の探索候補
	//	新しいノードに到達時に、隣接ノードが未探索だったらOpenSetに追加
	TArray<int32> OpenSet;

	// 内部ユーティリティ
	int32 GetNearestVertexIndex(const FVector& Pos) const;
	float HeuristicCostByIndex(int32 AIndex, int32 BIndex) const;
	float CostBetweenIndices(int32 AIndex, int32 BIndex) const;
	int32 PopLowestF();
	void MakeResultPath(int32 GoalIndex);

private:
	bool m_bLastPath = false;
};
