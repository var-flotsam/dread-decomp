// Unit 2689 (the player movement): fn_71010dc54c, when the game is in one of two modes, sends each frozen actor
// touched by the contacts in `span` a request (the contact's segment reversed, the movement's weak handle, the
// actor's weak handle and a mode flag), then switches off the actor's "frozenplatform" collision shapes.
// Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CCharacterMovement.h"
#include "game/CDroppableLifeComponent.h"
#include "game/CFrozenComponent.h"
#include "game/UnkBroadphaseObject.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU1370.h"
#include "game/UnkTU2523.h"
#include "game/UnkTU2553.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::global::CRntVector;
using base::global::CStrId;

struct UnkMove_b152;
struct UnkSpanArg_b152 {
    const void* data;
    u64 count;
};

struct UnkStruct_71008651c8;
struct UnkStruct_71008651c8_Entry {
    u8 pad_0x0[0x8];
    void* field_0x8;
    u8 pad_0x10[0x8];
    bool field_0x18;
    u8 pad_0x19[0x17];
};
static_assert(sizeof(UnkStruct_71008651c8_Entry) == 0x30, "entry size");
using UnkOut_71008651c8 = base::global::CRntVector<UnkStruct_71008651c8_Entry*>;
void fn_71008651c8(UnkStruct_71008651c8*, void* const*, UnkOut_71008651c8*);

UnkWeakTarget* fn_71006f4a30(void* owner);

// The shape entries fn_71008651c8 collects, as this function reads them.
struct UnkShapeEntry_l0dc54c {
    u8 pad_0x0[0x28];
    UnkEntry* field_0x28;
};

struct UnkVec3_l0dc54c {
    f32 x, y, z;
};

// A contact's segment: its two ends and the position along it.
struct UnkSegment_l0dc54c {
    f32 field_0x0[4];
    UnkVec3_l0dc54c start;  // 0x10
    UnkVec3_l0dc54c end;    // 0x1c
    f32 t;                  // 0x28
    f32 field_0x2c;
};

struct UnkCollision_l0dc54c : base::reflection::UnkReflObject {
    u8 pad_0x8[0x10];
    UnkWeakTarget* owner;  // 0x18
};

struct UnkContactLink_l0dc54c {
    u8 pad_0x0[0x10];
    UnkCollision_l0dc54c* collision;  // 0x10
};

struct UnkContactObj_l0dc54c {
    u8 pad_0x0[0x10];
    UnkContactLink_l0dc54c* link;  // 0x10
};

struct UnkContact_l0dc54c {
    UnkContactObj_l0dc54c* obj;
    u8 pad_0x8[0x8];
    UnkSegment_l0dc54c segment;  // 0x10
    u8 pad_0x40[0x8];

    UnkContactLink_l0dc54c* Link() const { return obj ? obj->link : nullptr; }
};
static_assert(sizeof(UnkContact_l0dc54c) == 0x48, "contact size");

struct UnkFrozenLife_l0dc54c {
    u8 pad_0x0[0x50];
    f32 field_0x50;
    u8 pad_0x54[0x1c];
    s32 state;  // 0x70
};

struct UnkFrozenRequest_l0dc54c {
    u8 pad_0x0[0x8];
    UnkWeakHandle* source;  // 0x8
    UnkWeakHandle* target;  // 0x10
    u8 pad_0x18[0x48];
    UnkContactLink_l0dc54c* link;  // 0x60
    UnkSegment_l0dc54c segment;    // 0x68
    UnkShape* shape;               // 0x98
    u8 pad_0xa0[0x14];
    u32 flags;  // 0xb4
    u8 pad_0xb8[0x48];

    UnkFrozenRequest_l0dc54c() { fn_71008c0c74(reinterpret_cast<UnkStruct_71008c0c74*>(this)); }
    ~UnkFrozenRequest_l0dc54c() { fn_71008c1558(reinterpret_cast<UnkBase_71003cc474*>(this)); }
};
static_assert(sizeof(UnkFrozenRequest_l0dc54c) == 0x100, "request layout");

void fn_7101160e70(UnkFrozenLife_l0dc54c* life, UnkFrozenRequest_l0dc54c* request);

struct UnkFreezeMovement_l0dc54c {
    u8 pad_0x0[0x30];
    UnkWeakTarget* owner;  // 0x30
};

void fn_71010dc54c(UnkMove_b152* movement, UnkSpanArg_b152* span) {
    auto* self = reinterpret_cast<UnkFreezeMovement_l0dc54c*>(movement);
    bool modeA = fn_7100df5874(reinterpret_cast<UnkPlayer*>(g_unk_7101d47310));
    bool modeB = fn_7100df5a4c(g_unk_7101d47310);
    if (!modeA && !modeB)
        return;
    auto* contacts = static_cast<const UnkContact_l0dc54c*>(span->data);
    for (const UnkContact_l0dc54c* contact = contacts; contact != contacts + span->count; ++contact) {
        UnkContactLink_l0dc54c* link = contact->Link();
        if (!link)
            continue;
        UnkCollision_l0dc54c* collision = link->collision;
        if (!collision)
            continue;
        if (!fn_710009d50c(collision->vf_0x0(), fn_710085fc44()))
            continue;
        UnkWeakTarget* actor = collision->owner;
        if (!actor)
            continue;
        UnkWeakTarget* frozen = fn_71006f4a30(actor);
        auto* life = static_cast<UnkFrozenLife_l0dc54c*>(fn_71007bb940(reinterpret_cast<UnkEntity_b0165*>(actor)));
        if (!life || !frozen)
            continue;
        if (life->state == 2)
            continue;
        if (!fn_71007f9f20(frozen))
            continue;
        if (fn_71007f9fd0(reinterpret_cast<UnkFrostbiteComponent*>(frozen)))
            continue;

        UnkFrozenRequest_l0dc54c request;
        // The contact seen from the other side.
        const UnkSegment_l0dc54c& from = contact->segment;
        request.segment = from;
        request.segment.start.x = from.end.x;
        request.segment.start.y = from.end.y;
        request.segment.start.z = from.end.z;
        request.segment.end.x = from.start.x;
        request.segment.end.y = from.start.y;
        request.segment.end.z = from.start.z;
        request.segment.t = 1.0f - from.t;
        request.link = contact->Link();
        request.shape = fn_7100976588(reinterpret_cast<UnkFlagged*>(self));
        AssignRef(&request.source, fn_7100d78378(self->owner));
        AssignRef(&request.target, GetWeakHandle(actor));
        fn_71008c1538(reinterpret_cast<UnkRequest*>(&request), life->field_0x50);
        request.flags = modeA ? 0x2000000 : (modeB ? 0x8000 : 0);
        fn_7101160e70(life, &request);

        UnkOut_71008651c8 shapes(0xf);
        {
            CStrId key("frozenplatform", true);
            fn_71008651c8(reinterpret_cast<UnkStruct_71008651c8*>(collision), reinterpret_cast<void* const*>(&key),
                          &shapes);
        }
        UnkStruct_71008651c8_Entry** end = shapes.data + shapes.count;
        for (UnkStruct_71008651c8_Entry** it = shapes.data; it != end; ++it)
            fn_7100d35a98(reinterpret_cast<UnkShapeEntry_l0dc54c*>(*it)->field_0x28, false);
        shapes.Free();
    }
}
