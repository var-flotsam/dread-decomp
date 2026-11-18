// CMorphBallMovement's unit: the ball's movement step. Runs the base motion and the step functions
// on the step state at +0x280, resets the link at +0x24d0 unless vf_0x178 holds, lets vf_0x190
// adjust the end position, then turns the ball's bone control to the angles of its transform,
// steering towards the target at +0x84 (halfway past it while fn_710033e288 holds). Names and
// layouts are placeholders.
#include <math.h>

#include "base/UnkMatrix44.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkAssignedSlot.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2553.h"
#include "game/UnkTU399.h"
#include "game/UnkTU54.h"
#include "game/UnkVec3fOps.h"
#include "game/UnkXformNode.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

struct UnkPlayer;
struct UnkMovement3_ovb0048;
struct UnkLegNode;
struct UnkWaterEntry_b57;

void fn_7100976344(char* p0);
void fn_71010d1e88(UnkMovement3_ovb0048* self, void* a, void* b, f32 t, void* c, void* d, void* e);
void fn_71010db084(void* unused, void* arg);
void fn_71002d60c4(UnkLegNode* self, f32 weight);
bool fn_710033e288(UnkAbility* ability);
extern __attribute__((visibility("hidden"))) char g_unk_71727394b0[];

struct UnkWaterLevel_b57 {
    u8 pad_0x0[0x1c];
    f32 field_0x1c;
    u8 pad_0x20[0x8];
    u64 field_0x28;
    u8 pad_0x30[0x10];
};
UnkWaterLevel_b57 fn_71002e2958(UnkWaterEntry_b57* entry);

struct UnkBallBone_293c {
    void** vtable;
    const base::reflection::CClass* vf_0x0() {
        return reinterpret_cast<const base::reflection::CClass* (*)(UnkBallBone_293c*)>(vtable[0])(this);
    }
};
void fn_71002e28d0(UnkBallBone_293c* self, const UnkMatrix44* m);

struct UnkBallComponent_293c {
    void** vtable;
    u8 pad_0x8[0x144 - 0x8];
    f32 field_0x144;

    const base::reflection::CClass* vf_0x0() {
        return reinterpret_cast<const base::reflection::CClass* (*)(UnkBallComponent_293c*)>(vtable[0])(this);
    }
};

struct UnkBallAnimation_293c {
    u8 pad_0x0[0x40];
    void* bones;  // 0x40
};

struct UnkBallOwner_293c {
    u8 pad_0x0[0x28];
    UnkBallComponent_293c* component;  // 0x28
    u8 pad_0x30[0x220 - 0x30];
    UnkBallAnimation_293c* animation;  // 0x220
};

// The step's transforms: the one it starts from (+0x4) and the one it writes (+0xb8).
struct UnkMorphBallStep_293c {
    u32 field_0x0;
    UnkXform from;  // 0x4
    UnkVec3f field_0x60;
    UnkVec3f field_0x6c;
    u8 field_0x78[0xb8 - 0x78];
    UnkXform to;  // 0xb8
};
static_assert(__builtin_offsetof(UnkMorphBallStep_293c, to) == 0xb8, "layout");

static inline void ReleaseRef_293c(void* p) {
    UnkRefObject* object = static_cast<UnkRefObject*>(p);
    if (object && --object->ref_count == 0)
        object->vf_0x10();
}

// Two counted references, an id and its data.
struct UnkBallLink_293c {
    UnkAssignedSlot first;
    UnkAssignedSlot second;
    s32 id;
    u32 data[12];

    UnkBallLink_293c() : id(-1), data() {
        first.ptr = nullptr;
        second.ptr = nullptr;
    }
    ~UnkBallLink_293c() {
        ReleaseRef_293c(second.ptr);
        ReleaseRef_293c(first.ptr);
    }
    UnkBallLink_293c& operator=(const UnkBallLink_293c& other) {
        if (&other != this) {
            first = other.first;
            second = other.second;
            id = other.id;
            for (u32 i = 0; i < 12; i++)
                data[i] = other.data[i];
        }
        return *this;
    }
};
static_assert(sizeof(UnkBallLink_293c) == 0x48, "");

struct UnkMorphBallMovement_293c {
    void** vtable;
    u8 pad_0x8[0x30 - 0x8];
    UnkBallOwner_293c* owner;  // 0x30
    u8 pad_0x38[0x84 - 0x38];
    UnkVec3f target;  // 0x84
    u8 pad_0x90[0x1d8 - 0x90];
    UnkVec3f velocity;  // 0x1d8
    u8 pad_0x1e4[0x208 - 0x1e4];
    UnkVec3f field_0x208;  // 0x208
    u8 pad_0x214[0x280 - 0x214];
    UnkMorphBallStep_293c step;  // 0x280
    u8 pad_0x394[0x2288 - 0x394];
    s32 mode;  // 0x2288
    u8 pad_0x228c[0x24a4 - 0x228c];
    f32 field_0x24a4;
    u8 pad_0x24a8[0x24d0 - 0x24a8];
    UnkBallLink_293c link;  // 0x24d0
    bool field_0x2518;
    u8 pad_0x2519[0x265c - 0x2519];
    f32 field_0x265c;

