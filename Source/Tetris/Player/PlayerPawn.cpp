// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerPawn.h"

#include "../Actors/Board.h"


// Sets default values
APlayerPawn::APlayerPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	

}

// Called when the game starts or when spawned
void APlayerPawn::BeginPlay()
{
	Super::BeginPlay();
	
	{
		FVector location(0.0f, 0.0f, 0.0f);
		FRotator rotation(0.0f, 0.0f, 0.0f);
		FActorSpawnParameters spawnInfo;
		spawnInfo.Owner = this;
		m_board = GetWorld()->SpawnActor< ABoard >(BoardClass, location, rotation, spawnInfo);

		FAttachmentTransformRules attachRules(EAttachmentRule::KeepRelative, false);
		m_board->AttachToActor(this, attachRules);
	}

	if (InputComponent)
	{
		InputComponent->BindAction("MoveLeft", IE_Pressed, m_board, &ABoard::MoveLeft);
		InputComponent->BindAction("MoveRight", IE_Pressed, m_board, &ABoard::MoveRight);
		InputComponent->BindAction("MoveDown", IE_Pressed, m_board, &ABoard::MoveDown);
		InputComponent->BindAction("RotateCW", IE_Pressed, m_board, &ABoard::RotateCW);
		InputComponent->BindAction("RotateCCW", IE_Pressed, m_board, &ABoard::RotateCCW);
		InputComponent->BindAction("Drop", IE_Pressed, m_board, &ABoard::Drop);
	}

}

// Called every frame
void APlayerPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// TODO: Move to GameMode


}

// Called to bind functionality to input
void APlayerPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}