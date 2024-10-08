#pragma once

#include <Sapfire/ui/Popup.hpp>
#include <Sapfire/ui/TextInput.hpp>
#include "../sources/ModListSource.hpp"
#include "../SapfireStyle.hpp"
#include <server/Server.hpp>

using namespace sapfire::prelude;

class FiltersPopup : public SapfirePopup<ModListSource*> {
protected:
    ModListSource* m_source;
    CCMenu* m_tagsMenu;
    std::unordered_set<std::string> m_selectedTags;
    EventListener<server::ServerRequest<std::unordered_set<std::string>>> m_tagsListener;
    CCMenuItemToggler* m_enabledModsOnly = nullptr;
    TextInput* m_developerNameInput = nullptr;

    bool setup(ModListSource* src) override;
    void updateTags();

    void onLoadTags(typename server::ServerRequest<std::unordered_set<std::string>>::Event* event);
    void onResetTags(CCObject*);
    void onResetDevName(CCObject*);
    void onSelectTag(CCObject* sender);
    void onClose(CCObject* sender) override;

public:
    static FiltersPopup* create(ModListSource* src);
};
