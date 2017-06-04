// Fill out your copyright notice in the Description page of Project Settings.

#include "UnrealSandboxTerrainPrivatePCH.h"
#include "PolygonalVoxelGenerator.h"
#include "TerrainZoneComponent.h"
#include "PolygonalTerrainController.h"


void APolygonalTerrainController::BeginPlay()
{
	AActor::BeginPlay();
	if (PolygonalMapGenerator == NULL)
	{
		UE_LOG(LogTemp, Error, TEXT("No polygonal map generator defined!"));
		return;
	}
	UE_LOG(LogTemp, Log, TEXT("Polygonal Terrain Controller -> Begin Play"));
	FIslandGeneratorDelegate initializeController;
	initializeController.BindDynamic(this, &APolygonalTerrainController::InitializeController);
	PolygonalMapGenerator->CreateMap(initializeController);
}

void APolygonalTerrainController::InitializeController()
{
	//PolygonalMapGenerator->DrawDelaunayGraph();
	PolygonalVoxelGenerator::ClearCachedPositions();
	Super::InitializeController();
}

SandboxVoxelGenerator* APolygonalTerrainController::newTerrainGenerator(TVoxelData &voxel_data)
{
	UE_LOG(LogTemp, Log, TEXT("Polygonal Terrain Controller -> Fetch Generator"));
	return new PolygonalVoxelGenerator(voxel_data, Seed, PolygonalMapGenerator->GetGraph(), (UWhittakerBiomeManager*)PolygonalMapGenerator->BiomeManager);
}

float groundLevel(FVector v);

void APolygonalTerrainController::GenerateNewFoliage(UTerrainZoneComponent* Zone)
{
	if (FoliageMap.Num() == 0) return;
	if (groundLevel(Zone->GetComponentLocation()) > Zone->GetComponentLocation().Z + 500) return;

	FRandomStream rnd = FRandomStream();
	rnd.Initialize(0);
	rnd.Reset();

	static const float s = 500;
	static const float step = 25;
	float counter = 0;

	FVector origin(Zone->getVoxelData()->getOrigin());
	EWhittakerBiome biome = ((UWhittakerBiomeManager*)PolygonalMapGenerator->BiomeManager)->WhittakerBiomeFromWorldPoint(PolygonalMapGenerator->GetGraph(), origin);
	if (!BiomeFoliageMap.Contains(biome))
	{
		return;
	}

	FBiomeFoliageList FoliageTypeList = BiomeFoliageMap[biome];
	TArray<int32> FoilageTypeIds = FoliageTypeList.BiomeFoliageTypes;

	for (auto x = -s; x <= s; x += step) {
		for (auto y = -s; y <= s; y += step) {
			FVector v = origin + FVector(x, y, 0);
			for (int i = 0; i < FoilageTypeIds.Num(); i++)
			{
				uint32 FoliageTypeId = (uint32)FoilageTypeIds[i];
				if(!FoliageMap.Contains(FoliageTypeId))
				{
					continue;
				}
				FSandboxFoliage FoliageType = FoliageMap[FoliageTypeId];

				float r = std::sqrt(v.X * v.X + v.Y * v.Y);
				if ((int)counter % (int)FoliageType.SpawnStep == 0)
				{
					SpawnFoliage(FoliageTypeId, FoliageType, v, rnd, Zone);
				}
			}

			counter += step;
		}
	}
}