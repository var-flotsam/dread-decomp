// CProjectileMovement unit (tools/tu_map.py unit 1623): the movement's hit handler: runs the shared hit
// (fn_7100ac7f08), makes sure the owner's timeline has the first static key unless the hit object is
// one that is ignored, and when the hit did not stop it applies the definition's follow-ups (moving the
// owner to the stored position, marking the movement). Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "game/CProjectileMovement.h"
#include "game/CTimelineComponent.h"
#include "game/UnkTimelineComponent.h"
#include "types.h"

using base::global::CStrId;

struct UnkGrappleBeamHit_ovb0047;
struct UnkGrappleHitData_ovb0047;
s32 fn_7100ac7f08(UnkGrappleBeamHit_ovb0047* self, void* object, UnkGrappleHitData_ovb0047* hit, void* extra);

// Static timeline keys, 0x18 bytes each: a pointer to the key's record (its id at +0x30), then two words.
struct UnkTimelineKeyHead_b46a3 {
    u8 pad_0x0[0x30];
    u64 id;  // 0x30
};
struct UnkTimelineKey2_b46a3 {
    UnkTimelineKeyHead_b46a3* head;
    u8 data[0x10];
};
extern UnkTimelineKey2_b46a3 g_unk_71727556b8[2] __attribute__((visibility("hidden")));

struct UnkHitOwner_lac4ec0 {
    u8 pad_0x0[0x44];
    u32 x;  // 0x44
    u8 pad_0x48[0xc];
    u32 y;  // 0x54
    u8 pad_0x58[0xc];
    u32 z;  // 0x64
    u8 pad_0x68[0x280];
    UnkTimelineComponent* timeline;  // 0x2e8
    u8 pad_0x2f0[0xc0];
    bool moved;  // 0x3b0
};

struct UnkHitDefinition_lac4ec0 {
    u8 pad_0x0[0xec];
    bool move_owner;  // 0xec
    u8 pad_0xed[0x44];
    bool mark_on_hit;  // 0x131
};

struct UnkHitKind_lac4ec0 {
    u8 pad_0x0[0x70];
    u32 kind;  // 0x70
};

struct UnkHitObject_lac4ec0 {
    u8 pad_0x0[0x1b8];
    UnkHitKind_lac4ec0* field_0x1b8;
    u8 pad_0x1c0[0x80];
    void* field_0x240;
};

struct UnkHitMovement_lac4ec0 {
    u8 pad_0x0[0x30];
    UnkHitOwner_lac4ec0* owner;  // 0x30
    u8 pad_0x38[0x37c];
    u32 pos_x, pos_y, pos_z;  // 0x3b4
    u8 pad_0x3c0[0x3c];
    u32 flags;  // 0x3fc
    u8 pad_0x400[0x178];
    bool marked;  // 0x578
    u8 pad_0x579[0x1f];
    CStrId id;  // 0x598
    u8 pad_0x5a0[0x28];
    UnkHitDefinition_lac4ec0* definition;  // 0x5c8
};

// Whether the hit object is one the timeline key is not added for.
static inline bool IgnoresObject(const UnkHitMovement_lac4ec0* self, const UnkHitObject_lac4ec0* object) {
    u32 flags = self->flags;
    bool candidate = ((flags & 0xc) == 0) & (object != nullptr) & ((flags & 2) == 0);
    return candidate && object->field_0x240 && object->field_0x1b8 && object->field_0x1b8->kind == 2;
}

bool fn_7100ac4ec0(UnkObj* obj, void* a1, void* a2, void* a3, void* a4, void* a5) {
    UnkHitMovement_lac4ec0* self = reinterpret_cast<UnkHitMovement_lac4ec0*>(obj);
    UnkHitObject_lac4ec0* object = static_cast<UnkHitObject_lac4ec0*>(a1);
    s32 result = fn_7100ac7f08(reinterpret_cast<UnkGrappleBeamHit_ovb0047*>(obj), a1,
                               static_cast<UnkGrappleHitData_ovb0047*>(a4), a5);
    if (!IgnoresObject(self, object)) {
        if (UnkTimelineComponent* timeline = self->owner->timeline) {
            u64 id = g_unk_71727556b8[0].head->id;
            if (!fn_7100a2be94(timeline, &id))
                fn_7100a2cddc(timeline, &g_unk_71727556b8[0], &self->id, 0.0f, true);
        }
    }
    if (result == 0) {
        if (self->definition->move_owner) {
            UnkHitOwner_lac4ec0* owner = self->owner;
            u32 x = self->pos_x, y = self->pos_y, z = self->pos_z;
            owner->x = x;
            owner->y = y;
            owner->z = z;
            owner->moved = true;
        }
        if (self->definition->mark_on_hit)
            self->marked = true;
    }
    return result == 0;
}
