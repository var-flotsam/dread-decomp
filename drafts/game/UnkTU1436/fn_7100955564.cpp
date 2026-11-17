// Unit 1436: picks the action an owner plays from its target's facing, falling back to named
// animations when the target gives none. Names and layouts are placeholders.
#include "base/UnkMatrix44.h"
#include "base/global/CStrId.h"
#include "game/CAnimationComponent.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkXformNode.h"
#include "types.h"

using base::global::UnkStringData;

// What the target's queries fill in: the action id (-1 for none) and its parameters.
struct UnkTU1436ActionParams {
    u64 field_0x0;
    u64 field_0x8;
    u64 field_0x10;
};

// An action id (-1 for none), copied on its own.
struct UnkTU1436ActionId {
    u64 value;
    UnkTU1436ActionId& operator=(const UnkTU1436ActionId& o) {
        value = o.value;
        return *this;
    }
};

struct UnkTU1436Action {
    UnkTU1436ActionId id;
    UnkTU1436ActionParams field_0x8;
    UnkVec3f field_0x20;
};

struct UnkTU1436Target {
    u8 pad_0x0[0x40];
    f32 field_0x40;
    f32 field_0x44;
    f32 field_0x48;
    f32 field_0x4c;
    f32 field_0x50;
    f32 field_0x54;
    u8 pad_0x58[0x24];
    f32 field_0x7c;
};

struct UnkTU1436Component {
    u8 pad_0x0[0x1610];
    void* field_0x1610;
};

struct UnkTU1436Entity {
    u8 pad_0x0[0x38];
    UnkMatrix44 field_0x38;  // the transform; fn_710002614c returns its matrix
    u8 pad_0x78[0x1a0];
    UnkTU1436Target* field_0x218;
    UnkAnimationComponent* field_0x220;
    u8 pad_0x228[0x18];
    UnkTU1436Component* field_0x240;
};

struct UnkTU1436Defaults {
    u8 pad_0x0[0x148];
    UnkStringData* field_0x148;
};

struct UnkTU1436 {
    u8 pad_0x0[0x30];
    UnkTU1436Entity* field_0x30;
    u8 pad_0x38[0x8];
    UnkTU1436Defaults* field_0x40;
    UnkStringData* field_0x48;
    UnkStringData* field_0x50;
};

UnkTU1436Action fn_71009767f4(UnkTU1436Target* target, bool flag);
UnkTU1436Action fn_7100976944(UnkTU1436Target* target, bool flag);

static inline void FillDefault(UnkTU1436* self, UnkTU1436Action* out) {
    if (out->id.value == static_cast<u64>(-1))
        out->id.value = self->field_0x40->field_0x148->Key();
}

void fn_7100955564(UnkTU1436* self, s32* state, UnkTU1436Action* out) {
    UnkTU1436Entity* entity = self->field_0x30;
    if (entity) {
        UnkTU1436Component* component = entity->field_0x240;
        if (component) {
            UnkTU1436Target* target = entity->field_0x218;
            UnkAnimationComponent* animation = entity->field_0x220;
            void* ready = component->field_0x1610;
            if (target && animation && ready) {
                s32 next;
                if (target->field_0x7c == 0.0f) {
                    next = 1;
                    *out = fn_71009767f4(target, true);
                    FillDefault(self, out);
                } else {
                    f32 bx = target->field_0x4c;
                    f32 by = target->field_0x50;
                    f32 bz = target->field_0x54;
                    f32 ax = target->field_0x40;
                    f32 ay = target->field_0x44;
                    f32 az = target->field_0x48;
                    const UnkMatrix44& m = fn_710002614c(&entity->field_0x38);
                    // Facing away from the entity, or off its plane.
                    if (bx * ax + by * ay + bz * az < 0.0f ||
                        ax * m.m[2] + ay * m.m[6] + az * m.m[10] != 0.0f) {
                        *out = fn_7100976944(target, true);
                        u64 id = out->id.value;
                        if (id == static_cast<u64>(-1) && *state != 2 &&
                            self->field_0x50 != &g_unk_7101d47108->empty &&
                            fn_7101176d34(reinterpret_cast<UnkStruct_7101176ea8*>(animation),
                                          reinterpret_cast<void*>(self->field_0x50->Key())) &&
                            !fn_710117ae38(animation, self->field_0x48->Key(), false)) {
                            out->id.value = self->field_0x50->Key();
                        } else if (id == static_cast<u64>(-1)) {
                            if (self->field_0x50 != &g_unk_7101d47108->empty &&
                                fn_710117ae38(animation, self->field_0x50->Key(), false))
                                out->id.value = self->field_0x50->Key();
                            else
                                out->id.value = self->field_0x48->Key();
                        }
                        next = 2;
                    } else {
                        next = 1;
                        *out = fn_71009767f4(target, true);
                        FillDefault(self, out);
                    }
                }
                *state = next;
            }
        }
    }
    FillDefault(self, out);
}
