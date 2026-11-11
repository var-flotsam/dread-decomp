// CSubAreaManager::CTunableSubAreaManager unit (tools/tu_map.py unit 2577): fn_7100ee6ae4: takes the player as the follow actor and, unless the sub-area ids are equal, stores the anchor and the direction to the camera. Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CAbilityEnergyWave.h"
#include "game/CAbilityGhostAura.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkCameraCtrl.h"
#include "game/UnkGameManager.h"
#include "game/UnkXformNode.h"
#include "types.h"

using base::global::CStrId;
using base::reflection::CClass;

struct UnkPlayer;
UnkPlayer* fn_7100dec788(UnkGameManager*);

struct UnkAbility {
    virtual const CClass* vf_0x0();
};
struct UnkAbilitySet;
bool fn_710030a7b8(UnkAbility* ability);

struct UnkForcedMovementTarget_A5_b68;
void fn_71010db848(UnkForcedMovementTarget_A5_b68* self, const CStrId* name, const void* params);
extern CStrId g_unk_71729daee8 __attribute__((visibility("hidden")));
void fn_7100eed9ec(void* self, void* target);

// A sub-area (a placeholder for the fields used here): its first id.
struct UnkSubArea {
    CStrId field_0x0;
};
static inline const CStrId& FirstId(const UnkSubArea* area) { return area ? area->field_0x0 : g_unk_7101d470e0; }

// The player as far as it is read here.
struct UnkFollowPlayer_b190 {
    u8 pad_0x0[0x38];
    UnkXform xform;  // 0x38
    u8 pad_0x94[0x1d8 - 0x94];
    void* field_0x1d8;
    u8 pad_0x1e0[0x20];
    UnkForcedMovementTarget_A5_b68* field_0x200;
    u8 pad_0x208[0x18];
    UnkAnimationComponent* field_0x220;
    u8 pad_0x228[0xb8];
    UnkAbilitySet* abilities;  // 0x2e0
};
static_assert(__builtin_offsetof(UnkFollowPlayer_b190, xform) == 0x38, "layout");
static_assert(__builtin_offsetof(UnkFollowPlayer_b190, field_0x1d8) == 0x1d8, "layout");
static_assert(__builtin_offsetof(UnkFollowPlayer_b190, abilities) == 0x2e0, "layout");

struct UnkSubAreaMgr_b190 {
    u8 pad_0x0[0x11c];
    UnkVec3f field_0x11c;
    UnkVec3f field_0x128;
    UnkVec3f field_0x134;
    u8 pad_0x140[4];
    u32 field_0x144;
    UnkFollowPlayer_b190* actor;  // 0x148
    bool active;                  // 0x150
    u8 pad_0x151[0x29f];
    UnkSubArea* field_0x3f0;
    UnkSubArea* field_0x3f8;
};
static_assert(__builtin_offsetof(UnkSubAreaMgr_b190, actor) == 0x148, "layout");
static_assert(__builtin_offsetof(UnkSubAreaMgr_b190, field_0x3f0) == 0x3f0, "layout");

void fn_7100ee6ae4(UnkSubAreaMgr_b190* self) {
    UnkFollowPlayer_b190* player = reinterpret_cast<UnkFollowPlayer_b190*>(fn_7100dec788(g_unk_7101d47310));
    if (player) {
        UnkAnimationComponent* character = player->field_0x220;
        if (character && fn_710117b348(character, fn_7100c4041c()))
            return;
    }
    self->actor = player;
    self->active = true;
    const CStrId& prev = FirstId(self->field_0x3f8);
    const CStrId& cur = FirstId(self->field_0x3f0);
    a_u32 oz = 0, oy = 0, ox = 0;
    if (player) {
        if (UnkAbilitySet* abilities = player->abilities) {
            UnkAbility* ability = fn_71003013ec(abilities, fn_7100308c74());
            if (ability && fn_710009d50c(ability->vf_0x0(), fn_7100306914()) && fn_710030a7b8(ability))
                self->active = false;
        }
        UnkForcedMovementTarget_A5_b68* movement = player->field_0x200;
        void* target = player->field_0x1d8;
        if (movement && target && prev.data != cur.data) {
            void* holder = fn_7100defcec(g_unk_7101d47310);
            fn_7100eed9ec(holder, target);
            UnkVec3f angle = player->xform.GetAngle();
            self->field_0x144 = *(a_u32*)&angle.y;
            f32 px = player->xform.m[0][3];
            f32 py = player->xform.m[1][3];
            f32 pz = player->xform.m[2][3];
            const UnkVec3f* camera = static_cast<const UnkVec3f*>(fn_71010e537c(holder));
            f32 dx = px - camera->x;
            f32 dy = py - camera->y;
            f32 dz = pz - camera->z;
            self->field_0x128.y = dy;
            self->field_0x128.x = dx;
            self->field_0x128.z = dz;
            if (!*(reinterpret_cast<u8*>(movement) + 0x7bb)) {
                f32 params[2];
                params[0] = (dx > 0.0f) - (dx < 0.0f);
                params[1] = 0.0f;
                fn_71010db848(movement, &g_unk_71729daee8, params);
            }
            ox = *(a_u32*)&player->xform.m[0][3];
            oy = *(a_u32*)&player->xform.m[1][3];
            oz = *(a_u32*)&player->xform.m[2][3];
            ((a_u32*)&self->field_0x134)[0] = ox;
            ((a_u32*)&self->field_0x134)[1] = oy;
            ((a_u32*)&self->field_0x134)[2] = oz;
        }
    }
    ((a_u32*)&self->field_0x11c)[0] = ox;
    ((a_u32*)&self->field_0x11c)[1] = oy;
    ((a_u32*)&self->field_0x11c)[2] = oz;
}
