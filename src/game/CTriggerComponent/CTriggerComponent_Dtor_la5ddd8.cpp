// CTriggerComponent unit (tools/tu_map.py, unit 1578): the component's destructor. Removes its records, deletes
// its owned objects, hands its collision object back to the manager, destroys its members in reverse order and
// runs the component base's destructor. Names and layouts are placeholders.
// build-flags: no-strict-aliasing
#include "base/UnkStrIdTable.h"
#include "base/global/CRntString.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "game/CNavMeshItemComponent.h"
#include "game/CTriggerComponent.h"
#include "game/UnkComponent.h"
#include "game/UnkTU2506.h"
#include "game/UnkTU365.h"
#include "types.h"

using base::global::CRntString;
using base::global::CStrId;

extern void* g_unk_7101a8f5c8[];
// The collision manager (as src/game/CBreakableTileGroup_Reset_b49.cpp declares it).
void fn_7100d46804(UnkManager_7100863a8c* self);

// A reference-counted object, deleted through its vtable slot at +0x10 when the last reference goes.
struct UnkTriggerRefTarget {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    u32 ref_count;
};

// A record of the inline array at +0x64: a counted reference and two words.
struct __attribute__((packed)) UnkTriggerRecord {
    UnkTriggerRefTarget* target;
    u8 pad_0x8[0x10];

    void Release() {
        if (UnkTriggerRefTarget* t = target) {
            if (--t->ref_count == 0)
                t->vf_0x10();
        }
    }
};
static_assert(sizeof(UnkTriggerRecord) == 0x18, "layout");

// An owning pointer of the objects in the array at +0x250.
struct UnkTriggerOwnedPtr {
    UnkTriggerOwned_b56* ptr;

    void Reset() {
        UnkTriggerOwned_b56* old = ptr;
        ptr = nullptr;
        if (old) {
            fn_7100a632e8(old);
            operator delete(old);
        }
    }
    ~UnkTriggerOwnedPtr() { Reset(); }
};

// The member at +0x280: a word, a string id and an id table.
struct UnkTriggerNamedTable {
    u64 field_0x0;
    CStrId id;
    UnkOwnedStrIdTable table;
};
static_assert(sizeof(UnkTriggerNamedTable) == 0x38, "layout");

// The member at +0x2b8: a string and, at +0xd0, a member destroyed by fn_71009c172c.
struct UnkTriggerShape {
    CRntString text;
    u8 pad_0x30[0xa0];
    u8 field_0xd0[0x98];

    void Destroy() {
        fn_71009c172c(field_0xd0);
        fn_710007ed40(&text);
        fn_710007ecd4(&text);
    }
};
static_assert(sizeof(UnkTriggerShape) == 0x168, "layout");

struct UnkTriggerComponent_la5ddd8 : UnkBaseComponent {
    u8 pad_0x40[0x18];
    UnkCollisionObject* collision;  // 0x58
    u32 count;  // 0x60
    UnkTriggerRecord records[16];  // 0x64
    u8 pad_0x1e4[4];
    CStrId name;  // 0x1e8
    CStrId field_0x1f0;
    CStrId field_0x1f8;
    u8 pad_0x200[8];
    CRntString text;  // 0x208
    bool field_0x238;
    u8 pad_0x239[0x17];
    base::global::CRntVector<UnkTriggerOwnedPtr> owned;  // 0x250
    u8 pad_0x278[8];
    UnkTriggerNamedTable field_0x280;
    UnkTriggerShape field_0x2b8;
    CStrId field_0x420;
};
static_assert(__builtin_offsetof(UnkTriggerComponent_la5ddd8, records) == 0x64, "layout");
static_assert(__builtin_offsetof(UnkTriggerComponent_la5ddd8, owned) == 0x250, "layout");
static_assert(__builtin_offsetof(UnkTriggerComponent_la5ddd8, field_0x420) == 0x420, "layout");

// The collision manager, made on first use.
static inline UnkManager_7100863a8c* CollisionManager() {
    UnkManager_7100863a8c* m = g_unk_71726a4788;
    if (!m) {
        m = static_cast<UnkManager_7100863a8c*>(fn_71000839a0(0x6b0, 0x3f));
        fn_7100d46804(m);
        g_unk_71726a4788 = m;
    }
    return m;
}

void fn_7100a5ddd8(UnkTriggerComponent_la5ddd8* self) {
    self->vtable = g_unk_7101a8f5c8 + 2;
    while (self->count != 0) {
        UnkTriggerEntry_b50a5* last =
            reinterpret_cast<UnkTriggerEntry_b50a5*>((self->records - 1) + self->count);
        fn_7100a5fdcc(reinterpret_cast<UnkTrigger_b50a5*>(self), &last);
    }
    for (UnkTriggerOwnedPtr* it = self->owned.begin(); it != self->owned.end(); ++it)
        it->Reset();
    self->owned.Clear();
    self->field_0x238 = false;
    if (self->collision) {
        fn_7100d46c60(CollisionManager(), self->collision);
        self->collision = nullptr;
    }

    self->field_0x420.~CStrId();
    self->field_0x2b8.Destroy();
    self->field_0x280.table.~UnkOwnedStrIdTable();
    self->field_0x280.id.~CStrId();
    self->owned.Free();
    fn_710007ecd4(&self->text);
    self->field_0x1f8.~CStrId();
    self->field_0x1f0.~CStrId();
    self->name.~CStrId();
    for (u32 i = 0; i < self->count; ++i)
        self->records[i].Release();
    self->count = 0;
    fn_71010c4194(self);
}
