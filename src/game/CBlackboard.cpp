// The lookups of a blackboard's sections by id: whether there is one, the section itself, and the
// section created when it is missing (the search is inlined into each).
#include "game/CBlackboard.h"

using base::global::CStrId;

namespace {

// The slot of the section `key` names, or the slot past the last one. The ids are compared as raw
// 64-bit words (the original loads the key after the array); the slot is returned, and compared
// with the end by the callers, as the keyed arrays of base/UnkCloneCalls.h do (written otherwise
// the compiler folds the comparison away).
__attribute__((always_inline)) inline CBlackboard::CSection** FindSlot(CBlackboard* self, const CStrId& key) {
    for (u64 i = 0; i < self->count; ++i) {
        if (*reinterpret_cast<const u64*>(&self->ids[i]) == *reinterpret_cast<const u64*>(&key))
            return &self->sections[i];
    }
    return &self->sections[self->count];
}

}  // namespace

bool fn_7101047208(CBlackboard* self, const CStrId& key) {
    return FindSlot(self, key) != &self->sections[self->count];
}

CBlackboard::CSection* fn_7101047270(CBlackboard* self, const CStrId& key) {
    CBlackboard::CSection** slot = FindSlot(self, key);
    return slot != &self->sections[self->count] ? *slot : nullptr;
}

// Two more copies of the lookup above (identical code, other translation units' instantiations).
CBlackboard::CSection* fn_7101047414(CBlackboard* self, const CStrId& key) {
    CBlackboard::CSection** slot = FindSlot(self, key);
    return slot != &self->sections[self->count] ? *slot : nullptr;
}

CBlackboard::CSection* fn_7101047484(CBlackboard* self, const CStrId& key) {
    CBlackboard::CSection** slot = FindSlot(self, key);
    return slot != &self->sections[self->count] ? *slot : nullptr;
}

CBlackboard::CSection* fn_71010472e0(CBlackboard* self, const CStrId& key) {
    CBlackboard::CSection** slot = FindSlot(self, key);
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
