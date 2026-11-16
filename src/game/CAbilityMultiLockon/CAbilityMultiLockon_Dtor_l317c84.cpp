// CAbilityMultiLockon's unit (tools/tu_map.py, unit 431): the multi-lockon ability's destructor. Shuts
// the ability down (fn_7100318130), takes its lock-on marks off the render manager, releases its members
// (last first), then destroys the CAbility part (its six value holders, the member at +0x10) and runs the
// game object's destructor (fn_7101043934). Names and layouts are placeholders.
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CAbility.h"
#include "game/CAbilityMultiLockon.h"
#include "game/CGameObject.h"
#include "game/UnkRenderManager.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::global::CFilePathStrId;
using base::global::CRntVector;
using base::global::CStrId;

struct UnkCameraCtrl_ovb0049;

extern void* g_unk_7101a22db8[];  // CAbilityMultiLockon's vtable
extern void* g_unk_7101a23040[];  // the vtable of the member at +0x3f0

// Drops a counted reference.
static inline void Release(UnkRefObject* p) {
    if (p && --p->ref_count == 0)
        p->vf_0x10();
}

// A value holder: a reflected value and `N` pairs cleared on destruction, padded to 0x70 bytes.
template <int N>
struct UnkValueHolder_l317c84 {
    void** vtable;
    u64 field_0x8;
    u64 field_0x10;
    base::reflection::UnkReflValue value;
    u64 pairs[N][2];
    u64 tail[4 - N][2];

    void Destroy(void** holder_vtable) {
        for (int i = N - 1; i >= 0; --i) {
            pairs[i][0] = 0;
            pairs[i][1] = 0;
        }
        vtable = holder_vtable;
        fn_71000a1c60(&value);
    }
};

struct UnkMember_l317c84 {
    void** vtable;
    u64 field_0x8;
    u64 field_0x10;
    u64 field_0x18;
    u32 field_0x20;

    void Destroy(void** member_vtable) {
        vtable = member_vtable;
        field_0x20 = 0;
        field_0x10 = 0;
        field_0x18 = 0;
    }
};

// A counted handle in the locked-target list.
struct UnkLockedHandle_l317c84 {
    UnkRefObject* p;
    ~UnkLockedHandle_l317c84() { Release(p); }
};

// Two counted references.
struct UnkRefPair_l317c84 {
    UnkRefObject* first;
    UnkRefObject* second;
    u8 pad_0x10[0x20];

    void Destroy() {
        Release(second);
        Release(first);
    }
};

// A mark on screen: its fade time and the mark object (drawn through its item at +8).
struct UnkLockonEntry_l317c84 {
    f32 time;
    u8* mark;
};

struct UnkMultiLockon_l317c84 {
    void** vtable;
    u64 field_0x8;
    UnkMember_l317c84 member;  // 0x10
    u8 pad_0x38[0xb0];
    UnkValueHolder_l317c84<4> holders[6];  // 0xe8
    u8 pad_0x388[0x10];
    CFilePathStrId field_0x398;
    UnkRefObject* slots[5];                       // 0x3a0
    CRntVector<UnkLockedHandle_l317c84> locked;  // 0x3c8
    UnkMember_l317c84 field_0x3f0;
    u8 pad_0x418[0x88];
    UnkValueHolder_l317c84<3> values[3];  // 0x4a0
    u8 pad_0x5f0[0x8];
    CStrId field_0x5f8;
    u8 pad_0x600[0xc];
    u32 pair_count;                       // 0x60c
    UnkRefPair_l317c84 pairs[5];          // 0x610
    CRntVector<UnkLockonEntry_l317c84> marks;  // 0x700

    // CAbility's part of the destructor.
    void DestroyAbility() {
        vtable = &g_unk_7101a223f8[2];
        for (int i = 5; i >= 0; --i)
            holders[i].Destroy(&g_unk_7101a225f8[2]);
        member.Destroy(&g_unk_7101a22650[2]);
        fn_7101043934(reinterpret_cast<UnkCameraCtrl_ovb0049*>(this));
    }
};
static_assert(sizeof(UnkValueHolder_l317c84<3>) == 0x70, "holder size");
static_assert(__builtin_offsetof(UnkMultiLockon_l317c84, field_0x398) == 0x398, "layout");
static_assert(__builtin_offsetof(UnkMultiLockon_l317c84, values) == 0x4a0, "layout");
static_assert(__builtin_offsetof(UnkMultiLockon_l317c84, field_0x5f8) == 0x5f8, "layout");
static_assert(__builtin_offsetof(UnkMultiLockon_l317c84, marks) == 0x700, "layout");

void fn_7100318130(UnkMultiLockon_l317c84* self);

void fn_7100317c84(UnkMultiLockon_l317c84* self) {
    self->vtable = &g_unk_7101a22db8[2];
    fn_7100318130(self);
    UnkLockonEntry_l317c84* e = self->marks.data;
    for (UnkLockonEntry_l317c84* end = e + self->marks.count; e != end; ++e) {
        if (e->mark)
            fn_71002199d8(g_unk_7101d47300, reinterpret_cast<UnkItemBase_71011a91c4*>(e->mark + 8), 0);
    }
    self->marks.Clear();
    self->marks.Free();
    for (u32 i = 0; i < self->pair_count; ++i)
        self->pairs[i].Destroy();
    self->pair_count = 0;
    self->field_0x5f8.Reset();
    for (int i = 2; i >= 0; --i)
        self->values[i].Destroy(reinterpret_cast<void**>(g_unk_7101a22fe8) + 2);
    self->field_0x3f0.Destroy(&g_unk_7101a23040[2]);
    self->locked.Free();
    for (int i = 4; i >= 0; --i)
        Release(self->slots[i]);
    self->field_0x398.~CFilePathStrId();
    self->DestroyAbility();
}
