// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerPawn.h"
#include "Kismet/GameplayStatics.h"
#include "../Actors/Board.h"
#include "../GameMode/TetrisGameMode.h"
#include "../Audio/TetrisAudioManager.h"
#include "../Theme/TetrisTheme.h"
#include "../TetrisGameInstance.h"

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

	m_gameMode = (ATetrisGameMode*)GetWorld()->GetAuthGameMode();
	m_gameMode->OnGameEvent().AddUObject(this, &APlayerPawn::OnGameEvent);

	CreateBoard();

	m_gameMode->StartGame();
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

	PlayerInputComponent->BindAction("MoveLeft", IE_Pressed, this, &APlayerPawn::MoveLeft);
	PlayerInputComponent->BindAction("MoveRight", IE_Pressed, this, &APlayerPawn::MoveRight);
	PlayerInputComponent->BindAction("MoveDown", IE_Pressed, this, &APlayerPawn::MoveDown);
	PlayerInputComponent->BindAction("RotateCW", IE_Pressed, this, &APlayerPawn::RotateCW);
	PlayerInputComponent->BindAction("RotateCCW", IE_Pressed, this, &APlayerPawn::RotateCCW);
	PlayerInputComponent->BindAction("Drop", IE_Pressed, this, &APlayerPawn::Drop);
	PlayerInputComponent->BindAction("Hold", IE_Pressed, this, &APlayerPawn::Hold);
}

void APlayerPawn::CreateBoard()
{
	if (BoardClass != nullptr)
	{
		FVector location(0.0f, 0.0f, 0.0f);
		FRotator rotation(0.0f, 0.0f, 0.0f);
		FActorSpawnParameters spawnInfo;
		spawnInfo.Owner = this;
		m_board = GetWorld()->SpawnActor< ABoard >(BoardClass, location, rotation, spawnInfo);

		FAttachmentTransformRules attachRules(EAttachmentRule::KeepRelative, false);
		m_board->AttachToActor(this, attachRules);

		// Bind to events
		m_board->OnPlaceTetromino().AddUObject(this, &APlayerPawn::OnBoardPlaceTetromino);
		m_board->OnClearLines123().AddUObject(this, &APlayerPawn::OnBoardClearLines123);
		m_board->OnClearTetris().AddUObject(this, &APlayerPawn::OnBoardClearTetris);
		m_board->OnGameOver().AddUObject(this, &APlayerPawn::OnBoardGameOver);
	}
}

UTetrisGameInstance* APlayerPawn::GetTetrisGameInstance()
{
	return Cast<UTetrisGameInstance>(GetGameInstance());
}

bool APlayerPawn::IsGameActive() const
{
	return (m_gameMode->IsGameStarted() && !m_gameMode->IsGamePaused());
}

void APlayerPawn::MoveLeft()
{
	if (!IsGameActive()) return;
	if (m_board) m_board->MoveLeft();
}

void APlayerPawn::MoveRight()
{
	if (!IsGameActive()) return;
	if (m_board) m_board->MoveRight();
}

void APlayerPawn::MoveDown()
{
	if (!IsGameActive()) return;
	if (m_board) m_board->MoveDown();
}

void APlayerPawn::RotateCW()
{
	if (!IsGameActive()) return;
	if (m_board) m_board->RotateCW();
}

void APlayerPawn::RotateCCW()
{
	if (!IsGameActive()) return;
	if (m_board) m_board->RotateCCW();
}

void APlayerPawn::Drop()
{
	if (!IsGameActive()) return;
	if (m_board) m_board->Drop();
}

void APlayerPawn::Hold()
{
	if (!IsGameActive()) return;
	if (m_board) m_board->Hold();
}

void APlayerPawn::OnGameEvent(EGameEventType eventType)
{
	switch (eventType)
	{
	case EGameEventType::Start:
	{
		m_board->SetPaused(false);
		m_board->ResetBoard();
	}
	break;

	case EGameEventType::Pause:
	{
		m_board->SetPaused(true);
	}
	break;

	case EGameEventType::Unpause:
	{
		m_board->SetPaused(false);
	}
	break;

	case EGameEventType::GameOver:
	{
		GetTetrisGameInstance()->GetAudioManager()->PlaySound(m_gameMode->GetCurrentTheme().GameOverSound);
	}
	break;

	case EGameEventType::End:
	{
		m_board->SetPaused(true);
	}
	break;
	}
}

void APlayerPawn::OnBoardPlaceTetromino()
{
	GetTetrisGameInstance()->GetAudioManager()->PlaySound(m_gameMode->GetCurrentTheme().DropPieceSound);
}

void APlayerPawn::OnBoardClearLines123( int8 numLines )
{
	GetTetrisGameInstance()->GetAudioManager()->PlaySound(m_gameMode->GetCurrentTheme().ClearLinesSound);
}

void APlayerPawn::OnBoardClearTetris()
{
	GetTetrisGameInstance()->GetAudioManager()->PlaySound(m_gameMode->GetCurrentTheme().TetrisSound);
}

void APlayerPawn::OnBoardGameOver()
{
	GetTetrisGameInstance()->GetAudioManager()->PlaySound(m_gameMode->GetCurrentTheme().GameOverSound);
}
