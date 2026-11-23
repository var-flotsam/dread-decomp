// Unit 2726 (gui/scripts/msemenu_ingame.lua): the in-game menu's per-frame update, a state machine over the
// player's turn, the effect fades, the death and the game-over prompt. Names and layouts are placeholders.
#include <algorithm>

#include "../../../src/game_s7/UnkIter.h"
#include "../../../src/game_s7/UnkPropBagS7.h"
#include "../../../src/game_s7/UnkGuiS7.h"
#include "base/UnkMatrix44.h"
#include "base/global/CRntFile.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "base/input/CRumbleManager/CTunableRumbleManager.h"
#include "base/input/UnkRumbleManager.h"
#include "base/math/CVector.h"
#include "base/math/CVector4D.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "engine/scene/UnkToggle.h"
#include "game/CBossAIComponent.h"
#include "game/GUI/CPopUpComposition.h"
#include "game/UnkGameManager.h"
#include "game/UnkGuiDisplayObject.h"
#include "game/UnkTU0.h"
#include "game/UnkTU223.h"
#include "game/UnkTU23.h"
#include "game/UnkTU253.h"
#include "game/UnkTU2553.h"
#include "game/UnkTU2578.h"
#include "game/UnkTU2656.h"
#include "game/UnkTU341.h"
#include "game/UnkTU54.h"
#include "game/UnkTU56.h"
#include "game/UnkXformNode.h"
#include "game/sound/CMusicManager.h"
#include "game/sound/CMusicManager/CTunableMusicManager.h"
#include "types.h"

using base::global::CStrId;
using base::math::CVector3D;
using base::math::CVector4D;
using unk_prop_bag::Key;
using unk_prop_bag::UnkPropBag;

// Calls the script function `name`, collecting what it returns in `results`.
bool fn_7100dfb758(UnkPropBag* results, const char* name, const char* fmt, ...);
void fn_710010825c(UnkSoundMgr* manager, const u32* id, f32 time);
UnkMatrix44 fn_7100078e54(f32 a, f32 b, f32 c);
// As src/game/UnkTU2523/UnkTU2523_Misc_ovb0220.cpp defines it.
struct UnkEntityUpd_b0220;
void fn_7100d7b068(UnkEntityUpd_b0220* self, f32 dt, bool flag);
void fn_7100d7b1c0(void* entity, u32 index, f32 dt);
// As src/game/animtree/CAnimTreeNode/CAnimTreeNode_GlobalWeight_ovb0097.cpp defines it.
struct UnkAnimTreeNode_ovb0097;
void fn_71002df844(UnkAnimTreeNode_ovb0097* self);
// As src/game/UnkTU2726/UnkTU2726_Death_l18cdfc.cpp defines it.
struct UnkIngameMenu_l18cdfc;
void fn_710118cdfc(UnkIngameMenu_l18cdfc* self);
// As src/game/UnkTU2726/UnkTU2726_Ingame_ovb0168.cpp defines it.
struct UnkIngameMenu_ovb0168b;
void fn_710118a4c0(UnkIngameMenu_ovb0168b* self);
struct UnkMapCallback_ovb0090;
struct UnkGuiComposition_ovb0090;

struct UnkIngameMenu_l188c2c;
void fn_710118bf50(UnkIngameMenu_l188c2c* self);
void fn_710118c7a0(UnkIngameMenu_l188c2c* self);
void fn_71011898b4(UnkIngameMenu_l188c2c* self);
void fn_710118fd20(void* self);
void fn_710118fd24(void* self);
void fn_71011a2d94(UnkGameManagerSub_0x608* points);

// A counted reference to a GUI object.
struct UnkMenuTarget_l188c2c {
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
};

struct UnkMenuRef_l188c2c {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void Destroy();  // 0x10
    s32 count;  // 0x8
    UnkMenuTarget_l188c2c* target;  // 0x10
};

struct UnkFadeOwner_l188c2c {
    u8 pad_0x0[0x160];
    void* field_0x160;
};

