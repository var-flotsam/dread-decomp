// behaviortree::CInPathPatrolAction's unit (tools/tu_map.py, unit 488): updates the node. While the patrol
// waits for the body to reach its target it does nothing; otherwise it reads the AI's blackboard flags,
// decides from the AI's mode (+0x1610, +0xec) whether the patrol is in range of its goal (state 2) or too
// far (state 3), checks the distance to the stored point (+0x58, radius +0x64) and the owner's path, and
// runs the state step (fn_710038b6ec) and the wait (fn_710038c8c0). Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "base/reflection/ReflectedAttributes.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CAbilityShinespark.h"
#include "game/CBlackboard.h"
#include "game/CEnemyMovement.h"
#include "game/UnkGameManagerIdTable.h"
#include "game/UnkKraidCreatureVec.h"
#include "game/UnkTU2702.h"
#include "game/UnkTypedValue.h"
#include "game/behaviortree/CBehavior.h"
#include "types.h"

using base::global::CStrId;

struct UnkBigFistAI3_ovb0050;
struct UnkBigFistKind_ovb0050;
struct UnkBigFistLimit_ovb0050;

struct UnkPatrolBody_l38bca4 {
    void** vtable;
    u8 pad_0x8[0x68];
    f32 target[3];  // 0x70
    UnkKraidCreatureVec vf_0x370() {
        UnkKraidCreatureVec v = reinterpret_cast<UnkKraidCreatureVec (*)(UnkPatrolBody_l38bca4*)>(vtable[0x370 / 8])(this);
        return v;
    }
};

struct UnkPatrolMode_l38bca4 {
    u8 pad_0x0[0xec];
    s32 kind;  // 0xec
};

struct UnkPatrolAI_l38bca4 {
    u8 pad_0x0[0x1138];
    f32 field_0x1138;
    u8 pad_0x113c[0x121e - 0x113c];
    bool field_0x121e;
    u8 pad_0x121f[0x1260 - 0x121f];
    CBlackboard blackboard;  // 0x1260
    u8 pad_0x1350[0x1610 - 0x1350];
    UnkPatrolMode_l38bca4* mode;  // 0x1610
    u8 pad_0x1618[0x14689 - 0x1618];
    bool field_0x14689;
};
static_assert(__builtin_offsetof(UnkPatrolAI_l38bca4, mode) == 0x1610, "layout");

struct UnkPatrolPathKind_l38bca4 {
    u8 pad_0x0[0x10];
    s32 kind;  // 0x10
};
struct UnkPatrolPathEntry_l38bca4 {
    u8 pad_0x0[0x10];
    UnkPatrolPathKind_l38bca4* kind;  // 0x10
};
struct UnkPatrolPathRecord_l38bca4 {
    UnkPatrolPathEntry_l38bca4* entry;
};
struct UnkPatrolNav_l38bca4 {
    u8 pad_0x0[0xb0];
    UnkPatrolPathRecord_l38bca4* record;  // 0xb0
};

struct UnkPatrolMovement_l38bca4 {
    u8 pad_0x0[0x971];
    bool field_0x971;
    u8 pad_0x972[2];
    f32 x;  // 0x974
    f32 y;  // 0x978
};

struct UnkPatrolOwner_l38bca4 {
    u8 pad_0x0[0x44];
    f32 x;  // 0x44
    u8 pad_0x48[0xc];
    f32 y;  // 0x54
    u8 pad_0x58[0xc];
    f32 z;  // 0x64
    u8 pad_0x68[0x1f0 - 0x68];
    base::reflection::UnkReflObject* movement;  // 0x1f0
    u8 pad_0x1f8[0x20];
    UnkPatrolBody_l38bca4* body;  // 0x218
    u8 pad_0x220[0x78];
    UnkPatrolNav_l38bca4* navigation;  // 0x298
};

struct UnkPatrol_l38bca4 {
    u8 pad_0x0[0x18];
    UnkPatrolOwner_l38bca4* owner;  // 0x18
    u8 pad_0x20[0x10];
    UnkPatrolAI_l38bca4* ai;  // 0x30
    u8 pad_0x38[0x18];
    bool field_0x50;
    u8 pad_0x51[3];
    s32 state;  // 0x54
    f32 point[3];  // 0x58
    f32 radius;    // 0x64
    u8 pad_0x68[0x31];
    bool field_0x99;
    u8 pad_0x9a[0xb];
    bool field_0xa5;
    u8 pad_0xa6[0x22];
    bool field_0xc8;
    bool field_0xc9;
};
static_assert(__builtin_offsetof(UnkPatrol_l38bca4, field_0xc9) == 0xc9, "layout");

bool fn_710038c28c(UnkPatrol_l38bca4* self);
bool fn_710038c4c8(UnkPatrol_l38bca4* self);
void fn_710038b6ec(UnkPatrol_l38bca4* self, s32* state);
void fn_710038c8c0(UnkPatrol_l38bca4* self, f32 dt);

inline UnkGameManagerIdTable& IdTable_l38bca4() {
    static UnkGameManagerIdTable s_table asm("g_unk_7101d552d0");
    return s_table;
}

static inline bool ReadFlag_l38bca4(CBlackboard* blackboard, const CStrId& section, const CStrId& name, bool* out) {
    if (CBlackboard::CSection* s = fn_7101047270(blackboard, section))
        return fn_710006d11c(s, &name, out);
    return false;
}

