// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PolygonalMapGenerator/Public/Maps/IslandMapGenerator.h"
#include "PolygonalMapGenerator/Public/Maps/Biomes/WhittakerBiomeManager.h"
#include "UnrealSandboxTerrain/Public/SandboxVoxelGenerator.h"

/**
 * A class which generates voxels based on a triangle map
 */
class PolygonalVoxelGenerator : public SandboxVoxelGenerator
{
public:
	PolygonalVoxelGenerator(TVoxelData& vd, int32 Seed, UPolygonMap* PolygonMapData, UWhittakerBiomeManager* BiomeMapData);
	~PolygonalVoxelGenerator() {};

	virtual float density(FVector& local, FVector& world) override;
	virtual unsigned char material(FVector& local, FVector& world) override;

	static void ClearCachedPositions();

	// Smooth edges are fairly smooth and more "realistic", but also take more processing time
	// If smooth edges are not used, the resulting terrain is created much faster but has a "Minecraft"-esque look.
	bool bUseSmoothEdges;

protected:
	static TMap<FVector2D, float> CachedPositions;
	UPolygonMap* MapData;
	UWhittakerBiomeManager* BiomeData;

private:
	// Calculates the Z position of a triangle given an XY coordinate and the triangle vertices
	float CalculateZPosition(FVector p1, FVector p2, FVector p3, FVector2D xyLoc);
};
