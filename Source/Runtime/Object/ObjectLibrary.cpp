#include "ObjectLibrary.h"

bool ObjectLibrary::bIsInitialized = RegisterService<ObjectLibrary>({DataManager::GetStaticName()});

void ObjectLibrary::Initialize()
{
    std::string_view data = GetService<DataManager>()->GetName();
}

void ObjectLibrary::Deinitialize()
{
}
