// CPowerGeneratorComponent's unit (tools/tu_map.py unit 1588): the destructor of the class whose vtable
// the constructor in UnkPowerGeneratorCtor.cpp stores. The listener at +0x140,
// the callback lists at +0xf8 and +0xb8, the string-id table at +0x90 and the name at +0x88, then the
// activatable component's destructor (fn_71007b00cc). Names and layouts are placeholders.
#include "base/UnkCloneHelpers.h"
#include "base/UnkDestructors.h"
#include "base/UnkStrIdTable.h"
#include "game/CAbilityGhostAura.h"
#include "game/CCentralUnitAIComponent.h"
#include "game/CPowerGeneratorComponent.h"
#include "game/UnkTU341.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

template <u32 IdOffset>
struct UnkActivatable_b44a2;
void fn_71007b00cc(UnkActivatable_b44a2<0x48>* self);

struct UnkPowerGeneratorCallbacks_la7e39c;
template <>
struct base::UnkCallbackTraits<UnkPowerGeneratorCallbacks_la7e39c> {
    static char* vtable() { return reinterpret_cast<char*>(g_unk_7101a33fa0); }
    static char* base_vtable() { return reinterpret_cast<char*>(g_unk_7101a20998); }
};
using UnkPowerGeneratorCallbackList_la7e39c = base::UnkCallbackListShort<UnkPowerGeneratorCallbacks_la7e39c>;

// The counted link a listener holds; the object it is registered with is at +0x10.
struct UnkPowerGeneratorLink_la7e39c {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void Release();  // 0x10
    u32 ref_count;
    void* owner;
};

// A listener that unregisters from its link's owner (fn_710117b3f0) on destruction.
struct UnkPowerGeneratorListener_la7e39c {
    void** vtable;
    u8 pad_0x8[0x10];
    UnkPowerGeneratorLink_la7e39c* link;

    ~UnkPowerGeneratorListener_la7e39c() {
        vtable = &g_unk_7101a22d48[2];
        if (UnkPowerGeneratorLink_la7e39c* l = link) {
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

struct UnkPowerGeneratorTable_la7e39c : UnkStrIdTable {
    ~UnkPowerGeneratorTable_la7e39c() {
        fn_71004f11f8(this);
        if (keys)
            allocator->vf_0x48(keys);
        if (values)
            allocator->vf_0x48(values);
    }
};

struct UnkPowerGenerator_la7e39c {
    void** vtable;
    u8 pad_0x8[0x80];
    base::UnkStrIdMember name;                           // 0x88
    UnkPowerGeneratorTable_la7e39c table;                // 0x90
    UnkPowerGeneratorCallbackList_la7e39c callbacks_a;   // 0xb8
    UnkPowerGeneratorCallbackList_la7e39c callbacks_b;   // 0xf8
    u8 pad_0x138[0x8];
    UnkPowerGeneratorListener_la7e39c listener;          // 0x140
};
static_assert(__builtin_offsetof(UnkPowerGenerator_la7e39c, table) == 0x90, "layout");
static_assert(__builtin_offsetof(UnkPowerGenerator_la7e39c, callbacks_b) == 0xf8, "layout");
static_assert(__builtin_offsetof(UnkPowerGenerator_la7e39c, listener) == 0x140, "layout");

void fn_7100a7e39c(UnkPowerGenerator_la7e39c* self) {
    self->vtable = g_unk_7101a90f90 + 2;
    self->listener.~UnkPowerGeneratorListener_la7e39c();
    self->callbacks_b.~UnkPowerGeneratorCallbackList_la7e39c();
    self->callbacks_a.~UnkPowerGeneratorCallbackList_la7e39c();
    self->table.~UnkPowerGeneratorTable_la7e39c();
    using base::UnkStrIdMember;
    self->name.~UnkStrIdMember();
    fn_71007b00cc(reinterpret_cast<UnkActivatable_b44a2<0x48>*>(self));
}
