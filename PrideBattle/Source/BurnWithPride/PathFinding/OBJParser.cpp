// Fill out your copyright notice in the Description page of Project Settings.


#include "PathFinding/OBJParser.h"

/**
 * @brief			objから頂点情報を取り出し、面情報から隣接データをセットする。
 *					・面が三角形化された球メッシュの使用を想定(Blenderで作成)
 *					・空白で区切られたデータを想定(v 1.0 0.7 0.5 / f 1 2 3)
 * @param obj			obj
 * @param OutputData	
 * @return 
 */
bool OBJParser::ParseOBJ(const FString& obj, TArray<FVertex>& OutputData)
{
	TArray<FString> Lines;
	obj.ParseIntoArrayLines(Lines, true);

	int startLine = 0;
	//	先頭の頂点データまで進める
	for (const FString& RawLine : Lines)
	{
		FString Line = RawLine.TrimStartAndEnd();
		if (Line.IsEmpty())			continue;
		if (Line[0] == TEXT('v'))	break;

		startLine++;
	}

	//	頂点を取得
	for (int i = startLine; i < Lines.Num(); i++)
	{
		FString Line = Lines[i].TrimStartAndEnd();
		if (Line.IsEmpty()) break;

		TArray<FString> Positions;
		Line.ParseIntoArrayWS(Positions);
		if (!Positions[0].Equals(TEXT("v"), ESearchCase::CaseSensitive)) break;

		if (Positions.Num() == 4)
		{
			const float x = FCString::Atof(*Positions[1]);
			const float y = FCString::Atof(*Positions[2]);
			const float z = FCString::Atof(*Positions[3]);
			const FVector VerPos = FVector(x, y, z);
			OutputData.Add(FVertex(VerPos));
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("OBJParse::ParseOBJ__Failde Data Mismatch"));
			return false;
		}

		startLine++;
	}

	if (OutputData.Num() == 0)
	{
		UE_LOG(LogTemp, Error, TEXT("OBJParse::ParseOBJ__Failde No Vertex Data"));
		return false;
	}

	//	面データまで進める
	for (int i = startLine; i < Lines.Num(); i++)
	{
		FString Line = Lines[i].TrimStartAndEnd();
		if (Line.IsEmpty()) continue;
		if (Line[0] == TEXT('f')) break;

		startLine++;
	}
	//	面を取得して、隣接データとして利用
	for (int i = startLine; i < Lines.Num(); i++)
	{
		FString Line = Lines[i].TrimStartAndEnd();
		if (Line.IsEmpty()) break;

		TArray<FString> Faces;
		Line.ParseIntoArrayWS(Faces);
		if (!Faces[0].Equals(TEXT("f"), ESearchCase::CaseSensitive)) break;

		if (Faces.Num() == 4)
		{
			int32 index1 = FCString::Atoi(*Faces[1]);
			int32 index2 = FCString::Atoi(*Faces[2]);
			int32 index3 = FCString::Atoi(*Faces[3]);
			//	objのインデックスは1からなので-1
			index1--;
			index2--;
			index3--;

			//	範囲チェック
			if (!OutputData.IsValidIndex(index1) || 
				!OutputData.IsValidIndex(index2) || 
				!OutputData.IsValidIndex(index3))
			{
				UE_LOG(LogTemp, Error, TEXT("OBJParse::ParseOBJ_Failde Out of Range Vertex Index"));
				return false;
			}

			//	双方向に追加
			FNeighbor::AddNeighborIfNotExists(OutputData, index1, index2);
			FNeighbor::AddNeighborIfNotExists(OutputData, index2, index1);
			FNeighbor::AddNeighborIfNotExists(OutputData, index1, index3);
			FNeighbor::AddNeighborIfNotExists(OutputData, index3, index1);
			FNeighbor::AddNeighborIfNotExists(OutputData, index2, index3);
			FNeighbor::AddNeighborIfNotExists(OutputData, index3, index2);
		}
	}

	return true;
}
