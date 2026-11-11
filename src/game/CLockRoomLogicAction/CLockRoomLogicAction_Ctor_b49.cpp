// CLockRoomLogicAction's unit (tools/tu_map.py unit 2531): the constructor, on the logic action base
// (fn_7100a55aa4): the vtable, a callback list at +0x10 (its base vtable, then its own), an empty
// string id at +0x50, three growable arrays (allocation flags 0xf) and two cleared flags. Names and
// layouts are placeholders.
#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "game/CCentralUnitAIComponent.h"
#include "game/CLockRoomLogicAction.h"
#include "game/CTriggerLogicAction.h"
#include "game/UnkTU341.h"
#include "types.h"

using base::global::CStrId;

struct UnkChangeSetupLogicActionDef;

struct UnkLockRoomCallable {
    u64 storage;
    const void* ops;
};

struct UnkLockRoomCallbackList {
    char* vtable;
    base::UnkVector<UnkLockRoomCallable> callables;
    u32 field_0x30;
    void* owner;
};

struct UnkLockRoomLogicAction {
    char* vtable;
    u8 unk_0x8[0x8];
    UnkLockRoomCallbackList callbacks;  // 0x10
    CStrId id;                          // 0x50
    base::UnkVector<void*> field_0x58;
    base::UnkVector<void*> field_0x80;
    base::UnkVector<void*> field_0xa8;
    bool field_0xd0;
    u8 pad_0xd1[7];
    bool field_0xd8;
};
static_assert(__builtin_offsetof(UnkLockRoomLogicAction, field_0xd8) == 0xd8, "layout");

void fn_7100da2fb0(UnkLockRoomLogicAction* self) {
    fn_7100a55aa4(reinterpret_cast<UnkChangeSetupLogicActionDef*>(self));
    self->vtable = g_unk_7101aa9180 + 0x10;
    self->callbacks.vtable = reinterpret_cast<char*>(g_unk_7101a20998) + 0x10;
    new (&self->callbacks.callables) base::UnkVector<UnkLockRoomCallable>(0xf);
    self->callbacks.field_0x30 = 0;
    self->callbacks.owner = nullptr;
    self->callbacks.vtable = reinterpret_cast<char*>(g_unk_7101a33fa0) + 0x10;
    self->id.data = nullptr;
    if (!g_unk_7101d47108)
        base::global::UnkEnsureStringPool();
    self->id.data = &g_unk_7101d47108->empty;
    new (&self->field_0x58) base::UnkVector<void*>(0xf);
    new (&self->field_0x80) base::UnkVector<void*>(0xf);
    new (&self->field_0xa8) base::UnkVector<void*>(0xf);
    self->field_0xd0 = false;
    self->field_0xd8 = false;
}