    bool vf_0x178() { return reinterpret_cast<bool (*)(UnkMorphBallMovement_293c*)>(vtable[0x178 / 8])(this); }
    void vf_0x190(UnkVec3f* position) {
        reinterpret_cast<void (*)(UnkMorphBallMovement_293c*, UnkVec3f*)>(vtable[0x190 / 8])(this, position);
    }
    void vf_0x208(UnkVec3f* v) {
        reinterpret_cast<void (*)(UnkMorphBallMovement_293c*, UnkVec3f*)>(vtable[0x208 / 8])(this, v);
    }
    void vf_0x2f8(UnkMorphBallStep_293c* step, f32 dt) {
        reinterpret_cast<void (*)(UnkMorphBallMovement_293c*, UnkMorphBallStep_293c*, f32)>(vtable[0x2f8 / 8])(
            this, step, dt);
    }
};
static_assert(__builtin_offsetof(UnkMorphBallMovement_293c, step) == 0x280, "layout");
static_assert(__builtin_offsetof(UnkMorphBallMovement_293c, field_0x2518) == 0x2518, "layout");
static_assert(__builtin_offsetof(UnkMorphBallMovement_293c, field_0x265c) == 0x265c, "layout");

void fn_71010daf1c(UnkMorphBallMovement_293c* self, UnkMorphBallStep_293c* step, f32 dt, bool flag);
void fn_71010db138(UnkMorphBallMovement_293c* self, UnkMorphBallStep_293c* step, f32 dt);
void fn_7100978c24(UnkMorphBallStep_293c* step, UnkVec3f* velocity, UnkVec3f* v, f32 k);
void fn_71010d8fcc(UnkMorphBallMovement_293c* self, UnkMorphBallStep_293c* step);
void fn_7100942e14(UnkMorphBallMovement_293c* self, UnkVec3f target, UnkVec3f through, const UnkVec3f* angles,
                   f32 k);

// The owner's component at +0x28 when it is a CCharClass, else null.
inline UnkBallComponent_293c* Character(UnkMorphBallMovement_293c* self) {
    UnkBallComponent_293c* component = self->owner->component;
    return fn_710009d50c(component->vf_0x0(), fn_71011abbd4()) ? component : nullptr;
}

// The owner's bone control (a CBoneControl registered under g_unk_71727394b0), or null.
inline UnkBallBone_293c* FindBone(UnkMorphBallMovement_293c* self) {
    UnkBallBone_293c* bone =
        reinterpret_cast<UnkBallBone_293c*>(fn_71002d57ac(self->owner->animation->bones, g_unk_71727394b0));
    return bone && fn_710009d50c(bone->vf_0x0(), fn_71002e2544()) ? bone : nullptr;
}

// The rotation angles of the bone control's frame.
inline UnkVec3f BoneAngles(UnkBallBone_293c* bone) {
    UnkWaterLevel_b57 frame = fn_71002e2958(reinterpret_cast<UnkWaterEntry_b57*>(bone));
    return fn_7100076acc(reinterpret_cast<const UnkXform*>(&frame));
}

void fn_710094293c(UnkMorphBallMovement_293c* self, f32 dt) {
    fn_7100976344(reinterpret_cast<char*>(self));
    UnkVec3f motion(0.0f, 0.0f, 0.0f);
    UnkMorphBallStep_293c* step = &self->step;
    fn_71010d1e88(reinterpret_cast<UnkMovement3_ovb0048*>(self), &step->to, &step->field_0x60, dt,
                  &step->field_0x6c, step->field_0x78, &motion);
    self->vf_0x208(&motion);

    UnkVec3f from = step->from.GetTranslation();
    UnkVec3f velocity = self->velocity;
    if (self->mode != 3)
        fn_71010daf1c(self, step, dt, true);
    fn_71010db084(self, step);
    fn_71010db138(self, step, dt);
    fn_7100978c24(step, &self->velocity, &self->field_0x208, Character(self)->field_0x144);
    if (!self->vf_0x178()) {
        self->field_0x2518 = false;
        self->link = UnkBallLink_293c();
        self->vf_0x2f8(step, dt);
    }
    UnkVec3f moved = velocity * dt;
    fn_71010d8fcc(self, step);

    UnkVec3f position = step->to.GetTranslation();
    self->vf_0x190(&position);
    step->to.SetTranslation(position);

    UnkBallBone_293c* bone = FindBone(self);
    UnkVec3f predicted = from + moved;
    UnkVec3f angles = BoneAngles(bone);

    if (fn_710033e288(fn_7100df5928(reinterpret_cast<UnkPlayer*>(g_unk_7101d47310)))) {
        UnkVec3f through = self->target + (self->target - predicted) * 0.5f;
        fn_7100942e14(self, self->target, through, &angles, self->field_0x265c);
    } else {
        const UnkXform& end = step->to;
        bool settled = fabsf(end.m[0][3] - self->target.x) < 0.1f && fabsf(end.m[1][3] - self->target.y) < 0.1f &&
                       fabsf(end.m[2][3] - self->target.z) < 0.1f && !(fabsf(self->field_0x24a4) >= 0.1f);
        if (!settled)
            fn_7100942e14(self, self->target, predicted, &angles, self->field_0x265c);
    }

    if (!fn_7100df5a4c(g_unk_7101d47310)) {
        UnkBallBone_293c* control = FindBone(self);
        UnkMatrix44 rotation = fn_7100078fc8(reinterpret_cast<const UnkMatrix44&>(angles));
        fn_71002e28d0(control, &rotation);
        fn_71002d60c4(reinterpret_cast<UnkLegNode*>(control), 1.0f);
    }
}
