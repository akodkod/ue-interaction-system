// Fill out your copyright notice in the Description page of Project Settings.


#include "SimplyInteractableActor.h"


// Sets default values
ASimplyInteractableActor::ASimplyInteractableActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create a root component
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	// Create an interaction box
	InteractionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("InteractionBox"));
	InteractionBox->SetupAttachment(RootComponent);
	InteractionBox->SetRelativeLocation(FVector(0.f, 0.f, 25.f));
	InteractionBox->SetBoxExtent(FVector(50.f, 50.f, 50.f));
	InteractionBox->SetCollisionResponseToAllChannels(ECR_Ignore);
	InteractionBox->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Block);
	
	// Create a trigger preview box
	TriggerPreviewBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerPreviewBox"));
	TriggerPreviewBox->SetupAttachment(RootComponent);
	TriggerPreviewBox->SetRelativeLocation(FVector(0.f, 0.f, 25.f));
	TriggerPreviewBox->SetBoxExtent(FVector(250.f, 250.f, 50.f));
	TriggerPreviewBox->SetCollisionResponseToAllChannels(ECR_Ignore);
	TriggerPreviewBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
}

// Called when the game starts or when spawned
void ASimplyInteractableActor::BeginPlay()
{
	Super::BeginPlay();

	TriggerPreviewBox->OnComponentBeginOverlap.AddDynamic(this, &ASimplyInteractableActor::OnPlayerEnterTriggerPreviewBox);
	TriggerPreviewBox->OnComponentEndOverlap.AddDynamic(this, &ASimplyInteractableActor::OnPlayerLeaveTriggerPreviewBox);
}

// Called every frame
void ASimplyInteractableActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASimplyInteractableActor::OnPlayerEnterTriggerPreviewBox(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!Execute_CanPlayerInteract(this)) return;

	bPlayerIsNear = true;
	Execute_PlayerEntersPreviewArea(this);	
}

void ASimplyInteractableActor::OnPlayerLeaveTriggerPreviewBox(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (!bPlayerIsNear) return;

	bPlayerIsNear = false;
	Execute_PlayerLeavesPreviewArea(this);
}
