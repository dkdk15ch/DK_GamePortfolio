// Fill out your copyright notice in the Description page of Project Settings.


#include "PridePlayerState.h"

APridePlayerState::APridePlayerState()
{
	m_DustCounter = CreateDefaultSubobject<UDustCounter>(TEXT("DustCounter"));
}

void APridePlayerState::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CountComboTime(DeltaTime);
}

void APridePlayerState::DamageHP(float damageValue)
{
	HP -= damageValue;

	if (HP <= 0)
	{
		HP = 0;
		OnDead.Broadcast();
	}

	SetMPC();
}

void APridePlayerState::SetCombo(int NewCombo)
{
	if (NewCombo < 0) return;

	Combo = NewCombo;
	OnCombo.Broadcast(Combo);
}

void APridePlayerState::AddCombo(int AddValue)
{
	Combo += AddValue;
	ComboTimer = 0.0f;
	OnCombo.Broadcast(Combo);
}

void APridePlayerState::CountComboTime(float DeltaTime)
{
	if (Combo <= 0) return;

	ComboTimer += DeltaTime;
	if (ComboTimer >= ComboLimit)
	{
		SetCombo(0);
		ComboTimer = 0.0f;
		UE_LOG(LogTemp, Log, TEXT("Combo Reset"));
	}
}

