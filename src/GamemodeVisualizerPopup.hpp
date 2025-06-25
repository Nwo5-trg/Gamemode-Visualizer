#pragma once

struct DrawSegmentStruct;

class GamemodeVisualizerPopup : public geode::Popup<> {
private:
    GJGameLevel* m_level;

    cocos2d::CCDrawNode* m_draw;
    cocos2d::CCDrawNode* m_draw2;
    cocos2d::CCLayer* m_labelLayer;
    cocos2d::CCLabelBMFont* m_disclaimerLabel;
    cocos2d::CCLabelBMFont* m_gamemodeDistributionLabel;
    cocos2d::CCLabelBMFont* m_extraDistributionLabel;
    std::vector<DrawSegmentStruct> m_portalSegments;
    std::vector<DrawSegmentStruct> m_speedSegments;
    std::vector<DrawSegmentStruct> m_sizeSegments;
    std::vector<DrawSegmentStruct> m_dualSegments;

    bool m_labelType = false;
    bool m_showDisclaimer = false;
    int m_extraPortalsType = 0;

    void onToggleLabelLayer(cocos2d::CCObject* sender);
    void onChangeLabelType(cocos2d::CCObject* sender);
    void onChangeExtraPortalsType(cocos2d::CCObject* sender);
    
public:
    static GamemodeVisualizerPopup* create(GJGameLevel* level);
    bool setup() override;

    void drawVisualizer();
};
