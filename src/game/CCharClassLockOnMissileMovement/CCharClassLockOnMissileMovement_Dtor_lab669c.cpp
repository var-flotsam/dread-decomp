// CCharClassLockOnMissileMovement's unit (tools/tu_map.py, unit 1616): the movement's destructor. Takes the
// trail effect (+0x860) out of the render manager, destroys the path at +0x870, frees the array at +0x830,
// drops the reference at +0x810, clears the member at +0x760 and the two value holders, then runs the
// base destructor fn_7100aba38c. Names and layouts are placeholders.
#include "base/UnkCloneHelpers.h"
#include "base/global/CRntVector.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkRenderManager.h"
#include "game/navmesh/SNavMeshPath.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

struct UnkItemBase_71011a91c4;
struct UnkObject_71002ad0dc;

extern char g_unk_7101a95ac0[];  // the movement's vtable
extern char g_unk_7101a95f80[];
extern char g_unk_7101a95f28[];

// The base destructor.
void fn_7100aba38c(char* self);

struct UnkLockOnValueTag_lab669c;
template <>
struct base::UnkValueHolderTraits<UnkLockOnValueTag_lab669c> {
    static char* vtable() { return g_unk_7101a95f28; }
};
using UnkLockOnValue_lab669c = base::UnkValueHolder<UnkLockOnValueTag_lab669c>;

struct UnkLockOnCleared_lab669c {
    char* vtable;
    u8 pad_0x8[8];
    u64 field_0x10;
    u64 field_0x18;
    u32 field_0x20;

    ~UnkLockOnCleared_lab669c() {
        vtable = g_unk_7101a95f80 + 0x10;
        field_0x10 = 0;
        field_0x18 = 0;
        field_0x20 = 0;
    }
};

// The trail effect: its render item starts at +8.
struct UnkLockOnEffect_lab669c {
    void* vtable;
    u8 item[8];  // 0x8
};

struct UnkLockOnMissileMovement_lab669c {
    char* vtable;
    u8 pad_0x8[0x680 - 0x8];
    UnkLockOnValue_lab669c values[2];  // 0x680
    UnkLockOnCleared_lab669c cleared;  // 0x760
    u8 pad_0x788[0x810 - 0x788];
    UnkRefObject* ref;  // 0x810
    u8 pad_0x818[0x830 - 0x818];
    base::global::CRntVector<void*> points;  // 0x830
    u8 pad_0x858[8];
    UnkLockOnEffect_lab669c* effect;  // 0x860
    u8 pad_0x868[8];
    u8 path[8];  // 0x870
};
static_assert(__builtin_offsetof(UnkLockOnMissileMovement_lab669c, effect) == 0x860, "layout");

void fn_7100ab669c(UnkLockOnMissileMovement_lab669c* self) {
    self->vtable = g_unk_7101a95ac0 + 0x10;
    if (UnkLockOnEffect_lab669c* effect = self->effect) {
        fn_71002199d8(g_unk_7101d47300, reinterpret_cast<UnkItemBase_71011a91c4*>(effect->item), 0);
        self->effect = nullptr;
    }
    fn_71002ad0dc(reinterpret_cast<UnkObject_71002ad0dc*>(self->path));
    self->points.Free();
    if (UnkRefObject* r = self->ref) {
        if (--r->ref_count == 0)
            r->vf_0x10();
    }
    self->cleared.~UnkLockOnCleared_lab669c();
    for (int i = 1; i >= 0; --i)
        base::refl_ValueHolderDtor(&self->values[i]);
    fn_7100aba38c(reinterpret_cast<char*>(self));
}
