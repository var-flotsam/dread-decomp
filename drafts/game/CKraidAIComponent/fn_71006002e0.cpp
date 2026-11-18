// CKraidAIComponent's unit (tools/tu_map.py 921): handles a message. Turns the music manager's flag on;
// while the component holds messages back it keeps this one for later, otherwise it passes it to the
// boss base and, for the activation message, links to the AI manager's list and, unless the manager
// takes the resolved target, unlinks again and runs the stage 2 activation script.
// Names and layouts are placeholders.
#include "base/UnkDestructors.h"
#include "base/UnkRefHandle.h"
#include "base/global/CRntVector.h"
#include "game/CBossAIComponent.h"
#include "game/UnkManager_71726af468.h"
#include "game/UnkTU0.h"
#include "game/UnkTU437.h"
#include "game/navmesh/SNavMeshPath.h"
#include "game/sound/CMusicManager.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

struct UnkSelf;
void fn_71003ad45c(UnkSelf* self);
void fn_7100fdbd84(UnkMgr_0xf18* self, u32 flag);

struct UnkTimedRef;
bool fn_7100da9018(UnkTimedRef* self, UnkWeakTarget* target, f32 time);

extern u64 g_unk_71726f3490;  // the activation message's id

struct UnkKraidMessage_l6002e0 {
    u8 pad_0x0[0x10];
    u64 id;  // 0x10
};

// A reference fetched from its source the first time it is asked for.
struct UnkLazyRef_l6002e0 {
    u8 source[0x28];
    s32 state;  // 0x28: 1 until fetched, then 2
    base::UnkRefHandle handle;  // 0x30

    UnkWeakTarget* Target() {
        if (state == 1) {
            {
                base::UnkRefPtrMember fetched = fn_7100353be8(source);
                fn_710031d208(&handle, reinterpret_cast<base::UnkRefHandle*>(&fetched));
            }
            state = 2;
        }
        UnkWeakHandle* weak = reinterpret_cast<UnkWeakHandle*>(handle.ptr);
        return weak ? weak->target : nullptr;
    }
};

// A link into a list.
struct UnkListLink_l6002e0 {
    UnkLinkList* list;

    void Attach(UnkLinkList* to) {
        list = to;
        fn_71002af070(to, &list);
    }
    void Detach() {
        if (list) {
            fn_71002aeb9c(list, &list);
            list = nullptr;
        }
    }
};

struct UnkKraidAI_l6002e0 {
    u8 pad_0x0[0x149d5];
    bool holding;  // 0x149d5
    bool has_held;  // 0x149d6
    u8 pad_0x149d7;
    UnkKraidMessage_l6002e0* held;  // 0x149d8
    u8 pad_0x149e0[0x14d48 - 0x149e0];
    UnkLazyRef_l6002e0 target;  // 0x14d48
    u8 pad_0x14d80[0x14e38 - 0x14d80];
    UnkListLink_l6002e0 link;  // 0x14e38
};
static_assert(__builtin_offsetof(UnkKraidAI_l6002e0, link) == 0x14e38, "layout");

// The music manager, created on first use.
static inline UnkMgr_0xf18* MusicManager_l6002e0() {
    void* instance = g_unk_71726bc560;
    if (!instance) {
        instance = fn_71000839a0(0xf18, 0x3f);
        fn_7100fd3804(instance);
        g_unk_71726bc560 = static_cast<UnkMgr_0xf18*>(instance);
    }
    return static_cast<UnkMgr_0xf18*>(instance);
}

// The AI manager, created on first use.
static inline UnkManager_71726af468* AIManager_l6002e0() {
    UnkManager_71726af468* instance = g_unk_71726af468;
    if (!instance) {
        instance = static_cast<UnkManager_71726af468*>(fn_71000839a0(0x2610, 0x3f));
        fn_7100da7e2c(instance);
        g_unk_71726af468 = instance;
    }
    return instance;
}

// The manager's list at +0x24a8.
static inline UnkLinkList* ManagerList_l6002e0(UnkManager_71726af468* manager) {
    return reinterpret_cast<UnkLinkList*>(&manager->field_0x330[0x24a8 - 0x330]);
}

using UnkBaseHandle_l6002e0 = void (*)(UnkKraidAI_l6002e0*, UnkKraidMessage_l6002e0*);

void fn_71006002e0(UnkKraidAI_l6002e0* self, UnkKraidMessage_l6002e0* message) {
    fn_7100fdbd84(MusicManager_l6002e0(), 1);
    if (self->holding) {
        self->held = message;
        self->has_held = true;
        return;
    }
    reinterpret_cast<UnkBaseHandle_l6002e0>(&fn_71003ad45c)(self, message);
    if (message->id != g_unk_71726f3490)
        return;

    UnkListLink_l6002e0* link = &self->link;
    UnkManager_71726af468* manager = AIManager_l6002e0();
    link->Detach();
    link->Attach(ManagerList_l6002e0(manager));
    manager = AIManager_l6002e0();
    if (!fn_7100da9018(reinterpret_cast<UnkTimedRef*>(manager), self->target.Target(), 0.2f)) {
        link->Detach();
        fn_7100000790("CurrentScenario.OnEnter_Kraid_Activation_Stage_02", "");
    }
}
