// CHyperBeamGun's unit (tools/tu_map.py): the reasons the beam can't fire, its reset, and the array of
// 0x14-byte entries (copy assignment and the grow-to-index accessor), and the beam's collision query
// (each hit handed to fn_71008472d4, then the shape to the hit manager). Names and layouts are placeholders.
// GrowAt is inlined into its out-of-line accessor here.
#define UNK_VECTOR_GROW_AT_ATTR __attribute__((always_inline))
#include "base/UnkVector.h"
#include "base/reflection/UnkClassGetters.h"
#include "game/CDroppableComponent.h"
#include "game/CHydrogigaAIComponent.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU1501.h"
#include "game/UnkTU2510.h"
#include "game/UnkTU2523.h"
#include "game/UnkTU2562.h"
#include "types.h"
#include "base/UnkMatrix44.h"
#include "game_s2/UnkWeakHandle.h"

struct UnkPlayer;
UnkPlayer* fn_7100dec788(UnkGameManager*);
struct UnkFlagged;
bool fn_71009e85b4(UnkFlagged*);
bool fn_71009ebf5c(UnkFlagged*);
bool fn_71009e6940(UnkFlagged*);
bool fn_71009ebf7c(UnkFlagged*);

struct UnkHyperBeamPlayer_ovb0127 {
    u8 pad_0x0[0x208];
    UnkFlagged* gun;  // 0x208
};

static inline bool GunVf0x3b8(UnkFlagged* gun) {
    void** vtable = *reinterpret_cast<void***>(gun);
    return reinterpret_cast<bool (*)(UnkFlagged*)>(vtable[0x3b8 / 8])(gun);
}

struct UnkHyperBeamOwner_ovb0127 {
    u8 pad_0x0[0x220];
    UnkAnimationComponent* animation;  // 0x220
};

struct CHyperBeamGun_ovb0127 {
    u8 pad_0x0[0x18];
    UnkHyperBeamOwner_ovb0127* owner;  // 0x18
};

// The reasons the hyper beam can't fire, as flags (0 while the animation forces it to fire).
u32 fn_71008434b8(CHyperBeamGun_ovb0127* self) {
    UnkFlagged* gun = reinterpret_cast<UnkHyperBeamPlayer_ovb0127*>(fn_7100dec788(g_unk_7101d47310))->gun;
    UnkAnimationComponent* animation = self->owner->animation;
    if (animation && fn_710117b348(animation, fn_7100c1e458()))
        return 0;
    bool ignore = animation && fn_710117b348(animation, fn_7100c2fe88());
    u32 flags = fn_710117b348(self->owner->animation, fn_7100cf0d38()) ? 0x100 : 0;
    if (gun) {
        flags |= fn_71009e75e8(gun);
        if (GunVf0x3b8(gun))
            flags |= 2;
        if (fn_71009e85b4(gun))
            flags |= 4;
        if (fn_71009ebf5c(gun))
            flags |= 8;
        if (fn_71009e6940(gun))
            flags |= 0x20;
        if (fn_71009ebf7c(gun))
            flags |= 0x200;
    }
    if (ignore)
        flags |= 0x80;
    return flags;
}

struct UnkWorld;
struct UnkPart;
UnkPart* fn_7101152b90(UnkWorld*, u64);
struct UnkSoundHandle;
struct UnkShipRechargeEntity_b49a4;

struct UnkHyperBeamPart_ovb0127 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual void vf_0x40(bool enabled);
};

struct UnkHyperBeamSoundEntry_ovb0127 {
    void* playing;
    s64 serial;
};

struct UnkHyperBeamSound_ovb0127 {
    UnkHyperBeamSoundEntry_ovb0127* entry;
    s64 serial;
    bool IsPlaying() const { return entry && entry->serial == serial && serial != -1 && entry->playing; }
};

struct UnkHyperBeamOwner2_ovb0127 {
    u8 pad_0x0[0x280];
    UnkWorld* parts;  // 0x280
};

struct CHyperBeamGunState_ovb0127 {
    u8 pad_0x0[0x18];
    UnkHyperBeamOwner2_ovb0127* owner;  // 0x18
    u8 pad_0x20[0x1880];
    u64 field_0x18a0;
    u8 pad_0x18a8[0x10];
    UnkHyperBeamSound_ovb0127 sound_a;  // 0x18b8
    UnkHyperBeamSound_ovb0127 sound_b;  // 0x18c8
    u8 pad_0x18d8[0x22];
    bool charging;  // 0x18fa
    u8 pad_0x18fb[0x1];
    u32 field_0x18fc;
    bool field_0x1900;
    bool field_0x1901;
    bool field_0x1902;
    u8 pad_0x1903[0x1];
    u32 field_0x1904;
};

void fn_71008458f8(CHyperBeamGunState_ovb0127* self, bool flag);

static inline void DisablePart(CHyperBeamGunState_ovb0127* self, u64 hash) {
    if (auto* part = reinterpret_cast<UnkHyperBeamPart_ovb0127*>(fn_7101152b90(self->owner->parts, hash)))
        part->vf_0x40(false);
}

