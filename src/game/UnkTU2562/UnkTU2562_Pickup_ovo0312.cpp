// Unit 2562 of tools/tu_map.py (no reliable name; hint CLogicDroppedItemManager::CTunableDroppableBillboard):
// a default drop request handed to the billboard singleton (fn_7100e225e4, like fn_7100e2c124 in
// UnkTU2562_Billboard_ovb0208.cpp), and picking up an item (fn_7100e2e8a4): unless the player's smart object
// (fn_71006eb114) holds an object that is busy, the item's amount goes to the actor's life (fn_7101160348)
// and, with bit 0 of flags, the player is told (fn_7101023f04). Names and layouts are placeholders.
#include "base/color/CColor4F.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "game/CDizzeanSwarmControllerComponent.h"
#include "game/CLifeComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU0.h"
#include "game/UnkTU1102.h"
#include "game/UnkTU2560.h"
#include "game/sound/CSoundManager.h"
#include "types.h"

using base::global::CStrId;

struct UnkDropTarget_ovb0208;

static inline void* Singleton() {
    void* p = g_unk_71726da048;
    if (!p) {
        p = fn_71000839a0(0xe0, 0x3f);
        fn_7100e1bca4(p);
        g_unk_71726da048 = p;
    }
    return p;
}

void fn_7100030938(base::color::CColor4F* self, u32 mode, f32 r, f32 g, f32 b, f32 a);

struct UnkDropRequest2_ovb0208 {
    u32 field_0x0;
    u32 field_0x4;
    u32 field_0x8;
    base::color::CColor4F color0;  // 0xc
    u32 field_0x1c;
    base::color::CColor4F color1;  // 0x20
    u32 field_0x30;
    u32 field_0x34;
    base::global::UnkFilePathStrIdBase path;  // 0x38
    bool field_0x40;
    bool field_0x41;
    bool field_0x42;
    bool field_0x43;
    bool field_0x44;
    u8 pad_0x45[3];
    u64 field_0x48;

    UnkDropRequest2_ovb0208()
        : field_0x0(0), field_0x4(0), field_0x8(0), color0(1.0f, 1.0f, 1.0f, 1.0f), field_0x1c(0),
          color1(1.0f, 1.0f, 1.0f, 1.0f), field_0x30(0), field_0x34(0), path{g_unk_7101d470e8.data} {
        if (&g_unk_7101d47200->empty != path.data)
            ++path.data->ref_count;
    }

    // The two-step release the original does (it reloads the id after the decrement).
    void ReleasePath() {
        if (base::global::UnkStringPool* pool = g_unk_7101d47200) {
            if (path.data && path.data != &pool->empty) {
                --path.data->ref_count;
                if (path.data->ref_count == 0) {
                    fn_71000803a4(pool, path.data);
                    pool = g_unk_7101d47200;
                }
            }
            path.data = &pool->empty;
        }
    }
};
static_assert(sizeof(UnkDropRequest2_ovb0208) == 0x50, "request size");

int fn_7100e1c014(void* self, UnkDropTarget_ovb0208* target, UnkDropRequest2_ovb0208* request);

int fn_7100e225e4(void* self, u32 a, UnkDropTarget_ovb0208* target, u32 b, u32 c, u32 d) {
    UnkDropRequest2_ovb0208 request;
    request.field_0x48 = 0;
    request.field_0x44 = true;
    request.field_0x0 = a;
    request.field_0x4 = b;
    request.field_0x40 = true;
    request.field_0x41 = false;
    request.field_0x42 = true;
    request.field_0x43 = true;
    request.field_0x30 = d;
    request.field_0x8 = c;
    fn_7100030938(&request.color0, 0, 1.0f, 1.0f, 1.0f, 1.0f);
    fn_7100030938(&request.color1, 0, 0.0f, 0.0f, 0.0f, 0.0f);
    request.field_0x41 = true;
    request.field_0x42 = false;
    int result = fn_7100e1c014(Singleton(), target, &request);
    request.ReleasePath();
    return result;
}

struct UnkPlayer;
struct UnkDroppedItemManager;
struct UnkDroppedItem;
struct UnkDropPlayer_ovb0208;

struct UnkDropAbility_o0312 {
    u8 pad_0x0[0xb0];
    struct UnkDropHolder_o0312* holder;  // 0xb0
};
struct UnkDropHolder_o0312 {
    u8 pad_0x0[0x10];
    void* object;  // 0x10
};
struct UnkDropInfo_o0312 {
    u8 pad_0x0[0x10];
    void* field_0x10;
};
struct UnkDropPlayerSub_o0312 {
    u8 pad_0x0[0x7e8];
    UnkDropInfo_o0312* info;  // 0x7e8
    u8 pad_0x7f0[0x2d08 - 0x7f0];
    u64 field_0x2d08;
};
struct UnkDropPlayer_o0312 {
    u8 pad_0x0[0x208];
    UnkDropPlayerSub_o0312* sub;  // 0x208
};
struct UnkDropActorSub_o0312 {
    u8 pad_0x0[0x70];
    s32 state;  // 0x70
};
struct UnkDropActor_o0312 {
    u8 pad_0x0[0x1b8];
    UnkDropActorSub_o0312* sub;  // 0x1b8
};
struct UnkDropPickup_o0312 {
    u8 pad_0x0[0x78];
    f32 amount;  // 0x78
};
struct UnkDropManager_o0312 {
    u8 pad_0x0[0xa068];
    u8 entries[0x18 * 4];  // 0xa068
};
struct UnkDropTarget;

UnkPlayer* fn_7100dec788(UnkGameManager*);
void* fn_71006eb2cc(void* h);

// The object the player's smart object holds, or null.
static inline void* Holder(UnkDropPlayer_o0312* player) {
    if (!player)
        return nullptr;
    UnkDropPlayerSub_o0312* sub = player->sub;
    if (!sub)
        return nullptr;
    UnkDropInfo_o0312* info = sub->info;
    if (!info)
        return nullptr;
    if ((sub->field_0x2d08 == 0) | (info->field_0x10 == nullptr))
        return nullptr;
    auto* ability = static_cast<UnkDropAbility_o0312*>(fn_71006eb114(info->field_0x10));
    if (!ability)
        return nullptr;
    if (!ability->holder)
        return nullptr;
    return ability->holder->object;
}

void fn_7100e2e8a4(UnkDroppedItemManager* self_, UnkDroppedItem* item_, void* arg_, u32 flags) {
    auto* self = reinterpret_cast<UnkDropManager_o0312*>(self_);
    auto* item = reinterpret_cast<UnkDropPickup_o0312*>(item_);
    auto* arg = static_cast<UnkDropActor_o0312*>(arg_);
    void* holder = Holder(reinterpret_cast<UnkDropPlayer_o0312*>(fn_7100dec788(g_unk_7101d47310)));
    if (holder) {
        if (fn_71006eb2cc(holder))
            return;
        void* r = fn_71006eb1f0(holder);
        if (!arg || r)
            return;
    } else if (!arg) {
        return;
    }
    UnkDropActorSub_o0312* sub = arg->sub;
    if (!sub)
        return;
    if (sub->state == 2)
        return;
    fn_7101160348(sub, item->amount);
    if (flags & 1) {
        if (UnkPlayer* player = fn_7100dec788(g_unk_7101d47310))
            fn_7101023f04(g_unk_7101d47318, self->entries, reinterpret_cast<UnkDropPlayer_ovb0208*>(player), nullptr, 0);
    }
}
