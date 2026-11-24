#pragma once

// The reflection types under the placeholder names they had before they were named, for the
// hand-written sources that still spell them so (base/reflection/CClass.h includes this). A source moves
// to the names in CClass.h and ReflectedAttributes.h when it is next worked on; nothing new uses these.

#include "base/reflection/CClass.h"

namespace base::reflection {

// ReflectedObject with its class getter under the slot's placeholder. Classes, not aliases: sources
// forward-declare them. Each destructor is declared and not defined, as the stand-in's was, so no unit
// emits its virtual table.
struct UnkReflObject : ReflectedObject {
    ~UnkReflObject() override;
    const CClass* vf_0x0() const { return refl_GetDynamicClass(); }
};

struct UnkReflObjectB : ReflectedObjectDestructorFirst {
    ~UnkReflObjectB() override;
    const CClass* vf_0x18() const { return refl_GetDynamicClass(); }
};

struct AttributeKey;
struct Attributes;
struct TypedValueRef;
using UnkReflAttrKey = AttributeKey;
using UnkReflAttrs = Attributes;
using UnkReflValueRef = TypedValueRef;

}  // namespace base::reflection
