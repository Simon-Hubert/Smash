// Fill out your copyright notice in the Description page of Project Settings.


#include "Camera/CameraWorldSubsystem.h"

#include "Camera/CameraComponent.h"
#include "Camera/CameraFollowTarget.h"
#include "Kismet/GameplayStatics.h"

void UCameraWorldSubsystem::PostInitialize()
{
	Super::PostInitialize();
}

void UCameraWorldSubsystem::OnWorldBeginPlay(UWorld& InWorld)
{
	Super::OnWorldBeginPlay(InWorld);
	CameraMain = FindCameraByTag(TEXT("CameraMain"));
}

void UCameraWorldSubsystem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	TickUpdateCameraPosition(DeltaTime);
}

void UCameraWorldSubsystem::AddFollowTarget(UObject* FollowTarget)
{
	FollowTargets.Add(FollowTarget);
}

void UCameraWorldSubsystem::RemoveFollowTarget(UObject* FollowTarget)
{
	FollowTargets.Remove(FollowTarget);
}

void UCameraWorldSubsystem::TickUpdateCameraPosition(float DeltaTime)
{
	float y  = CameraMain->GetOwner()->GetActorLocation().Y;
	FVector CamPos = CalculateAveragePositionBetweenTargets();
	CamPos.Y = y;
	CameraMain->GetOwner()->SetActorLocation(CamPos);
}

FVector UCameraWorldSubsystem::CalculateAveragePositionBetweenTargets()
{
	FVector Barycenter = FVector::Zero();
	for (UObject* Target : FollowTargets)
	{
		ICameraFollowTarget* FollowTarget =  Cast<ICameraFollowTarget>(Target);
		if(FollowTarget && FollowTarget->IsFollowable())
		{
			Barycenter += FollowTarget->GetFollowPosition();
		}
	}
	return Barycenter / FollowTargets.Num();
}

UCameraComponent* UCameraWorldSubsystem::FindCameraByTag(const FName& Tag) const
{
	TArray<AActor*> Cameras;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), Tag, Cameras);

	return Cameras[0]->FindComponentByClass<UCameraComponent>();
}
