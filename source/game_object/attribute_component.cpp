#include "attribute_component.hpp"

namespace QDUEngine
{
    AttributeComponent::AttributeComponent(char* name) : m_name(name) {}

    void AttributeComponent::set(char* key, float value)
    {
        m_attr.set(key, value);
    }

    float AttributeComponent::get(char* key)
    {
        return m_attr.get(key);
    }

    char* AttributeComponent::getName()
    {
        return m_name;
    }
}

