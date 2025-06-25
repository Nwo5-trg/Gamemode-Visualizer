#pragma once

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

struct ObjectStruct {
    int type;
    cocos2d::CCPoint pos;

    ObjectStruct(int ty, cocos2d::CCPoint po) {
        type = ty;
        pos = po;
    }
};

inline const std::unordered_map<int, int> portalIDMap = {
    {12, 0}, {13, 1}, {47, 2},
    {111, 3}, {660, 4}, {745, 5},
    {1331, 6}, {1933, 7}
};

inline const std::unordered_map<int, std::string> portalStringMap = {
    {0, "Cube"}, {1, "Ship"}, {2, "Ball"},
    {3, "Ufo"}, {4, "Wave"}, {5, "Robot"},
    {6, "Spider"}, {7, "Swing"}
};

 // ery - for some absurd stinky reason i need to swap slow and normal so that level settings are interpreted properly?????????????? fuck this game fuck you robtop
inline const std::unordered_map<int, int> speedIDMap = {
    {201, 1}, {201, 0}, {202, 2},
    {203, 3}, {1334, 4}
};

inline const std::unordered_map<int, std::string> speedStringMap = {
    {1, "0.5x"}, {0, "1x"}, {2, "2x"},
    {3, "3x"}, {4, "4x"}
};

inline const std::unordered_map<int, int> sizePortalIDMap = {
    {101, 1}, {99, 0}
};

inline const std::unordered_map<int, std::string> sizePortalStringMap = {
    {1, "Mini"}, {0, "Normal"}
};

inline const std::unordered_map<int, int> dualPortalIDMap = {
    {286, 1}, {287, 0}
};

inline const std::unordered_map<int, std::string> dualPortalStringMap = {
    {1, "Dual"}, {0, "Undual"}
};

inline const std::unordered_set<int> incompatibleObjects = {
    747, // teleport portal (blue)
    748, // teleport portal (orange)
    1917, // reverse trigger
    2900, // arrow trigger
    3022, // teleport trigger
    3027, // teleport orb
};

cocos2d::ccColor4F colorFromPortalType(int type);
cocos2d::ccColor4F colorFromSpeedType(int speed);
cocos2d::ccColor4F colorFromSizePortal(int mini);
cocos2d::ccColor4F colorFromDualPortal(int dual);

std::string ftofstr(float num, int rounding);