// CAIAttackComponent unit (tools/tu_map.py unit 2705). Names and layouts are placeholders.
// - fn_7101133dc8: with the owner's object at +0x218, unregisters the listener at +0x308 from what the
//   weak handle at +0x320 points at (fn_71010cb2fc), drops the handle, has the object fill the slot
//   again (fn_71006f552c) and registers the listener with the new target (fn_71010cb2f4).
// - fn_710113420c: the read: two strings and a u32 (each only while the file has data left) into
//   +0x138, +0x168 and +0x198.
// - fn_7101134648: unless the owner's AI (+0x218) is in state 2 (fn_71010cc3a4), starts the attack named
//   by fn_71003627f8 through vf_0x170.
#include "base/global/CRntFile.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "game/CAINavigationComponent.h"
#include "game/CEnemyMovement.h"
#include "game/UnkActor.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::global::CRntFile;
using base::global::CRntString;
using base::global::CStrId;

struct UnkAIAttackOwner2_ovb0050 {
    u8 pad_0x0[0x218];
    void* field_0x218;
};

struct UnkAIAttackComponent2_ovb0050 {
    u8 pad_0x0[0x30];
    UnkAIAttackOwner2_ovb0050* owner;  // 0x30
    u8 pad_0x38[0x2d0];
    u8 link[0x18];          // 0x308
    UnkWeakHandle* handle;  // 0x320
};

// The handle's slot, addressed from the raw bytes: the original computes it apart from the other
// accesses to the handle.
static inline UnkWeakHandle** HandleSlot(UnkAIAttackComponent2_ovb0050* self) {
    return reinterpret_cast<UnkWeakHandle**>(reinterpret_cast<u8*>(self) + 0x320);
}

bool fn_7101133dc8(UnkAIAttackComponent2_ovb0050* self) {
    void* source = self->owner->field_0x218;
    if (!source)
        return true;
    void* link = self->link;
    if (UnkWeakHandle* handle = self->handle) {
        if (handle->target) {
            fn_71010cb2fc(handle->target, link);
            if (!self->handle)
                goto fetch;
        }
        ResetRef(&self->handle);
    }
fetch:
    fn_71006f552c(source, HandleSlot(self));
    fn_71010cb2f4(self->handle ? self->handle->target : nullptr, link);
    return true;
}

// As src/game/CTargetComponent/CTargetComponent_c2_2.cpp declares it.
void fn_710007ca94(CRntFile* file, char* buf);

struct UnkAIAttackComponent3_ovb0050 {
    u8 pad_0x0[0x138];
    CRntString field_0x138;
    CRntString field_0x168;
    u32 field_0x198;
};

void fn_710113420c(UnkAIAttackComponent3_ovb0050* self, CRntFile* file) {
    char first[0x200] = {};
    if (file->position < file->size)
        fn_710007ca94(file, first);
    char second[0x200] = {};
    if (file->position < file->size)
        fn_710007ca94(file, second);
    u32 value = 0;
    if (file->position < file->size)
        fn_710007c714(file, &value, 4, 1);
    {
        CRntString text(first);
        fn_710007eaa0(&self->field_0x138, &text);
    }
    {
        CRntString text(second);
        fn_710007eaa0(&self->field_0x168, &text);
    }
    self->field_0x198 = value;
}

struct UnkGoliathXSlamAttack_b58a3;
CStrId fn_71003627f8(UnkGoliathXSlamAttack_b58a3*, UnkActor*);

struct UnkAIAttackComponent4_ovb0050 {
    void** vtable;
    u8 pad_0x8[0x28];
    UnkActor* owner;  // 0x30

    bool vf_0x170(const CStrId* id, bool flag) {
        return reinterpret_cast<bool (*)(UnkAIAttackComponent4_ovb0050*, const CStrId*, bool)>(
            vtable[0x170 / 8])(this, id, flag);
    }
};

bool fn_7101134648(UnkAIAttackComponent4_ovb0050* self, UnkGoliathXSlamAttack_b58a3* attack, bool flag) {
    void* ai = self->owner->field_0x218;
    if (!ai || fn_71010cc3a4(ai) != 2) {
        bool started;
        {
            CStrId id = fn_71003627f8(attack, self->owner);
            started = self->vf_0x170(&id, flag);
        }
        if (started)
            return true;
    }
    return false;
}
