// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractionCharacterComponent.h"

#include "Interactable.h"
#include "InteractableActor.h"
#include "Camera/CameraComponent.h"


// Sets default values for this component's properties
UInteractionCharacterComponent::UInteractionCharacterComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UInteractionCharacterComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UInteractionCharacterComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UInteractionCharacterComponent::DetectInteractableObject()
{
	APawn* OwningPawn = GetOwner<APawn>();
	if (!OwningPawn) return;

	// Get camera
	UCameraComponent* Camera = OwningPawn->FindComponentByClass<UCameraComponent>();
	if (!Camera) return;

	FVector Start = OwningPawn->GetActorLocation();
	FVector End = Start + Camera->GetForwardVector() * TraceDistance;

	// Start.Z += TraceHeightOffset;
	End.Z += TraceHeightOffset;

	FHitResult HitResult;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(OwningPawn);

	bool bHit = GetWorld()->SweepSingleByChannel(HitResult, Start, End, FQuat::Identity, ECC_GameTraceChannel1, FCollisionShape::MakeSphere(TraceSphereRadius), CollisionParams);
	if (bDebug) DrawDebugLine(GetWorld(), Start, End, FColor::Blue, false, 1.f, 0, 1.f);
	
	if (bHit)
	{
		// Draw debug sphere
		if (bDebug) DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, TraceSphereRadius, 8, FColor::Green, false, 1.f, 0, 1.f);

		// Check if the hit actor implements the interactable interface
		AActor* HitActor = HitResult.GetActor();

		if (HitActor && HitActor->Implements<UInteractable>())
		{
			FocusInteractableActor(HitActor);
		}
		else
		{
			UnfocusCurrentInteractableActor();
		}
	}
	else
	{
		// Draw debug sphere
		if (bDebug) DrawDebugSphere(GetWorld(), End, TraceSphereRadius, 8, FColor::Red, false, 1.f, 0, 1.f);
		
		UnfocusCurrentInteractableActor();
	}
}

void UInteractionCharacterComponent::FocusInteractableActor(AActor* Interactable)
{
	if (FocusedInteractableActor == Interactable) return;
	if (FocusedInteractableActor) UnfocusCurrentInteractableActor();

	FocusedInteractableActor = Interactable;
	IInteractable::Execute_BeginFocus(FocusedInteractableActor);
}

void UInteractionCharacterComponent::UnfocusCurrentInteractableActor()
{
	if (!FocusedInteractableActor) return;

	IInteractable::Execute_EndFocus(FocusedInteractableActor);
	FocusedInteractableActor = nullptr;
}

void UInteractionCharacterComponent::InteractWithFocusedObject()
{
	if (!FocusedInteractableActor) return;

	IInteractable::Execute_Interact(FocusedInteractableActor);
}

