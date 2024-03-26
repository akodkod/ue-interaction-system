// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InteractionCharacterComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class THIRDPERSON_API UInteractionCharacterComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UInteractionCharacterComponent();

	// Trace distance
	UPROPERTY(EditAnywhere, Category = "Interaction")
	float TraceDistance = 500.f;

	// Trace sphere radius
	UPROPERTY(EditAnywhere, Category = "Interaction")
	float TraceSphereRadius = 200.f;
	
	// Trace height offset
	UPROPERTY(EditAnywhere, Category = "Interaction")
	float TraceHeightOffset = 120.f;

	// Debug
	UPROPERTY(EditAnywhere, Category = "Interaction")
	bool bDebug = false;

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// Perform a line trace to detect interactable objects
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void DetectInteractableObject();

	// Interact with the currently focused interactable object
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void InteractWithFocusedObject();
	
protected:
	// Store the interactable object that is currently being focused
	class AActor* FocusedInteractableActor = nullptr;
	
	// Called when the game starts
	virtual void BeginPlay() override;

	void FocusInteractableActor(class AActor* Interactable);
	void UnfocusCurrentInteractableActor();
};
