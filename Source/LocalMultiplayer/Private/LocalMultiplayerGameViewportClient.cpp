// Fill out your copyright notice in the Description page of Project Settings.


#include "LocalMultiplayerGameViewportClient.h"

#include "LocalMultiplayerSettings.h"
#include "LocalMultiplayerSubsystem.h"
#include "GameFramework/PlayerInput.h"
#include "Kismet/GameplayStatics.h"

void ULocalMultiplayerGameViewportClient::PostInitProperties()
{
	Super::PostInitProperties();
	MaxSplitscreenPlayers = 8;
}

bool ULocalMultiplayerGameViewportClient::InputKey(const FInputKeyEventArgs& EventArgs)
{
	ULocalMultiplayerSubsystem* Multiplayer = GameInstance->GetSubsystem<ULocalMultiplayerSubsystem>();
	const ULocalMultiplayerSettings* Settings = GetDefault<ULocalMultiplayerSettings>();
	int PlayerIndex = -1;
	
	if(EventArgs.IsGamepad())
	{
		GEngine->AddOnScreenDebugMessage(1,5.0f, FColor::Red, "GamepadInput");
		if(Multiplayer->GetAssignedPlayerIndexFromGamepadDeviceID(EventArgs.InputDevice.GetId()) < 0)
		{
			PlayerIndex = Multiplayer->AssignNewPlayerToGamepadDeviceID(EventArgs.InputDevice.GetId());
			Multiplayer->AssignGamepadInputMapping(
				PlayerIndex,
				ELocalMultiplayerInputMappingType::InGame);
		}
		else
		{
			PlayerIndex = Multiplayer->GetAssignedPlayerIndexFromGamepadDeviceID(EventArgs.InputDevice.GetId());
		}
	}
	else
	{
		int KeyboardProfileIndex = Settings->FindKeyboardProfileIndexFromKey(EventArgs.Key, ELocalMultiplayerInputMappingType::InGame);
		if(KeyboardProfileIndex < 0)
		{
			GEngine->AddOnScreenDebugMessage(1, 0.5f, FColor::Red, "Key not assigned");
		} else if (Multiplayer->GetAssignedPlayerIndexFromKeyboardProfileIndex(KeyboardProfileIndex) < 0)
		{
			PlayerIndex = Multiplayer->AssignNewPlayerToKeyboardProfile(KeyboardProfileIndex);
			Multiplayer->AssignKeyboardMapping(
				PlayerIndex,
				KeyboardProfileIndex,
				ELocalMultiplayerInputMappingType::InGame);
		}
		else
		{
			PlayerIndex = Multiplayer->GetAssignedPlayerIndexFromKeyboardProfileIndex(KeyboardProfileIndex);
		}
	}

	if(PlayerIndex >= 0)
	{
		FInputKeyParams inputArgs(EventArgs.Key, EventArgs.Event, static_cast<double>(EventArgs.AmountDepressed), EventArgs.IsGamepad());
		return UGameplayStatics::GetPlayerController(GetGameInstance()->GetWorld(), PlayerIndex)->InputKey(inputArgs);
	}
	return Super::InputKey(EventArgs);
}

bool ULocalMultiplayerGameViewportClient::InputAxis(FViewport* InViewport, FInputDeviceId InputDevice, FKey Key,
	float Delta, float DeltaTime, int32 NumSamples, bool bGamepad)
{
	ULocalMultiplayerSubsystem* Multiplayer = GameInstance->GetSubsystem<ULocalMultiplayerSubsystem>();
	const ULocalMultiplayerSettings* Settings = GetDefault<ULocalMultiplayerSettings>();

	int PlayerIndex = -1;
	if(bGamepad)
	{
		GEngine->AddOnScreenDebugMessage(1,5.0f, FColor::Red, "GamepadAxis");

		if(Multiplayer->GetAssignedPlayerIndexFromGamepadDeviceID(InputDevice.GetId()) < 0)
		{
			PlayerIndex = Multiplayer->AssignNewPlayerToGamepadDeviceID(InputDevice.GetId());
			Multiplayer->AssignGamepadInputMapping(
				PlayerIndex,
				ELocalMultiplayerInputMappingType::InGame);
		}
		else
		{
			PlayerIndex = Multiplayer->GetAssignedPlayerIndexFromGamepadDeviceID(InputDevice.GetId());
		}
	}
	else
	{
		int KeyboardProfileIndex = Settings->FindKeyboardProfileIndexFromKey(Key, ELocalMultiplayerInputMappingType::InGame);
		if(KeyboardProfileIndex < 0)
		{
			GEngine->AddOnScreenDebugMessage(1, 0.5f, FColor::Red, "Key not assigned");
		} else if (Multiplayer->GetAssignedPlayerIndexFromKeyboardProfileIndex(KeyboardProfileIndex) < 0)
		{
			PlayerIndex = Multiplayer->AssignNewPlayerToKeyboardProfile(KeyboardProfileIndex);
			Multiplayer->AssignKeyboardMapping(
				PlayerIndex,
				KeyboardProfileIndex,
				ELocalMultiplayerInputMappingType::InGame);
		}
		else
		{
			PlayerIndex = Multiplayer->GetAssignedPlayerIndexFromKeyboardProfileIndex(KeyboardProfileIndex);
		}
	}
	
	if(PlayerIndex >= 0)
	{
		return UGameplayStatics::GetPlayerController(GetGameInstance()->GetWorld(), PlayerIndex)->InputAxis(Key, Delta, DeltaTime, NumSamples, bGamepad);
	}
	return Super::InputAxis(Viewport, InputDevice, Key, Delta, DeltaTime, NumSamples, bGamepad);
}
