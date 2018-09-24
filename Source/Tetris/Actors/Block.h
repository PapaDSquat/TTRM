// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Block.generated.h"

// TODO: Block Theme should be readable from somewhere external/
USTRUCT()
struct FBlockTheme
{
	GENERATED_USTRUCT_BODY()

	FBlockTheme();

	UPROPERTY(EditAnywhere, Category = "Block Theme")
	FColor Tint;

	UPROPERTY(EditAnywhere, Category = "Block Theme")
	float Opacity;
};

UCLASS()
class TETRIS_API ABlock : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABlock();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetTheme(const FBlockTheme& theme);
	
private:
	FBlockTheme m_theme;

	UStaticMeshComponent* m_meshComponent;

	
};
