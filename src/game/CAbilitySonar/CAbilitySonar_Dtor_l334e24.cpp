// CAbilitySonar's unit (tools/tu_map.py unit 434): the sonar's destructor. Unregisters (fn_7100a0f758,
// fn_7100a0df38), hands its two models back to the model manager, destroys its ids, strings and lists, then
// CAbility's members and the base destructor fn_7101043934. Names and layouts are placeholders.
#include "base/global/CRntString.h"
#include "base/global/CRntVector.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CAbility.h"
#include "game/CGameObject.h"
#include "game/UnkModelManager.h"
#include "game/UnkTU2739.h"
#include "game/UnkTU32.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

extern void* g_unk_7101a235c0[];  // CAbilitySonar's vtable

void fn_7100a0f758();
void fn_7100a0df38();

struct UnkCameraCtrl_ovb0049;

static inline UnkModelManager* ModelManager() {
    UnkModelManager* m = g_unk_71726b0cd8;
    if (!m) {
        m = static_cast<UnkModelManager*>(fn_71000839a0(0x1e8, 0x3f));
        fn_71011a870c(m);
        g_unk_71726b0cd8 = m;
    }
    return m;
}

struct UnkSonarClearedPair_l334e24 {
    u64 a, b;

    void Destroy() {
        a = 0;
        b = 0;
    }
};

// A value holder: a reflected value and four pairs cleared on destruction.
struct UnkSonarValueHolder_l334e24 {
    void** vtable;
    u64 field_0x8;
    u64 field_0x10;
    base::reflection::UnkReflValue value;
    UnkSonarClearedPair_l334e24 pairs[4];

    void Destroy() {
        for (int i = 3; i >= 0; --i)
            pairs[i].Destroy();
        vtable = &g_unk_7101a225f8[2];
        fn_71000a1c60(&value);
    }
};
static_assert(sizeof(UnkSonarValueHolder_l334e24) == 0x70, "holder size");

struct UnkSonarMember_l334e24 {
    void** vtable;
    u64 field_0x8;
    u64 field_0x10;
    u64 field_0x18;
    u32 field_0x20;

    void Destroy() {
        vtable = &g_unk_7101a22650[2];
        field_0x20 = 0;
        field_0x10 = 0;
        field_0x18 = 0;
    }
};

// base::global::CRntVector's layout, of counted references.
struct UnkSonarTargetList_l334e24 {
    UnkRefObject** data;
    u32 count;
    u32 capacity;
    u32 step;
    bool growable;
    u32 flags;
    base::IAllocator* allocator;

    void Destroy() {
        if (data) {
            for (u32 i = 0; i < count; ++i) {
                if (UnkRefObject* t = data[i]) {
                    if (--t->ref_count == 0)
                        t->vf_0x10();
                }
            }
            count = 0;
            allocator->vf_0x48(data);
            capacity = 0;
        }
    }
};

struct UnkAbilitySonar_l334e24 {
    void** vtable;
    u64 field_0x8;
    UnkSonarMember_l334e24 member;  // 0x10
    u8 pad_0x38[0xe8 - 0x38];
    UnkSonarValueHolder_l334e24 holders[6];  // 0xe8
    u8 pad_0x388[0x390 - 0x388];
    void* model_0x390;  // 0x390
    void* model_0x398;  // 0x398
    UnkSonarTargetList_l334e24 targets;  // 0x3a0
    u8 pad_0x3c8[0x3d4 - 0x3c8];
    u32 pulse_count;  // 0x3d4
    u8 pad_0x3d8[0x4d8 - 0x3d8];
    u32 hit_count;  // 0x4d8
    u8 pad_0x4dc[0x6f8 - 0x4dc];
    base::global::CRntString string_0x6f8;  // 0x6f8
    base::global::CRntString string_0x728;  // 0x728
    u8 pad_0x758[0x760 - 0x758];
    u8 ids_0x760[0x18];  // 0x760
    u8 ids_0x778[0x18];  // 0x778
};
static_assert(__builtin_offsetof(UnkAbilitySonar_l334e24, holders) == 0xe8, "layout");
static_assert(__builtin_offsetof(UnkAbilitySonar_l334e24, targets) == 0x3a0, "layout");
static_assert(__builtin_offsetof(UnkAbilitySonar_l334e24, string_0x6f8) == 0x6f8, "layout");
static_assert(__builtin_offsetof(UnkAbilitySonar_l334e24, ids_0x778) == 0x778, "layout");

static void ReleaseModel(void*& model) {
    if (void* m = model)
        fn_71011a91c4(ModelManager(), m, 0);
    model = nullptr;
}

void fn_7100334e24(UnkAbilitySonar_l334e24* self) {
    self->vtable = &g_unk_7101a235c0[2];
    fn_7100a0f758();
    fn_7100a0df38();
    ReleaseModel(self->model_0x398);
    ReleaseModel(self->model_0x390);
    fn_7100026f70(reinterpret_cast<UnkIdPair*>(self->ids_0x778));
    fn_7100026f70(reinterpret_cast<UnkIdPair*>(self->ids_0x760));
    CRntString_Destroy(&self->string_0x728);
    CRntString_Destroy(&self->string_0x6f8);
    self->hit_count = 0;
    self->pulse_count = 0;
    self->targets.Destroy();

    // CAbility's members, then its base.
    self->vtable = &g_unk_7101a223f8[2];
    for (int i = 5; i >= 0; --i)
        self->holders[i].Destroy();
    self->member.Destroy();
    fn_7101043934(reinterpret_cast<UnkCameraCtrl_ovb0049*>(self));
}
