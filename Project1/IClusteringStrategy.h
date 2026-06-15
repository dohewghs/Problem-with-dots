#pragma once
#include "Points.h"
#include "AppConfig.h"
#include "ClusteringResult.h"

class IClusteringStrategy
{
public:
	virtual ~IClusteringStrategy() = default;
	
	virtual ClusteringResult proccess(const Points& source, const AppConfig& config) = 0;
};