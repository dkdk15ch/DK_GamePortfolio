// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "DustCounter.h"
#include "PridePlayerState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDead);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCombo, int, NewScore);


/**
 * 
 */
UCLASS()
class BURNWITHPRIDE_API APridePlayerState : public APlayerState
{
	GENERATED_BODY()
public:
	APridePlayerState();

	virtual void Tick(float DeltaTime) override;

	//	デリゲートプロパティ
	UPROPERTY(BlueprintAssignable, Category = "Player|OnDead")
	FOnDead OnDead;

	UPROPERTY(BlueprintAssignable)
	FOnCombo OnCombo;

	//	ダストカウンター取得
	UFUNCTION(BlueprintCallable, Category = "DustCounter")
	UDustCounter* GetDustCounter() const { return m_DustCounter; }

	//	HP取得
	UFUNCTION(BlueprintCallable, Category = "Player|HP")
	float GetHP()const { return HP; }
	//	HPダメージ
	void DamageHP(float damageValue);

protected:
	UPROPERTY(BlueprintReadOnly, Category = "DustCounter")
	UDustCounter* m_DustCounter;

	UFUNCTION(BlueprintImplementableEvent, Category = "Player|HP")
	void SetMPC();

	///////////////////////////////////////////////////////////////////////
	//	コンボ関連
	//////////////////////////////////////////////////////////////////////
	UFUNCTION(BlueprintCallable, Category = "Player|Combo")
	void SetCombo(int NewCombo);

	UFUNCTION(BlueprintCallable, Category = "Player|Combo")
	void AddCombo(int AddValue);

	UFUNCTION(BlueprintCallable, Category = "Player|Combo")
	void RestCombo() { Combo = 0; }

	UFUNCTION(BlueprintCallable, Category = "Player|Combo")
	float GetComboTime()const { return ComboTimer; }

	UFUNCTION(BlueprintCallable, Category = "Player|Combo")
	int GetCombo()const { return Combo; }

	////////////////////////////////////////////////////////////////////////

private:
	float HP = 10.0f;

	///////////////////////////////////////////////////////////////////////
	//	コンボ関連
	//////////////////////////////////////////////////////////////////////
	int Combo = 0;
	const float ComboLimit = 2.0f;
	float ComboTimer = 0.0f;

	void CountComboTime(float DeltaTime);
};
