// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PolygonalMapGenerator/Public/Maps/IslandMapGenerator.h"
#include "PolygonalMapGenerator/Public/Maps/Biomes/WhittakerBiomeManager.h"
#include "SandboxTerrainController.h"
#include "PolygonalTerrainController.generated.h"

class UTerrainZoneComponent;

USTRUCT()
struct FBiomeFoliageList
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Terrain Foliage")
	TArray<int32> BiomeFoliageTypes;
};

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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UnrealSandbox Terrain Foliage")
	TMap<EWhittakerBiome, FBiomeFoliageList> BiomeFoliageMap;

	virtual void BeginPlay() override;
	UFUNCTION()
	virtual void InitializeController() override;
	virtual SandboxVoxelGenerator* newTerrainGenerator(TVoxelData &voxel_data) override;
	virtual void GenerateNewFoliage(UTerrainZoneComponent* Zone) override;
};
