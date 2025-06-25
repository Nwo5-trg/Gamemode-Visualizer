#include "GamemodeVisualizerPopup.hpp"
#include "Variables.hpp"
#include "Parse.hpp"
#include "Utils.hpp"

using namespace geode::prelude;

GamemodeVisualizerPopup* GamemodeVisualizerPopup::create(GJGameLevel* level) { 
    auto ret = new GamemodeVisualizerPopup();
    ret->m_level = level;
    if (ret->initAnchored(Variables::width <= 325.0f ? 400.0f : Variables::width + 75.0f, Variables::height * 2 + 150.0f)) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

bool GamemodeVisualizerPopup::setup() { // cant be fucked to refactor
    this->setTitle("Gamemode Visualizer :333", "goldFont.fnt");

    auto draw = CCDrawNode::create();
    draw->setBlendFunc({GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA});
    m_mainLayer->addChild(draw);
    m_draw = draw;

    auto draw2 = CCDrawNode::create();
    draw2->setBlendFunc({GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA});
    m_mainLayer->addChild(draw2);
    m_draw2 = draw2;

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

    auto changeExtraPortalsTypeButton = CCMenuItemSpriteExtra::create(
        CCSprite::createWithSpriteFrameName("GJ_statsBtn_001.png"),
        this, menu_selector(GamemodeVisualizerPopup::onChangeExtraPortalsType)
    );
    changeExtraPortalsTypeButton->setPosition(
        changeLabelTypeButton->getPositionX() - changeLabelTypeButton->getScaledContentWidth() - 5.0f, 
        changeLabelTypeButton->getPositionY()
    );
    changeExtraPortalsTypeButton->setScale(0.5f);
    changeExtraPortalsTypeButton->m_baseScale = 0.5f;
    m_buttonMenu->addChild(changeExtraPortalsTypeButton);

    auto disclaimerLabel = CCLabelBMFont::create("level contains some silly silly 2.2 shenanigans, visualizer might not be accurate 3:", "chatFont.fnt");
    disclaimerLabel->setScale(0.5f);
    disclaimerLabel->setAnchorPoint({0.5f, 1.0f});
    disclaimerLabel->setPosition(m_size.width / 2, m_size.height - 35.0f);
    m_mainLayer->addChild(disclaimerLabel);
    m_disclaimerLabel = disclaimerLabel;

    auto gamemodeDistributionLabel = CCLabelBMFont::create("", "bigFont.fnt");
    gamemodeDistributionLabel->setAnchorPoint({0.0f, 0.0f});
    gamemodeDistributionLabel->setPosition(10.0f, 10.0f);
    m_mainLayer->addChild(gamemodeDistributionLabel);
    m_gamemodeDistributionLabel = gamemodeDistributionLabel;

    auto extraDistributionLabel = CCLabelBMFont::create("", "bigFont.fnt");
    extraDistributionLabel->setAnchorPoint({0.0f, 0.0f});
    m_mainLayer->addChild(extraDistributionLabel);
    m_extraDistributionLabel = extraDistributionLabel;

    drawVisualizer();

    return true;
}

void GamemodeVisualizerPopup::drawVisualizer() {
    m_draw->clear();
    m_labelLayer->removeAllChildrenWithCleanup(true);

    if (m_portalSegments.empty()) {
        auto vectorVectorVectorVector = createDrawSegmentsFrom(m_level);
        m_portalSegments = vectorVectorVectorVector[0];
        m_speedSegments = vectorVectorVectorVector[1];
        m_sizeSegments = vectorVectorVectorVector[2];
        m_dualSegments = vectorVectorVectorVector[3];
        if (vectorVectorVectorVector.size() == 5) m_showDisclaimer = true;
    }

    m_disclaimerLabel->setVisible(m_showDisclaimer);

    for (int i = 0; i < 2; i++) {
        auto draw = m_draw;
        if (i == 1) draw = m_draw2;

        auto distributionLabel = m_gamemodeDistributionLabel;
        if (i == 1) distributionLabel = m_extraDistributionLabel;

        auto* segments = &m_portalSegments;
        int loops = 8;
        auto* stringMap = &portalStringMap;
        if (i == 1) {
            switch (m_extraPortalsType) {
                case 0: {
                    segments = &m_speedSegments; 
                    loops = 5;
                    stringMap = &speedStringMap;
                    break;
                }
                case 1: {
                    segments = &m_sizeSegments; 
                    loops = 2;
                    stringMap = &sizePortalStringMap;
                    break;
                }
                case 2: {
                    segments = &m_dualSegments; 
                    loops = 2;
                    stringMap = &dualPortalStringMap;
                    break;
                }
            }
        }

        auto drawPos = ccp((m_size.width - (Variables::width * 100)) / 2, m_size.height - Variables::height - 50 - ((Variables::height + 25) * i));
        draw->setPosition(drawPos);

        bool firstSeperator = false;
        for (const auto& segment : *segments) {
            if (segment.end - segment.start < 0.001f) continue;
            float start = segment.start * Variables::width;
            float end = segment.end * Variables::width;
            draw->drawRect({start, 0.0f}, {end, Variables::height}, segment.col, 0, segment.col);
            if (Variables::separators && !firstSeperator) {
                draw->drawSegment(
                    {start, Variables::separatorThickness / 2}, 
                    {start, Variables::height - Variables::separatorThickness / 2},
                    Variables::separatorThickness, Variables::seperatorCol
                );
                firstSeperator = true;
            }

            std::string labelString = "";
            if (m_labelType == 0) {
                labelString = numToString(segment.end - segment.start, 2) + "%"; 
            } else if (m_labelType == 1) {
                labelString = stringMap->at(segment.type);
            }
            
            auto label = CCLabelBMFont::create(labelString.c_str(), "bigFont.fnt");
            label->setScale(0.15f * Variables::width);
            label->limitLabelWidth((end - start) * 0.9f, label->getScale(), 0.0f);
            if (label->getScale() < Variables::minimumLabelSize) label->setVisible(false);
            label->setPosition(start + ((end - start) / 2) + drawPos.x, drawPos.y - 15.0f);
            m_labelLayer->addChild(label);
        }

        // ok so idc if i loop thru the vector like 9 times cuz like im lazy
        std::string distributionString = "";
        for (int j = 0; j < loops; j++) {
            float totalPercentage = 0.0f;
            int type = j;
            for (const auto& segment : *segments) {
                if (segment.type == type) totalPercentage += segment.end - segment.start;
            }
            if (totalPercentage == 0.0f) continue;
            distributionString += (stringMap->at(type) + ": " + ftofstr(totalPercentage, 2) + "%, ");
        }
        if (distributionString.length() > 2) distributionString.erase(distributionString.end() - 2);
        distributionLabel->setString(distributionString.c_str());
        distributionLabel->limitLabelWidth(m_size.width - 100.0f, 0.45f, 0.0f);
        if (i == 1) distributionLabel->setPosition(10.0f, m_gamemodeDistributionLabel->getScaledContentHeight() + 10);
    }
}

void GamemodeVisualizerPopup::onToggleLabelLayer(CCObject* sender) {
    m_labelLayer->setVisible(!m_labelLayer->isVisible());
}

void GamemodeVisualizerPopup::onChangeLabelType(CCObject* sender) {
    m_labelType = !m_labelType;
    drawVisualizer();
}

void GamemodeVisualizerPopup::onChangeExtraPortalsType(CCObject* sender) {
    if (++m_extraPortalsType == 3) m_extraPortalsType = 0;
    drawVisualizer();
}