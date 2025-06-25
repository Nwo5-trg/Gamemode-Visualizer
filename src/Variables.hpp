#pragma once

namespace Variables {
    inline bool hasDirectionalChanges = false;
    inline bool endsEarly = false;

    inline bool enabled = true;
    inline bool separators = true;

    inline float separatorThickness = 1.0f;
    inline float height = 325.0f;
    inline float width = 100.0f;
    inline float minimumLabelSize = 1.0f;

    inline cocos2d::ccColor4F seperatorCol;

    void updateSettings();
};