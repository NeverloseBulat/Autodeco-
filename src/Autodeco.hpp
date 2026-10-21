#pragma once
#include <Geode/Geode.hpp>
using namespace geode::prelude;

struct DecoStyle {
    const char* name;
    ccColor3B bg, block, outline, accent, glow;
    int density;
    float glowScale;
};

inline const std::vector<DecoStyle>& decoStyles() {
    static const std::vector<DecoStyle> s = {
        {"Lagoon",    {10,30,90},  {20,60,160},  {80,220,255},  {40,160,230}, {120,240,255}, 2, 0.5f},
        {"Nightmare", {30,0,10},   {80,0,20},    {255,40,60},   {200,0,40},   {255,90,90},   3, 0.6f},
        {"Frost",     {10,30,50},  {60,110,150}, {200,240,255}, {140,200,240},{230,250,255}, 2, 0.5f},
        {"Moonlit",   {25,10,50},  {70,30,120},  {220,120,255}, {160,60,220}, {240,170,255}, 3, 0.6f},
        {"Lumen",     {15,15,15},  {60,60,60},   {255,255,255}, {170,170,170},{255,255,200}, 1, 0.4f},
        {"Sunset",    {50,15,10},  {140,50,20},  {255,180,60},  {255,110,40}, {255,220,120}, 2, 0.5f},
        {"Toxic",     {5,30,10},   {20,90,30},   {120,255,80},  {60,200,40},  {180,255,140}, 3, 0.5f},
    };
    return s;
}

class AutoDecoPopup : public geode::Popup<LevelEditorLayer*> {
protected:
    LevelEditorLayer* m_lel = nullptr;
    int m_style = 0;
    CCLabelBMFont* m_styleLabel = nullptr;
    bool setup(LevelEditorLayer* lel) override;
    void onPrev(CCObject*);
    void onNext(CCObject*);
    void onDecorate(CCObject*);
    void refresh();
public:
    static AutoDecoPopup* create(LevelEditorLayer* lel);
};
