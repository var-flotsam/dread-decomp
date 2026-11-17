// CWaterTriggerChangeComponent's unit (tools/tu_map.py unit 1579): a component's destructor. The
// counted references at +0x1b0 and +0x1a8, the callback lists at +0x128, +0xe8 and +0xa8, the
// string-id table at +0x78 and the name at +0x70, then the component base destructor (fn_71010c4194).
// Names and layouts are placeholders.
#include "base/UnkCloneHelpers.h"
#include "base/UnkDestructors.h"
#include "base/UnkStrIdTable.h"
#include "game/CCentralUnitAIComponent.h"
#include "game/CWaterTriggerChangeComponent.h"
#include "game/UnkComponent.h"
#include "game/UnkTU341.h"
#include "types.h"

struct UnkWaterTriggerCallbacks_la65dac;
template <>
struct base::UnkCallbackTraits<UnkWaterTriggerCallbacks_la65dac> {
    static char* vtable() { return reinterpret_cast<char*>(g_unk_7101a33fa0); }
    static char* base_vtable() { return reinterpret_cast<char*>(g_unk_7101a20998); }
};
using UnkWaterTriggerCallbackList_la65dac = base::UnkCallbackListShort<UnkWaterTriggerCallbacks_la65dac>;

struct UnkWaterTriggerTable_la65dac : UnkStrIdTable {
    ~UnkWaterTriggerTable_la65dac() {
        fn_71004f11f8(this);
        if (keys)
            allocator->vf_0x48(keys);
        if (values)
            allocator->vf_0x48(values);
    }
};

struct UnkWaterTriggerComponent_la65dac {
    void** vtable;
    u8 pad_0x8[0x68];
    base::UnkStrIdMember name;                         // 0x70
    UnkWaterTriggerTable_la65dac table;                // 0x78
    void* field_0xa0;
    UnkWaterTriggerCallbackList_la65dac callbacks_a;   // 0xa8
    UnkWaterTriggerCallbackList_la65dac callbacks_b;   // 0xe8
    UnkWaterTriggerCallbackList_la65dac callbacks_c;   // 0x128
    u8 pad_0x168[0x40];
    base::UnkRefPtrMember ref_a;                       // 0x1a8
    base::UnkRefPtrMember ref_b;                       // 0x1b0
};
static_assert(__builtin_offsetof(UnkWaterTriggerComponent_la65dac, table) == 0x78, "layout");
static_assert(__builtin_offsetof(UnkWaterTriggerComponent_la65dac, callbacks_c) == 0x128, "layout");
static_assert(__builtin_offsetof(UnkWaterTriggerComponent_la65dac, ref_b) == 0x1b0, "layout");

void fn_7100a65dac(UnkWaterTriggerComponent_la65dac* self) {
    self->vtable = g_unk_7101a8f860 + 2;
    using base::UnkRefPtrMember;
    self->ref_b.~UnkRefPtrMember();
    self->ref_a.~UnkRefPtrMember();
    self->callbacks_c.~UnkWaterTriggerCallbackList_la65dac();
    self->callbacks_b.~UnkWaterTriggerCallbackList_la65dac();
    self->callbacks_a.~UnkWaterTriggerCallbackList_la65dac();
    self->table.~UnkWaterTriggerTable_la65dac();
    using base::UnkStrIdMember;
    self->name.~UnkStrIdMember();
    fn_71010c4194(reinterpret_cast<UnkBaseComponent*>(self));
}
