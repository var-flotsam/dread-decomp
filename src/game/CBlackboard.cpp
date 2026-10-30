// The lookups of a blackboard's sections by id: whether there is one, the section itself, and the
// section created when it is missing (the search is inlined into each).
#include "base/UnkParallelFind.h"
#include "game/CBlackboard.h"

using base::global::CStrId;

bool fn_7101047208(CBlackboard* self, const CStrId& key) {
    return UnkBlackboardFindSlot(self, key) != &self->sections[self->count];
}

CBlackboard::CSection* fn_7101047270(CBlackboard* self, const CStrId& key) {
    CBlackboard::CSection** slot = UnkBlackboardFindSlot(self, key);
    return slot != &self->sections[self->count] ? *slot : nullptr;
}

// Two more copies of the lookup above (identical code, other translation units' instantiations).
CBlackboard::CSection* fn_7101047414(CBlackboard* self, const CStrId& key) {
    CBlackboard::CSection** slot = UnkBlackboardFindSlot(self, key);
    return slot != &self->sections[self->count] ? *slot : nullptr;
}

CBlackboard::CSection* fn_7101047484(CBlackboard* self, const CStrId& key) {
    CBlackboard::CSection** slot = UnkBlackboardFindSlot(self, key);
    return slot != &self->sections[self->count] ? *slot : nullptr;
}

CBlackboard::CSection* fn_71010472e0(CBlackboard* self, const CStrId& key) {
    CBlackboard::CSection** slot = UnkBlackboardFindSlot(self, key);
    if (slot != &self->sections[self->count])
        return *slot;
    CBlackboard::CSection* section = static_cast<CBlackboard::CSection*>(fn_71000839a0(sizeof(CBlackboard::CSection), 0x15));
    new (&section->key) CStrId(key);
    section->data.reserve = 200;
    section->blackboard = self;
    section->field_0x10 = nullptr;
    section->data.properties.values = nullptr;
    section->data.properties.ids = nullptr;
    section->data.properties.count = 0;
    section->data.properties.capacity = 0;
    section->data.properties.step = 10;
    section->data.properties.flags = 0x15;
    section->data.properties.allocator = fn_7100083990();
    fn_7100093dd0(&section->data.properties, 200);
    section->data.flags = 0x15;
    section->data.field_0x38 = false;
    self->vf_0x28(key, section);
    return section;
}

// ---- fn_7101046974: the constructor ----

#include <new>

#include "base/UnkChainedMap.h"
#include "base/UnkNamedRegistry.h"
#include "base/global/CFilePathStrId.h"

// The blackboard's vtable (the vtable pointer is its address + 0x10).
extern char g_unk_7101cac7c8[];

// What the constructor sets: the sections and their ids (parallel arrays with the layout of
// UnkPropertyArrays: growth step 10, flags 0x15), a map at 0x30 ("hashSections"?), two strings
// built from the empty literal, and the lock flag.
struct UnkBlackboardMembers {
    char* vtable;
    UnkPropertyArrays sections;
    UnkChainedMap field_0x30;
    u64 field_0x78;
    base::UnkNamedEntry field_0x80;  // a CRntString
    base::UnkNamedEntry field_0xb0;  // a CRntString
    u64 field_0xe0;
    bool field_0xe8;
};
static_assert(sizeof(UnkBlackboardMembers) == sizeof(CBlackboard), "UnkBlackboardMembers layout");

void fn_7101046974(CBlackboard* blackboard) {
    UnkBlackboardMembers* self = reinterpret_cast<UnkBlackboardMembers*>(blackboard);
    self->vtable = g_unk_7101cac7c8 + 0x10;
    self->sections.values = nullptr;
    self->sections.ids = nullptr;
    self->sections.count = 0;
    self->sections.capacity = 0;
    self->sections.step = 10;
    self->sections.flags = 0x15;
    self->sections.allocator = fn_7100083990();
    new (&self->field_0x30) UnkChainedMap();
    self->field_0x30.ResizeBuckets(11);
    self->field_0x78 = 0;
    fn_710007e704(&self->field_0x80, g_unk_710162e2ed);
    fn_710007e704(&self->field_0xb0, g_unk_710162e2ed);
    self->field_0xe0 = 0;
    self->field_0xe8 = false;
}

// A section's property values (24 bytes each) as the lookup below walks them.
struct UnkPropertyValue_0x18 {
    u8 data[0x18];
};

// The same arrays with the value type spelled out, for base/UnkParallelFind.h.
struct UnkPropertyMap {
    UnkPropertyValue_0x18* values;
    CStrId* ids;
    u32 count;
};

// Whether the section `section` names has a property named `name`.
bool fn_71010474f4(CBlackboard* self, const CStrId& section, const CStrId& name) {
    CBlackboard::CSection** slot = UnkBlackboardFindSlot(self, section);
    CBlackboard::CSection* found = slot != &self->sections[self->count] ? *slot : nullptr;
    if (!found)
        return false;
    UnkPropertyArrays& properties = found->data.properties;
    UnkPropertyValue_0x18* it = UnkParallelFindSlot(reinterpret_cast<UnkPropertyMap*>(&properties), name);
    return it != &static_cast<UnkPropertyValue_0x18*>(properties.values)[properties.count];
}

// The list at +0x70 of a blackboard: nodes holding an object whose flag at +0x60 is cleared below.
struct UnkBlackboardListed {
    u8 pad_0x0[0x60];
    bool field_0x60;
};
struct UnkBlackboardListNode {
    u8 pad_0x0[0x8];
    UnkBlackboardListed* object;
    UnkBlackboardListNode* next;
};
struct UnkBlackboardList {
    u8 pad_0x0[0x70];
    UnkBlackboardListNode* head;
};

// Clears every section's flag (its data's field_0x38) and the flag of every object on the list at
// +0x70.
void fn_7101047178(CBlackboard* self) {
    for (CBlackboard::CSection **it = self->sections, **end = it + self->count; it != end; ++it)
        if (*it)
            (*it)->data.field_0x38 = false;
    for (UnkBlackboardListNode* node = reinterpret_cast<UnkBlackboardList*>(self)->head; node; node = node->next)
        if (node->object)
            node->object->field_0x60 = false;
}
