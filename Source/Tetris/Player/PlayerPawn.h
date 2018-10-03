// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "PlayerPawn.generated.h"

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
	ABoard* m_board;

public:
	UPROPERTY(EditAnywhere, Category = "Tetris Actors")
	TSubclassOf<ABoard> BoardClass;

	UFUNCTION(BlueprintCallable, Category = "Tetris")
	ABoard* GetBoard() const { return m_board; }
};
