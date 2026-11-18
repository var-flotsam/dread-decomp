// CCutsceneComponent's unit (tools/tu_map.py unit 1208): finishes the cutscene unless it is "0112endgame".
// The first time the GUI (g_unk_7101d47330) is busy with it, it only marks it (+0x215); otherwise it
// enables the sub-areas the cutscene names (+0x80) again, drops its references (+0x30) and hands the
// shared records (+0x200) back to the GUI. Names and layouts are placeholders.
#include "base/UnkDestructors.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "game/UnkTU0.h"
#include "game/UnkTU2577.h"
#include "game/UnkTU2730.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::global::CStrId;

struct UnkManager;

base::UnkStrIdMember* fn_71000ce740(base::global::CRntVector<base::UnkStrIdMember>* self, u32 index);

// A bit mask passed by value (as src/game/CAnimatedCameraCtrl/CAnimatedCameraCtrl_Lifecycle_ovb0101.cpp).
struct UnkCamMask_b0101 {
    u32 bits;
    UnkCamMask_b0101() : bits(0) {}
    UnkCamMask_b0101(u32 b) : bits(b) {}
    UnkCamMask_b0101(const UnkCamMask_b0101& o) : bits(o.bits) {}
};
void fn_7100eed7b0(void* manager, const base::UnkStrIdMember* name, UnkCamMask_b0101 mask, bool enable,
                   bool a, bool b);

struct UnkSubareaStaticZero_l7a9e60 {
    u32 value;
    UnkSubareaStaticZero_l7a9e60() : value(0) {}
};

inline const UnkSubareaStaticZero_l7a9e60& UnkSubareaStaticZero() {
    static UnkSubareaStaticZero_l7a9e60 s_value asm("g_unk_7172719980");
    return s_value;
}

inline u32 UnkSubareaAllMask() {
    static u32 s_mask asm("g_unk_7172719970") = ~UnkSubareaStaticZero().value & 0x1ff;
    return s_mask;
}

struct UnkSubarea_l7a9e60 {
    CStrId name;
};

struct UnkSubareaManager_l7a9e60 {
    u8 pad_0x0[0x3f0];
    UnkSubarea_l7a9e60* current;  // 0x3f0

    const CStrId& CurrentName() { return current ? current->name : g_unk_7101d470e0; }
};

static inline bool SameId(const base::UnkStrIdMember& a, const CStrId& b) { return a.data == b.data; }

static inline void* SubareaManager() {
    void* manager = g_unk_7101d47360;
    if (!manager) {
        manager = fn_71000839a0(0xae8, 0x3f);
        fn_7100ee1f34(manager);
        g_unk_7101d47360 = static_cast<UnkManager*>(manager);
    }
    return manager;
}

// A shared object (an atomic count at +8, released through vtable slot 2).
struct UnkCutsceneRecords_l7a9e60 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    s32 ref_count;  // 0x8
    u8 pad_0xc[0xc];
    CStrId name;  // 0x18
    u8 pad_0x20[0x2f0];
    u8 field_0x310[0x8];  // 0x310
};
static_assert(__builtin_offsetof(UnkCutsceneRecords_l7a9e60, field_0x310) == 0x310, "layout");

struct UnkPathIdMember;
void fn_710119a110(void* gui, const UnkPathIdMember* object);

struct UnkCutsceneGui_l7a9e60 {
    u8 pad_0x0[0x58];
    UnkCutsceneRecords_l7a9e60* records;  // 0x58
};

struct UnkCutsceneRef_l7a9e60 {
    UnkRefObject* object;
    u8 pad_0x8[0x8];
};

struct UnkCutsceneComponent_l7a9e60 {
    u8 pad_0x0[0x30];
    UnkCutsceneRef_l7a9e60* refs;  // 0x30
    u32 ref_size;                  // 0x38
    u8 pad_0x3c[0x44];
    base::global::CRntVector<base::UnkStrIdMember> subareas;  // 0x80
    u8 pad_0xa8[0x200 - 0x80 - sizeof(base::global::CRntVector<base::UnkStrIdMember>)];
    UnkCutsceneRecords_l7a9e60* records;  // 0x200
    u8 pad_0x208[0x4];
    bool field_0x20c;
    u8 pad_0x20d[0x8];
    bool field_0x215;
};
static_assert(__builtin_offsetof(UnkCutsceneComponent_l7a9e60, records) == 0x200, "layout");
static_assert(__builtin_offsetof(UnkCutsceneComponent_l7a9e60, field_0x215) == 0x215, "layout");

void fn_71007a9e60(UnkCutsceneComponent_l7a9e60* self) {
    if (UnkCutsceneRecords_l7a9e60* records = self->records) {
        if (records->name == CStrId("0112endgame", true))
            return;
    }
    if ((fn_7101196fd8(g_unk_7101d47330) ||
         static_cast<UnkCutsceneGui_l7a9e60*>(g_unk_7101d47330)->records == self->records) &&
        !self->field_0x215) {
        self->field_0x215 = true;
        return;
    }
    if (self->field_0x20c && self->subareas.count) {
        void* manager = SubareaManager();
        for (u32 i = 0; i < self->subareas.count; ++i) {
            fn_7100eed7b0(manager, fn_71000ce740(&self->subareas, i), UnkCamMask_b0101(UnkSubareaAllMask()), false,
                          true, true);
            if (SameId(*fn_71000ce740(&self->subareas, i),
                       static_cast<UnkSubareaManager_l7a9e60*>(manager)->CurrentName()))
                fn_7100eed7b0(manager, fn_71000ce740(&self->subareas, i), UnkCamMask_b0101(UnkSubareaAllMask()),
                              true, true, false);
        }
    }
    for (u64 i = 0; i < self->ref_size; ++i) {
        UnkRefObject* object = self->refs[i].object;
        if (object && --object->ref_count == 0)
            object->vf_0x10();
    }
    self->ref_size = 0;
    if (self->records && __atomic_load_n(&self->records->ref_count, __ATOMIC_SEQ_CST) != 0) {
        fn_710119a110(g_unk_7101d47330, reinterpret_cast<const UnkPathIdMember*>(self->records->field_0x310));
        if (UnkCutsceneRecords_l7a9e60* p = self->records) {
            __atomic_fetch_sub(&p->ref_count, 1, __ATOMIC_SEQ_CST);
            if (__atomic_load_n(&p->ref_count, __ATOMIC_SEQ_CST) == 0)
                p->vf_0x10();
            self->records = nullptr;
        }
    }
}
