#include "Parse.hpp"
#include "Utils.hpp"
#include "Variables.hpp"

using namespace geode::prelude;

std::vector<std::vector<DrawSegmentStruct>> createDrawSegmentsFrom(GJGameLevel* level) {
    auto splitLevelString = string::split(ZipUtils::decompressString(level->m_levelString, true, 0), ";"); // ethically sourced from literal level lengths
    if (splitLevelString.size() < 2) return {}; // damn u cooked

    std::map<float, PortalStruct> portalStructs; // sorting :3
    std::map<float, SpeedStruct> speedStructs; // sorting :3

    { // you just gotta believe
        auto levelSettings = splitLevelString[0];

        auto start = levelSettings.find("kA2,");
        if (start != std::string::npos) {
            start += 4;
            auto end = levelSettings.find(",", start);
            portalStructs.insert({0.0f, {
                static_cast<PortalType>(geode::utils::numFromString<int>(levelSettings.substr(start, end - start)).unwrapOr(0L)), ccp(0.0f, 0.0f)
            }});
        }

        start = levelSettings.find("kA4,");
        if (start != std::string::npos) {
            start += 4;
            auto end = levelSettings.find(",", start);
            speedStructs.insert({0.0f, {
                static_cast<SpeedType>(geode::utils::numFromString<int>(levelSettings.substr(start, end - start)).unwrapOr(1L)), ccp(0.0f, 0.0f)
            }});
        }
    }
    
    splitLevelString.erase(splitLevelString.begin());

    float maxX = FLT_MIN;

    for (const auto& objString : splitLevelString) {
        auto splitObjString = string::split(objString, ",");

        if (splitObjString.size() < 6) continue;

        auto pos = ccp(geode::utils::numFromString<float>(splitObjString[3]).unwrapOr(0.f), geode::utils::numFromString<float>(splitObjString[5]).unwrapOr(0.f));
        if (pos.x < 0) continue;
        if (pos.x > maxX) maxX = pos.x;

        int id = geode::utils::numFromString<int>(splitObjString[1]).unwrapOr(-1);
        if (!Variables::hasDirectionalChanges && incompatibleTriggers.contains(id)) {
            Variables::hasDirectionalChanges = true;
            continue;
        }
        if (!Variables::endsEarly && id == endTrigger) {
            Variables::endsEarly = true;
            continue;
        }
        if (!portalIDMap.contains(id) && !speedIDMap.contains(id)) continue;

        if (portalIDMap.contains(id)) portalStructs.insert({pos.x, {portalIDMap.at(id), pos}});
        else if (speedIDMap.contains(id)) speedStructs.insert({pos.x, {speedIDMap.at(id), pos}});
    }

    std::vector<PortalStruct> portalStructsVector;
    for (const auto& [key, portal] : portalStructs) portalStructsVector.emplace_back(portal);

    std::vector<SpeedStruct> speedStructsVector;
    for (const auto& [key, speed] : speedStructs) speedStructsVector.emplace_back(speed);

    std::vector<std::vector<DrawSegmentStruct>> segments;
    std::vector<DrawSegmentStruct> portalSegmentStructs;
    std::vector<DrawSegmentStruct> speedSegmentStructs;

    if (maxX == 0) maxX = 1;
    float percentageMultiplier = 100 / maxX;
    int i = 0;
    for (const auto& portal : portalStructsVector) {
        float portalPercentage = portal.pos.x * percentageMultiplier; // idc if this technically gets calculated twice for every portal i actually cant anymore
        auto col = colorFromPortalType(static_cast<PortalType>(portal.type));

        if (i == portalStructsVector.size() - 1) {
            portalSegmentStructs.emplace_back(portalPercentage, 100.0f, col, static_cast<int>(portal.type));
        } else {
            float nextPortalPercentage = portalStructsVector[i + 1].pos.x * percentageMultiplier;
            portalSegmentStructs.emplace_back(portalPercentage, nextPortalPercentage, col, static_cast<int>(portal.type));
        }

        i++;
    };
    
    i = 0;
    for (const auto& speed : speedStructsVector) {
        float speedPercentage = speed.pos.x * percentageMultiplier;
        auto col = colorFromSpeedType(static_cast<SpeedType>(speed.speed));

        if (i == speedStructsVector.size() - 1) {
            speedSegmentStructs.emplace_back(speedPercentage, 100.0f, col, static_cast<int>(speed.speed));
        } else {
            float nextSpeedPercentage = speedStructsVector[i + 1].pos.x * percentageMultiplier;
            speedSegmentStructs.emplace_back(speedPercentage, nextSpeedPercentage, col, static_cast<int>(speed.speed));
        }

        i++;
    };

    segments.emplace_back(portalSegmentStructs);
    segments.emplace_back(speedSegmentStructs);

    return segments;
}