struct UnkIngameMenu_l188c2c {
    u8 pad_0x0[0x258];
    UnkMenuRef_l188c2c* field_0x258;
    s32 state;  // 0x260
    f32 timer;  // 0x264
    f32 angle;  // 0x268
    u8 field_0x26c[0x2b0 - 0x26c];  // what the turn blends with (fn_7100078834)
    UnkFadeOwner_l188c2c* field_0x2b0;
    f32 field_0x2b8;
};

struct UnkMenuAnim_l188c2c {
    u8 pad_0x0[0x40];
    UnkAnimTreeNode_ovb0097* field_0x40;
};

struct UnkMenuEntity_l188c2c {
    u8 pad_0x0[0x220];
    UnkMenuAnim_l188c2c* animation;  // 0x220
};

struct UnkMenuPlayer_l188c2c {
    u8 pad_0x0[0x38];
    UnkXform xform;  // 0x38
    u8 pad_0x94[0x3c0 - 0x94];
    bool field_0x3c0;
};

struct UnkMenuHandle_l188c2c {
    u8 pad_0x0[0x10];
    UnkMenuEntity_l188c2c* target;  // 0x10
};

struct UnkGameManagerView_l188c2c {
    u8 pad_0x0[0x29a8];
    UnkMenuHandle_l188c2c* field_0x29a8;
    UnkMenuHandle_l188c2c* field_0x29b0;
};

struct UnkPromptHolder_l188c2c {
    u8 pad_0x0[0x10];
    UnkGuiDisplayObject* prompt;  // 0x10
};

struct UnkFadeTarget_l188c2c {
    u8 pad_0x0[0xf8];
    f32 x, y, z;  // 0xf8
};

struct UnkGuiPanel_l188c2c {
    u8 pad_0x0[0x78];
    UnkFadeTarget_l188c2c* field_0x78;
};

struct UnkGuiItem_l188c2c {
    u8 pad_0x0[0x18];
    u8 toggle[0x178 - 0x18];  // 0x18
    UnkGuiPanel_l188c2c* field_0x178;
};

// The game-over prompt's texts and objects.
struct UnkGameOverGui_l188c2c {
    CStrId text;  // 0x0
    CStrId back;  // 0x8
    CStrId accept;  // 0x10
    u8 pad_0x18[0x70 - 0x18];
    UnkGuiDisplayObject* field_0x70;
    UnkGuiDisplayObject* field_0x78;
    UnkGuiItem_l188c2c* field_0x80;
    UnkGuiItem_l188c2c* field_0x88;
};
extern UnkGameOverGui_l188c2c g_unk_7172a0b870 __attribute__((visibility("hidden")));
extern const base::global::CRntString g_unk_7172a0b978 __attribute__((visibility("hidden")));
extern unk_gui::UnkColor g_unk_7172a0b7f8;
extern const unk_gui::UnkColor g_unk_710164d1cc;

struct UnkSceneState_l188c2c {
    u8 pad_0x0[0x11c];
    u32 field_0x11c;
};

struct UnkMusicMode_l188c2c {
    u32 kind;
};

struct UnkColorValue_l188c2c {
    f32 r, g, b, a;
    UnkColorValue_l188c2c(f32 r_, f32 g_, f32 b_, f32 a_) : r(r_), g(g_), b(b_), a(a_) {}
};

struct UnkPromptCallback_l188c2c {
    void* owner;
    void (*fn)(void*);
};

// A value a script call returned: where it is in the bag's stream, the hash of its name and its type.
struct UnkScriptValue_l188c2c {
    u64 offset;
    u64 key;
    s32 type;  // 'b' for a bool
};

// A zero whose constructor isn't constexpr, so the static is guarded.
struct UnkRumbleZero_l188c2c {
    u32 value;
    UnkRumbleZero_l188c2c() : value(0) {}
};

inline const UnkRumbleZero_l188c2c& RumbleChannelsNone_l188c2c() {
    static UnkRumbleZero_l188c2c s_value asm("g_unk_7101d58738");
    return s_value;
}

// Every rumble channel: the complement of that zero, limited to five bits.
inline u32 RumbleChannelsAll_l188c2c() {
    static u32 s_mask asm("g_unk_7101d58748") = ~RumbleChannelsNone_l188c2c().value & 0x1f;
    return s_mask;
}

