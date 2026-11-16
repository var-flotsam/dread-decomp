// CAbilityEnergyWave unit: the energy wave's destructor. Releases its three sets of lists (last first) and
// its four string ids, then destroys the CAbility part (its six value holders, the member at +0x10) and runs
// the game object's destructor (fn_7101043934). Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CAbility.h"
#include "game/CGameObject.h"
#include "types.h"

using base::global::CRntVector;
using base::global::CStrId;

struct UnkCameraCtrl_ovb0049;

extern void* g_unk_7101a22898[];  // CAbilityEnergyWave's vtable
extern void* g_unk_7101a225f8[];  // the value holders' vtable
extern void* g_unk_7101a22650[];  // the vtable of the member at +0x10

// Three lists; the first holds the addresses of slots that refer back to this set.
struct UnkListSet_l305dc0 {
    CRntVector<void**> slots;
    CRntVector<void*> field_0x28;
    CRntVector<void*> field_0x50;
    u8 pad_0x78[0x8];

    void Release() {
        void*** end = slots.data + slots.count;
        for (void*** it = slots.data; it != end; ++it)
            **it = nullptr;
        slots.Clear();
        field_0x28.Clear();
        field_0x50.Clear();
        field_0x50.Free();
        field_0x28.Free();
        slots.Free();
    }
};
static_assert(sizeof(UnkListSet_l305dc0) == 0x80, "UnkListSet_l305dc0 layout");

// A value holder: a reflected value and four pairs cleared on destruction.
struct UnkValueHolder_l305dc0 {
    void** vtable;
    u64 field_0x8;
    u64 field_0x10;
    base::reflection::UnkReflValue value;
    u64 pairs[4][2];

    void Destroy() {
        for (int i = 3; i >= 0; --i) {
            pairs[i][0] = 0;
            pairs[i][1] = 0;
        }
        vtable = &g_unk_7101a225f8[2];
        fn_71000a1c60(&value);
    }
};
static_assert(sizeof(UnkValueHolder_l305dc0) == 0x70, "UnkValueHolder_l305dc0 size");

struct UnkAbilityMember_l305dc0 {
    void** vtable;
    u64 field_0x8;
    u64 field_0x10;
    u64 field_0x18;
    u32 field_0x20;
};

struct UnkEnergyWave_l305dc0 {
    void** vtable;
    u64 field_0x8;
    UnkAbilityMember_l305dc0 member;  // 0x10
    u8 pad_0x38[0xb0];
    UnkValueHolder_l305dc0 holders[6];  // 0xe8
    u8 pad_0x388[0x8];
    CStrId field_0x390;
    CStrId field_0x398;
    CStrId field_0x3a0;
    CStrId gun_id;  // 0x3a8
    u8 pad_0x3b0[0x60];
    UnkListSet_l305dc0 sets[3];  // 0x410

    // CAbility's part of the destructor.
    void DestroyAbility() {
        vtable = &g_unk_7101a223f8[2];
        for (int i = 5; i >= 0; --i)
            holders[i].Destroy();
        member.vtable = &g_unk_7101a22650[2];
        member.field_0x20 = 0;
        member.field_0x10 = 0;
        member.field_0x18 = 0;
        fn_7101043934(reinterpret_cast<UnkCameraCtrl_ovb0049*>(this));
    }
};
static_assert(__builtin_offsetof(UnkEnergyWave_l305dc0, field_0x390) == 0x390, "layout");
static_assert(__builtin_offsetof(UnkEnergyWave_l305dc0, sets) == 0x410, "layout");
static_assert(sizeof(UnkEnergyWave_l305dc0) == 0x590, "layout");

void fn_7100305dc0(UnkEnergyWave_l305dc0* self) {
    self->vtable = &g_unk_7101a22898[2];
    self->sets[2].Release();
    self->sets[1].Release();
    self->sets[0].Release();
    self->gun_id.Reset();
    self->field_0x3a0.Reset();
    self->field_0x398.Reset();
    self->field_0x390.Reset();
    self->DestroyAbility();
}
