  // Fill out your copyright notice in the Description page of Project Settings.


#include "PathFinderComponent.h"

UPathFinderComponent::UPathFinderComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

bool UPathFinderComponent::FindPath(const FVector& StartPos, const FVector& GoalPos)
{
	if (!m_GraphData)
	{
		UE_LOG(LogTemp, Warning, TEXT("UPathFinderComponent::FindPath__Failde GraphData is null"));
		return false;
	}
	
	//	GraphDataの頂点分の配列を確保して、初期化
	const int32 NodeNum = m_GraphData->m_Vertices.Num();
	if (NodeNum == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("UPathFinderComponent::FindPath__Failde no vertices"));
		return false;
	}
	Nodes.SetNum(NodeNum);
	for (FPathNodeRuntimeData& node : Nodes)
	{
		node.Reset();
	}
	OpenSet.Empty();
	m_ResultPath.Empty();
	NextPathIndex = 0;
	m_bLastPath = false;

	//	Start, Goal の近隣ノードを探す
	int32 StartIndex = GetNearestVertexIndex(StartPos);
	int32 GoalIndex = GetNearestVertexIndex(GoalPos);

	// 初期化
	Nodes[StartIndex].HCost			= HeuristicCostByIndex(StartIndex, GoalIndex);
	Nodes[StartIndex].GCost			= 0.0f;
	Nodes[StartIndex].FCost			= Nodes[StartIndex].CalcFCost();
	Nodes[StartIndex].InOpenFlags	= true;
	OpenSet.Add(StartIndex);

	int32 Iter = 0;
	while (OpenSet.Num() > 0 && Iter++ < MaxIterations)
	{
		int32 Current = PopLowestF();
		if (Current == INDEX_NONE) break;

		//	Goalチェック
		if (Current == GoalIndex)
		{		
			//	Goal時に、最終的な経路を作成
			MakeResultPath(GoalIndex);

			return true;
		}

		Nodes[Current].ClosedFlags = true;

		//	隣接ノードを走査
		const FVertex& CurrentVert = m_GraphData->m_Vertices[Current];
		for (const FNeighbor& neigh : CurrentVert.Neighbors)
		{
			int32 NeighIndex = neigh.VertexIndex;
			if (NeighIndex == INDEX_NONE )			continue;
			if (!Nodes.IsValidIndex(NeighIndex))	continue;
			if (Nodes[NeighIndex].ClosedFlags)		continue;

			//	合計移動コスト
			float TotalG = Nodes[Current].GCost + neigh.Cost;

			UE_LOG(LogTemp, Log, TEXT("UPathFinderComponent::FindPath__Check Neighbor=%3d from Current=%3d: PrevG=%.2f NewG=%.2f F=%.2f [Parent=%d]"),
				NeighIndex,
				Current,
				Nodes[NeighIndex].GCost,
				TotalG,
				Nodes[NeighIndex].FCost,
				Nodes[NeighIndex].Parent);

			//	初到達のノード or 前回よりも低コスト　の場合　
			//	実行時の探索ノードを更新
			if (TotalG >= Nodes[NeighIndex].GCost) continue;

			UE_LOG(LogTemp, Log, TEXT("UPathFinderComponent::FindPath__Update Neighbor=%3d Node: Set Parent=%3d G=%.2f H=%.2f F=%.2f"),
				NeighIndex,
				Current,
				TotalG,
				HeuristicCostByIndex(NeighIndex, GoalIndex),
				TotalG + HeuristicCostByIndex(NeighIndex, GoalIndex));
			
			Nodes[NeighIndex].Parent	= Current;
			Nodes[NeighIndex].GCost		= TotalG;
			Nodes[NeighIndex].HCost		= HeuristicCostByIndex(NeighIndex, GoalIndex);
			Nodes[NeighIndex].FCost		= Nodes[NeighIndex].CalcFCost();
			if (!Nodes[NeighIndex].InOpenFlags)
			{
				OpenSet.Add(NeighIndex);
				Nodes[NeighIndex].InOpenFlags = true;
			}
		}
	}

	// 探索失敗
	UE_LOG(LogTemp, Warning, TEXT("UPathFinderComponent::FindPath__path not found (iter=%d)"), Iter);
	return false;

}

//	次の移動場所を取得
FVector UPathFinderComponent::GetNextMovePos()
{
	if (m_ResultPath.Num()==0)
	{
		UE_LOG(LogTemp, Warning, TEXT("UPathFinderComponent::GetNextMove__Failde m_ResultPath = 0"));
		return FVector(0,0,0);
	}
	if (NextPathIndex >= m_ResultPath.Num())
	{
		UE_LOG(LogTemp, Warning, TEXT("UPathFinderComponent::GetNextMove__Failde NextPathIndex >= m_ResultPath"));
		return FVector(0,0,0);
	}

	FVector NextPos = m_ResultPath[NextPathIndex++];
	if (NextPathIndex >= m_ResultPath.Num())
	{
		m_bLastPath = true;
	}
	
	return NextPos;
}

/**
 * @brief	Posから最も近いノードを検索して、インデックスで返す
 */
