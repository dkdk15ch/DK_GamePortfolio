// Fill out your copyright notice in the Description page of Project Settings.


#include "PathGraphData.h"

/**
 * @brief			保存済み確認して、隣接データを追加
 * @param Vertices  頂点データ
 * @param FromIndex 始点インデックス
 * @param ToIndex	終点インデックス
 */
void FNeighbor::AddNeighborIfNotExists(TArray<FVertex>& Vertices, int32 FromIndex, int32 ToIndex)
{
	{
		if (!Vertices.IsValidIndex(FromIndex) || !Vertices.IsValidIndex(ToIndex)) return;
		auto& Neis = Vertices[FromIndex].Neighbors;
		for (const FNeighbor& n : Neis)
		{
			if (n.VertexIndex == ToIndex) return;
		}
		float Dist = FVector::Dist(Vertices[FromIndex].Position, Vertices[ToIndex].Position);

		Neis.Add(FNeighbor(ToIndex, Dist));
	}
}

