// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/States/SmashCharacterStateJump.h"

#include "Characters/SmashCharacter.h"
#include "Characters/SmashCharacterStateMachine.h"
#include "GameFramework/CharacterMovementComponent.h"

ESmashCharacterStateID USmashCharacterStateJump::GetStateID()
{
	return ESmashCharacterStateID::Jump;
}

void USmashCharacterStateJump::StateEnter(ESmashCharacterStateID PreviousStateID)
{
	Super::StateEnter(PreviousStateID);
	Character->GetCharacterMovement()->AirControl = JumpAirControl;
	float Gravity = 8.f*JumpMaxHeight/(JumpDuration*JumpDuration);
	float GravityScale = Gravity/980.f;
	float V0 = Gravity*JumpDuration/2.f;
	Character->GetCharacterMovement()->JumpZVelocity = V0;
	Character->GetCharacterMovement()->GravityScale = GravityScale;
	Character->Jump();
}

void USmashCharacterStateJump::StateExit(ESmashCharacterStateID NextStateID)
{
	Super::StateExit(NextStateID);
}

void USmashCharacterStateJump::StateTick(float DeltaTime)
{
	Super::StateTick(DeltaTime);
	
	if(FMath::Abs(Character->GetInputMoveX()) > CharacterSettings->InputMoveXThreshold)
	{
		Character->AddMovementInput(FVector::ForwardVector, Character->GetInputMoveX());
	}
	if(Character->GetVelocity().Z < 0)
	{
		StateMachine->ChangeState(ESmashCharacterStateID::Fall);
	}
}

UAnimMontage* USmashCharacterStateJump::GetAnimationMontage()
{
	return JumpAnimation;
}
