#pragma once

#include "EngineMinimal.h"

#include "UnrealSandboxTerrain/Private/SandboxVoxeldata.h"
#include "UnrealSandboxTerrain/Private/SandboxPerlinNoise.h"

class SandboxVoxelGenerator {
private:
	bool cavern;

public:
	SandboxVoxelGenerator(TVoxelData& vd, int32 Seed);
	virtual float density(FVector& local, FVector& world);
	virtual unsigned char material(FVector& local, FVector& world);
};