static inline UnkScriptValue_l188c2c* Values_l188c2c(UnkPropBag& results) {
    return reinterpret_cast<UnkScriptValue_l188c2c*>(results.values.data);
}

// The value named `id`, or the end of the values when there is none.
static inline UnkScriptValue_l188c2c* FindValue_l188c2c(UnkPropBag& results, const CStrId& id) {
    UnkScriptValue_l188c2c* values = Values_l188c2c(results);
    u64 key = id.data->Key();
    UnkIter<UnkScriptValue_l188c2c> first{values};
    UnkIter<UnkScriptValue_l188c2c> last{&values[results.values.count]};
    return std::find_if(first, last, [key](const UnkScriptValue_l188c2c& v) { return v.key == key; }).p;
}

static inline UnkSceneState_l188c2c* SceneState() {
    UnkStruct_0x128* s = g_unk_7101d47320;
    if (!s) {
        s = static_cast<UnkStruct_0x128*>(fn_71000839a0(0x150, 0x3f));
        fn_7100ef3494(s);
        g_unk_7101d47320 = s;
    }
    return reinterpret_cast<UnkSceneState_l188c2c*>(s);
}

static inline UnkStruct_0x128* Scene() {
    return reinterpret_cast<UnkStruct_0x128*>(SceneState());
}

static inline UnkEntityUpd_b0220* Entity(void* entity) {
    return static_cast<UnkEntityUpd_b0220*>(entity);
}

// Counts the time down; whether it was still running.
static inline bool Wait(UnkIngameMenu_l188c2c* self, f32 dt) {
    if (self->timer > 0.0f) {
        self->timer -= dt;
        return true;
    }
    return false;
}

static inline void StopEntity(UnkMenuEntity_l188c2c* entity) {
    fn_7100232864(reinterpret_cast<UnkEntityState*>(entity), false);
    fn_71002df844(entity->animation->field_0x40);
}

