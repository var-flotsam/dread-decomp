// CTriggerComponent unit (tools/tu_map.py, unit 1578): the component's constructor: the component base,
// the vtable, the name (empty), two string ids, a string, the record count, an array, two reflected
// members, then the state reset (both ids set to the global empty id, the flags cleared, the string
// emptied). Names and layouts are placeholders.
#include <new>

#include "base/global/CRntString.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "game/CComponent.h"
#include "game/CTriggerComponent.h"
#include "game/UnkTU57.h"
#include "game/UnkTUProperty.h"
#include "types.h"

using base::global::CRntString;
using base::global::CStrId;

void fn_7100a62f34(void* member);

struct UnkTriggerComponent_la5dac0 {
    void** vtable;
    u8 pad_0x8[0x35];
    bool field_0x3d;
    u8 pad_0x3e[2];
    s64 field_0x40;
    bool field_0x48;
    bool field_0x49;
    bool field_0x4a;
    bool field_0x4b;
    bool field_0x4c;
    bool field_0x4d;
    bool field_0x4e;
    bool field_0x4f;
    u32 field_0x50;
    u8 pad_0x54[4];
    u64 field_0x58;
    u32 count;  // 0x60
    u8 records[16][0x18];  // 0x64
    u8 pad_0x1e4[4];
    CStrId name;  // 0x1e8
    CStrId field_0x1f0;
    CStrId field_0x1f8;
    bool field_0x200;
    bool field_0x201;
    bool field_0x202;
    bool field_0x203;
    bool field_0x204;
    bool field_0x205;
    u8 pad_0x206[2];
    CRntString text;  // 0x208
    bool field_0x238;
    u8 pad_0x239[3];
    u32 field_0x23c;
    bool field_0x240;
    u8 pad_0x241[3];
    u32 field_0x244;
    u32 field_0x248;
    u32 field_0x24c;
    base::global::CRntVector<void*> field_0x250;
    bool field_0x278;
    u8 pad_0x279[7];
    u8 field_0x280[0x38];
    u8 field_0x2b8[0x160];
    bool field_0x418;
    u8 pad_0x419[7];
    CStrId field_0x420;
};
static_assert(__builtin_offsetof(UnkTriggerComponent_la5dac0, text) == 0x208, "layout");
static_assert(__builtin_offsetof(UnkTriggerComponent_la5dac0, field_0x250) == 0x250, "layout");
static_assert(__builtin_offsetof(UnkTriggerComponent_la5dac0, field_0x420) == 0x420, "layout");

// An empty string id: the default constructor, inlined.
static inline void MakeEmpty(CStrId* id) {
    id->data = nullptr;
    if (!g_unk_7101d47108)
        base::global::UnkEnsureStringPool();
    id->data = &g_unk_7101d47108->empty;
}

// The component's state as a fresh one has it.
static inline void ResetTrigger(UnkTriggerComponent_la5dac0* self) {
    self->field_0x1f0 = base::global::CStrId::Empty();
    self->field_0x1f8 = base::global::CStrId::Empty();
    self->field_0x3d = false;
    self->field_0x278 = false;
    self->field_0x200 = false;
    self->field_0x201 = false;
    self->field_0x202 = false;
    self->field_0x203 = false;
    self->field_0x204 = false;
    self->field_0x205 = false;
    CRntString_AssignCString(&self->text, g_unk_710162e2ed);
    self->field_0x240 = false;
    self->field_0x244 = 0;
    self->field_0x248 = 0;
    self->field_0x24c = 0;
    self->field_0x4a = true;
    self->field_0x4b = true;
    self->field_0x4c = true;
    self->field_0x4d = true;
    self->field_0x4e = true;
    self->field_0x4f = true;
    self->field_0x238 = false;
    self->field_0x23c = 0;
}

// Declared as src/game/CColliderTriggerComponent/CColliderTriggerComponent.cpp declares it.
void fn_7100a5dac0(void* p) {
    UnkTriggerComponent_la5dac0* self = static_cast<UnkTriggerComponent_la5dac0*>(p);
    fn_71010c4148(static_cast<UnkStateObject*>(p));
    self->vtable = g_unk_7101a8f5c8 + 2;
    self->field_0x40 = -1;
    self->field_0x48 = false;
    self->field_0x49 = false;
    self->field_0x50 = 0x1090;
    self->field_0x58 = 0;
    self->count = 0;
    new (&self->name) CStrId(g_unk_710162e2ed, true);
    MakeEmpty(&self->field_0x1f0);
    MakeEmpty(&self->field_0x1f8);
    fn_710007e6c4(&self->text);
    self->field_0x244 = 0;
    self->field_0x248 = 0;
    self->field_0x24c = 0;
    new (&self->field_0x250) base::global::CRntVector<void*>(0xf);
    fn_7100a62f34(self->field_0x280);
    fn_7100ae9cc8(self->field_0x2b8);
    self->field_0x418 = true;
    new (&self->field_0x420) CStrId(base::global::CStrId::Empty());
    ResetTrigger(self);
}
