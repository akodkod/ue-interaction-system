// Fill out your copyright notice in the Description page of Project Settings.


#include "SimplyInteractableCharacterComponent.h"

#include "SimplyInteractable.h"
#include "Camera/CameraComponent.h"


// Sets default values for this component's properties
USimplyInteractableCharacterComponent::USimplyInteractableCharacterComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void USimplyInteractableCharacterComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void USimplyInteractableCharacterComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void USimplyInteractableCharacterComponent::TraceForInteractableObject()
{
	APawn* OwningPawn = GetOwner<APawn>();
	if (!OwningPawn) return;

	UCameraComponent* Camera = OwningPawn->FindComponentByClass<UCameraComponent>();
	if (!Camera) return;

	FVector CameraForwardVector = Camera->GetForwardVector();
	FVector Start = Camera->GetComponentLocation() + (CameraForwardVector * TraceDistanceOffset) + TraceStartOffset;
	FVector End = Start + (CameraForwardVector * TraceDistance) + TraceEndOffset;
	
	// Draw debug line
	if (bDebug) DrawDebugLine(GetWorld(), Start, End, FColor::Blue, false, 1.f, 0, 1.f);

	// Trace using a sphere
	FHitResult HitResult;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(OwningPawn);

	bool bHit = GetWorld()->SweepSingleByChannel(
		HitResult,
		Start,
		End,
		FQuat::Identity,
		TraceChannel,
		FCollisionShape::MakeSphere(TraceSphereRadius),
		CollisionParams
	);

	if (bHit)
	{
		// Draw debug sphere
		if (bDebug) DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, TraceSphereRadius, 8, FColor::Green, false, 1.f, 0, 0.5);
		
		AActor* HitActor = HitResult.GetActor();
		
		if (HitActor && HitActor->Implements<USimplyInteractable>() && ISimplyInteractable::Execute_CanPlayerInteract(HitActor))
		{
			FocusInteractableActor(HitActor);
		}
		else
		{
			// TODO: Keep current interactable actor if distance is less than a certain threshold
			UnfocusCurrentInteractableActor();
		}
	}
	else
	{
		// Draw debug sphere
		if (bDebug) DrawDebugSphere(GetWorld(), End, TraceSphereRadius, 8, FColor::Red, false, 1.f, 0, 0.5);
		
		UnfocusCurrentInteractableActor();
	}
}

void USimplyInteractableCharacterComponent::FocusInteractableActor(AActor* Interactable)
{
	if (FocusedInteractableActor == Interactable) return;
	if (FocusedInteractableActor) UnfocusCurrentInteractableActor();

	FocusedInteractableActor = Interactable;
	ISimplyInteractable::Execute_PlayerFocuses(FocusedInteractableActor);
}

void USimplyInteractableCharacterComponent::UnfocusCurrentInteractableActor()
{
	if (!FocusedInteractableActor) return;

	ISimplyInteractable::Execute_PlayerUnfocuses(FocusedInteractableActor);
	FocusedInteractableActor = nullptr;
}

void USimplyInteractableCharacterComponent::InteractWithFocusedObject()
{
	if (!FocusedInteractableActor) return;

	ISimplyInteractable::Execute_PlayerInteracts(FocusedInteractableActor);
}


