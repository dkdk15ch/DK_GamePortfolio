// Fill out your copyright notice in the Description page of Project Settings.


#include "BulletBase.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "Components/SkeletalMeshComponent.h"
#include "CombatDamageable.h"
#include "Math/UnrealMathUtility.h"

ABulletBase::ABulletBase()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ABulletBase::BeginPlay()
{
		Super::BeginPlay();

	//	生成時の初期設定
	//	StaticMeshComponent がセットされていることを確認
	if (!StaticMeshComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("ABulletBase::BeginPlay__StaticMeshComponent is not set."));
		return;
	}

	// 発射時に回転軸の向きを決定
	// プレイヤーの回転軸を使用
	ACharacter* Player = UGameplayStatics::GetPlayerCharacter(this, 0);
	if (Player)
	{
		AxisDirectionWorld = Player->GetActorRightVector();
		AxisDirectionWorld.Normalize();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("ABulletBase::BeginPlay__no Player"))
	}

	// 初期位置をセット
	USkeletalMeshComponent* Skel = Player->FindComponentByClass<USkeletalMeshComponent>();
	if (Skel && Skel->DoesSocketExist(PlayerSocketName))
	{
		const FVector SocketLoc = Skel->GetSocketLocation(PlayerSocketName); 
		FVector V = SocketLoc;
		V.Normalize();
		InitialWorldPos = V * TurningRadius;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("ABulletBase::BeginPlay__PlayerSocket is not found"));
	}

	// 初期配置（任意）
	StaticMeshComponent->SetWorldLocation(InitialWorldPos);

	CurrentAngle = 0.0f;
}

/**
 * @brief	弾が当たったアクタにダメージを与える
 */
void ABulletBase::DealDamage(AActor* actor)
{
	ICombatDamageable* Damageable = Cast<ICombatDamageable>(actor);
	FVector Impulselocation = FVector(0, 0, 0);

	Damageable->ApplyDamage(BulletDamage, this, this->GetActorLocation(), Impulselocation);
}

void ABulletBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 角度更新
	CurrentAngle += OrbitSpeed * DeltaTime;
	CurrentAngle = FMath::Fmod(CurrentAngle, 360.0f);

	// 原点中心・軸 AxisDirectionWorld 周りで回転（RotateAngleAxis は度単位）
	FVector Rotated = InitialWorldPos.RotateAngleAxis(CurrentAngle, AxisDirectionWorld);

	// メッシュのワールド位置を更新
	StaticMeshComponent->SetWorldLocation(Rotated);
}


