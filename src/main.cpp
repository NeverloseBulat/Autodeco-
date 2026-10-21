#include <Geode/Geode.hpp>
#include <Geode/modify/EditorPauseLayer.hpp>
#include "AutoDeco.hpp"

using namespace geode::prelude;

class $modify(AutoDecoPause, EditorPauseLayer) {
    bool init(LevelEditorLayer* lel) {
        if (!EditorPauseLayer::init(lel)) return false;
        auto spr = ButtonSprite::create("Auto Deco", "goldFont.fnt", "GJ_button_02.png", 0.7f);
        auto btn = CCMenuItemSpriteExtra::create(spr, this, menu_selector(AutoDecoPause::onAutoDeco));
        auto menu = CCMenu::create();
        menu->addChild(btn);
        auto win = CCDirector::get()->getWinSize();
        menu->setPosition({win.width - 70.f, win.height - 30.f});
        this->addChild(menu);
        return true;
    }
    void onAutoDeco(CCObject*) {
        AutoDecoPopup::create(m_editorLayer)->show();
    }
};
