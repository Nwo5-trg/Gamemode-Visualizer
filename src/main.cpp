#include <Geode/Geode.hpp>
#include <Geode/modify/EditLevelLayer.hpp>
#include <Geode/modify/LevelInfoLayer.hpp>
#include "GamemodeVisualizerPopup.hpp"
#include "Variables.hpp"

using namespace geode::prelude;

class $modify(UwUInfoLayer, LevelInfoLayer) {
    bool init(GJGameLevel* level, bool challenge) {
        if (!LevelInfoLayer::init(level, challenge)) return false;
        if (m_level->isPlatformer() || !Variables::enabled) return true;
        if (auto menu = this->getChildByID("left-side-menu")) {
            auto button = CCMenuItemSpriteExtra::create(
                CircleButtonSprite::create(CCSprite::create("button.png"_spr), 
                CircleBaseColor::Green, CircleBaseSize::Medium), 
                this, menu_selector(UwUInfoLayer::onVisualizeGamemodes)
            );
            menu->addChild(button);
            menu->updateLayout();
        }
        return true;
    }

    void onVisualizeGamemodes(CCObject* sender) {
        Variables::updateSettings();
        GamemodeVisualizerPopup::create(m_level)->show();
    }
};

class $modify(EditUwULayer, EditLevelLayer) {
    bool init(GJGameLevel* p0) {
        if (!EditLevelLayer::init(p0)) return false;
        if (m_level->isPlatformer() || !Variables::enabled) return true;
        if (auto menu = this->getChildByID("folder-menu")) {
            auto button = CCMenuItemSpriteExtra::create(
                CircleButtonSprite::create(CCSprite::create("button.png"_spr),
                CircleBaseColor::Green, CircleBaseSize::Medium),
                this, menu_selector(EditUwULayer::onVisualizeGamemodes)
            );
            menu->addChild(button);
            menu->updateLayout();
        }
        return true;
    }

    void onVisualizeGamemodes(CCObject* sender) {
        Variables::updateSettings();
        GamemodeVisualizerPopup::create(m_level)->show();
    }
};