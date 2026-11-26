#include <Geode/Geode.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include <Geode/modify/PlayLayer.hpp>

#include <cstdio>
#include <cstdint>

using namespace geode::prelude;

class AethaPopup final : public geode::Popup {
protected:
    bool init() {
        if (!Popup::init(300.f, 190.f)) {
            return false;
        }

        this->setTitle("AETHA CORE");

        auto status = CCLabelBMFont::create("CORE ONLINE", "bigFont.fnt");
        status->setScale(0.55f);
        status->setPosition({150.f, 112.f});
        m_mainLayer->addChild(status);

        auto info = CCLabelBMFont::create(
            "AETHA v0.1.0  |  Android + Windows",
            "bigFont.fnt"
        );
        info->setScale(0.32f);
        info->setPosition({150.f, 82.f});
        m_mainLayer->addChild(info);

        auto next = CCLabelBMFont::create(
            "Counter telemetry is active in gameplay.",
            "bigFont.fnt"
        );
        next->setScale(0.32f);
        next->setPosition({150.f, 57.f});
        m_mainLayer->addChild(next);

        return true;
    }

public:
    static AethaPopup* create() {
        auto ret = new AethaPopup();
        if (ret->init()) {
            ret->autorelease();
            return ret;
        }

        delete ret;
        return nullptr;
    }
};

class $modify(AethaMenuLayer, MenuLayer) {
    bool init() {
        if (!MenuLayer::init()) {
            return false;
        }

        auto menu = this->getChildByID("bottom-menu");
        if (!menu) {
            log::warn("AETHA: Could not find the bottom-menu node.");
            return true;
        }

        auto label = CCLabelBMFont::create("AETHA", "goldFont.fnt");
        if (!label) {
            log::warn("AETHA: Could not create the menu label.");
            return true;
        }

        label->setScale(0.45f);

        auto button = CCMenuItemSpriteExtra::create(
            label,
            this,
            menu_selector(AethaMenuLayer::onAethaButton)
        );

        if (!button) {
            log::warn("AETHA: Could not create the menu button.");
            return true;
        }

        button->setID("aetha-open-button"_spr);
        menu->addChild(button);
        menu->updateLayout();

        return true;
    }

    void onAethaButton(CCObject*) {
        if (auto popup = AethaPopup::create()) {
            popup->show();
        }
    }
};

class $modify(AethaPlayLayer, PlayLayer) {
    struct Fields {
        CCLabelBMFont* counter = nullptr;
        float sampleElapsed = 0.f;
        float sampleDeltaTotal = 0.f;
        int sampleUpdates = 0;
        std::uint64_t updateTicks = 0;
    };

    void onEnter() {
        PlayLayer::onEnter();

        if (!Mod::get()->getSettingValue<bool>("counter-enabled")) {
            return;
        }

        auto label = CCLabelBMFont::create("AETHA | starting...", "bigFont.fnt");
        if (!label) {
            return;
        }

        label->setAnchorPoint({0.f, 1.f});
        label->setScale(0.35f);

        auto winSize = CCDirector::sharedDirector()->getWinSize();
        label->setPosition({8.f, winSize.height - 8.f});
        label->setZOrder(10000);

        this->addChild(label);
        m_fields->counter = label;
    }

    void update(float dt) {
        PlayLayer::update(dt);

        if (!m_fields->counter) {
            return;
        }

        ++m_fields->updateTicks;
        ++m_fields->sampleUpdates;
        m_fields->sampleElapsed += dt;
        m_fields->sampleDeltaTotal += dt;

        auto refreshMs = Mod::get()->getSettingValue<int64_t>("counter-refresh-ms");
        auto refreshSeconds = static_cast<float>(refreshMs) / 1000.f;

        if (m_fields->sampleElapsed < refreshSeconds) {
            return;
        }

        float fpsEstimate = m_fields->sampleElapsed > 0.f
            ? static_cast<float>(m_fields->sampleUpdates) / m_fields->sampleElapsed
            : 0.f;

        float avgFrameMs = m_fields->sampleUpdates > 0
            ? (m_fields->sampleDeltaTotal / static_cast<float>(m_fields->sampleUpdates)) * 1000.f
            : 0.f;

        char buffer[160];
        if (Mod::get()->getSettingValue<bool>("counter-show-ticks")) {
            std::snprintf(
                buffer,
                sizeof(buffer),
                "AETHA | %.1f FPS* | %.2f ms | TICK %llu",
                fpsEstimate,
                avgFrameMs,
                static_cast<unsigned long long>(m_fields->updateTicks)
            );
        } else {
            std::snprintf(
                buffer,
                sizeof(buffer),
                "AETHA | %.1f FPS* | %.2f ms",
                fpsEstimate,
                avgFrameMs
            );
        }

        m_fields->counter->setString(buffer);
        m_fields->sampleElapsed = 0.f;
        m_fields->sampleDeltaTotal = 0.f;
        m_fields->sampleUpdates = 0;
    }
};
