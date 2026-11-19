// GUI::CModel3D (unit 2753 of tools/tu_map.py): places the model's entity. The depth (+0x224) and the turn
// (+0x23c) ease toward their targets, the entity is shown while the display object is, and then it is moved to
// its offset turned by the turn and set back by the depth. Names and layouts are placeholders.
#include "base/UnkAtExit.h"
#include "base/global/CStrId.h"
#include "game/UnkGameManagerIdTable.h"
#include "game/UnkGuiDisplayObject.h"
#include "game/UnkTU341.h"
#include "game/UnkTU54.h"
#include "game/UnkXformNode.h"
#include "types.h"

using base::global::CStrId;

struct UnkFXComponent_ovb51;
void fn_71011576dc(UnkFXComponent_ovb51* self, f32 scale);

void fn_71001435f0(void* model, const UnkVec3f* scale);

// The display state as this model reads it (fn_71011cde14).
struct UnkModelState_l1f48dc {
    u8 pad_0x0[0x10];
    f32 turn;  // 0x10
    u8 pad_0x14[0x8];
    f32 scale;  // 0x1c
    u8 pad_0x20[0x14];
    f32 alpha;  // 0x34
    s32 frame;  // 0x38
    bool active;  // 0x3c
    bool shown;   // 0x3d
};

struct UnkModelPart_l1f48dc {
    u8 pad_0x0[0x50];
    void* model;  // 0x50
    u8 pad_0x58[0x8];
    f32 height;  // 0x60
};

// Whether the part has the slot the id names.
bool fn_71011735d0(UnkModelPart_l1f48dc* part, const u64* id);

struct UnkModelEntity_l1f48dc {
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
    virtual void SetPosition(const UnkVec3f* position, bool snap);  // vf_0xd0

    u8 pad_0x8[0x30];
    UnkXform xform;  // 0x38
    u8 pad_0x94[0x228 - 0x94];
    UnkModelPart_l1f48dc* part;  // 0x228
    u8 pad_0x230[0x280 - 0x230];
    UnkFXComponent_ovb51* fx;  // 0x280
    u8 pad_0x288[0x3b0 - 0x288];
    bool position_changed;  // 0x3b0
    u8 pad_0x3b1[0xf];
    bool angle_changed;  // 0x3c0
};
static_assert(__builtin_offsetof(UnkModelEntity_l1f48dc, angle_changed) == 0x3c0, "");

struct UnkGuiModel3D_l1f48dc {
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

    u8 pad_0x8[0x188];
    f32 height;  // 0x190
    bool scale_parts;  // 0x194
    u8 pad_0x195[0x220 - 0x195];
    f32 depth_speed;   // 0x220
    f32 depth;         // 0x224
    f32 depth_target;  // 0x228
    u8 pad_0x22c[0xc];
    f32 turn_speed;  // 0x238
    f32 turn;        // 0x23c
    UnkVec3f offset;   // 0x240
    UnkVec3f offset2;  // 0x24c
    bool snap;  // 0x258
    u8 pad_0x259[0x7];
    UnkModelEntity_l1f48dc* entity;  // 0x260
};
static_assert(__builtin_offsetof(UnkGuiModel3D_l1f48dc, entity) == 0x260, "");

static inline UnkVec3f Add(const UnkVec3f& a, const UnkVec3f& b) {
    return UnkVec3f(a.x + b.x, a.y + b.y, a.z + b.z);
}

static inline void AddTo(UnkVec3f& a, const UnkVec3f& b) {
    a.x += b.x;
    a.y += b.y;
    a.z += b.z;
}

// `v` turned by `turn` around the vertical axis.
static inline UnkVec3f Turned(const UnkVec3f& v, f32 turn) {
    UnkVec3f angle(0.0f, turn, 0.0f);
    return fn_710007a450(const_cast<UnkVec3f*>(&v), &angle);
}

static inline bool Visible(const UnkModelState_l1f48dc* state) {
    return state->active && state->shown && state->alpha > 0.0f;
}

// The id table's slot at +0xd18: the name of the part's height slot.
static inline const CStrId& HeightSlot() {
    return *reinterpret_cast<const CStrId*>(GameManagerIdTable().pad_0xcd0 + (0xd18 - 0xcd0));
}

// Eases `value` toward `target` at `speed`, a thirtieth of the way per unit.
static inline void Ease(f32& value, f32 target, f32 speed) {
    if (__builtin_fabsf(target - value) > 0.01f)
        value = value + speed * (target - value) / 30.0f;
}

void fn_71011f48dc(UnkGuiModel3D_l1f48dc* self) {
    UnkModelState_l1f48dc* state =
        reinterpret_cast<UnkModelState_l1f48dc*>(fn_71011cde14(reinterpret_cast<UnkGuiDisplayObject*>(self)));
    if (state->frame == 0x7fffffff)
        return;

    if (!(self->depth >= 0.0f) || !(self->depth_speed >= 0.0f))
        self->depth = self->depth_target;
    else
        Ease(self->depth, self->depth_target, self->depth_speed);

    if (!(self->turn > -10000.0f) || !(self->turn_speed >= 0.0f))
        self->turn = state->turn;
    else
        Ease(self->turn, state->turn, self->turn_speed);

    if (self->entity) {
        f32 height = self->height;
        fn_7100232864(reinterpret_cast<UnkEntityState*>(self->entity), Visible(state));
        if (Visible(state)) {
            if (self->entity->fx && self->scale_parts)
                fn_71011576dc(self->entity->fx, state->scale);
            UnkModelPart_l1f48dc* part = self->entity->part;
            if (part) {
                if (self->scale_parts && part->model) {
                    UnkVec3f scale(state->scale, state->scale, state->scale);
                    fn_71001435f0(part->model, &scale);
                }
                u64 id = HeightSlot().data->Key();
                if (fn_71011735d0(part, &id))
                    height = part->height;
            }
            UnkVec3f position = Add(Add(UnkVec3f(0.0f, height, 0.0f), self->offset), self->offset2);
            UnkVec3f world = Turned(position, self->turn);
            AddTo(world, UnkVec3f(0.0f, 0.0f, -self->depth));
            if (self->snap) {
                self->entity->SetPosition(&world, true);
                self->snap = false;
            } else {
                UnkModelEntity_l1f48dc* entity = self->entity;
                entity->xform.SetTranslation(world);
                entity->position_changed = true;
            }
            UnkModelEntity_l1f48dc* entity = self->entity;
            entity->xform.SetAngle(UnkVec3f(0.0f, self->turn, 0.0f));
            entity->angle_changed = true;
        }
        self->vf_0x68();
    }
    self->vf_0x90();
}
