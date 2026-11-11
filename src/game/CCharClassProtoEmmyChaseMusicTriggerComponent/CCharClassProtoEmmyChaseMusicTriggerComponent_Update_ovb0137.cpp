// CCharClassProtoEmmyChaseMusicTriggerComponent's unit (tools/tu_map.py): the trigger's update (the music manager as src/game/CCharClassFadeOutMusicEvent/CCharClassFadeOutMusicEvent_Run_ovb0116.cpp reaches it).
// Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "game/CAbilityEnergyWave.h"
#include "game/CActorComponent.h"
#include "game/CCharClassBaseTriggerComponent.h"
#include "game/UnkGameBlackboard.h"
#include "game/UnkGameManager.h"
#include "game/UnkItemProperties.h"
#include "game/sound/CMusicManager.h"
#include "game/sound/CMusicManager/CTunableMusicManager.h"
#include "types.h"

using base::global::CRntString;

struct UnkFan_A4_b74;
struct UnkPlayer;
UnkPlayer* fn_7100dec788(UnkGameManager*);
struct UnkInventory;
bool fn_71011428bc(UnkInventory* inventory, const UnkQualifiedId& item);
struct UnkActor;
bool fn_7100d7bea0(UnkActor* owner, bool flag);
struct UnkStruct_710024fa0c;
bool fn_7100fd76d4(void* manager, const CRntString* name);

extern void* g_unk_71726bc560;
struct UnkEmmyRegistry_b55;

// The channel mask as src/game/UnkLoadingScreen.cpp defines it (passed through a copy).
struct UnkLoadingMask {
    u32 bits;
    UnkLoadingMask() : bits(0) {}
    UnkLoadingMask(u32 b) : bits(b) {}
    UnkLoadingMask(const UnkLoadingMask& o) : bits(o.bits) {}
};

// The music manager, created on first use.
static inline void* MusicManager() {
    void* manager = g_unk_71726bc560;
    if (!manager) {
        manager = fn_71000839a0(0xf18, 0x3f);
        fn_7100fd3804(manager);
        g_unk_71726bc560 = manager;
    }
    return manager;
}

struct UnkChaseEmmyEntity_ovb0137 {
    u8 pad_0x0[0x30];
    UnkActor* actor;  // 0x30
};

struct UnkChaseEmmy_ovb0137 {
    u8 pad_0x0[0x10];
    UnkChaseEmmyEntity_ovb0137* entity;  // 0x10
};

struct __attribute__((packed, aligned(4))) UnkChaseEmmyPtr_ovb0137 {
    UnkChaseEmmy_ovb0137* ptr;
};

// The E.M.M.I. list of the registry (+0x8: a count and the entries).
struct UnkChaseEmmyList_ovb0137 {
    u32 count;
    UnkChaseEmmyPtr_ovb0137 items[2];
};

struct UnkChasePlayer_ovb0137 {
    u8 pad_0x0[0x270];
    UnkInventory* inventory;  // 0x270
};

struct UnkChaseMusicTrigger_ovb0137 {
    u8 pad_0x0[0x9b8];
    s32 field_0x9b8;
};

// After the base update, once the player has the central unit energy, fades the chase music out
// (channel 2, mask 0x80) and switches the trigger off while an E.M.M.I. chases and the cutscene
// music is playing.
void fn_7100717108(UnkChaseMusicTrigger_ovb0137* self, f32 dt) {
    fn_7100a474f8(reinterpret_cast<UnkFan_A4_b74*>(self), dt);
    if (self->field_0x9b8 < 1)
        return;
    UnkChasePlayer_ovb0137* player = reinterpret_cast<UnkChasePlayer_ovb0137*>(fn_7100dec788(g_unk_7101d47310));
    if (!player)
        return;
    UnkInventory* inventory = player->inventory;
    if (!inventory)
        return;
    UnkQualifiedId& qid = ItemQualifiedId(g_unk_717275ac18);
    if ((u64)qid.id.data == (u64)g_unk_7101d47108 + 0x10)
        fn_7100ae9a10(&qid, ItemSource(g_unk_717275ac18), g_unk_7101d470e0);
    if (!fn_71011428bc(inventory, qid))
        return;
    UnkChaseEmmyList_ovb0137* list =
        reinterpret_cast<UnkChaseEmmyList_ovb0137*>(reinterpret_cast<char*>(g_unk_71726af0a8) + 8);
    for (UnkChaseEmmyPtr_ovb0137 *it = list->items, *end = list->items + list->count; it != end; ++it) {
        if (!fn_7100d7bea0(it->ptr->entity->actor, true))
            continue;
        void* manager = MusicManager();
        bool playing;
        {
            CRntString name("cut_0048firstcupowerobtention");
            playing = fn_7100fd76d4(manager, &name);
        }
        if (!playing)
            continue;
        fn_7100fd7b44(MusicManager(), UnkLoadingMask(2), UnkLoadingMask(0x80), -1.0f);
        fn_710024fa0c(reinterpret_cast<UnkStruct_710024fa0c*>(self), false);
    }
}
