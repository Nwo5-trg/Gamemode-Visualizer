#include "Parse.hpp"
#include "Utils.hpp"
#include "Variables.hpp"

using namespace geode::prelude;

std::vector<std::vector<DrawSegmentStruct>> createDrawSegmentsFrom(GJGameLevel* level) {
    auto splitLevelString = string::split(ZipUtils::decompressString(level->m_levelString, true, 0), ";"); // ethically sourced from literal level lengths

    std::map<float, ObjectStruct> portalStructs; // sorting :3
    std::map<float, ObjectStruct> speedStructs;
    std::map<float, ObjectStruct> sizeStructs;
    std::map<float, ObjectStruct> dualStructs;

    auto levelSettings = splitLevelString[0];

    portalStructs.insert({0.0f, {getLevelSetting("kA2,", levelSettings), {0.0f, 0.0f}}});
    speedStructs.insert({0.0f, {getLevelSetting("kA4,", levelSettings), {0.0f, 0.0f}}});
    sizeStructs.insert({0.0f, {getLevelSetting("kA3,", levelSettings), {0.0f, 0.0f}}});
    dualStructs.insert({0.0f, {getLevelSetting("kA8,", levelSettings), {0.0f, 0.0f}}});
    
    splitLevelString.erase(splitLevelString.begin());

    bool hasIncompatibleObjects = false;
    bool hasEndTrigger = false;
    float endTriggerPos = FLT_MAX;

    float maxX = -FLT_MAX;

    // you just gotta believe
    for (const auto& objString : splitLevelString) {
        auto splitObjString = string::split(objString, ",");

        if (splitObjString.size() < 6) continue;

        auto pos = ccp(std::strtof(splitObjString[3].c_str(), nullptr), std::strtof(splitObjString[5].c_str(), nullptr));
        if (pos.x < 0) continue;
        if (pos.x > maxX) maxX = pos.x;

        int id = std::strtol(splitObjString[1].c_str(), nullptr, 10);
        if (portalIDMap.contains(id)) {
            portalStructs.insert({pos.x, {portalIDMap.at(id), pos}});
        } else if (speedIDMap.contains(id)) {
            speedStructs.insert({pos.x, {speedIDMap.at(id), pos}});
        } else if (sizePortalIDMap.contains(id)) {
            sizeStructs.insert({pos.x, {sizePortalIDMap.at(id), pos}});
        } else if (dualPortalIDMap.contains(id)) {
            sizeStructs.insert({pos.x, {dualPortalIDMap.at(id), pos}});
        }  else if (! hasIncompatibleObjects && incompatibleObjects.contains(id)) hasIncompatibleObjects = true;
        else if (Variables::tryHandleEndTrigger && !hasEndTrigger && id == 3600) {
            hasEndTrigger = true;
            endTriggerPos = pos.x;
        }
    }

    maxX = std::min(endTriggerPos, maxX);
    if (maxX == 0) maxX = 1;
    float percentageMultiplier = 100 / maxX;
    
    std::vector<std::vector<DrawSegmentStruct>> outputVector;
    std::map<float, ObjectStruct>* maps[4] = {&portalStructs, &speedStructs, &sizeStructs, &dualStructs};
    for (int i = 0; i < 4; i++) {
        auto& map = *maps[i];

        std::vector<ObjectStruct> vector;
        for (const auto& [key, obj] : map) vector.emplace_back(obj);

        std::vector<DrawSegmentStruct> drawSegmentVector;

        int j = 0;
        for (const auto& obj : vector) {
            float objPercentage = obj.pos.x * percentageMultiplier; // idc if this technically gets calculated twice for every portal i actually cant anymore
            auto col = colorFromPortalType(obj.type);
            switch (i) {
                case 1: col = colorFromSpeedType(obj.type); break;
                case 2: col = colorFromSizePortal(obj.type); break;
                case 3: col = colorFromDualPortal(obj.type); break;
            }

            if (j == vector.size() - 1) {
                drawSegmentVector.emplace_back(objPercentage, 100.0f, col, obj.type);
            } else {
                float nextObjPercentage = vector[j + 1].pos.x * percentageMultiplier;
                drawSegmentVector.emplace_back(objPercentage, nextObjPercentage, col, obj.type);
            }

            j++;
        };

        outputVector.emplace_back(drawSegmentVector);
    }

    if (hasIncompatibleObjects) outputVector.emplace_back(); // could use a global but like, this is way cooler
    
    return outputVector;
}

int getLevelSetting(const std::string& setting, const std::string& levelSettings) {
    auto start = levelSettings.find(setting);
    if (start != std::string::npos) {
        start += 4;
        auto end = levelSettings.find(",", start);
        if (end != std::string::npos) return std::strtol(levelSettings.substr(start, end - start).c_str(), nullptr, 10);
    }
    return 0;
}