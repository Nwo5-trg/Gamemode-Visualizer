#pragma once

class GamemodeVisualizerPopup : public geode::Popup<> {
private:
    GJGameLevel* m_level;

    cocos2d::CCDrawNode* m_draw;
    cocos2d::CCLayer* m_labelLayer;
    cocos2d::CCLabelBMFont* m_gamemodeDistributionLabel;

    int m_labelType = 0;

    void onToggleLabelLayer(cocos2d::CCObject* sender);
    void onChangeLabelType(cocos2d::CCObject* sender);
    
public:
    static GamemodeVisualizerPopup* create(GJGameLevel* level);
    bool setup() override;

    void drawVisualizer();
};