static inline void StopSound(CHyperBeamGunState_ovb0127* self, UnkHyperBeamSound_ovb0127* sound) {
    if (sound->IsPlaying())
        fn_7100d7c510(reinterpret_cast<UnkShipRechargeEntity_b49a4*>(self->owner),
                      reinterpret_cast<UnkSoundHandle*>(sound), 0.0f, 0.0f);
}

void fn_7100846830(CHyperBeamGunState_ovb0127* self) {
    if (self->charging && *reinterpret_cast<u32*>(&self->field_0x18a0) == 0)
        fn_71008458f8(self, true);
    self->charging = false;
    self->field_0x1900 = true;
    self->field_0x1901 = true;
    self->field_0x1902 = false;
    self->field_0x18a0 = 0;
    self->field_0x18fc = 0;
    self->field_0x1904 = 0;
    DisablePart(self, 0x805489787ebd4a19);
    DisablePart(self, 0x7089587c48475c03);
    StopSound(self, &self->sound_a);
    StopSound(self, &self->sound_b);
}

// An entry of 0x14 bytes whose last word (a float) starts at -1 whenever it is made or copied.
struct UnkHyperBeamEntry_ovb0127 {
    u32 values[4];
    f32 field_0x10;
    UnkHyperBeamEntry_ovb0127() : values{0, 0, 0, 0}, field_0x10(-1.0f) {}
    UnkHyperBeamEntry_ovb0127(const UnkHyperBeamEntry_ovb0127& o) {
        for (int i = 0; i < 4; ++i)
            __builtin_memcpy(&values[i], &o.values[i], sizeof(u32));
        field_0x10 = -1.0f;
    }
};

using UnkHyperBeamEntries_ovb0127 = base::UnkVector<UnkHyperBeamEntry_ovb0127>;

__attribute__((flatten)) UnkHyperBeamEntries_ovb0127* fn_710084888c(UnkHyperBeamEntries_ovb0127* self,
                                                                    const UnkHyperBeamEntries_ovb0127* other) {
    if (self != other) {
        self->Clear();
        self->Reserve(other->capacity);
        for (const UnkHyperBeamEntry_ovb0127 *p = other->data, *end = p + other->count; p != end; ++p)
            self->PushBackCopy(*p);
    }
    return self;
}

UnkHyperBeamEntry_ovb0127* fn_7100848ab8(UnkHyperBeamEntries_ovb0127* self, u32 index) {
    return &self->GrowAt(index);
}

struct UnkTree;
struct UnkShape;
struct UnkMtx44;
extern UnkMatrix44 g_unk_7101a1a680;  // the identity matrix

struct __attribute__((packed)) UnkBeamHitRecord_ovb0127 {
    UnkRefObject* field_0x0;
    UnkRefObject* field_0x8;
    u8 pad_0x10[0x38];
};

static inline void ReleaseRef(UnkRefObject* p) {
    if (p && --p->ref_count == 0)
        p->vf_0x10();
}

// The hits of a collision query (the list of src/game/CLogicDroppedItemManager.cpp).
struct __attribute__((packed)) UnkBeamHitList_ovb0127 {
    void** vtable;
    u32 count;
    u8 storage[32 * sizeof(UnkBeamHitRecord_ovb0127)];

    UnkBeamHitRecord_ovb0127* items() { return reinterpret_cast<UnkBeamHitRecord_ovb0127*>(storage); }

    UnkBeamHitList_ovb0127() {
        vtable = g_unk_7101a70eb0 + 2;
        count = 0;
    }
    ~UnkBeamHitList_ovb0127() {
        vtable = g_unk_7101a70eb0 + 2;
        for (u64 i = 0; i < count; i++) {
            ReleaseRef(items()[i].field_0x8);
            ReleaseRef(items()[i].field_0x0);
        }
        count = 0;
    }
};

struct UnkHyperBeamGunHits_ovb0127;
void fn_71008472d4(UnkHyperBeamGunHits_ovb0127* self, UnkBeamHitRecord_ovb0127* hit, void* unused, void* arg, bool flag);
void fn_7100e2f10c(void* manager, UnkShape* shape);

static inline void* HitManager() {
    void* instance = g_unk_71726eb3e8;
    if (!instance) {
        instance = fn_71000839a0(0xa0f8, 0x3f);
        fn_7100e211cc(instance);
        g_unk_71726eb3e8 = instance;
    }
    return instance;
}

void fn_71008481f4(UnkHyperBeamGunHits_ovb0127* self, UnkShape* shape, void* arg) {
    if (!shape)
        return;
    UnkBeamHitList_ovb0127 hits;
    UnkTree* tree = static_cast<UnkTree*>(g_unk_7101d47310->field_0x608->field_0x820);
    if (fn_7100d5b484(tree, 0x81481, shape, reinterpret_cast<const UnkMtx44*>(&g_unk_7101a1a680), &hits, 0)) {
        // The third argument is never read by the callee; the original leaves its register as it is.
        void* unused;
        for (UnkBeamHitRecord_ovb0127 *hit = hits.items(), *end = hit + hits.count; hit != end; ++hit)
            fn_71008472d4(self, hit, unused, arg, false);
    }
    fn_7100e2f10c(HitManager(), shape);
}
