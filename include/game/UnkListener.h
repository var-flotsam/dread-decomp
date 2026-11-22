#pragma once

#include "game/CAbilityGhostAura.h"
#include "game/UnkListenerOwner.h"
#include "types.h"

struct UnkListener {
    void* vtable;
    u8 pad_0x8[0x10];
    UnkListenerOwner* owner;

    void Destroy() {
        vtable = g_unk_7101a22d48 + 2;
        if (UnkListenerOwner* o = owner) {
            if (o->field_0x10)
                fn_710117b3f0(o->field_0x10, this);
            if (UnkListenerOwner* p = owner) {
                if (--p->ref_count == 0)
                    p->vf_0x10();
            }
        }
    }
};
