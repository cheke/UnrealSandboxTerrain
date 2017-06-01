// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PolygonalMapGenerator/Public/Maps/IslandMapGenerator.h"
#include "SandboxTerrainController.h"
#include "PolygonalTerrainController.generated.h"

/**
 * 
 */
UCLASS()
class UNREALSANDBOXTERRAIN_API APolygonalTerrainController : public ASandboxTerrainController
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Polygonal Map Generation")
	AIslandMapGenerator* PolygonalMapGenerator;

	virtual void BeginPlay() override;
	UFUNCTION()
	virtual void InitializeController() override;
	virtual SandboxVoxelGenerator* newTerrainGenerator(TVoxelData &voxel_data) override;
};
