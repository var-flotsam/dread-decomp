// CDaivoAIComponent's unit (tools/tu_map.py unit 766): the destructor. The attack at +0x14898 (its path
// data at +0x30, fn_7101059d40), the counted reference at +0x14890, the three callback lists at +0x14840,
// +0x14800 and +0x147c0, then the base AI component's destructor (fn_71006c5c8c). Names and layouts are
// placeholders.
#include "base/UnkCloneHelpers.h"
#include "base/UnkDestructors.h"
#include "game/UnkTU341.h"
#include "types.h"

extern void* g_unk_7101a40960[];  // CDaivoAIComponent's vtable
extern void* g_unk_7101a40548[];  // the attack's vtable

void fn_71003606c0(void*);
void fn_7101059d40(char* self);
void fn_71006c5c8c(void* self);

struct UnkDaivoCallbacks_l4df274;
template <>
struct base::UnkCallbackTraits<UnkDaivoCallbacks_l4df274> {
    static char* vtable() { return reinterpret_cast<char*>(g_unk_7101a20950); }
    static char* base_vtable() { return reinterpret_cast<char*>(g_unk_7101a20998); }
};
using UnkDaivoCallbackList_l4df274 = base::UnkCallbackListShort<UnkDaivoCallbacks_l4df274>;

// The attack's path data, destroyed by fn_7101059d40.
struct UnkDaivoPath_l4df274 {
    u8 pad_0x0[0x38];
    ~UnkDaivoPath_l4df274() { fn_7101059d40(reinterpret_cast<char*>(this)); }
};

// The attack base: its destructor is fn_71003606c0.
struct UnkDaivoAttackBase_l4df274 {
    void** vtable;
    ~UnkDaivoAttackBase_l4df274() { fn_71003606c0(this); }
};

struct UnkDaivoAttack_l4df274 : UnkDaivoAttackBase_l4df274 {
    u8 pad_0x8[0x28];
    UnkDaivoPath_l4df274 path;  // 0x30
    ~UnkDaivoAttack_l4df274() { vtable = &g_unk_7101a40548[2]; }
};

struct UnkDaivoAIComponent_l4df274 {
    void** vtable;
    u8 pad_0x8[0x147c0 - 0x8];
    UnkDaivoCallbackList_l4df274 list_a;  // 0x147c0
    UnkDaivoCallbackList_l4df274 list_b;  // 0x14800
    UnkDaivoCallbackList_l4df274 list_c;  // 0x14840
    u8 pad_0x14880[0x10];
    base::UnkRefPtrMember ref;       // 0x14890
    UnkDaivoAttack_l4df274 attack;  // 0x14898
};
static_assert(__builtin_offsetof(UnkDaivoAIComponent_l4df274, list_c) == 0x14840, "layout");
static_assert(__builtin_offsetof(UnkDaivoAIComponent_l4df274, attack) == 0x14898, "layout");

void fn_71004df274(UnkDaivoAIComponent_l4df274* self) {
    self->vtable = &g_unk_7101a40960[2];
    self->attack.~UnkDaivoAttack_l4df274();
    using base::UnkRefPtrMember;
    self->ref.~UnkRefPtrMember();
    self->list_c.~UnkDaivoCallbackList_l4df274();
    self->list_b.~UnkDaivoCallbackList_l4df274();
    self->list_a.~UnkDaivoCallbackList_l4df274();
    fn_71006c5c8c(self);
}
