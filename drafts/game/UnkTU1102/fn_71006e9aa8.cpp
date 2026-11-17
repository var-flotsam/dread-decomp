// Unit 1102 (hint CXParasiteAIComponent): fn_71006e9aa8 moves the parasite with its prey. While it holds a
// target point it pulls the prey toward the grabbed object, takes the grabbed object's position as the new
// target when it comes near, and blends its own position toward the prey's (or the prey model's attachment
// point) by the timer at +0x146fc. Names and layouts are placeholders.
#include "base/UnkMatrix44.h"
#include "base/global/CStrId.h"
#include "game/CModelUpdaterComponent.h"
#include "game/UnkTU54.h"
#include "game/UnkXformNode.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::global::CStrId;

struct UnkMatrix44Words {
    u32 m[16];

    UnkMatrix44Words() {}
    UnkMatrix44Words(const UnkMatrix44Words& other);
};

UnkMatrix44Words fn_7100076dc4(const UnkMatrix44Words* src);

// The prey's model, as far as this source reaches: the name of its attachment and the attachment's offset.
struct UnkXParasiteModel_l6e9aa8;
const CStrId* fn_710110f224(UnkXParasiteModel_l6e9aa8* model);
const UnkVec3f* fn_710110f230(UnkXParasiteModel_l6e9aa8* model);

// An actor as this source sees it: its transform, a flag, its model and the flag that marks the transform
// changed.
struct UnkXParasiteBody_l6e9aa8 {
    u8 pad_0x0[0x38];
    UnkXform transform;  // 0x38
    u8 pad_0x94[0x11e];
    bool field_0x1b2;
    u8 pad_0x1b3[0x75];
    UnkEntity* field_0x228;
    u8 pad_0x230[0x10];
    UnkXParasiteModel_l6e9aa8* field_0x240;
    u8 pad_0x248[0x168];
    u8 moved;  // 0x3b0: set when the transform changed
};
static_assert(__builtin_offsetof(UnkXParasiteBody_l6e9aa8, field_0x228) == 0x228, "layout");
static_assert(__builtin_offsetof(UnkXParasiteBody_l6e9aa8, moved) == 0x3b0, "layout");

// The X parasite AI component (game/UnkXParasiteAI.h), with the timer at +0x146fc as the float it is.
struct UnkXParasiteAI_l6e9aa8 {
    u8 pad_0x0[0x30];
    UnkXParasiteBody_l6e9aa8* owner;  // 0x30
    u8 pad_0x38[0x146e4 - 0x38];
    s32 field_0x146e4;
    u8 pad_0x146e8[8];
    u64 field_0x146f0;
    u32 field_0x146f8;
    f32 field_0x146fc;
    u8 pad_0x14700[0xc];
    f32 field_0x1470c;
    u32 field_0x14710;
    f32 field_0x14714;
    f32 field_0x14718;
    f32 field_0x1471c;
    bool field_0x14720;
    u8 pad_0x14721[0x14790 - 0x14721];
    UnkWeakHandle* link;           // 0x14790: the grabbed object's weak handle
    UnkWeakHandle* field_0x14798;  // the prey's weak handle
};
static_assert(__builtin_offsetof(UnkXParasiteAI_l6e9aa8, field_0x146fc) == 0x146fc, "layout");
static_assert(__builtin_offsetof(UnkXParasiteAI_l6e9aa8, link) == 0x14790, "layout");

static inline UnkXParasiteBody_l6e9aa8* Prey(UnkXParasiteAI_l6e9aa8* self) {
    return reinterpret_cast<UnkXParasiteBody_l6e9aa8*>(self->field_0x14798->target);
}

static inline UnkXParasiteBody_l6e9aa8* Grabbed(UnkXParasiteAI_l6e9aa8* self) {
    return reinterpret_cast<UnkXParasiteBody_l6e9aa8*>(self->link->target);
}

static inline f32 Clamp(f32 v, f32 min, f32 max) {
    f32 r = v >= max ? max : v;
    if (v <= min)
        return min;
    return r;
}

static inline UnkVec3f Position(const UnkXParasiteBody_l6e9aa8* body) { return body->transform.GetTranslation(); }

static inline void SetPosition(UnkXParasiteBody_l6e9aa8* body, const UnkVec3f& pos) {
    body->transform.SetTranslation(pos);
    body->moved = 1;
}

