#include "RawAsset.h"

#include <Log/Log.h>

RawAsset::RawAsset(std::string_view path, DataView view)
{
    SetType(RawAsset::StaticType());

    if(!view) {
        LOG(Error, LogData, "Failed to load RawAsset, invalid DataView");
        return;
    }

    m_data.insert(m_data.begin(), view.begin(), view.end());
}