int32 UPathFinderComponent::GetNearestVertexIndex(const FVector& Pos) const
{
	if (!m_GraphData)
	{
		UE_LOG(LogTemp, Warning, TEXT("UPathFinderComponent::GetNearestVertexIndex__Failde"));
		return INDEX_NONE;
	}

	int32 Best = INDEX_NONE;
	float BestDist2 = FLT_MAX;
	const int32 N = m_GraphData->m_Vertices.Num();
	for (int32 i = 0; i < N; ++i)
	{
		float d2 = FVector::DistSquared(Pos, m_GraphData->m_Vertices[i].Position);
		if (d2 < BestDist2)
		{
			BestDist2 = d2;
			Best = i;
		}
	}
	return Best;
}

/**
 * @brief			引数で与えたノード間のヒューリスティック(直線)距離を取得
 * @param AIndex	スタートのインデックス
 * @param BIndex	ゴールのインデックス(最終的なゴール)
 * @return			hコスト 
 */
float UPathFinderComponent::HeuristicCostByIndex(int32 AIndex, int32 BIndex) const
{
	if (!m_GraphData) return 0.f;
	return FVector::Dist(m_GraphData->m_Vertices[AIndex].Position, m_GraphData->m_Vertices[BIndex].Position);
}

/**
 * @brief			引数で与えたノード間の移動コストを計算
 * @param AIndex	スタートのインデックス	
 * @param BIndex	ゴールのインデックス
 * @return			gコスト
 */
float UPathFinderComponent::CostBetweenIndices(int32 AIndex, int32 BIndex) const
{
	//	GraphDataのNeighborsにコストを保持しているので、それを使用する。
	if (!m_GraphData) return FLT_MAX;
	const FVertex& V = m_GraphData->m_Vertices[AIndex];
	for (const FNeighbor& nb : V.Neighbors)
	{
		if (nb.VertexIndex == BIndex)
		{
			return nb.Cost;
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("UPathFinderComponent::CostBetweenIndices__Failde"));
	return FLT_MAX;
}


/**
 * @brief	OpenSetの中で、Fコストが最も小さいノードを取り出す
 * @return 
 */
int32 UPathFinderComponent::PopLowestF()
{
	//　最小コストを比較して取り出し
	int32 BestIdxInOpen = INDEX_NONE;
	float BestF = FLT_MAX;
	for (int32 i = 0; i < OpenSet.Num(); ++i)
	{
		int32 v = OpenSet[i];

		if (v < 0 || v >= Nodes.Num()) continue;

		UE_LOG(LogTemp, Log, TEXT("UPathFinderComponent::PopLowestF__Node=%3d F=%.2f G=%.2f H=%.2f"), v, Nodes[v].FCost, Nodes[v].GCost, Nodes[v].HCost);

		if (Nodes[v].FCost < BestF)
		{
			BestF = Nodes[v].FCost;
			BestIdxInOpen = i;
		}
	}

	if (BestIdxInOpen == INDEX_NONE)
	{
		UE_LOG(LogTemp, Warning, TEXT("UPathFinderComponent::PopLowestF__Failde"));
		return INDEX_NONE;
	}
	
	int32 NodeIndex = OpenSet[BestIdxInOpen];
	UE_LOG(LogTemp, Log, TEXT("UPathFinderComponent::PopLowestF__PopNode=%3d F=%.2f G=%.2f H=%.2f OpenSet.Num=%d"), NodeIndex, Nodes[NodeIndex].FCost, Nodes[NodeIndex].GCost, Nodes[NodeIndex].HCost,OpenSet.Num());

	OpenSet.RemoveAtSwap(BestIdxInOpen);
	Nodes[NodeIndex].InOpenFlags = 0;

	return NodeIndex;
}

//	パスの初期化
void UPathFinderComponent::ClearResultPath()
{
	m_ResultPath.Empty();
	NextPathIndex = 0;
	m_bLastPath = false;
}

/**
 * @brief	GoalIndexから親を辿って最終的な経路を作成	
 */
void UPathFinderComponent::MakeResultPath(int32 GoalIndex)
{
	m_ResultPath.Empty();
	if (!m_GraphData)
	{
		UE_LOG(LogTemp, Warning, TEXT("UPathFinderComponent::MakeResultPath__Failde"));
		return;
	}

	//	ゴールから親を辿って、最終的な経路を作成
	int32 CurrentIndex = GoalIndex;
	while (CurrentIndex != INDEX_NONE)
	{
		m_ResultPath.Add(m_GraphData->m_Vertices[CurrentIndex].Position);
		
		if (Nodes.IsValidIndex(CurrentIndex))
		{
			CurrentIndex = Nodes[CurrentIndex].Parent;
		}
		else
		{
			CurrentIndex = INDEX_NONE;
		}

	}
	//	スタートからになるように逆順に
	Algo::Reverse(m_ResultPath); 
	NextPathIndex = 0;

	UE_LOG(LogTemp, Warning, TEXT("UPathFinderComponent::MakeResultPath__Success m_ResultPath.Num = %d"),m_ResultPath.Num());

}

/**
 * @brief	ランダムなノードの位置を取得
 */
FVector UPathFinderComponent::GetRandomLocation()
{
	if (m_GraphData->m_Vertices.Num() == 0)
	{
		return FVector();
	}

	int32 Index = FMath::RandHelper(m_GraphData->m_Vertices.Num());

	return m_GraphData->m_Vertices[Index].Position;

}
