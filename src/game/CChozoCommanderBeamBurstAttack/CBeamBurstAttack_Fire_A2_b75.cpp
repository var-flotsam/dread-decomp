// CChozoCommanderBeamBurstAttack's unit (tools/tu_map.py unit 646): with the owner's (+0x8) animation (+0x220),
// picks the animation hash: 0xa6de296d57c32a98 unless a CCharClass track query (fn_7100c11094, fn_710117b08c)
// finds a record, whose kind (+0x18) 1 or 0 chooses between two pairs of global hashes, the member of the pair
// by the side the other actor (fn_7100d7c990) is on and the animation's state 5 (fn_7101178e6c). It sends that
// hash as a mode-0 request (as src/game_s2/UnkSpitSend.h), hooks the callback fn_710042a7e0 (link at +0x88,
// handle at +0xa0) up to the animation as src/game/CChozoWarriorXGlaiveSpinAttack/UnkAttackTrackLink_b54.cpp,
// clears +0xa8, switches the cached component (+0x60) off when it is on (slot 0x58) and tells the owner's world
// (+0x280) 0x28d8e7efe4423be0, hands the navigation object (+0x298) a query (vtable 0x7101a24b58) facing the
// other actor and sets the state (+0x58) to 4. Names and layouts are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "base/reflection/UnkClassGetters.h"
#include "game/CAnimationComponent.h"
#include "game/UnkActor.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkSwitchObject.h"
#include "game_s2/UnkParams.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

struct UnkTakumakuComponent;
struct UnkWorld;
void fn_710114fe8c(UnkWorld*, u64, u32, u32);
void fn_7100316018(UnkWeakTarget* self, UnkWeakHandle** out);
void fn_710117b3e8(UnkWeakTarget* target, void* link);
void fn_710042a7e0();

extern void* g_unk_7101a24b58[];
extern const u64 g_unk_7101649430;  // reached through the GOT
extern const u64 g_unk_7101649438;  // reached through the GOT
extern const u64 g_unk_7101649440;  // reached through the GOT
extern const u64 g_unk_7101649448;  // reached through the GOT

struct UnkBeamBurstDir_A2_b75 {
    f32 x, y, z;
};

struct UnkBeamBurstQuery_A2_b75 {
    void** vtable;
    UnkBeamBurstDir_A2_b75 field_0x8;
    u8 field_0x14;
    u8 pad_0x15[7];
    u8 field_0x1c;
    u8 pad_0x1d[3];
    u64 field_0x20;
    u64 field_0x28;
    u8 field_0x30;
    u8 pad_0x31[3];
    u32 field_0x34;

    __attribute__((always_inline)) void Init() {
        field_0x1c = 0;
        field_0x34 = 0;
        field_0x20 = 0;
        field_0x28 = 0;
        vtable = g_unk_7101a24b58 + 2;
        field_0x30 = 0;
    }
};

struct UnkBeamBurstComponent_A2_b75 {
    void** vtable;
};

struct UnkBeamBurstLink_A2_b75 {
    u8 pad_0x0[0x8];
    void* context;
    void (*callback)();
};

struct UnkBeamBurstFire_A2_b75 {
    u8 pad_0x0[0x8];
    UnkActor* owner;
    u8 pad_0x10[0x48];
    s32 state;  // 0x58
    u8 pad_0x5c[0x4];
    UnkBeamBurstComponent_A2_b75* component;  // 0x60
    u8 pad_0x68[0x20];
    UnkBeamBurstLink_A2_b75 link;  // 0x88
    UnkWeakHandle* handle;         // 0xa0
    bool field_0xa8;
};

static inline f32 PosX_A2_b75(UnkActor* actor) {
    return *reinterpret_cast<f32*>(reinterpret_cast<char*>(actor) + 0x44);
}

void fn_7100429d10(UnkBeamBurstFire_A2_b75* self) {
    void* anim = self->owner->field_0x220;
    if (!anim)
        return;
    // Declared past a goto (no lifetime markers), so each keeps its own slot.
    goto start;
    base::reflection::UnkReflTypeIdArg mode;
    UnkTrackQuery query;
    base::reflection::UnkReflTypeIdArg mask;
    UnkBeamBurstQuery_A2_b75 params;
start:
    u64 hash = 0xa6de296d57c32a98;
    query.cls = fn_7100c11094();
    query.result = nullptr;
    query.value = -1.0f;
    fn_710117b08c(static_cast<UnkAnimationComponent*>(anim), &query);
    if (query.result) {
        f32 d = PosX_A2_b75(fn_7100d7c990(self->owner)) - PosX_A2_b75(self->owner);
        bool r = fn_7101178e6c(static_cast<UnkTakumakuComponent*>(anim), 5);
        bool flip = (r && d < 0.0f) || ((d > 0.0f) & !r);
        u32 kind = *reinterpret_cast<u32*>(reinterpret_cast<char*>(query.result) + 0x18);
        const u64* left;
        const u64* right;
        if (kind == 1) {
            left = &g_unk_7101649440;
            right = &g_unk_7101649448;
        } else if (kind == 0) {
            left = &g_unk_7101649430;
            right = &g_unk_7101649438;
        } else {
            goto send;
        }
        hash = *(flip ? right : left);
    }
send:
    mode.id = 0;
    mask.id = UnkAll_SpitSend().value;
    fn_7101176690(anim, hash, 0xf4061ff25a2163d6, &mode, &g_unk_710164b958, &g_unk_7172769740, &mask);
    UnkBeamBurstLink_A2_b75* link = &self->link;
    link->context = self;
    link->callback = fn_710042a7e0;
    UnkWeakTarget* track = static_cast<UnkWeakTarget*>(self->owner->field_0x220);
    if (UnkWeakHandle* handle = self->handle) {
        if (UnkWeakTarget* target = handle->target)
            fn_710117b3f0(target, link);
        if (UnkWeakHandle* p = self->handle) {
            if (--p->ref_count == 0)
                p->vf_0x10();
            self->handle = nullptr;
        }
    }
    if (track) {
        asm("" : "+r"(self));
        fn_7100316018(track, &self->handle);
        fn_710117b3e8(self->handle ? self->handle->target : nullptr, link);
    }
    self->field_0xa8 = false;
    UnkWorld* world = *reinterpret_cast<UnkWorld**>(reinterpret_cast<char*>(self->owner) + 0x280);
    if (world && self->component &&
        reinterpret_cast<bool (*)(UnkBeamBurstComponent_A2_b75*)>(self->component->vtable[0x58 / 8])(self->component)) {
        fn_7100145e94(reinterpret_cast<UnkSwitchObject*>(self->component), false);
        fn_710114fe8c(world, 0x28d8e7efe4423be0, 0, 1);
    }
    void* nav = self->owner->field_0x298;
    UnkActor* other = fn_7100d7c990(self->owner);
    if (nav) {
        f32 dir = PosX_A2_b75(other) >= PosX_A2_b75(self->owner) ? 1.0f : -1.0f;
        params.Init();
        params.field_0x8.x = dir;
        params.field_0x8.y = 0.0f;
        params.field_0x8.z = 0.0f;
        params.field_0x14 = 1;
        fn_71006f4b0c(nav, &params, 0);
    }
    self->state = 4;
}
