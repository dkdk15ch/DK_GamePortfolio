// Fill out your copyright notice in the Description page of Project Settings.

//	下の方法
//	https://dev.epicgames.com/community/learning/tutorials/w6l7/unreal-engine-custom-gravity-in-ue-5-4

#include "GravityController.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

void AGravityController::UpdateRotation(float DeltaTime)
{
	FVector GravityDirection = FVector::DownVector;
	if (ACharacter* PlayerCharacter = Cast<ACharacter>(GetPawn()))
	{
		if (UCharacterMovementComponent* MoveComp = PlayerCharacter->GetCharacterMovement())
		{
			GravityDirection = MoveComp->GetGravityDirection();
		}
	}
	// 現在のコントローラの向きを取得
	FRotator ViewRotation = GetControlRotation();
	// 重力方向が変化していたら、カメラを回転
	if (!LastFrameGravity.Equals(FVector::ZeroVector))
	{
		const FQuat DeltaGravityRotation = FQuat::FindBetweenNormals(LastFrameGravity, GravityDirection);
		const FQuat WarpedCameraRotation = DeltaGravityRotation * FQuat(ViewRotation);
		ViewRotation = WarpedCameraRotation.Rotator();
	}
	LastFrameGravity = GravityDirection;
	// ビュー回転をワールド空間から重力を基準に変換
	ViewRotation = GetGravityRelativeRotation(ViewRotation, GravityDirection);
	// ビュー回転の回転変化量を計算
	FRotator DeltaRot(RotationInput);
	if (PlayerCameraManager)
	{
		ACharacter* PlayerCharacter = Cast<ACharacter>(GetPawn());
		PlayerCameraManager->ProcessViewRotation(DeltaTime, ViewRotation, DeltaRot);
		// カメラロールを０にして、重力に対して水平をとる
		ViewRotation.Roll = 0;
		//	新しい回転をセット
		SetControlRotation(GetGravityWorldRotation(ViewRotation, GravityDirection));
	}
	APawn* const P = GetPawnOrSpectator();
	if (P)
	{
		P->FaceRotation(ViewRotation, DeltaTime);
	}

}

/**
 * @brief 重力を基準とした回転に変換
 * @param Rotation			現在の向き		
 * @param GravityDirection	重力方向
 * @return 
 */
FRotator AGravityController::GetGravityRelativeRotation(FRotator Rotation, FVector GravityDirection)
{
	if (!GravityDirection.Equals(FVector::DownVector))
	{
		//	現在の重力方向を、下向きベクトルに合わせる回転
		FQuat GravityRotation = FQuat::FindBetweenNormals(GravityDirection, FVector::DownVector);
		//	重力基準に回転
		return (GravityRotation * Rotation.Quaternion()).Rotator();
	}

	return Rotation;
}

/**
 * @brief ワールド基準とした回転に変換
 * @param Rotation			現在の向き
 * @param GravityDirection	重力方向
 * @return
 */
FRotator AGravityController::GetGravityWorldRotation(FRotator Rotation, FVector GravityDirection)
{
	if (!GravityDirection.Equals(FVector::DownVector))
	{
		FQuat GravityRotation = FQuat::FindBetweenNormals(FVector::DownVector, GravityDirection);
		//	ワールド基準で回転
		return (GravityRotation * Rotation.Quaternion()).Rotator();
	}

	return Rotation;
}
