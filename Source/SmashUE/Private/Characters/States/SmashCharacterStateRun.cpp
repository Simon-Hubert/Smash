// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/States/SmashCharacterStateRun.h"

#include "Characters/SmashCharacter.h"


ESmashCharacterStateID USmashCharacterStateRun::GetStateID()
{
	return ESmashCharacterStateID::Run;
}

void USmashCharacterStateRun::StateEnter(ESmashCharacterStateID PreviousStateID)
{
	Super::StateEnter(PreviousStateID);
	
	GEngine->AddOnScreenDebugMessage(
		-1,
		3.f,
		FColor::Cyan,
		TEXT("Enter StateRun"));
}

void USmashCharacterStateRun::StateExit(ESmashCharacterStateID NextStateID)
{
	Super::StateExit(NextStateID);

	GEngine->AddOnScreenDebugMessage(
		-1,
		3.f,
		FColor::Red,
		TEXT("Exit StateRun"));
}

void USmashCharacterStateRun::StateTick(float DeltaTime)
{
	Super::StateTick(DeltaTime);

	FVector position = Character->GetActorLocation();
	position.X += RunMaxSpeed*DeltaTime*Character->GetOrientX();
	Character->SetActorLocation(position);
	GEngine->AddOnScreenDebugMessage(
		-1,
		.1f,
		FColor::Green,
		TEXT("Tick StateRun"));
}

UAnimMontage* USmashCharacterStateRun::GetAnimationMontage()
{
	return RunAnimation;
}

