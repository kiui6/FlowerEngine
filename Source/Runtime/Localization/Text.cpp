#include "Text.h"

#include "LocaleManager.h"

std::string_view Text::Get()
{
    if(m_cached.size()) {
        return m_cached;
    }

    LocaleManager* mgr = GetService<LocaleManager>();

    std::optional<std::string_view> locale = mgr->FetchLocaleString(m_id);
    if(!locale.has_value()) {
        return m_id;
    }

    m_cached = locale.value();
    return m_cached;
}
