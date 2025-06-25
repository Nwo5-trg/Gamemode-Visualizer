#include "GamemodeVisualizerPopup.hpp"
#include "Variables.hpp"
#include "Parse.hpp"
#include "Utils.hpp"

using namespace geode::prelude;

GamemodeVisualizerPopup* GamemodeVisualizerPopup::create(GJGameLevel* level) { 
    auto ret = new GamemodeVisualizerPopup();
    ret->m_level = level;
    if (ret->initAnchored(Variables::width <= 325.0f ? 400.0f : Variables::width + 75.0f, Variables::height + 110.0f)) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

bool GamemodeVisualizerPopup::setup() {
    this->setTitle("Gamemode Visualizer :333", "goldFont.fnt");

    auto portalDraw = CCDrawNode::create();
    portalDraw->setBlendFunc({GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA});
    m_mainLayer->addChild(portalDraw);
    m_portalDraw = portalDraw;

    auto speedDraw = CCDrawNode::create();
    speedDraw->setBlendFunc({GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA});
    m_mainLayer->addChild(speedDraw);
    m_speedDraw = speedDraw;

    auto labelLayer = CCLayer::create();
    labelLayer->setPosition(0.0f, 0.0f);
    m_mainLayer->addChild(labelLayer);
    m_labelLayer = labelLayer;

    auto hideOnSpr = CCSprite::createWithSpriteFrameName("hideBtn_001.png");
    hideOnSpr->setOpacity(100);
    hideOnSpr->setColor(ccGRAY);
    auto hideLabelButton = CCMenuItemToggler::create(
        CCSprite::createWithSpriteFrameName("hideBtn_001.png"), hideOnSpr,
        this, menu_selector(GamemodeVisualizerPopup::onToggleLabelLayer)
    );
    hideLabelButton->setPosition(m_size.width - 20.0f, 20.0f);
    m_buttonMenu->addChild(hideLabelButton);

    auto changeLabelTypeButton = CCMenuItemSpriteExtra::create(
        CCSprite::createWithSpriteFrameName("GJ_redoBtn_001.png"),
        this, menu_selector(GamemodeVisualizerPopup::onChangeLabelType)
    );
    changeLabelTypeButton->setPosition(
        hideLabelButton->getPositionX() - hideLabelButton->getContentWidth() - 5.0f, 
        hideLabelButton->getPositionY()
    );
    changeLabelTypeButton->setScale(0.65f);
    changeLabelTypeButton->m_baseScale = 0.65f;
    m_buttonMenu->addChild(changeLabelTypeButton);

    auto gamemodeDistributionLabel = CCLabelBMFont::create("", "bigFont.fnt");
    gamemodeDistributionLabel->setAnchorPoint(ccp(0.0f, 0.0f));
    gamemodeDistributionLabel->setPosition(10.0f, 10.0f);
    m_mainLayer->addChild(gamemodeDistributionLabel);
    m_gamemodeDistributionLabel = gamemodeDistributionLabel;

    drawVisualizer();

    if (!Variables::hasDirectionalChanges && !Variables::endsEarly) return true;
    auto mayBeUnsupportedLabel = CCLabelBMFont::create("(This level could change directions and/or end early. Results may be inaccurate.)", "chatFont.fnt");
    mayBeUnsupportedLabel->setPosition(this->m_title->getPosition());
    mayBeUnsupportedLabel->setPositionY(mayBeUnsupportedLabel->getPositionY() - 16.f);
    mayBeUnsupportedLabel->limitLabelWidth(this->m_mainLayer->getContentSize().width * .75f, 1.f, 0.001f);
    m_mainLayer->addChild(mayBeUnsupportedLabel);

    return true;
}

void GamemodeVisualizerPopup::drawVisualizer() {
    m_portalDraw->clear();
    m_speedDraw->clear();
    m_labelLayer->removeAllChildrenWithCleanup(true);
    
    auto drawPos = ccp((m_size.width - (Variables::width * 100)) / 2, m_size.height - Variables::height - 50.0f);
    m_portalDraw->setPosition(drawPos);
    m_speedDraw->setPosition(drawPos);

    m_speedDraw->setPositionY(drawPos->getPositionY() - 25.f);

    if (m_segments.empty()) m_segments = createDrawSegmentsFrom(m_level);
    if (m_segments.empty()) return this->removeMeAndCleanup();
    
    drawSegmentGroupAtY(m_segments.at(0), m_portalDraw);
    drawSegmentGroupAtY(m_segments.at(1), m_speedDraw);

    // ok so idc if i loop thru the vector like 9 times cuz like im lazy and even if u have like 100 portals thats only like 900 iterations of rly basic operations so wtv

    std::string distributionString = "";
    for (int j = 0; j < 8; j++) {
        float totalPercentage = 0.0f;
        auto type = static_cast<PortalType>(j);
        for (const auto& segment : m_segments) {
            if (segment.type == type) totalPercentage += segment.end - segment.start;
        }
        if (totalPercentage == 0.0f) continue;
        distributionString += (portalStringMap.at(type) + ": " + ftofstr(totalPercentage, 2) + "%, ");
    }
    if (distributionString.length() > 2) distributionString.erase(distributionString.end() - 2);
    m_gamemodeDistributionLabel->setString(distributionString.c_str());
    m_gamemodeDistributionLabel->limitLabelWidth(m_size.width - 100.0f, 0.45f, 0.0f);
}

void GamemodeVisualizerPopup::drawSegmentGroup(std::vector<DrawSegmentStruct> segmentGroup, cocos2d::CCDrawNode* drawNode) {
    int i = 0;
    auto drawPos = drawNode->getPosition();
    for (const auto& segment : segmentGroup) {
        float start = segment.start * Variables::width;
        float end = segment.end * Variables::width;
        drawNode->drawRect(ccp(start, 0.0f), ccp(end, Variables::height), segment.col, 0, segment.col);
        if (Variables::separators && i != 0) {
            drawNode->drawSegment(
                ccp(start, Variables::separatorThickness / 2), 
                ccp(start, Variables::height - Variables::separatorThickness / 2),
                Variables::separatorThickness, Variables::seperatorCol
            );
        }

        std::string labelString = "";
        if (m_labelType == 0) {
            labelString = numToString(segment.end - segment.start, 2) + "%"; 
        } else if (m_labelType == 1) {
            labelString = portalStringMap.at(segment.type);
        }
        
        auto label = CCLabelBMFont::create(labelString.c_str(), "bigFont.fnt");
        label->setScale(0.15f * Variables::width);
        label->limitLabelWidth((end - start) * 0.9f, label->getScale(), 0.0f);
        if (label->getScale() < Variables::minimumLabelSize) label->setVisible(false);
        label->setPosition(start + ((end - start) / 2) + drawPos.x, drawPos.y - 15.0f);
        m_labelLayer->addChild(label); 

        i++;
    }
}

void GamemodeVisualizerPopup::onToggleLabelLayer(CCObject* sender) {
    m_labelLayer->setVisible(!m_labelLayer->isVisible());
}

void GamemodeVisualizerPopup::onChangeLabelType(CCObject* sender) {
    if (++m_labelType == 2) m_labelType = 0; // i wanna add one more label type eventually but idk what to make it for now so
    drawVisualizer();
}