﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "SmashCharacterState.h"

#include "Characters/SmashCharacter.h"
#include "Characters/SmashCharacterStateID.h"
#include "Characters/SmashCharacterSettings.h"
#include "Characters/SmashCharacterStateMachine.h"


// Sets default values for this component's properties
USmashCharacterState::USmashCharacterState()
{
	PrimaryComponentTick.bCanEverTick = false;
}

ESmashCharacterStateID USmashCharacterState::GetStateID()
{
	return ESmashCharacterStateID::None;
}

void USmashCharacterState::StateInit(USmashCharacterStateMachine* InStateMachine)
{
	StateMachine = InStateMachine;
	Character = InStateMachine->GetCharacter();
	CharacterSettings = GetDefault<USmashCharacterSettings>();
	GEngine->AddOnScreenDebugMessage(-1,
		3.f,
		FColor::Magenta,
		FString::Printf(TEXT("Init State %d"), GetStateID()));
}

void USmashCharacterState::StateEnter(ESmashCharacterStateID PreviousStateID)
{
	if(GetAnimationMontage() != nullptr) Character->PlayAnimMontage(GetAnimationMontage());
}

void USmashCharacterState::StateExit(ESmashCharacterStateID NextStateID)
{
	
}

void USmashCharacterState::StateTick(float DeltaTime)
{
}

UAnimMontage* USmashCharacterState::GetAnimationMontage()
{
	return nullptr;
}
