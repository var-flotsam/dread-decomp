// CGoliathXSlamAttack's unit (tools/tu_map.py unit 836): the attack's weight. When the owner's AI
// component used this attack second to last, or a random roll exceeds 0.4, it clears the component's
// next attack name and gives 0; otherwise it names this attack next and gives 0.3. Names and layouts
// are placeholders.
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "types.h"

extern "C" int rand();

using base::global::CStrId;
using base::reflection::CClass;

__attribute__((visibility("hidden"))) extern CStrId g_unk_71726e5730;  // the empty attack name

// A circular doubly linked list of the attacks used, oldest first; the list itself is the end node
// and keeps the count where an entry keeps its value's extra word.
struct UnkSlamHistory_l587050 {
    void* field_0x0;
    void* value;  // 0x8
    u32 count;    // 0x10
    UnkSlamHistory_l587050* prev;  // 0x18
    UnkSlamHistory_l587050* next;  // 0x20

    // The entry at `index` (null past the end), walking from whichever end is nearer.
    UnkSlamHistory_l587050* At(u32 index) {
        if (index >= count)
            return nullptr;
        UnkSlamHistory_l587050* node = this;
        if (index < count - index) {
            for (u32 i = 0; i < index + 1; ++i)
                node = node->next;
        } else {
            for (u32 i = 0; i < count - index; ++i)
                node = node->prev;
        }
        return node;
    }
};

struct UnkSlamAIComponent_l587050 {
    void** vtable;
    const CClass* vf_0x0() { return reinterpret_cast<const CClass* (*)(UnkSlamAIComponent_l587050*)>(vtable[0])(this); }
    u8 pad_0x8[0x1158];
    UnkSlamHistory_l587050 history;  // 0x1160
    u8 pad_0x1188[0x5f8];
    CStrId next_attack;  // 0x1780
};
static_assert(__builtin_offsetof(UnkSlamAIComponent_l587050, next_attack) == 0x1780, "layout");

struct UnkSlamOwner_l587050 {
    u8 pad_0x0[0x240];
    UnkSlamAIComponent_l587050* ai;  // 0x240
};

struct UnkSlamDef_l587050 {
    u8 pad_0x0[0x38];
    CStrId name;  // 0x38
};

struct UnkSlamAttack_l587050 {
    void** vtable;
    UnkSlamOwner_l587050* owner;  // 0x8
    u8 pad_0x10[0x60];
    UnkSlamDef_l587050* def;  // 0x70
};

static inline f32 Random01() { return (f32)((rand() & 0x7fff) * (1.0 / 32767.0)); }

f32 fn_7100587050(UnkSlamAttack_l587050* self) {
    UnkSlamAIComponent_l587050* ai = self->owner->ai;
    if (ai && fn_710009d50c(ai->vf_0x0(), fn_71010f6b50())) {
        UnkSlamHistory_l587050* previous = ai->history.At(ai->history.count - 2);
        if ((previous && previous->value == self) || Random01() > 0.4f) {
            ai->next_attack = g_unk_71726e5730;
            return 0.0f;
        }
        ai->next_attack = self->def->name;
    }
    return 0.3f;
}
