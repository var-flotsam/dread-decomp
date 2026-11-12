// CKraidSpikeMovablePlatformComponent: fn_7100608cd4 damages the player when the platform's spikes
// hit: nothing to do while fn_7100952694 holds; no hit when the player is grabbing this platform's
// collision (its grab record's object is a CCollisionComponent of the same entity); without a delay at
// +0x26c a player not yet in the air flags +0x269 instead. Otherwise it builds a damage request on the
// stack (fn_71008c0c74) with the tunable at +0x28c of g_unk_7172716160 and a unit-static id, hands it
// to the player's component at +0x1b8 (virtual slot 49) and answers whether that took it.
// Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflAttributes.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU1370.h"
#include "game/UnkTU2553.h"
#include "types.h"

using base::global::CStrId;
using base::reflection::UnkReflObject;

struct UnkRequest;
struct UnkStruct_71008c0c74;
struct UnkBase_71003cc474;
struct UnkPlayer;

struct UnkSpikeDamageRequest {
    u8 pad_0x0[0xb8];
    union {
        CStrId id;  // 0xb8
    };
    u8 pad_0xc0[0x40];

    UnkSpikeDamageRequest() {}
    ~UnkSpikeDamageRequest() {}
};

struct UnkSpikeDamageTarget {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual void vf_0x40();
    virtual void vf_0x48();
    virtual void vf_0x50();
    virtual void vf_0x58();
    virtual void vf_0x60();
    virtual void vf_0x68();
    virtual void vf_0x70();
    virtual void vf_0x78();
    virtual void vf_0x80();
    virtual void vf_0x88();
    virtual void vf_0x90();
    virtual void vf_0x98();
    virtual void vf_0xa0();
    virtual void vf_0xa8();
    virtual void vf_0xb0();
    virtual void vf_0xb8();
    virtual void vf_0xc0();
    virtual void vf_0xc8();
    virtual void vf_0xd0();
    virtual void vf_0xd8();
    virtual void vf_0xe0();
    virtual void vf_0xe8();
    virtual void vf_0xf0();
    virtual void vf_0xf8();
    virtual void vf_0x100();
    virtual void vf_0x108();
    virtual void vf_0x110();
    virtual void vf_0x118();
    virtual void vf_0x120();
    virtual void vf_0x128();
    virtual void vf_0x130();
    virtual void vf_0x138();
    virtual void vf_0x140();
    virtual void vf_0x148();
    virtual void vf_0x150();
    virtual void vf_0x158();
    virtual void vf_0x160();
    virtual void vf_0x168();
    virtual void vf_0x170();
    virtual void vf_0x178();
    virtual void vf_0x180();
    virtual s32 vf_0x188(UnkSpikeDamageRequest* request);
};

struct UnkSpikeGrabObject {
    u8 pad_0x0[0x30];
    void* entity;  // 0x30
};

struct UnkSpikeGrabLink {
    u8 pad_0x0[0x10];
    UnkReflObject* object;  // 0x10
};

struct UnkSpikeGrabRecord {
    u8 pad_0x0[0x10];
    UnkSpikeGrabLink* link;  // 0x10
};

struct UnkSpikeMovement {
    u8 pad_0x0[0x51d8];
    UnkSpikeGrabRecord* grab;  // 0x51d8
};

struct UnkSpikeState {
    u8 pad_0x0[0x3a8];
    bool field_0x3a8;
};

struct UnkSpikePlayer {
    u8 pad_0x0[0x1b8];
    UnkSpikeDamageTarget* field_0x1b8;
    u8 pad_0x1c0[0x38];
    UnkSpikeState* field_0x1f8;
    u8 pad_0x200[0x8];
    UnkSpikeMovement* field_0x208;
};

struct UnkKraidSpikePlatform {
    u8 pad_0x0[0x30];
    void* entity;  // 0x30
    u8 pad_0x38[0x269 - 0x38];
    bool field_0x269;
    u8 pad_0x26a[0x2];
    f32 field_0x26c;
};

extern base::reflection::UnkTunable g_unk_7172716160;
extern CStrId g_unk_71726f4628 __attribute__((visibility("hidden")));
bool fn_7100952694();
void fn_71008c1538(UnkRequest* request, f32 value);

static inline bool GrabsPlatform(UnkSpikePlayer* player, UnkKraidSpikePlatform* self) {
    if (UnkSpikeMovement* movement = player->field_0x208)
        if (UnkSpikeGrabRecord* grab = movement->grab)
            if (UnkSpikeGrabLink* link = grab->link)
                if (UnkReflObject* obj = link->object)
                    if (fn_710009d50c(obj->vf_0x0(), fn_710085fc44()) &&
                        reinterpret_cast<UnkSpikeGrabObject*>(obj)->entity == self->entity)
                        return true;
    return false;
}

bool fn_7100608cd4(UnkKraidSpikePlatform* self) {
    bool result = true;
    if (fn_7100952694())
        return result;
    result = false;
    UnkSpikePlayer* player = reinterpret_cast<UnkSpikePlayer*>(fn_7100dec788(g_unk_7101d47310));
    if (!player || GrabsPlatform(player, self))
        return result;
    if (self->field_0x26c > 0.0f || player->field_0x1f8->field_0x3a8) {
        if (player->field_0x1b8) {
            UnkSpikeDamageRequest request;
            fn_71008c0c74(reinterpret_cast<UnkStruct_71008c0c74*>(&request));
            fn_71008c1538(reinterpret_cast<UnkRequest*>(&request),
                          *reinterpret_cast<const f32*>(reinterpret_cast<const char*>(&g_unk_7172716160) + 0x28c));
            request.id = g_unk_71726f4628;
            result = player->field_0x1b8->vf_0x188(&request) == 1;
            fn_71008c1558(reinterpret_cast<UnkBase_71003cc474*>(&request));
        }
    } else {
        self->field_0x269 = true;
    }
    return result;
}
