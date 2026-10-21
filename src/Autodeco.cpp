#include "AutoDeco.hpp"
#include <random>

AutoDecoPopup* AutoDecoPopup::create(LevelEditorLayer* lel) {
    auto ret = new AutoDecoPopup();
    if (ret && ret->init(lel)) { ret->autorelease(); return ret; }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool AutoDecoPopup::init(LevelEditorLayer* lel) {
    if (!Popup::init(280.f, 160.f)) return false;
    m_lel = lel;
    this->setTitle("Auto Deco");

    m_styleLabel = CCLabelBMFont::create("", "bigFont.fnt");
    m_styleLabel->setScale(0.7f);
    m_mainLayer->addChildAtPosition(m_styleLabel, Anchor::Center, {0, 20});

    auto l = CCSprite::createWithSpriteFrameName("GJ_arrow_01_001.png");
    auto r = CCSprite::createWithSpriteFrameName("GJ_arrow_01_001.png");
    r->setFlipX(true);
    auto menu = CCMenu::create();
    auto bl = CCMenuItemSpriteExtra::create(l, this, menu_selector(AutoDecoPopup::onPrev));
    auto br = CCMenuItemSpriteExtra::create(r, this, menu_selector(AutoDecoPopup::onNext));
    bl->setPosition({-100, 20}); br->setPosition({100, 20});
    menu->addChild(bl); menu->addChild(br);
    m_mainLayer->addChildAtPosition(menu, Anchor::Center);

    auto go = CCMenuItemSpriteExtra::create(
        ButtonSprite::create("Decorate level"), this, menu_selector(AutoDecoPopup::onDecorate));
    auto m2 = CCMenu::create(); m2->addChild(go);
    m_mainLayer->addChildAtPosition(m2, Anchor::Bottom, {0, 30});

    refresh();
    return true;
}

void AutoDecoPopup::refresh() {
    auto& s = decoStyles();
    m_styleLabel->setString(fmt::format("{} ({}/{})", s[m_style].name, m_style + 1, s.size()).c_str());
}
void AutoDecoPopup::onPrev(CCObject*) { auto n = decoStyles().size(); m_style = (m_style + n - 1) % n; refresh(); }
void AutoDecoPopup::onNext(CCObject*) { m_style = (m_style + 1) % decoStyles().size(); refresh(); }

static void setChannel(LevelEditorLayer* lel, int ch, ccColor3B c) {
    if (auto act = lel->m_levelSettings->m_effectManager->getColorAction(ch)) {
        act->m_color = c;
    }
}

void AutoDecoPopup::onDecorate(CCObject*) {
    auto& st = decoStyles()[m_style];
    auto lel = m_lel;

    setChannel(lel, 1000, st.bg);
    setChannel(lel, 1001, st.bg);
    setChannel(lel, 1004, st.block);
    setChannel(lel, 1,    st.glow);
    setChannel(lel, 2,    st.accent);
    setChannel(lel, 3,    st.outline);

    std::vector<GameObject*> blocks;
    for (auto obj : CCArrayExt<GameObject*>(lel->m_objects)) {
        int id = obj->m_objectID;
        if (id == 1 || id == 2 || id == 3 || id == 4 || id == 40) blocks.push_back(obj);
    }

    std::mt19937 rng(12345);
    std::uniform_real_distribution<float> rnd(-1.f, 1.f);
    int added = 0;

    auto place = [&](int id, CCPoint pos, float scale, int chan, int z) {
        auto o = lel->createObject(id, pos, false);
        if (!o) return;
        o->setScale(scale);
        o->m_zOrder = z;
        if (o->m_baseColor) o->m_baseColor->m_colorID = chan;
        added++;
    };

    for (auto b : blocks) {
        auto p = b->getPosition();
        place(1, p, 1.0f, 3, -5);
        for (int i = 0; i < st.density; i++) {
            CCPoint off = {rnd(rng) * 15.f, rnd(rng) * 15.f};
            place(1, p + off, 0.2f + 0.1f * (i % 3), (i % 2) ? 2 : 1, -3);
        }
    }

    FLAlertLayer::create("Auto Deco",
        fmt::format("Стиль <cy>{}</c>: добавлено объектов: <cg>{}</c>.", st.name, added),
        "OK")->show();
    this->onClose(nullptr);
}
