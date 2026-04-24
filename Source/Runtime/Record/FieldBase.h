#pragma once

#include <concepts>
#include <string_view>
#include <string>

class FieldBase
{
public:
    virtual void Serialize();
    virtual void Deserialize();
};

template<typename T>
concept FieldClass = std::is_base_of<FieldBase, T>::value;

class FieldContainerBase
{

};

template<typename T>
concept FieldContainerClass = std::is_base_of<FieldContainerBase, T>::value;

class StringField : public FieldContainerBase
{
public:
    StringField() {}
    StringField(int) {}
    StringField(std::string_view && string) {}
    StringField(std::string_view string) {}
    StringField(const char* string) {}
};

class BoolField : public FieldContainerBase
{
    bool value = false;
public:
    BoolField() {}
    explicit BoolField(int) {}
    BoolField(bool init) : value(init) {}
};