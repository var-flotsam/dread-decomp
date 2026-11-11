// CSwifterSpawnGroupComponent's unit (tools/tu_map.py): after the base step (fn_710111b64c), turns off
// (fn_710024fa0c) every spawned member's component whose definition is named `name` and that is on
// (fn_710024fbc8); then counts the step up to 3, clearing the flag at +0x42c on reaching it. Names and
// layouts are placeholders.
#include "base/global/CRntString.h"
#include "game/CActorComponent.h"
#include "types.h"

using base::global::CRntString;

struct UnkStruct_710024fa0c;

// A string id's pooled data as the comparison sees it: its key and a read count.
struct UnkSwifterIdData_ovb0114 {
    u8 pad_0x0[0x30];
    s64 key;  // 0x30
    u8 pad_0x38[0xc];
    union {
        s32 reads;  // 0x44
    };
};

struct UnkSwifterDef_ovb0114 {
    u8 pad_0x0[0x30];
    UnkSwifterIdData_ovb0114* name;  // 0x30

    s64 Key() const {
        ++name->reads;
        return name->key;
    }
};

struct UnkSwifterComponent_ovb0114 {
    u8 pad_0x0[0x18];
    UnkSwifterDef_ovb0114* def;  // 0x18
};

struct UnkSwifterActor_ovb0114 {
    u8 pad_0x0[0x10];
    UnkSwifterComponent_ovb0114* component;  // 0x10
};

// A spawned member (0x40 bytes); fn_71003df2d0 gives its actor.
struct UnkSwifterMember_ovb0114 {
    u8 pad_0x0[0x40];
};
UnkSwifterActor_ovb0114** fn_71003df2d0(UnkSwifterMember_ovb0114* member);

struct UnkSwifterSpawnGroup_ovb0114 {
    u8 pad_0x0[0x68];
    UnkSwifterMember_ovb0114* members;  // 0x68
    u32 memberCount;                    // 0x70
    u8 pad_0x74[0x3b4];
    s32 step;   // 0x428
    bool flag;  // 0x42c
};

void fn_710111b64c(UnkSwifterSpawnGroup_ovb0114* self, const char* name);  // the base step

void fn_7100a130d0(UnkSwifterSpawnGroup_ovb0114* self, const char* name) {
    fn_710111b64c(self, name);
    for (UnkSwifterMember_ovb0114 *it = self->members, *end = it + self->memberCount; it != end; ++it) {
        UnkSwifterComponent_ovb0114* component = (*fn_71003df2d0(it))->component;
        UnkSwifterDef_ovb0114* def = component->def;
        bool off;
        {
            CRntString key(name);
            off = def->Key() == key.key && fn_710024fbc8(component);
        }
        if (off)
            fn_710024fa0c(reinterpret_cast<UnkStruct_710024fa0c*>(component), false);
    }
    s32 step = self->step;
    if (step < 3)
        self->step = ++step;
    if (step == 3 && self->flag)
        self->flag = false;
}
