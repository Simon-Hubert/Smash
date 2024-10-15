// Fill out your copyright notice in the Description page of Project Settings.


#include "LocalMultiplayerSubsystem.h"

#include "EnhancedInputSubsystems.h"
#include "LocalMultiplayerSettings.h"
#include "Kismet/GameplayStatics.h"

void ULocalMultiplayerSubsystem::CreateAndInitPlayers(ELocalMultiplayerInputMappingType MappingType)
{
	int Kbnb = GetDefault<ULocalMultiplayerSettings>()->GetNbKeyboardProfiles();
	for (int i = 1; i < Kbnb + GetDefault<ULocalMultiplayerSettings>()->NbMaxGamePad; i++)
	{
		UGameplayStatics::CreatePlayer(GetWorld(), i, true);
	}

	for (auto& elem : PlayerIndexFromKeyboardProfileIndex)
	{
		AssignKeyboardMapping(elem.Value, elem.Key, MappingType);
	}
	for (auto& elem : PlayerIndexFromGamepadProfileIndex)
	{
		AssignGamepadInputMapping(elem.Value, MappingType);
	}
}

int ULocalMultiplayerSubsystem::GetAssignedPlayerIndexFromKeyboardProfileIndex(int KeyboardProfileIndex)
{
	if(PlayerIndexFromKeyboardProfileIndex.Contains(KeyboardProfileIndex))
	{
		return *PlayerIndexFromKeyboardProfileIndex.Find(KeyboardProfileIndex);
	}
	return -1;
}

int ULocalMultiplayerSubsystem::AssignNewPlayerToKeyboardProfile(int KeyboardProfileIndex)
{
	LastAssignedPlayerIndex++;
	PlayerIndexFromKeyboardProfileIndex.Add(KeyboardProfileIndex, LastAssignedPlayerIndex);
	GEngine->AddOnScreenDebugMessage(1, 5.0f, FColor::Red, "Assign player:" + FString::SanitizeFloat(LastAssignedPlayerIndex), true);
	return LastAssignedPlayerIndex;
}

void ULocalMultiplayerSubsystem::AssignKeyboardMapping(int PlayerIndex, int KeyboardProfileIndex,
	ELocalMultiplayerInputMappingType MappingType) const
{
	ULocalPlayer* Player = UGameplayStatics::GetPlayerController(GetWorld(), PlayerIndex)->GetLocalPlayer();
	Player->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>()
	->AddMappingContext(GetDefault<ULocalMultiplayerSettings>()
	->KeyboardProfileData[KeyboardProfileIndex].GetIMCFromType(MappingType), 1);
}

int ULocalMultiplayerSubsystem::GetAssignedPlayerIndexFromGamepadDeviceID(int DeviceID)
{
	if(PlayerIndexFromGamepadProfileIndex.Contains(DeviceID))
	{
		return *PlayerIndexFromGamepadProfileIndex.Find(DeviceID);
	}
	return -1;
}

int ULocalMultiplayerSubsystem::AssignNewPlayerToGamepadDeviceID(int DeviceID)
{
	LastAssignedPlayerIndex++;
	PlayerIndexFromGamepadProfileIndex.Add(DeviceID, LastAssignedPlayerIndex);
	GEngine->AddOnScreenDebugMessage(1, 5.0f, FColor::Red, "Assign player:" + FString::SanitizeFloat(LastAssignedPlayerIndex), true);
	return LastAssignedPlayerIndex;
}

void ULocalMultiplayerSubsystem::AssignGamepadInputMapping(int PlayerIndex,
	ELocalMultiplayerInputMappingType MappingType) const
{
	ULocalPlayer* Player = UGameplayStatics::GetPlayerController(GetWorld(), PlayerIndex)->GetLocalPlayer();	
	Player->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>()
	->AddMappingContext(GetDefault<ULocalMultiplayerSettings>()
	->GamepadProfileData.GetIMCFromType(MappingType), 1);
}