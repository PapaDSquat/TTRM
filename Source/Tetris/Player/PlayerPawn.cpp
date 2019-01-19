// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerPawn.h"
#include "Kismet/GameplayStatics.h"
#include "../Player/TetrisPlayerState.h"
#include "../Actors/Board.h"
#include "../GameMode/TetrisGameMode.h"
#include "../Audio/TetrisAudioManager.h"
#include "../Theme/TetrisTheme.h"
#include "../TetrisGameInstance.h"

APlayerPawn::APlayerPawn()
	: TimeBetweenHoldMoves( 0.1f )
	, TimeAfterFirstHoldMove( 0.25f )
{
	PrimaryActorTick.bCanEverTick = true;
}

void APlayerPawn::BeginPlay()
{
	Super::BeginPlay();

	m_gameMode = (ATetrisGameMode*)GetWorld()->GetAuthGameMode();
	m_gameMode->OnGameEvent().AddUObject(this, &APlayerPawn::OnGameEvent);

	SetupBoard();

	m_gameMode->StartGame();
}

void APlayerPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Update inputs
	if (m_timeBeforeMoveH > 0.f) m_timeBeforeMoveH -= DeltaTime;
	if (m_timeBeforeMoveV > 0.f) m_timeBeforeMoveV -= DeltaTime;
}

void APlayerPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("MoveLeft", IE_Pressed, this, &APlayerPawn::MoveLeft);
	PlayerInputComponent->BindAction("MoveLeft", IE_Repeat, this, &APlayerPawn::MoveLeftHold);
	PlayerInputComponent->BindAxis("MoveAxisHorizontal", this, &APlayerPawn::MoveAxisHorizontal);
	PlayerInputComponent->BindAction("MoveRight", IE_Pressed, this, &APlayerPawn::MoveRight);
	PlayerInputComponent->BindAction("MoveRight", IE_Repeat, this, &APlayerPawn::MoveRightHold);
	PlayerInputComponent->BindAxis("MoveAxisVertical", this, &APlayerPawn::MoveAxisVertical);
	PlayerInputComponent->BindAction("MoveDown", IE_Pressed, this, &APlayerPawn::MoveDown);
	PlayerInputComponent->BindAction("MoveDown", IE_Repeat, this, &APlayerPawn::MoveDownHold);
	PlayerInputComponent->BindAction("RotateCW", IE_Pressed, this, &APlayerPawn::RotateCW);
	PlayerInputComponent->BindAction("RotateCCW", IE_Pressed, this, &APlayerPawn::RotateCCW);
	PlayerInputComponent->BindAction("Drop", IE_Pressed, this, &APlayerPawn::Drop);
	PlayerInputComponent->BindAction("Hold", IE_Pressed, this, &APlayerPawn::Hold);

	PlayerInputComponent->BindAction("MoveLeft", IE_Released, this, &APlayerPawn::ReleaseHorizontal);
	PlayerInputComponent->BindAction("MoveRight", IE_Released, this, &APlayerPawn::ReleaseHorizontal);
	PlayerInputComponent->BindAction("MoveAxisHorizontal", IE_Released, this, &APlayerPawn::ReleaseHorizontal);
}

void APlayerPawn::SetupBoard()
{
	if (UChildActorComponent* comp = Cast<UChildActorComponent>(GetComponentByClass(UChildActorComponent::StaticClass())))
	{
		m_board = Cast<ABoard>(comp->GetChildActor());
	}

	if (m_board == nullptr && BoardClass != nullptr)
	{
		FVector location(0.0f, 0.0f, 0.0f);
		FRotator rotation(0.0f, 0.0f, 0.0f);
		FActorSpawnParameters spawnInfo;
		spawnInfo.Owner = this;
		m_board = GetWorld()->SpawnActor< ABoard >(BoardClass, location, rotation, spawnInfo);
	}

	if( m_board )
	{
		FAttachmentTransformRules attachRules(EAttachmentRule::KeepRelative, false);
		m_board->AttachToActor(this, attachRules);
		m_board->SetOwner(this);

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
	m_timeBeforeMoveH = m_lastMoveDirection == EMoveDirection::MAX ? TimeAfterFirstHoldMove : TimeBetweenHoldMoves;
	m_lastMoveDirection = EMoveDirection::Left;
}
void APlayerPawn::MoveRight()
{
	if (!IsGameActive()) return;
	if (m_board) m_board->MoveRight();
	m_timeBeforeMoveH = m_lastMoveDirection == EMoveDirection::MAX ? TimeAfterFirstHoldMove : TimeBetweenHoldMoves;
	m_lastMoveDirection = EMoveDirection::Right;
}
void APlayerPawn::MoveDown()
{
	if (!IsGameActive()) return;
	if (m_board) m_board->MoveDown();
	m_timeBeforeMoveV = TimeBetweenHoldMoves;
	m_lastMoveDirection = EMoveDirection::Down;
}

void APlayerPawn::MoveLeftHold()
{
	if (CanAxisMove(EMoveDirection::Left)) MoveLeft();
}
void APlayerPawn::MoveRightHold()
{
	if (CanAxisMove(EMoveDirection::Right)) MoveRight();
}
void APlayerPawn::MoveDownHold()
{
	if (CanAxisMove(EMoveDirection::Down)) MoveDown();
}

void APlayerPawn::MoveAxisHorizontal(float value)
{
	if (value < 0.f) MoveLeftHold();
	else if (value > 0.f) MoveRightHold();
}

void APlayerPawn::MoveAxisVertical(float value)
{
	if (value == 0.f)
		m_canAxisDrop = true;
	else if( value < 0.f ) 
		MoveDownHold();
	else if (value > 0.f && m_canAxisDrop)
		Drop();
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
	m_canAxisDrop = false;
}

void APlayerPawn::Hold()
{
	if (!IsGameActive()) return;
	if (m_board) m_board->Hold();
}

void APlayerPawn::ReleaseHorizontal()
{
	m_lastMoveDirection = EMoveDirection::MAX;
	m_timeBeforeMoveH = 0.f;
}

bool APlayerPawn::CanAxisMove(EMoveDirection direction) const
{
	//if (direction != m_lastMoveDirection) 
	//	return true;
	if (direction == EMoveDirection::Down)
		return m_timeBeforeMoveV <= 0.f;
	return m_timeBeforeMoveH <= 0.f;
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

FPlayerRoundStats& APlayerPawn::GetRoundStats() const
{
	return Cast<ATetrisPlayerState>(PlayerState)->GetRoundStats();
}
