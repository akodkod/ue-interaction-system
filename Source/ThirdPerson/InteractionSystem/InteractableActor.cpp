// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractableActor.h"

#include "Camera/CameraComponent.h"


// Sets default values
AInteractableActor::AInteractableActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create a root component
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	// Create a trace collision box that uses ECC_GameTraceChannel1
	TraceCollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TraceCollisionBox"));
	TraceCollisionBox->SetupAttachment(RootComponent);
	TraceCollisionBox->SetRelativeLocation(FVector(0.f, 0.f, 0.f));
	TraceCollisionBox->SetBoxExtent(FVector(50.f, 50.f, 50.f));
	TraceCollisionBox->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Block);
	
	// Create a scene component for the HUD
	HUDScene = CreateDefaultSubobject<USceneComponent>(TEXT("HUD"));
	HUDScene->SetupAttachment(RootComponent);
	HUDScene->SetRelativeLocation(FVector(0.f, 0.f, 0.f));

	// Create a HUD locations scene component
	HUDLocationsScene = CreateDefaultSubobject<USceneComponent>(TEXT("HUDLocations"));
	HUDLocationsScene->SetupAttachment(RootComponent);
	HUDLocationsScene->SetRelativeLocation(FVector(0.f, 0.f, 0.f));
}

// Called when the game starts or when spawned
void AInteractableActor::BeginPlay()
{
	Super::BeginPlay();
	
	HideHUD();
	GetHUDLocations();
}

// Called every frame
void AInteractableActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	PlaceHUDToNearestLocation();
	RotateHUD();
}

void AInteractableActor::BeginFocus_Implementation()
{
	// Print a message with actor name to the screen
	if (bDebug) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, GetName() + " is being focused");

	PlaceHUDToNearestLocation(true);
	RotateHUD(true);
	ShowHUD();
}

void AInteractableActor::EndFocus_Implementation()
{
	// Print a message with actor name to the screen
	if (bDebug) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, GetName() + " is no longer being focused");

	HideHUD();
}

void AInteractableActor::Interact_Implementation()
{
	// Print a message with actor name to the screen
	if (bDebug) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, GetName() + " is being interacted with");

	HideHUD();
}

void AInteractableActor::ShowHUD()
{
	// Show all children of the HUD scene
	for (int i = 0; i < HUDScene->GetNumChildrenComponents(); i++)
	{
		HUDScene->GetChildComponent(i)->SetVisibility(true);
	}

	bHUDVisible = true;
}

void AInteractableActor::HideHUD()
{
	// Hide all children of the HUD scene
	for (int i = 0; i < HUDScene->GetNumChildrenComponents(); i++)
	{
		HUDScene->GetChildComponent(i)->SetVisibility(false);
	}

	bHUDVisible = false;
}

void AInteractableActor::GetHUDLocations()
{
	// Get all children of the HUD locations scene
	for (int i = 0; i < HUDLocationsScene->GetNumChildrenComponents(); i++)
	{
		USceneComponent* Component = HUDLocationsScene->GetChildComponent(i);
		HUDLocations.Add(Component->GetComponentLocation());
		Component->SetVisibility(false);
		Component->SetHiddenInGame(true);
	}

	// Print message with number of HUD locations
	if (bDebug) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, GetName() + " has " + FString::FromInt(HUDLocations.Num()) + " HUD locations");
}

void AInteractableActor::PlaceHUDToNearestLocation(bool ignoreHUDVisibility)
{
	if (!bHUDVisible && !ignoreHUDVisibility) return;
	if (HUDLocations.Num() == 0) return;

	const APawn* OwningPawn = GetWorld()->GetFirstPlayerController()->GetPawn();
	if (!OwningPawn) return;

	UCameraComponent* Camera = OwningPawn->FindComponentByClass<UCameraComponent>();
	if (!Camera) return;

	const FVector CameraLocation = Camera->GetComponentLocation();
	FVector NearestLocation = HUDScene->GetComponentLocation();
	float NearestDistance = FVector::DistSquared(CameraLocation, NearestLocation);

	for (FVector Location : HUDLocations)
	{
		const float Distance = FVector::DistSquared(CameraLocation, Location);

		if (Distance < NearestDistance)
		{
			NearestLocation = Location;
			NearestDistance = Distance;
		}
	}

	// Print message with nearest location
	if (bDebug) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, GetName() + " is moving to the nearest HUD location");

	// Move the HUD to the nearest location
	HUDScene->SetWorldLocation(NearestLocation);
}

void AInteractableActor::RotateHUD(bool ignoreHUDVisibility)
{
	if (!bRotateHUD && !ignoreHUDVisibility) return;
	if (!bHUDVisible) return;
	
	APawn* OwningPawn = GetWorld()->GetFirstPlayerController()->GetPawn();
	if (!OwningPawn) return;

	UCameraComponent* Camera = OwningPawn->FindComponentByClass<UCameraComponent>();
	if (!Camera) return;

	HUDScene->SetWorldRotation(Camera->GetComponentRotation());
}
