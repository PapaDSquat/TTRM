// Fill out your copyright notice in the Description page of Project Settings.

#include "Block.h"

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

const FBlockTheme& ABlock::GetTheme() const
{
	return m_theme;
}

void ABlock::SetTheme(const FBlockTheme& theme)
{
	m_theme = theme;

	if (m_meshComponent)
	{
		// TODO: Move to BP
		m_meshComponent->SetVectorParameterValueOnMaterials(FName(TEXT("Tint")), FVector( m_theme.MaterialParams.Tint ));
		m_meshComponent->SetScalarParameterValueOnMaterials(FName(TEXT("Opacity")), m_theme.MaterialParams.Opacity);
		
		if (m_theme.MaterialParams.Texture != nullptr)
		{
			const int32 matCount = m_meshComponent->GetNumMaterials();
			for (int32 i = 0; i < matCount; ++i)
			{
				if (UMaterialInstanceDynamic* material = Cast<UMaterialInstanceDynamic>(m_meshComponent->GetMaterial(i)))
				{
					material->SetTextureParameterValue("Texture", m_theme.MaterialParams.Texture);
				}
			}
		}
	}
}

