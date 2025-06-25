#include "Utils.hpp"

using namespace geode::prelude;

ccColor4F colorFromPortalType(int type) {
    switch (type) {
        case 0: return ccc4f(0.4f, 1.0f, 0.2f, 1.0f);
        case 1: return ccc4f(1.0f, 0.2f, 1.0f, 1.0f);
        case 2: return ccc4f(1.0f, 0.2f, 0.2f, 1.0f);
        case 3: return ccc4f(0.95f, 0.6f, 0.04f, 1.0f);
        case 4: return ccc4f(0.01f, 0.8f, 1.0f, 1.0f);
        case 5: return ccc4f(0.9f, 0.9f, 0.9f, 1.0f);
        case 6: return ccc4f(0.4f, 0.1f, 0.85f, 1.0f);
        case 7: return ccc4f(1.0f, 1.0f, 0.5f, 1.0f);
    }
    return ccc4f(1.0f, 1.0f, 1.0f, 1.0f); // schizo real
}

ccColor4F colorFromSpeedType(int speed) {
    switch (speed) {
        case 1: return ccc4f(1.0f, 0.85f, 0.0f, 1.0f);
        case 0: return ccc4f(0.0f, 0.898f, 1.0f, 1.0f);
        case 2: return ccc4f(0.2f, 0.9f, 0.0f, 1.0f);
        case 3: return ccc4f(1.0f, 0.45f, 1.0f, 1.0f);
        case 4: return ccc4f(0.9f, 0.0f, 0.0f, 1.0f);
    }
    return ccc4f(1.0f, 1.0f, 1.0f, 1.0f); // schizo real
}

ccColor4F colorFromSizePortal(int mini) {
    if (mini == 1) return ccc4f(1.0f, 0.0f, 0.8f, 1.0f);
    return ccc4f(0.0f, 1.0f, 0.25f, 1.0f);
}

ccColor4F colorFromDualPortal(int dual) {
    if (dual == 1) return ccc4f(1.0f, 0.5f, 0.0f, 1.0f);
    return ccc4f(0.0f, 0.8f, 1.0f, 1.0f);
}

std::string ftofstr(float num, int rounding) {
    auto str = numToString(num, rounding);
    auto end = str.find_last_not_of('0');
    if (end != std::string::npos) str.erase(end + 1);
    if (!str.empty() && str.back() == '.') str.pop_back();
    return str;
}