#pragma once

struct DrawSegmentStruct;

class GamemodeVisualizerPopup : public geode::Popup<> {
private:
    GJGameLevel* m_level;

    cocos2d::CCDrawNode* m_portalDraw;
    cocos2d::CCDrawNode* m_speedDraw;
    cocos2d::CCLayer* m_labelLayer;
    cocos2d::CCLabelBMFont* m_gamemodeDistributionLabel;
    std::vector<std::vector<DrawSegmentStruct>> m_segments;

    int m_labelType = 0;

    void onToggleLabelLayer(cocos2d::CCObject* sender);
    void onChangeLabelType(cocos2d::CCObject* sender);
    
public:
    static GamemodeVisualizerPopup* create(GJGameLevel* level);
    bool setup() override;

    void drawVisualizer();
    void drawSegmentGroup(std::vector<DrawSegmentStruct> segmentGroup, CCDrawNode* drawNode);
};