// From `from` toward `to` by `t` (all the way when it reaches 1).
static inline UnkVec3f Lerp(const UnkVec3f& from, const UnkVec3f& to, f32 t) {
    if (t >= 1.0f)
        return to;
    f32 rest = 1.0f - t;
    return UnkVec3f(t * to.x + rest * from.x, t * to.y + rest * from.y, t * to.z + rest * from.z);
}

// The model's attachment matrix, looked up by the attachment's name.
static inline bool FindAttachment(UnkEntity* entity, UnkXParasiteModel_l6e9aa8* model, UnkMatrix44* out) {
    u64 key = fn_710110f224(model)->data->Key();
    return fn_71011735e8(entity, &key, out, 0);
}

static inline bool Moving(UnkXParasiteAI_l6e9aa8* s) {
    return s->field_0x14714 - 0.001f > 0.0f || s->field_0x14714 + 0.001f < 0.0f || s->field_0x14718 - 0.001f > 0.0f ||
           s->field_0x14718 + 0.001f < 0.0f || s->field_0x1471c - 0.001f > 0.0f || s->field_0x1471c + 0.001f < 0.0f;
}

static inline bool Stopped(UnkXParasiteAI_l6e9aa8* s) {
    return s->field_0x14714 - 0.001f <= 0.0f && s->field_0x14714 + 0.001f >= 0.0f &&
           s->field_0x14718 - 0.001f <= 0.0f && s->field_0x14718 + 0.001f >= 0.0f &&
           s->field_0x1471c - 0.001f <= 0.0f && s->field_0x1471c + 0.001f >= 0.0f;
}

void fn_71006e9aa8(UnkXParasiteAI_l6e9aa8* self) {
    if (!self->field_0x14798 || !Prey(self))
        return;
    UnkXParasiteBody_l6e9aa8* prey = Prey(self);
    f32 blend = Clamp(self->field_0x146fc / 0.2f, 0.0f, 1.0f);

    if (self->field_0x14720 && self->link && Grabbed(self)) {
        UnkXParasiteBody_l6e9aa8* grabbed = Grabbed(self);
        if (Moving(self)) {
            UnkVec3f target(self->field_0x14714, self->field_0x14718, self->field_0x1471c);
            SetPosition(prey, Lerp(target, Position(grabbed), blend));
        }
        if (Stopped(self) && Grabbed(self)->field_0x1b2) {
            UnkVec3f held = Position(Prey(self));
            UnkVec3f to = Position(Grabbed(self));
            f32 dx = held.x - to.x;
            f32 dy = held.y - to.y;
            f32 dz = held.z - to.z;
            if (dx * dx + dy * dy + dz * dz <= 62500.0f) {
                self->field_0x146e4 = 4;
                self->field_0x14714 = held.x;
                self->field_0x14718 = held.y;
                self->field_0x1471c = held.z;
                self->field_0x146fc = 0.0f;
                self->field_0x1470c = 0.7f;
            }
        }
    }

    UnkXParasiteBody_l6e9aa8* body = Prey(self);
    UnkVec3f pos = Position(body);
    if (UnkXParasiteModel_l6e9aa8* model = body->field_0x240) {
        UnkEntity* entity = body->field_0x228;
        const CStrId* name = fn_710110f224(model);
        if (entity && name->data != &g_unk_7101d47108->empty) {
            UnkMatrix44 attachment = {};
            attachment.m[15] = 1.0f;
            attachment.m[10] = 1.0f;
            attachment.m[5] = 1.0f;
            attachment.m[0] = 1.0f;
            if (FindAttachment(entity, model, &attachment)) {
                UnkVec3f origin(attachment.m[3], attachment.m[7], attachment.m[11]);
                UnkMatrix44Words rotation = fn_7100076dc4(reinterpret_cast<const UnkMatrix44Words*>(&attachment));
                UnkVec3f offset = fn_7100078170(reinterpret_cast<const UnkMat4f*>(&rotation), fn_710110f230(model));
                pos = UnkVec3f(origin.x + offset.x, origin.y + offset.y, origin.z + offset.z);
            }
        }
    }

    UnkXParasiteBody_l6e9aa8* owner = self->owner;
    SetPosition(owner, Lerp(Position(owner), pos, blend));
    self->field_0x146f0 = 0;
    self->field_0x146f8 = 0;
}