// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactable.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "InteractableActor.generated.h"

UCLASS()
class THIRDPERSON_API AInteractableActor : public AActor, public IInteractable
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AInteractableActor();

	// Create a trace collision box
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interaction")
	UBoxComponent* TraceCollisionBox;

	// Scene for showing/hiding the HUD
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interaction")
	USceneComponent* HUDScene;

	// Scene for HUD locations
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interaction")
	USceneComponent* HUDLocationsScene;

	// Rotate the HUD to face the camera
	UPROPERTY(EditAnywhere, Category = "Interaction")
	bool bRotateHUD = true;

	// HUD locations actor tag
	UPROPERTY(EditAnywhere, Category = "Interaction")
	FName HUDLocationsActorTag = "HUDLocation";

	// Debug mode
	UPROPERTY(EditAnywhere, Category = "Interaction")
	bool bDebug = false;
	
protected:
	bool bHUDVisible = false;

	// Store all possible HUD location vectors
	TArray<FVector> HUDLocations;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void ShowHUD();
	void HideHUD();
	void GetHUDLocations();
	void PlaceHUDToNearestLocation(bool ignoreHUDVisibility = false);
	void RotateHUD(bool ignoreHUDVisibility = false);

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void BeginFocus_Implementation() override;
	virtual void EndFocus_Implementation() override;
	virtual void Interact_Implementation() override;
};
