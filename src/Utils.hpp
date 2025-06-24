#pragma once

enum class PortalType {
    Cube = 0,
    Ship = 1,
    Ball = 2,
    Ufo = 3,
    Wave = 4,
    Robot = 5,
    Spider = 6,
    Swing = 7
};

struct DrawSegmentStruct {
    float start;
    float end;
    cocos2d::ccColor4F col;
    PortalType type;

    DrawSegmentStruct(float st, float en, cocos2d::ccColor4F co, PortalType ty) {
        start = st;
        end = en;
        col = co;
        type = ty;
    }
};

struct PortalStruct {
    PortalType type;
    cocos2d::CCPoint pos;

    PortalStruct(PortalType ty, cocos2d::CCPoint po) {
        type = ty;
        pos = po;
    }
};

inline const std::unordered_map<int, PortalType> portalIDMap = {
    {12, PortalType::Cube}, {13, PortalType::Ship}, {47, PortalType::Ball},
    {111, PortalType::Ufo}, {660, PortalType::Wave}, {745, PortalType::Robot},
    {1331, PortalType::Spider}, {1933, PortalType::Swing}
};

inline const std::unordered_map<PortalType, std::string> portalStringMap = {
    {PortalType::Cube, "Cube"}, {PortalType::Ship, "Ship"}, {PortalType::Ball, "Ball"},
    {PortalType::Ufo, "Ufo"}, {PortalType::Wave, "Wave"}, {PortalType::Robot, "Robot"},
    {PortalType::Spider, "Spider"}, {PortalType::Swing, "Swing"}
};

cocos2d::ccColor4F colorFromPortalType(PortalType type);

std::string ftofstr(float num, int rounding);