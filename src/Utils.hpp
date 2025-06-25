#pragma once

enum class GameplayPortalType {
    Portals = 0,
    Speeds = 0
};

enum class PortalType {
    Cube = 0,
    Ship = 1,
    Ball = 2,
    UFO = 3,
    Wave = 4,
    Robot = 5,
    Spider = 6,
    Swing = 7
};

enum class SpeedType {
    // for some absurd stinky reason i need to swap slow and normal so that level settings are interpreted properly?????????????? fuck this game fuck you robtop
    Normal = 0,
    Slow = 1,
    Double = 2,
    Triple = 3,
    Quadruple = 4
};

struct DrawSegmentStruct {
    float start;
    float end;
    cocos2d::ccColor4F col;
    int type;

    DrawSegmentStruct(float st, float en, cocos2d::ccColor4F co, int ty) {
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

struct SpeedStruct {
    SpeedType speed;
    cocos2d::CCPoint pos;

    SpeedStruct(SpeedType spd, cocos2d::CCPoint po) {
        speed = spd;
        pos = po;
    }
};

inline const std::unordered_map<int, PortalType> portalIDMap = {
    {12, PortalType::Cube}, {13, PortalType::Ship}, {47, PortalType::Ball},
    {111, PortalType::UFO}, {660, PortalType::Wave}, {745, PortalType::Robot},
    {1331, PortalType::Spider}, {1933, PortalType::Swing}
};

inline const std::unordered_map<PortalType, std::string> portalStringMap = {
    {PortalType::Cube, "Cube"}, {PortalType::Ship, "Ship"}, {PortalType::Ball, "Ball"},
    {PortalType::UFO, "UFO"}, {PortalType::Wave, "Wave"}, {PortalType::Robot, "Robot"},
    {PortalType::Spider, "Spider"}, {PortalType::Swing, "Swing"}
};

inline const std::unordered_map<PortalType, std::string> portalSpriteFrameMap = {
    {PortalType::Cube, "portal_03_extra_2_001-uhd.png"}, {PortalType::Ship, "portal_04_extra_2_001-uhd.png"}, {PortalType::Ball, "portal_07_extra_2_001-uhd.png"},
    {PortalType::UFO, "portal_10_extra_2_001-uhd.png"}, {PortalType::Wave, "portal_13_extra_2_001-uhd.png"}, {PortalType::Robot, "portal_14_extra_2_001-uhd.png"},
    {PortalType::Spider, "portal_17_extra_2_001-uhd.png"}, {PortalType::Swing, "portal_18_extra_2_001-uhd.png"}
};

inline const std::unordered_map<int, SpeedType> speedIDMap = {
    {200, SpeedType::Slow}, {201, SpeedType::Normal}, {202, SpeedType::Double},
    {203, SpeedType::Triple}, {1334, SpeedType::Quadruple}
};

inline const std::unordered_map<SpeedType, std::string> speedStringMap = {
    {SpeedType::Slow, ".5x"}, {SpeedType::Normal, "1x"}, {SpeedType::Double, "2x"},
    {SpeedType::Triple, "3x"}, {SpeedType::Quadruple, "4x"}
};

inline const std::unordered_map<SpeedType, std::string> speedSpriteFrameMap = {
    {SpeedType::Slow, "boost_01_001.png"}, {SpeedType::Normal, "boost_02_001.png"}, {SpeedType::Double, "boost_03_001.png"},
    {SpeedType::Triple, "boost_04_001.png"}, {SpeedType::Quadruple, "boost_05_001.png"}
};

inline const std::set incompatibleObjects = {
    747,    // teleport portal (blue)
    748,    // teleport portal (orange)
    1917,   // reverse trigger
    2900,   // arrow trigger
    3022,   // teleport trigger
    3027,   // teleport orb
};

inline const int endTrigger = 3600;

cocos2d::ccColor4F colorFromPortalType(PortalType type);
cocos2d::ccColor4F colorFromSpeedType(SpeedType speed);

std::string ftofstr(float num, int rounding);