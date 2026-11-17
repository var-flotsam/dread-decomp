// CBeamDoorLifeComponent's unit (tools/tu_map.py unit 1356): a hit on the door. Up to three hits
// are counted, each beam kind (by its hash) marking its own slot and sending its id to the owner's
// timeline; after the third, all three marked opens the door (the ids are cleared, the listener is
// moved onto the timeline and the hit is kept), otherwise the count starts over. Returns 0. Names
// and layouts are placeholders.
#include "game/CCentralUnitCannonBeamMovementComponent.h"
#include "game/CTimelineComponent.h"
#include "game/UnkActor.h"
#include "game/UnkTU2523.h"
#include "game/UnkTimelineComponent.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

extern u64 g_unk_710164afd8;
extern u64 g_unk_710164afe0;
extern u64 g_unk_710164afe8;
extern u64 g_unk_710164aff0;

void fn_7100a2d4c8(UnkWeakTarget* target, void* listener);
void fn_710082efe8(UnkTimelineComponent* timeline, UnkWeakHandle** out);

struct UnkBeamDoorLife_l8ace98;
void fn_71008ad2b8(UnkBeamDoorLife_l8ace98* self);

struct UnkBeamKind_l8ace98 {
    u8 pad_0x0[0x30];
    u64 hash;  // 0x30
};

struct UnkBeamWeapon_l8ace98 {
    u8 pad_0x0[0x20];
    UnkBeamKind_l8ace98* kind;  // 0x20
};

struct UnkBeamHit_l8ace98 {
    u8 pad_0x0[0x60];
    UnkBeamWeapon_l8ace98* weapon;  // 0x60
    u8 pad_0x68[0x4c];
    u16 flags;                      // 0xb4
    u8 pad_0xb6[0x40];
    bool counts;                    // 0xf6
};

// A listener: the object and function it calls, and the counted link to what it listens to.
struct UnkBeamDoorListener_l8ace98 {
    void** vtable;
    UnkBeamDoorLife_l8ace98* context;             // 0x8
    void (*callback)(UnkBeamDoorLife_l8ace98*);   // 0x10
    UnkWeakHandle* link;                          // 0x18
};

struct UnkBeamDoorLife_l8ace98 {
    u8 pad_0x0[0x30];
    UnkActor* owner;  // 0x30
    u8 pad_0x38[0x45c];
    s32 hits;         // 0x494
    u8 pad_0x498[0x8];
    bool opened;      // 0x4a0
    u8 pad_0x4a1[0x7];
    u8 last_hit[0x100];                    // 0x4a8
    UnkBeamDoorListener_l8ace98 listener;  // 0x5a8
    bool hit_a;       // 0x5c8
    bool hit_b;       // 0x5c9
    bool hit_c;       // 0x5ca
};

u32 fn_71008ace98(UnkBeamDoorLife_l8ace98* self, const UnkBeamHit_l8ace98* hit) {
    if (self->opened)
        return 0;
    if (self->hits < 3) {
        UnkActor* owner = self->owner;
        if (owner->field_0x2e8 && hit->counts) {
            bool strong = (hit->flags & 0x7020) != 0;
            switch (hit->weapon->kind->hash) {
            case 0xbc8344f7f82c2411:
                ++self->hits;
                self->hit_a = strong;
                fn_7100d7c828(owner, &g_unk_710164afd8, false);
                break;
            case 0xfb293e1e53cbdb25:
                ++self->hits;
                self->hit_b = strong;
                fn_7100d7c828(owner, &g_unk_710164afe0, false);
                break;
            case 0x480772a0506c844a:
                ++self->hits;
                self->hit_c = strong;
                fn_7100d7c828(owner, &g_unk_710164afe8, false);
                break;
            }
        }
    }
    if (self->hits == 3) {
        if (self->hit_c && self->hit_b && self->hit_a) {
            self->opened = true;
            fn_7100d7c94c(self->owner, &g_unk_710164afd8);
            fn_7100d7c94c(self->owner, &g_unk_710164afe0);
            fn_7100d7c94c(self->owner, &g_unk_710164afe8);
            fn_7100d7c828(self->owner, &g_unk_710164aff0, false);
            UnkTimelineComponent* timeline = static_cast<UnkTimelineComponent*>(self->owner->field_0x2e8);
            if (timeline) {
                self->listener.context = self;
                self->listener.callback = fn_71008ad2b8;
                if (UnkWeakHandle* link = self->listener.link) {
                    if (UnkWeakTarget* target = link->target)
                        fn_7100a2d4d0(target, &self->listener);
                    if (UnkWeakHandle* p = self->listener.link) {
                        if (--p->ref_count == 0)
                            p->vf_0x10();
                        self->listener.link = nullptr;
                    }
                }
                fn_710082efe8(timeline, &self->listener.link);
                UnkWeakHandle* link = self->listener.link;
                fn_7100a2d4c8(link ? link->target : nullptr, &self->listener);
            }
            fn_7100400364(reinterpret_cast<UnkWeaponDamage_ovb0054*>(self->last_hit),
                          reinterpret_cast<const UnkWeaponDamage_ovb0054*>(hit));
        } else {
            self->hits = 0;
        }
    }
    return 0;
}
