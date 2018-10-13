// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "PlayerPawn.generated.h"

class ATetrisGameMode;
struct FTetrisTheme;
class ABoard;

UCLASS()
class TETRIS_API APlayerPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APlayerPawn();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	void CreateBoard();
	void DestroyBoard();
	void ResetBoard();

	// Event Callbacks
	void OnSetTheme( const FTetrisTheme& theme );
	void OnBoardPlaceTetromino();
	void OnBoardClearLines123(int8 numLines);
	void OnBoardClearTetris();
	void OnBoardGameOver();

	ATetrisGameMode* m_gameMode;
	ABoard* m_board;

public:
	UPROPERTY(EditAnywhere, Category = "Tetris Actors")
	TSubclassOf<ABoard> BoardClass;

	UFUNCTION(BlueprintCallable, Category = "Tetris")
	ABoard* GetBoard() const { return m_board; }
};
