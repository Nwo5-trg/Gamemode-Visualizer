#pragma once

namespace Variables {
    inline bool enabled;
    inline bool separators;
    inline bool tryHandleEndTrigger;

    inline float separatorThickness;
    inline float height;
    inline float width;
    inline float minimumLabelSize;

    inline cocos2d::ccColor4F seperatorCol;

    void updateSettings();
};