#include "EntityComponentTemplateLibrary.h"

EntityComponentTemplateLibrary &EntityComponentTemplateLibrary::Get()
{
    static EntityComponentTemplateLibrary library;
    return library;
}