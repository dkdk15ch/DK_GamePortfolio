// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BulletBase.generated.h"

/**
 * @brief	プレイヤーから発射する弾
 *			・球状ステージでの使用
 *			・発射時に発射方向を決定し、回転させる
 */
UCLASS()
class BURNWITHPRIDE_API ABulletBase : public AActor
{
	GENERATED_BODY()
public:
	ABulletBase();

	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bullet|Mesh")
	class UStaticMeshComponent* StaticMeshComponent;

	// 原点からの距離
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bullet|Movement")
	float TurningRadius = 1200.0f;

	// 角速度（度／秒）
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bullet|Movement")
	float OrbitSpeed = 90.0f;

	// 初期位置のソケット名
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bullet|Movement")
	FName PlayerSocketName = "BulletSocket";

	UPROPERTY(EditAnywhere, Category = "Bullet|Damage")
	float BulletDamage = 10.0f;

	UFUNCTION(BlueprintCallable, Category = "Bullet|Damage")
	void DealDamage(AActor* actor);

private:
	// 回転軸（ワールド空間）
	FVector AxisDirectionWorld = FVector::UpVector;

	// 原点からの初期位置（軸に直交する方向に DistanceFromCenter）
	FVector InitialWorldPos = FVector::ZeroVector;

	// 現在の角度（度）
	float CurrentAngle = 0.0f;

};