static inline void SetFlag_l38bca4(CBlackboard* blackboard, const CStrId& section, const CStrId& name, bool* value) {
    if (CBlackboard::CSection* s = fn_71010472e0(blackboard, section)) {
        if (!blackboard->field_0xe8) {
            UnkTypedValue v;
            v.type = fn_710000d228();
            v.value = value;
            fn_7100332c80(s, reinterpret_cast<UnkPropertyName*>(const_cast<CStrId*>(&name)), &v);
        }
    }
}

static inline bool Outside_l38bca4(f32 a, f32 b) {
    f32 low = a - 0.1f;
    f32 high = a + 0.1f;
    return !(high >= b) || low > b;
}

u32 fn_710038bca4(UnkPatrol_l38bca4* self, f32 dt) {
    if (self->field_0xc8 && self->state == 2) {
        UnkKraidCreatureVec v = self->owner->body->vf_0x370();
        const UnkPatrolBody_l38bca4* t = self->owner->body;
        if (Outside_l38bca4(v.x, t->target[0]) || Outside_l38bca4(v.y, t->target[1]) || Outside_l38bca4(v.z, t->target[2]))
            return 2;
    }
    UnkPatrolAI_l38bca4* ai = self->ai;
    if (!ai)
        return 6;
    if (self->field_0xa5) {
        if (fn_710038c28c(self)) {
            self->field_0xa5 = false;
            self->field_0x50 = false;
            self->state = 2;
            goto step;
        }
        goto check;
    }
    {
        bool a = false;
        bool b = false;
        bool ready;
        if (ReadFlag_l38bca4(&ai->blackboard, g_unk_71726b7cb8, IdTable_l38bca4().field_0x8e8, &a) && a && !self->field_0xc9) {
            ready = true;
        } else {
            b = false;
            if (ReadFlag_l38bca4(&ai->blackboard, g_unk_71726b7cb8, IdTable_l38bca4().field_0x8f0, &b) && b) {
                bool off = false;
                SetFlag_l38bca4(&ai->blackboard, g_unk_71726b7cb8, IdTable_l38bca4().field_0x8f0, &off);
                ready = true;
            } else if (self->ai->field_0x121e && self->field_0x99) {
                ready = fn_710038c4c8(self) && !self->field_0xc9;
            } else {
                ready = false;
            }
        }
        bool pending = self->ai->field_0x14689;
        if (pending)
            self->ai->field_0x14689 = false;
        if (pending || ready) {
            s32 kind = self->ai->mode->kind;
            if (kind == 2) {
                u32 mode = 2;
                f32 min[3] = {0, 0, 0};
                f32 max[3] = {0, 0, 0};
                if (!fn_71010fc864(reinterpret_cast<UnkBigFistAI3_ovb0050*>(self->ai),
                                   reinterpret_cast<UnkBigFistKind_ovb0050*>(&mode),
                                   reinterpret_cast<UnkBigFistLimit_ovb0050*>(min),
                                   reinterpret_cast<UnkBigFistLimit_ovb0050*>(max)))
                    goto check;
                base::reflection::UnkReflObject* movement = self->owner->movement;
                if (movement && !fn_710009d50c(movement->vf_0x0(), fn_71010c74e0()))
                    movement = nullptr;
                if (fn_71010cc3a4(movement) != 1)
                    goto check;
                UnkPatrolMovement_l38bca4* m = reinterpret_cast<UnkPatrolMovement_l38bca4*>(movement);
                f32* lim = m->field_0x971 ? min : max;
                f32 dy = m->y - lim[1];
                f32 dx = m->x - lim[0];
                f32 dz = 0.0f - lim[2];
                self->field_0x50 = false;
                if (dx * dx + dy * dy + dz * dz > 22500.0f) {
                    self->state = 3;
                } else {
                    self->state = 2;
                    if (!self->field_0x99)
                        goto check;
                    self->field_0xa5 = true;
                }
            } else if (kind == 1) {
                f32 f = -1.0f;
                if (!fn_71010fc778(self->ai, &f))
                    goto check;
                if (!(f > -0.001f))
                    goto check;
                self->field_0x50 = false;
                self->state = f > 150.0f ? 3 : 2;
            } else {
                self->field_0x50 = false;
            }
        }
    }
check:
    if (!self->field_0x50)
        goto step;
    {
        UnkPatrolOwner_l38bca4* owner = self->owner;
        f32 dx = self->point[0] - owner->x;
        f32 dy = self->point[1] - owner->y;
        f32 dz = self->point[2] - owner->z;
        if (dx * dx + dy * dy + dz * dz < self->radius * self->radius) {
            self->field_0x50 = false;
            self->state = 1;
            fn_710038b6ec(self, &self->state);
            if (!self->field_0x50)
                goto step;
        }
        UnkPatrolOwner_l38bca4* o = self->owner;
        if (o && o->navigation && o->navigation->record && o->navigation->record->entry &&
            o->navigation->record->entry->kind->kind == 5) {
            self->field_0x50 = false;
        } else if (self->ai->field_0x1138 < 0.1f) {
            self->field_0x50 = false;
        } else {
            goto done;
        }
    }
step:
    fn_710038b6ec(self, &self->state);
done:
    fn_710038c8c0(self, dt);
    return 2;
}
