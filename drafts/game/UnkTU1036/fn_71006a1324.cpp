// Unit 1036: clips a 2D segment around the other registered actors: for each actor of the manager's list
// (other than the owner's) that falls within the segment, the segment keeps the longer of the two pieces
// left on either side of it, an owner's radius away. Names and layouts are placeholders.
#include "base/math/CVector.h"
#include "base/math/UnkSegment2D.h"
#include "base/global/CRntVector.h"
#include "game/UnkActor.h"
#include "game/UnkManager_71726af468.h"
#include "game/UnkTU2523.h"
#include "game/UnkTU852.h"
#include "types.h"

struct UnkCountedList_b260;
void fn_71006a15c8(UnkCountedList_b260* self, const UnkCountedList_b260* other);
f32 fn_71001261ac(const UnkSegment2D* seg, UnkVec2Result point);

// A position returned in memory (x8): 16 bytes, not trivially copyable.
struct UnkActorPosition {
    f32 x, y, z, w;
    UnkActorPosition(const UnkActorPosition& other);
};

// A counted reference to an actor.
struct UnkActorRef_l6a1324 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void Destroy();  // vf_0x10
    s32 ref_count;           // 0x8
    UnkActor* actor;         // 0x10
};

// Holds a reference for as long as it lives.
struct UnkActorRefHolder_l6a1324 {
    UnkActorRef_l6a1324* ref;
    bool empty;

    explicit UnkActorRefHolder_l6a1324(UnkActorRef_l6a1324* r) : ref(r), empty(r == nullptr) {
        if (!empty)
            ++ref->ref_count;
    }
    ~UnkActorRefHolder_l6a1324() {
        if (!empty && --ref->ref_count == 0)
            ref->Destroy();
    }
    UnkActor* Get() const { return empty ? nullptr : ref->actor; }
};

struct UnkActorRefNode_l6a1324 {
    UnkActorRef_l6a1324* value;
    UnkActorRefNode_l6a1324* next;
};

// A copy of the manager's actor list (UnkCountedList_b260; fn_71006a15c8 copies, fn_710059c7b8 destroys).
struct UnkActorRefList_l6a1324 {
    u8 pad_0x0[0x10];
    UnkActorRefNode_l6a1324* head;  // 0x10
    u8 pad_0x18[0x58];

    explicit UnkActorRefList_l6a1324(const u8* other) {
        fn_71006a15c8(reinterpret_cast<UnkCountedList_b260*>(this), reinterpret_cast<const UnkCountedList_b260*>(other));
    }
    ~UnkActorRefList_l6a1324() { fn_710059c7b8(this); }
};

struct UnkClipOwner_l6a1324 {
    u8 pad_0x0[0x8];
    UnkActor* actor;
};

// A piece of a segment.
struct UnkPiece_l6a1324 {
    base::math::CVector2D a;
    base::math::CVector2D b;
};

static inline f32 LengthSq(const UnkPiece_l6a1324& p) {
    return (p.b.x - p.a.x) * (p.b.x - p.a.x) + (p.b.y - p.a.y) * (p.b.y - p.a.y);
}

static inline UnkManager_71726af468* Manager() {
    if (!g_unk_71726af468) {
        UnkManager_71726af468* p = static_cast<UnkManager_71726af468*>(fn_71000839a0(0x2610, 0x3f));
        fn_7100da7e2c(p);
        g_unk_71726af468 = p;
    }
    return g_unk_71726af468;
}

void fn_71006a1324(UnkClipOwner_l6a1324* self, UnkSegment2D* seg) {
    // The actor list at +0x840, inside the registry.
    UnkActorRefList_l6a1324 actors(Manager()->field_0x330 + (0x840 - 0x330));
    for (UnkActorRefNode_l6a1324* n = actors.head; n; n = n->next) {
        UnkActorRefHolder_l6a1324 ref(n->value);
        if (ref.Get() == self->actor)
            continue;
        UnkActorPosition pos = fn_7100d7b7a8(ref.Get());
        base::math::CVector2D flat;
        fn_710007b430(&flat, reinterpret_cast<const base::math::CVector3D*>(&pos));
        f32 t = fn_71001261ac(seg, UnkVec2Result(flat.x, flat.y));
        if (!(t >= 0.0f && t <= 1.0f))
            continue;

        f32 dx = seg->end.x - seg->start.x;
        f32 dy = seg->end.y - seg->start.y;
        f32 length = fn_7100125fbc(seg);
        f32 half = (self->actor->field_0x37c + self->actor->field_0x37c) / length;
        f32 lo = t - half;
        f32 hi = t + half;
        UnkPiece_l6a1324 before = {};
        if (lo > 0.001f) {
            before.a = seg->start;
            before.b.x = dx * lo + seg->start.x;
            before.b.y = dy * lo + seg->start.y;
        }
        UnkPiece_l6a1324 after = {};
        if (hi < 0.999f) {
            after.a.x = dx * hi + seg->start.x;
            after.a.y = dy * hi + seg->start.y;
            after.b = seg->end;
        }
        // Keep the longer piece.
        bool longer = LengthSq(before) > LengthSq(after);
        seg->start.x = longer ? before.a.x : after.a.x;
        seg->start.y = longer ? before.a.y : after.a.y;
        seg->end.x = longer ? before.b.x : after.b.x;
        seg->end.y = longer ? before.b.y : after.b.y;
        seg->field_0x10 = -1.0f;  // the cached length is stale
    }
}
