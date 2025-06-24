#include "Utils.hpp"

using namespace geode::prelude;

ccColor4F colorFromPortalType(PortalType type) {
    switch (type) {
        case PortalType::Cube: return ccc4f(0.4f, 1.0f, 0.2f, 1.0f);
        case PortalType::Ship: return ccc4f(1.0f, 0.2f, 1.0f, 1.0f);
        case PortalType::Ball: return ccc4f(1.0f, 0.2f, 0.2f, 1.0f);
        case PortalType::Ufo: return ccc4f(0.95f, 0.6f, 0.04f, 1.0f);
        case PortalType::Wave: return ccc4f(0.01f, 0.8f, 1.0f, 1.0f);
        case PortalType::Robot: return ccc4f(0.9f, 0.9f, 0.9f, 1.0f);
        case PortalType::Spider: return ccc4f(0.4f, 0.1f, 0.85f, 1.0f);
        case PortalType::Swing: return ccc4f(1.0f, 1.0f, 0.5f, 1.0f);
    }
    return ccc4f(1.0f, 1.0f, 1.0f, 1.0f); // schizo real
}

std::string ftofstr(float num, int rounding) {
    auto str = numToString(num, rounding);
    auto end = str.find_last_not_of('0');
    if (end != std::string::npos) str.erase(end + 1);
    if (!str.empty() && str.back() == '.') str.pop_back();
    return str;
}