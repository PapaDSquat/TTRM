// Fill out your copyright notice in the Description page of Project Settings.

#include "Block.h"

FBlockTheme::FBlockTheme()
	: Tint(FColor::White)
	, Opacity(1.f)
{}


// Sets default values
ABlock::ABlock()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABlock::BeginPlay()
{
	Super::BeginPlay();

	TArray<UStaticMeshComponent*> Components;
	GetComponents<UStaticMeshComponent>(Components);
	for (int32 i = 0; i < Components.Num(); i++)
	{
		m_meshComponent = Components[i];
		// NOTE: Should only be one for now
	}
}

// Called every frame
void ABlock::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABlock::SetTheme(const FBlockTheme& theme)
{
	m_theme = theme;

	if (m_meshComponent)
	{
		m_meshComponent->SetVectorParameterValueOnMaterials(FName(TEXT("Tint")), FVector( m_theme.Tint ));
		m_meshComponent->SetScalarParameterValueOnMaterials(FName(TEXT("Opacity")), m_theme.Opacity);
	}
}

