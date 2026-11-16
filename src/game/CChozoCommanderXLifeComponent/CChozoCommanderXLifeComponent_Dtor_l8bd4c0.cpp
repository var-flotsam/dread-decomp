// CChozoCommanderXLifeComponent's unit (tools/tu_map.py unit 1369): the destructor. The callback lists
// at +0x2f0 and +0x2b0, the string-id table at +0x288, the name at +0x280 and the listener at +0x248,
// then the life component base destructor (fn_710115f91c). Names and layouts are placeholders.
#include "base/UnkCloneHelpers.h"
#include "base/UnkDestructors.h"
#include "base/UnkStrIdTable.h"
#include "game/CAbilityGhostAura.h"
#include "game/CCentralUnitAIComponent.h"
#include "game/CChozoCommanderXLifeComponent.h"
#include "game/CLifeComponent.h"
#include "game/UnkTU341.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

struct UnkCommanderXCallbacks_l8bd4c0;
template <>
struct base::UnkCallbackTraits<UnkCommanderXCallbacks_l8bd4c0> {
    static char* vtable() { return reinterpret_cast<char*>(g_unk_7101a33fa0); }
    static char* base_vtable() { return reinterpret_cast<char*>(g_unk_7101a20998); }
};
using UnkCommanderXCallbackList_l8bd4c0 = base::UnkCallbackListShort<UnkCommanderXCallbacks_l8bd4c0>;

// The counted link a listener holds; the object it is registered with is at +0x10.
struct UnkCommanderXLink_l8bd4c0 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void Release();  // 0x10
    u32 ref_count;
    void* owner;
};

// A listener that unregisters from its link's owner (fn_710117b3f0) on destruction.
struct UnkCommanderXListener_l8bd4c0 {
    void** vtable;
    u8 pad_0x8[0x10];
    UnkCommanderXLink_l8bd4c0* link;

    ~UnkCommanderXListener_l8bd4c0() {
        vtable = &g_unk_7101a22d48[2];
        if (UnkCommanderXLink_l8bd4c0* l = link) {
            bool held = true;
            if (void* owner = l->owner) {
                fn_710117b3f0(static_cast<UnkWeakTarget*>(owner), this);
                l = link;
                held = l != nullptr;
            }
            if (held && --l->ref_count == 0)
                l->Release();
        }
    }
};

struct UnkCommanderXTable_l8bd4c0 : UnkStrIdTable {
    ~UnkCommanderXTable_l8bd4c0() {
        fn_71004f11f8(this);
        if (keys)
            allocator->vf_0x48(keys);
        if (values)
            allocator->vf_0x48(values);
    }
};

struct UnkCommanderXLife_l8bd4c0 {
    void** vtable;
    u8 pad_0x8[0x240];
    UnkCommanderXListener_l8bd4c0 listener;        // 0x248
    u8 pad_0x268[0x18];
    base::UnkStrIdMember name;                     // 0x280
    UnkCommanderXTable_l8bd4c0 table;              // 0x288
    UnkCommanderXCallbackList_l8bd4c0 callbacks_a;  // 0x2b0
    UnkCommanderXCallbackList_l8bd4c0 callbacks_b;  // 0x2f0
};
static_assert(__builtin_offsetof(UnkCommanderXLife_l8bd4c0, table) == 0x288, "layout");
static_assert(__builtin_offsetof(UnkCommanderXLife_l8bd4c0, callbacks_b) == 0x2f0, "layout");

void fn_71008bd4c0(UnkCommanderXLife_l8bd4c0* self) {
    self->vtable = &g_unk_7101a7df38[2];
    self->callbacks_b.~UnkCommanderXCallbackList_l8bd4c0();
    self->callbacks_a.~UnkCommanderXCallbackList_l8bd4c0();
    self->table.~UnkCommanderXTable_l8bd4c0();
    using base::UnkStrIdMember;
    self->name.~UnkStrIdMember();
    self->listener.~UnkCommanderXListener_l8bd4c0();
    fn_710115f91c(self);
}
