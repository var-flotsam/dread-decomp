// CCharClassGoliathAIComponent's unit (tools/tu_map.py unit 843): fn_710059049c, the base character
// class's check (fn_7100414168, called with the same arguments) unless the actor's AI component is a
// CGoliathAIComponent: then 3 when its vf_0x868 accepts the info's +0x60, else, unless the flag at
// +0x50c is set, 2 or 3 by bit 15 of the info's flags at +0xb4. Names and layouts are placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CCharClassAIComponent.h"
#include "game/UnkActor.h"
#include "types.h"

struct UnkGoliathAI_b63a3 {
    void** vtable;
    const base::reflection::CClass* vf_0x0() {
        return reinterpret_cast<const base::reflection::CClass* (*)(UnkGoliathAI_b63a3*)>(vtable[0])(this);
    }
    bool vf_0x868(void* arg) {
        return reinterpret_cast<bool (*)(UnkGoliathAI_b63a3*, void*)>(vtable[0x868 / 8])(this, arg);
    }
};

struct UnkGoliathCharClass_b63a3 {
    u8 pad_0x0[0x50c];
    bool field_0x50c;
};

struct UnkGoliathInfo_b63a3 {
    u8 pad_0x0[0x60];
    void* field_0x60;
    u8 pad_0x68[0xb4 - 0x68];
    u32 flags;  // 0xb4
};

u32 fn_710059049c(UnkGoliathCharClass_b63a3* self, UnkActor* actor, void* arg, UnkGoliathInfo_b63a3* info) {
    u32 result = fn_7100414168();
    UnkGoliathAI_b63a3* ai = static_cast<UnkGoliathAI_b63a3*>(actor->field_0x240);
    if (ai && fn_710009d50c(ai->vf_0x0(), fn_71005908ac())) {
        if (ai->vf_0x868(info->field_0x60))
            result = 3;
        else if (!self->field_0x50c)
            result = (info->flags & 0x8000) ? 2 : 3;
    }
    return result;
}
