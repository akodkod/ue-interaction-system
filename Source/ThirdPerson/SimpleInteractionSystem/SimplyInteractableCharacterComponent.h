// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SimplyInteractableCharacterComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class THIRDPERSON_API USimplyInteractableCharacterComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	USimplyInteractableCharacterComponent();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Simply Interactable")
	float TraceSphereRadius = 100.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Simply Interactable")
	float TraceDistance = 250.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Simply Interactable")
	float TraceDistanceOffset = 250.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Simply Interactable")
	FVector TraceStartOffset = FVector(0.f, 0.f, 0.f);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Simply Interactable")
	FVector TraceEndOffset = FVector(0.f, 0.f, 0.f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Simply Interactable")
	TEnumAsByte<ECollisionChannel> TraceChannel = ECollisionChannel::ECC_Visibility;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Simply Interactable")
	bool bDebug = true;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	// Store the interactable object that is currently being focused
	AActor* FocusedInteractableActor = nullptr;

	void FocusInteractableActor(AActor* Interactable);
	void UnfocusCurrentInteractableActor();

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "Simply Interactable")
	void TraceForInteractableObject();
	
	UFUNCTION(BlueprintCallable, Category = "Simply Interactable")
	void InteractWithFocusedObject();
};
