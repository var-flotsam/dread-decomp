// CTutoEnterLogicAction unit (tools/tu_map.py): its constructor, on the logic-action base
// (fn_7100a55aa4): the vtable, a flag pair at +0x10, a string id at +0x18 built from "", a callback
// set at +0x20 (built through its base: a vtable, an empty array with allocator flags 0xf, a cleared
// word; then its own vtable and a cleared owner), an empty string id at +0x60, a delegate at +0x68
// bound to the object and fn_7100a59074 (CTutoEnterLogicAction_b48.cpp), then an empty string id at
// +0x80. Names and layouts are placeholders.
#include <new>

#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "game/CTriggerLogicAction.h"
#include "types.h"

using base::global::CStrId;
using base::global::UnkStringData;

// The base constructor, as src/game/CChangeSetupLogicAction/CChangeSetupLogicAction_b35.cpp declares it.
struct UnkChangeSetupLogicActionDef;
struct UnkTutoEnterAction_b48;
u32 fn_7100a59074(UnkTutoEnterAction_b48* self);
extern char g_unk_7101a8f518[];
extern void* g_unk_7101a20998[];  // the callback set's base vtable
extern void* g_unk_7101a20950[];  // the callback set's vtable

struct UnkTutoEnterCallable_b52;

struct UnkTutoEnterCallbackSet_b52 {
    void* vtable;
    base::UnkVector<UnkTutoEnterCallable_b52*> items;
    u32 field_0x30;
    void* owner;
};

static inline __attribute__((always_inline)) void InitEmpty_te52(UnkStringData** id) {
    *id = nullptr;
    if (!g_unk_7101d47108)
        base::global::UnkEnsureStringPool();
    *id = &g_unk_7101d47108->empty;
}

struct UnkTutoEnterLogicAction_b52 {
    void* vtable;
    u8 unk_0x8[0x8];
    bool field_0x10;
    bool field_0x11;
    CStrId id;                              // 0x18
    UnkTutoEnterCallbackSet_b52 callbacks;  // 0x20
    UnkStringData* name;                    // 0x60
    void* field_0x68;
    void* target;                                // 0x70
    u32 (*method)(UnkTutoEnterAction_b48*);      // 0x78
    UnkStringData* field_0x80;
};
static_assert(__builtin_offsetof(UnkTutoEnterLogicAction_b52, field_0x80) == 0x80, "layout");

void fn_7100a585a8(UnkTutoEnterLogicAction_b52* self) {
    fn_7100a55aa4(reinterpret_cast<UnkChangeSetupLogicActionDef*>(self));
    self->vtable = g_unk_7101a8f518 + 0x10;
    self->field_0x10 = true;
    self->field_0x11 = false;
    new (&self->id) CStrId("", true);
    self->callbacks.vtable = g_unk_7101a20998 + 2;
    new (&self->callbacks.items) base::UnkVector<UnkTutoEnterCallable_b52*>(0xf);
    self->callbacks.field_0x30 = 0;
    self->callbacks.vtable = g_unk_7101a20950 + 2;
    self->callbacks.owner = nullptr;
    InitEmpty_te52(&self->name);
    self->field_0x68 = nullptr;
    self->target = self;
    self->method = &fn_7100a59074;
    InitEmpty_te52(&self->field_0x80);
}
