// Fill out your copyright notice in the Description page of Project Settings.

#include "UnrealSandboxTerrainPrivatePCH.h"
#include "PolygonalVoxelGenerator.h"
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