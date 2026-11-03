// CChozoCommanderAIComponent (tools/tu_map.py): hands the found object of `owner` (fn_71004067a4) the
// object behind one of the commander's two lazily resolved handles (left: +0x15e70, source +0x15e40,
// state +0x15e68; right: +0x15eb0, source +0x15e80, state +0x15ea8; the shape of
// fn_7100452608.cpp), chosen by which side of the commander's owner (+0x18) the player stands on, after
// clearing the found object's flag (+0x521 of +0x200). Returns 1. Names and layouts are placeholders.
#include "base/UnkRefHandle.h"
#include "game/UnkGameManager.h"
#include "types.h"

struct UnkPlayer;
UnkPlayer* fn_7100dec788(UnkGameManager*);
void* fn_71004067a4(void*);

struct UnkCountedSide_b49 : base::UnkRefCounted {
    void* field_0x10;
};

struct UnkRef {
    UnkCountedSide_b49* ptr;
    ~UnkRef() {
        if (ptr && --ptr->ref_count == 0)
            delete ptr;
    }
};

UnkRef fn_7100353be8(void* from);

struct UnkLazySide_b49 {
    u8 source[0x28];
    u32 state;
    u32 pad;
    base::UnkRefHandle handle;

    __attribute__((always_inline)) void* Object() {
        if (state == 1) {
            {
                UnkRef fresh = fn_7100353be8(source);
                fn_710031d208(&handle, reinterpret_cast<const base::UnkRefHandle*>(&fresh));
            }
            state = 2;
        }
        auto* c = static_cast<UnkCountedSide_b49*>(handle.ptr);
        return c ? c->field_0x10 : nullptr;
    }
};

struct UnkSideActor_b49 {
    u8 pad_0x0[0x44];
    f32 x;
};

struct UnkSideFoundSub_b49 {
    u8 pad_0x0[0x521];
    bool field_0x521;
};
struct UnkSideFound_b49 {
    u8 pad_0x0[0x200];
    UnkSideFoundSub_b49* field_0x200;
};
void fn_71007a9268(UnkSideFound_b49* found, void* object);

struct UnkCommanderSide_b49 {
    u8 pad_0x0[0x18];
    UnkSideActor_b49* owner;
    u8 pad_0x20[0x15e20];
    UnkLazySide_b49 left;
    u8 pad_0x15e78[8];
    UnkLazySide_b49 right;
};
static_assert(__builtin_offsetof(UnkCommanderSide_b49, right) == 0x15e80, "layout");

u32 fn_7100452108(UnkCommanderSide_b49* self, void* owner) {
    auto* player = reinterpret_cast<UnkSideActor_b49*>(fn_7100dec788(g_unk_7101d47310));
    if (!player)
        return 1;
    f32 px = player->x;
    f32 sx = self->owner->x;
    auto* found = static_cast<UnkSideFound_b49*>(fn_71004067a4(owner));
    found->field_0x200->field_0x521 = false;
    void* object = px <= sx ? self->right.Object() : self->left.Object();
    fn_71007a9268(found, object);
    return 1;
}
