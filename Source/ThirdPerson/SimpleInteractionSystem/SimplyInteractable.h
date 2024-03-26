// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SimplyInteractable.generated.h"

// This class does not need to be modified.
UINTERFACE()
class USimplyInteractable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class THIRDPERSON_API ISimplyInteractable
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Simply Interactable")
	void PlayerEntersPreviewArea();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Simply Interactable")
	void PlayerLeavesPreviewArea();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Simply Interactable")
	void PlayerFocuses();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Simply Interactable")
	void PlayerUnfocuses();

	// Create CanPlayerInteract with default return true
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Simply Interactable")
	bool CanPlayerInteract();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Simply Interactable")
	void PlayerInteracts();
};
