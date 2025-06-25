#pragma once

struct DrawSegmentStruct;

std::vector<std::vector<DrawSegmentStruct>> createDrawSegmentsFrom(GJGameLevel* level);
int getLevelSetting(const std::string& setting, const std::string& levelSettings);