void fn_7101188c2c(UnkIngameMenu_l188c2c* self, f32 dt) {
    UnkMenuPlayer_l188c2c* player = reinterpret_cast<UnkMenuPlayer_l188c2c*>(fn_7100dec788(g_unk_7101d47310));
    UnkGameManagerView_l188c2c* view = reinterpret_cast<UnkGameManagerView_l188c2c*>(g_unk_7101d47310);
    UnkMenuEntity_l188c2c* left = view->field_0x29a8 ? view->field_0x29a8->target : nullptr;
    UnkMenuEntity_l188c2c* right = view->field_0x29b0 ? view->field_0x29b0->target : nullptr;

    switch (self->state) {
    case 1:
        // The player turns towards the menu.
        if (self->timer > 0.0f) {
            self->timer -= dt;
            f32 weight = (0.5f - self->timer) * 2.0f;
            weight = !(weight >= 1.0f) ? weight : 1.0f;
            UnkMatrix44 turn = fn_7100078e54(0.0f, self->angle, 0.0f);
            UnkMatrix44 blended =
                fn_7100078834(reinterpret_cast<const UnkKeyedEntry*>(self->field_0x26c), &turn, weight);
            UnkVec3f angle = fn_7100076acc(reinterpret_cast<const UnkXform*>(&blended));
            player->xform.SetAngle(angle);
            player->field_0x3c0 = true;
        }
        fn_7100d7b068(Entity(player), dt, false);
        fn_7100d7b1c0(player, 0, dt);
        if (self->timer <= 0.0f)
            fn_710118bf50(self);
        break;
    case 2:
        fn_7100d7b068(Entity(player), dt, false);
        fn_7100d7b1c0(player, 0, dt);
        if (Wait(self, dt))
            break;
        self->state = 3;
        self->timer = 1.0f;
        {
            CVector3D white;
            white.x = 1.0f;
            white.y = 1.0f;
            white.z = 1.0f;
            fn_7101053850(g_unk_7101d47310->field_0x1040, reinterpret_cast<UnkVec*>(&white), 1, 0.2f, 1.0f);
        }
        if (self->field_0x2b0) {
            UnkColorValue_l188c2c white(1.0f, 1.0f, 1.0f, 1.0f);
            fn_71001c2ec0(self->field_0x2b0->field_0x160, 0, 0, reinterpret_cast<CVector4D*>(&white), 1, 0.2f);
        }
        break;
    case 3:
        if (fn_7101053a98(g_unk_7101d47310->field_0x1040))
            break;
        {
            CVector3D white;
            white.x = 1.0f;
            white.y = 1.0f;
            white.z = 1.0f;
            fn_7101053c14(g_unk_7101d47310->field_0x1040, &white, 0.1f, 0.2f, 1.0f);
        }
        fn_710118c7a0(self);
        break;
    case 4:
        fn_7100d7b068(Entity(left), dt, false);
        fn_7100d7b1c0(left, 0, dt);
        fn_7100d7b068(Entity(right), dt, false);
        fn_7100d7b1c0(right, 0, dt);
        if (Wait(self, dt))
            break;
        self->timer = 1.0f;
        self->state = 5;
        break;
    case 5:
        fn_7100d7b068(Entity(left), dt, false);
        fn_7100d7b1c0(left, 0, dt);
        fn_7100d7b068(Entity(right), dt, false);
        fn_7100d7b1c0(right, 0, dt);
        if (Wait(self, dt))
            break;
        self->timer = 0.75f;
        self->state = 6;
        break;
    case 6:
        fn_7100d7b068(Entity(left), dt, false);
        fn_7100d7b1c0(left, 0, dt);
        fn_7100d7b068(Entity(right), dt, false);
        fn_7100d7b1c0(right, 0, dt);
        if (Wait(self, dt))
            break;
        fn_710118cdfc(reinterpret_cast<UnkIngameMenu_l18cdfc*>(self));
        break;
    case 8:
        if (self->timer > 0.0f)
            self->timer -= dt;
        if (self->timer <= 0.0f) {
            UnkVec3f black(0.0f, 0.0f, 0.0f);
            fn_7101053c14(g_unk_7101d47310->field_0x1040, reinterpret_cast<const CVector3D*>(&black), 0.01f, 0.0f,
                          1.0f);
            if (g_unk_7101d47310->field_0x608)
                fn_71011a2d94(g_unk_7101d47310->field_0x608);
            fn_710118cdfc(reinterpret_cast<UnkIngameMenu_l18cdfc*>(self));
        }
        // Then as the death.
    case 7: {
        bool overrides = false;
        UnkPropBag results;
        if (fn_7100dfb758(&results, "CurrentScenario.GetOnDeathOverrides", g_unk_710162e2ed)) {
            UnkScriptValue_l188c2c* it = FindValue_l188c2c(results, Key(0x1218));
            if (it != &Values_l188c2c(results)[results.values.count] && it->type == 'b') {
                CRntFile_Seek(&results.lock, it->offset, 0);
                fn_710007c70c(&results.lock, &overrides, 1);
            }
        }
        if (self->timer > 0.0f) {
            self->timer -= dt;
        } else {
            if (overrides) {
                fn_710118a4c0(reinterpret_cast<UnkIngameMenu_ovb0168b*>(self));
            } else {
                fn_71011898b4(self);
                StopEntity(left);
                StopEntity(right);
            }
            UnkMenuRef_l188c2c* ref = self->field_0x258;
            if (ref && ref->target) {
                ref->target->vf_0x90();
                if (UnkMenuRef_l188c2c* held = self->field_0x258) {
                    if (--held->count == 0)
                        held->Destroy();
                    self->field_0x258 = nullptr;
                }
            }
        }
        break;
    }
    case 9: {
        if (Wait(self, dt))
            break;
        {
            UnkVec3f black(0.0f, 0.0f, 0.0f);
            fn_7101053c14(g_unk_7101d47310->field_0x1040, reinterpret_cast<const CVector3D*>(&black), 0.75f, 1.0f,
                          1.0f);
        }
        if (g_unk_7101d47310->field_0x608)
            fn_71011a2d94(g_unk_7101d47310->field_0x608);
        UnkPromptHolder_l188c2c* holder = static_cast<UnkPromptHolder_l188c2c*>(g_unk_7101d47310->field_0x2258);
        if (!holder)
            break;
        UnkGuiDisplayObject* prompt = holder->prompt;
        if (!prompt)
            break;
        if (self->field_0x2b8 < 0.0f) {
            if (!fn_7100f00a24(Scene(), 0x4d)) {
                fn_7100f003e8(Scene(), -2002);
                fn_7100f01838(Scene(), 0x4d);
                SceneState()->field_0x11c = 0;
            }
            self->field_0x2b8 = 1.5f;
            u32 sound = 0xef;
            fn_710010825c(g_unk_7101d4d820, &sound, 0.2f);

            base::input::UnkRumbleManager* rumble = g_unk_7101d47348;
            if (!rumble) {
                rumble = static_cast<base::input::UnkRumbleManager*>(fn_71000839a0(0x2210, 0x3f));
                fn_71000733e4(rumble);
                g_unk_7101d47348 = rumble;
            }
            alignas(8) u32 channels = RumbleChannelsAll_l188c2c();
            fn_7100074f94(rumble, &channels);

            void* music = g_unk_71726bc560;
            if (!music) {
                music = fn_71000839a0(0xf18, 0x3f);
                fn_7100fd3804(music);
                g_unk_71726bc560 = static_cast<UnkMgr_0xf18*>(music);
            }
            UnkMusicMode_l188c2c mode;
            mode.kind = 6;
            fn_7100fd90d8(-1.0f, -1.0f, music, &g_unk_7172a0b978, 6, reinterpret_cast<const UnkMusicMode*>(&mode), 0,
                          0, nullptr);
        }
        fn_71011cdce8(prompt, true);
        UnkGameOverGui_l188c2c* gui = &g_unk_7172a0b870;
        UnkPromptCallback_l188c2c yes = {self, fn_710118fd20};
        UnkPromptCallback_l188c2c no = {self, fn_710118fd24};
        fn_7100b5fde0(reinterpret_cast<UnkGuiComposition_ovb0090*>(prompt), &gui->text, &gui->accept, &gui->back,
                      reinterpret_cast<const UnkMapCallback_ovb0090*>(&yes),
                      reinterpret_cast<const UnkMapCallback_ovb0090*>(&no), 0, 0.0f);
        fn_710020ac34(reinterpret_cast<UnkToggle*>(gui->field_0x80->toggle), false);
        fn_710020ac34(reinterpret_cast<UnkToggle*>(gui->field_0x88->toggle), true);
        fn_71011cdce8(gui->field_0x70, true);
        fn_71011cdc30(gui->field_0x70, true);
        fn_71011cdce8(gui->field_0x78, true);
        fn_71011cdc30(gui->field_0x78, true);
        UnkFadeTarget_l188c2c* target = gui->field_0x88->field_0x178->field_0x78;
        fn_710019c564(target, target->x, target->y, target->z, 0.0f);
        if (g_unk_7172a0b7f8.a == g_unk_710164d1cc.a) {
            UnkGuiDisplayObject* fade = fn_71011d98ec(prompt, CStrId("FadeScreen", true).data->Key());
            if (fade && fn_710009d50c(fade->GetClass(), fn_71011f6890())) {
                g_unk_7172a0b7f8 = *reinterpret_cast<const unk_gui::UnkColor*>(reinterpret_cast<u8*>(fade) + 0xa0);
                UnkColorValue_l188c2c clear(0.0f, 0.0f, 0.0f, 1.0f);
                unk_gui::fn_71011cfc24(fade, reinterpret_cast<const unk_gui::UnkColor&>(clear), 1);
            }
        }
        break;
    }
    }

    if (self->state > 1) {
        UnkGameManagerTarget* target = fn_7100decab0(g_unk_7101d47310);
        fn_7100d7b068(Entity(target), dt, false);
        fn_7100d7b068(Entity(left), dt, false);
        fn_7100d7b068(Entity(right), dt, false);
        fn_7100d7b1c0(target, 0, dt);
        fn_7100d7b1c0(left, 0, dt);
        fn_7100d7b1c0(right, 0, dt);
    }
}
