// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SimplyInteractable.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "SimplyInteractableActor.generated.h"

UCLASS()
class THIRDPERSON_API ASimplyInteractableActor : public AActor, public ISimplyInteractable
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASimplyInteractableActor();

	// Trigger preview box to detect player
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Simply Interactable")
	UBoxComponent* TriggerPreviewBox;

	// Allow interaction box to detect player near the object
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Simply Interactable")
	UBoxComponent* InteractionBox;

	// Debug
	UPROPERTY(EditAnywhere, Category = "Simply Interactable")
	bool bDebug = true;

protected:
	bool bPlayerIsNear = false;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnPlayerEnterTriggerPreviewBox(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnPlayerLeaveTriggerPreviewBox(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Add default function for ISimplyInteractable::CanPlayerInteract
	virtual bool CanPlayerInteract_Implementation() override
	{
		return true;
	}
};
