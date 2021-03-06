// Fill out your copyright notice in the Description page of Project Settings.

#include "UnrealSandboxTerrainPrivatePCH.h"
#include "PolygonalVoxelGenerator.h"
#include "PolygonalMapGenerator/Public/Maps/MapDataHelper.h"
#include "UnrealSandboxTerrain/Public/SandboxVoxelGenerator.h"

TMap<FVector2D, float> PolygonalVoxelGenerator::CachedPositions = TMap<FVector2D, float>();

PolygonalVoxelGenerator::PolygonalVoxelGenerator(TVoxelData& vd, int32 Seed, UPolygonMap* PolygonMapData, UWhittakerBiomeManager* BiomeMapData) : SandboxVoxelGenerator(vd, Seed)
{
	MapData = PolygonMapData;
	BiomeData = BiomeMapData;
	bUseSmoothEdges = false;
}

float PolygonalVoxelGenerator::CalculateZPosition(FVector p1, FVector p2, FVector p3, FVector2D xyLoc)
{
	float y1 = p1.Y;
	float y2 = p2.Y;
	float y3 = p3.Y;

	float x1 = p1.X;
	float x2 = p2.X;
	float x3 = p3.X;

	// Calculate determinant
	float det = (y2 - y3) * (x1 - x3) + (x3 - x2) * (y1 - y3);
	if (det == 0.0f)
	{
		// Shouldn't happen, but okay
		return 0.0f;
	}

	float x = xyLoc.X;
	float y = xyLoc.Y;

	// https://stackoverflow.com/questions/36090269/finding-height-of-point-on-height-map-triangles
	float lambda1 = ((y2 - y3) * (x - x3) + (x3 - x2) * (y - y3)) / det;
	float lambda2 = ((y3 - y1) * (x - x3) + (x1 - x3) * (y - y3)) / det;
	float lambda3 = 1 - lambda1 - lambda2;

	float z1 = p1.Z;
	float z2 = p2.Z;
	float z3 = p3.Z;

	// Calculate Z coordinate
	return lambda1 * z1 + lambda2 * z2 + lambda3 * z3;
}

float PolygonalVoxelGenerator::density(FVector& local, FVector& world)
{
	FVector2D mapCoordinates = MapData->ConvertWorldPointToGraphSpace(world);

	if (!bUseSmoothEdges)
	{
		mapCoordinates.X = FMath::RoundToInt(mapCoordinates.X);
		mapCoordinates.Y = FMath::RoundToInt(mapCoordinates.Y);
	}
	float groundLevel;

	if (CachedPositions.Contains(mapCoordinates))
	{
		groundLevel = CachedPositions[mapCoordinates];
	}
	else
	{
		FMapCorner corner = MapData->FindMapCornerForCoordinate(mapCoordinates);
		if (corner.Index < 0)
		{
			// Not a valid corner
			CachedPositions.Add(mapCoordinates, 0.0f);
			return 0.0f;
		}

		FMapCenter pointACenter = MapData->GetCenter(corner.Touches[0]);
		FMapCenter pointBCenter = MapData->GetCenter(corner.Touches[1]);
		FMapCenter pointCCenter = MapData->GetCenter(corner.Touches[2]);

		FVector pointA = FVector(pointACenter.CenterData.Point.X, pointACenter.CenterData.Point.Y, pointACenter.CenterData.Elevation);
		FVector pointB = FVector(pointBCenter.CenterData.Point.X, pointBCenter.CenterData.Point.Y, pointBCenter.CenterData.Elevation);
		FVector pointC = FVector(pointCCenter.CenterData.Point.X, pointCCenter.CenterData.Point.Y, pointCCenter.CenterData.Elevation);
		
		FWorldSpaceMapData worldData = MapData->WorldData;
		groundLevel = (CalculateZPosition(pointA, pointB, pointC, mapCoordinates) * worldData.ElevationScale) + worldData.ElevationOffset;
		
		//UE_LOG(LogTemp, Warning, TEXT("Elevation at coordinates (%f, %f): %f; Biome: %s"), mapCoordinates.X, mapCoordinates.Y, groundLevel, *corner.CornerData.Biome.ToString());
		
		CachedPositions.Add(mapCoordinates, groundLevel);
	}

	float val = 1;
	if (world.Z > groundLevel + 400)
	{
		val = 0;
	}
	else if (world.Z > groundLevel)
	{
		float d = (1 / (world.Z - groundLevel)) * 100.0f;
		val = d;
	}

	if (val > 1)
	{
		val = 1;
	}

	if (val < 0.003f)
	{ // minimal density = 1f/255
		val = 0.0f;
	}

	return val;
}

unsigned char PolygonalVoxelGenerator::material(FVector& local, FVector& world) {
	/*FVector test = FVector(world);
	test.Z += 30;

	float densityUpper = density(test, world);*/
	return (unsigned char)BiomeData->WhittakerBiomeFromWorldPoint(MapData, world);

	/*unsigned char mat;
	if (CachedBiomes.Contains(mapCoordinates))
	{
		mat = (unsigned char)CachedBiomes[mapCoordinates];
		UE_LOG(LogTemp, Log, TEXT("Material ID: %d"), mat);
	}
	else
	{
		mat = (unsigned char)EWhittakerBiome::Ocean;
	}
	return mat;

	/*float pointDensity;
	if (CachedPositions.Contains(mapCoordinates))
	{
		pointDensity = CachedPositions[mapCoordinates];
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Material cache miss!"));
		pointDensity = density(local, world);
	}*/

	/*unsigned char mat = 0;

	if (pointDensity < 0.0f)
	{
		// Guaranteed to have this position in the biome lookup, since density is not 0
		mat = (unsigned char)CachedBiomes[mapCoordinates];
	}
	else
	{
		if (world.Z < -350)
		{
			mat = (unsigned char)EWhittakerBiome::Scorched; // Rock
		}
		else
		{
			mat = (unsigned char)EWhittakerBiome::Bare; // Dirt
		}
	}
	return mat;*/
}

void PolygonalVoxelGenerator::ClearCachedPositions()
{
	CachedPositions.Empty();
}