#include "Parse.hpp"
#include "Utils.hpp"
#include "Variables.hpp"

using namespace geode::prelude;

std::vector<DrawSegmentStruct> mysupercoolandawesomeparsingfunctionthathasnoissuesorflawswhatsoeverthesearethepositiveaffirmationsitellmyselfinthemirror(GJGameLevel* level) {
    auto splitLevelString = string::split(ZipUtils::decompressString(level->m_levelString, true, 0), ";"); // ethically sourced from literal level lengths
    if (splitLevelString.size() < 2) return {{0.0f, 100.0f, {1.0f, 1.0f, 1.0f, 1.0f}, PortalType::Cube}}; // damn u cooked

    std::map<float, PortalStruct> portalStructs; // sorting :3

    { // you just gotta believe
        auto levelSettings = splitLevelString[0];

        auto start = levelSettings.find("kA2,");
        if (start != std::string::npos) {
            start += 4;
            auto end = levelSettings.find(",", start);
            portalStructs.insert({0.0f, {
                static_cast<PortalType>(geode::utils::numFromString<int>(levelSettings.substr(start, end - start), 10).unwrapOr(0L)), ccp(0.0f, 0.0f)
            }});
        }
    }
    
    splitLevelString.erase(splitLevelString.begin());

    float maxX = FLT_MIN;

    for (const auto& objString : splitLevelString) {
        auto splitObjString = string::split(objString, ",");

        if (splitObjString.size() < 6) continue;

        auto pos = ccp(geode::utils::numFromString<double>(splitObjString[3]), geode::utils::numFromString<double>(splitObjString[5]));
        if (pos.x < 0) continue;
        if (pos.x > maxX) maxX = pos.x;

        int id = geode::utils::numFromString<int>(splitObjString[1]);
        if (!hasDirectionalChanges && incompatibleTriggers.contains(id)) {
            hasDirectionalChanges = true;
            continue;
        }
        if (!endsEarly && id == endTrigger) {
            endsEarly = true;
            continue;
        }
        if (!portalIDMap.contains(id)) continue;

        portalStructs.insert({pos.x, {portalIDMap.at(id), pos}});
    }

    std::vector<PortalStruct> portalStructsVector;
    for (const auto& [key, portal] : portalStructs) portalStructsVector.emplace_back(portal);

    std::vector<DrawSegmentStruct> drawSegmentStructs;

    if (maxX == 0) maxX = 1;
    float percentageMultiplier = 100 / maxX;
    int i = 0;
    for (const auto& portal : portalStructsVector) {
        float portalPercentage = portal.pos.x * percentageMultiplier; // idc if this technically gets calculated twice for every portal i actually cant anymore
        auto col = colorFromPortalType(portal.type);

        if (i == portalStructsVector.size() - 1) {
            drawSegmentStructs.emplace_back(portalPercentage, 100.0f, col, portal.type);
        } else {
            float nextPortalPercentage = portalStructsVector[i + 1].pos.x * percentageMultiplier;
            drawSegmentStructs.emplace_back(portalPercentage, nextPortalPercentage, col, portal.type);
        }

        i++;
    };

    return drawSegmentStructs;
}