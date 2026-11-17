// CQuetzoaXAIComponent unit (tools/tu_map.py): the update. After the two base updates it counts a timer up
// and, once it passes the limit while armed, sends a fixed request to the owner's character class; then,
// under a boss camera, eases the camera's value toward a target and keeps last frame's box. Names and layouts
// are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CAttackComponent.h"
#include "game/UnkCameraCtrl.h"
#include "game/UnkGameManager.h"
#include "types.h"

// The camera controller once it tests as a CBossCameraCtrl (fn_710074eca8): the field written here.
struct UnkBossCameraCtrl : UnkCameraCtrl {
    u8 pad_0x28[0x31c];
    f32 field_0x344;
};

extern f32 g_unk_7101a58f74;
extern f32 g_unk_7101a58f78;
extern f32 g_unk_7101a58f7c;

struct UnkQuetzoaXParams_l63f6e8 {
    u8 pad_0x0[0x808];
    f32 field_0x808;
};

struct UnkQuetzoaXBox_l63f6e8 {
    f32 min_x, min_y, min_z;
    f32 max_x, max_y, max_z;

    UnkQuetzoaXBox_l63f6e8& operator=(const UnkQuetzoaXBox_l63f6e8& other) {
        min_x = other.min_x;
        min_y = other.min_y;
        min_z = other.min_z;
        max_x = other.max_x;
        max_y = other.max_y;
        max_z = other.max_z;
        return *this;
    }
};

struct UnkQuetzoaXState_l63f6e8 {
    UnkQuetzoaXParams_l63f6e8* params;
    u8 pad_0x8[0xa0];
    bool armed;  // 0xa8
    u8 pad_0xa9[3];
    f32 timer;  // 0xac
    UnkQuetzoaXBox_l63f6e8 prev;  // 0xb0
    UnkQuetzoaXBox_l63f6e8 cur;   // 0xc8
    f32 camera_value;  // 0xe0
};

struct UnkQuetzoaXOwner_l63f6e8 {
    u8 pad_0x0[0x220];
    void* field_0x220;
    u8 pad_0x228[0x50];
    void* field_0x278;
};

struct UnkQuetzoaXAI_l63f6e8 {
    u8 pad_0x0[0x30];
    UnkQuetzoaXOwner_l63f6e8* owner;
    u8 pad_0x38[0x14b20 - 0x38];
    UnkQuetzoaXState_l63f6e8 state;
};

void fn_710064ac98(UnkQuetzoaXAI_l63f6e8* self, f32 dt);
void fn_710064a8cc(UnkQuetzoaXAI_l63f6e8* self, f32 dt);

// Moves `value` toward `target` by at most `step`.
static inline f32 Approach(f32 value, f32 target, f32 step) {
    f32 down = value - step;
    f32 up = value + step;
    f32 low = !(target > down) ? down : target;
    f32 high = !(target < up) ? up : target;
    return !(target > value) ? low : high;
}

void fn_710063f6e8(UnkQuetzoaXAI_l63f6e8* self, f32 dt) {
    UnkQuetzoaXState_l63f6e8* state = &self->state;
    fn_710064ac98(self, dt);
    fn_710064a8cc(self, dt);
    state->timer += dt;
    if (state->armed && state->timer > state->params->field_0x808) {
        void* attack = self->owner->field_0x278;
        if (attack && !fn_7101136308(attack)) {
            if (void* target = self->owner->field_0x220)
                UnkSpitSend(target, 0x946efc035e67d306, 0xf4061ff25a2163d6);
        }
    }
    if (void* holder = fn_7100defcec(g_unk_7101d47310)) {
        UnkCameraCtrl* camera = fn_71010e2080(holder);
        if (camera && fn_710009d50c(camera->vf_0x0(), fn_710074eca8())) {
            f32 cur_width = (state->cur.max_x - state->cur.min_x) * 0.5f;
            f32 prev_width = (state->prev.max_x - state->prev.min_x) * 0.5f;
            if (cur_width > prev_width)
                state->camera_value = g_unk_7101a58f74;
            else if (state->camera_value == g_unk_7101a58f74)
                state->camera_value = g_unk_7101a58f78;
            else
                state->camera_value = Approach(state->camera_value, g_unk_7101a58f74, g_unk_7101a58f7c * dt);
            static_cast<UnkBossCameraCtrl*>(camera)->field_0x344 = state->camera_value;
        }
    }
    state->prev = state->cur;
}
