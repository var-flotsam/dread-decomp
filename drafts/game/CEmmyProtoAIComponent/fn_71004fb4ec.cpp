// CEmmyProtoAIComponent's unit (tools/tu_map.py): turns the floor-slide sequence on or off (flag at +0x2a70a).
// On: closes the tutorial, drops the camera slot at +0xee8 of the id table, pushes the player along its facing
// and blocks twelve inputs. Off: marks a CEmmyFloorSlideCameraCtrl as done, hooks the game manager, returns to
// "CAM_Default" with slow motion and a sound fade, and releases the player. Names and layouts are placeholders.
#include "base/UnkMatrix44.h"
#include "base/global/CRntString.h"
#include "base/math/CVector.h"
#include "base/reflection/UnkClassGetters.h"
#include "game/CCameraComponent.h"
#include "game/CEmmyProtoAIComponent.h"
#include "game/CInputComponent.h"
#include "game/GUI/CTutorialComposition.h"
#include "game/UnkCameraCtrl.h"
#include "game/UnkFlagListS7.h"
#include "game/UnkGameManager.h"
#include "game/UnkGameManagerIdTable.h"
#include "game/UnkTU23.h"
#include "game/UnkTU2553.h"
#include "game/UnkTU2689.h"
#include "types.h"

using base::global::CRntString;
using unk_flag_list::UnkFlagList;
using unk_flag_list::UnkFlagListKey;

__attribute__((visibility("hidden"))) extern UnkFlagListKey g_unk_71726dd210;
__attribute__((visibility("hidden"))) extern base::global::CStrId g_unk_71726dd218;

struct UnkCamComp6_ovb5;
void fn_71010e64a4(UnkCamComp6_ovb5* self, const CRntString& name, bool flag);
void fn_7100108c58(UnkSoundMgr* self, f32 time, const u32& kind);
void fn_71004fd854();

// The id table's slot this file reads.
struct UnkIdTableSlots_l4fb4ec {
    u8 pad_0x0[0xee8];
    base::global::CStrId field_0xee8;  // the floor-slide camera
};

struct UnkTutorialHolder_l4fb4ec {
    u8 pad_0x0[0x10];
    UnkTutorialComposition* composition;
};

// The game manager's words this file reads and writes.
struct UnkGameManagerSlots_l4fb4ec {
    u8 pad_0x0[0x720];
    void* field_0x720;            // the object fn_71004fd854 is called for
    void (*field_0x728)();
    u8 pad_0x730[0x2270 - 0x730];
    UnkTutorialHolder_l4fb4ec* tutorial;  // 0x2270
};

struct UnkFloorSlideCameraCtrl_l4fb4ec : UnkCameraCtrl {
    u8 pad_0x28[0x1d8 - 0x28];
    bool done;  // 0x1d8
};

struct UnkPlayer_l4fb4ec {
    u8 pad_0x0[0x38];
    u8 transform[0x1d8 - 0x38];
    void* input;  // 0x1d8
    u8 pad_0x1e0[0x20];
    UnkForcedMovementTarget_A5_b68* movement;  // 0x200
};

struct UnkEmmyProtoAI_l4fb4ec {
    u8 pad_0x0[0x2a70a];
    bool floor_slide;  // 0x2a70a
    bool field_0x2a70b;
};

static inline const base::global::CStrId& FloorSlideCameraId() {
    return reinterpret_cast<UnkIdTableSlots_l4fb4ec&>(GameManagerIdTable()).field_0xee8;
}

static inline UnkGameManagerSlots_l4fb4ec* Slots(UnkGameManager* manager) {
    return reinterpret_cast<UnkGameManagerSlots_l4fb4ec*>(manager);
}

static inline UnkPlayer_l4fb4ec* Player() {
    return reinterpret_cast<UnkPlayer_l4fb4ec*>(fn_7100dec788(g_unk_7101d47310));
}

void fn_71004fb4ec(UnkEmmyProtoAI_l4fb4ec* self, bool on) {
    if (self->floor_slide == on)
        return;
    self->floor_slide = on;

    if (on) {
        self->field_0x2a70b = false;
        if (UnkTutorialHolder_l4fb4ec* tutorial = Slots(g_unk_7101d47310)->tutorial) {
            if (tutorial->composition)
                fn_7100b8903c(tutorial->composition, nullptr);
        }
        fn_71010e4168(fn_7100defcec(g_unk_7101d47310), &FloorSlideCameraId(), false);

        UnkPlayer_l4fb4ec* player = Player();
        UnkForcedMovementTarget_A5_b68* movement = player->movement;
        if (!movement || !player->input)
            return;
        const UnkMatrix44& m = fn_710002614c(reinterpret_cast<const UnkMatrix44*>(player->transform));
        base::math::CVector3D facing;
        facing.x = m.m[2];
        facing.y = m.m[6];
        facing.z = m.m[10];
        base::math::CVector2D direction;
        fn_710007b430(&direction, &facing);
        fn_71010db848(movement, &g_unk_71726dd218, &direction);

        UnkFlagList blocked;
        blocked.PushBack(0x100);
        blocked.PushBack(0x8000);
        blocked.PushBack(0x20);
        blocked.PushBack(0x80);
        blocked.PushBack(0x100000);
        blocked.PushBack(0x800);
        blocked.PushBack(0x40000);
        blocked.PushBack(0x80000);
        blocked.PushBack(0x1);
        blocked.PushBack(0x2);
        blocked.PushBack(0x10000);
        blocked.PushBack(0x20000);
        fn_71008724f8(player->input, &g_unk_71726dd210, &blocked, true, true, false, true);
    } else {
        UnkCameraCtrl* ctrl = fn_71010e389c(fn_7100defcec(g_unk_7101d47310),
                                            const_cast<base::global::CStrId*>(&FloorSlideCameraId()));
        if (ctrl && ctrl->vf_0x0() == fn_71007d2ff4())
            static_cast<UnkFloorSlideCameraCtrl_l4fb4ec*>(ctrl)->done = true;
        Slots(g_unk_7101d47310)->field_0x720 = self;
        Slots(g_unk_7101d47310)->field_0x728 = fn_71004fd854;

        fn_71010e64a4(static_cast<UnkCamComp6_ovb5*>(fn_7100defcec(g_unk_7101d47310)), CRntString("CAM_Default"),
                      false);
        {
            CRntString slomo("s");
            fn_7100df6d3c(g_unk_7101d47310, &slomo, 1.0f, 0.3f);
        }
        fn_7100108c58(g_unk_7101d4d820, 0.3f, 2);

        UnkPlayer_l4fb4ec* player = Player();
        if (!player->movement || !player->input)
            return;
        fn_71010db850(player->movement, &g_unk_71726dd218);
        fn_7100872858(player->input, &g_unk_71726dd210);
    }
}
