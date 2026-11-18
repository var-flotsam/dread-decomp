// CChozoWarriorXLandAttack's unit: the landing's shockwaves. When the owner's AI (+0x240) is a
// CChozoWarriorXAIComponent and the owner has the component at +0x258, builds a shock wave
// description (fn_71009f35b0) from the owner and the attack's tunables, spawns it once toward each side
// (fn_71009f51f8) and hands both waves to the AI (fn_710048f9d4). Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "base/global/CRntVector.h"
#include "game/CAutsharpAIComponent.h"
#include "game/CCharClassAttackComponent.h"
#include "game/CShockWaveComponent.h"
#include "game/UnkTU1370.h"
#include "game/UnkTU2523.h"
#include "game/UnkXformNode.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::global::CStrId;
using base::reflection::UnkReflObject;

struct UnkTunable;
extern UnkTunable g_unk_71726ce200;
extern CStrId g_unk_71726cf060 __attribute__((visibility("hidden")));
extern CStrId g_unk_71726cf068 __attribute__((visibility("hidden")));

// CTunableCharClassChozoWarriorXLandAttack as this function reads it.
struct UnkLandTunables_l482aec {
    u8 pad_0x0[0x2c];
    f32 wave_speed;  // 0x2c
    f32 wave_size;   // 0x30
    f32 wave_time;   // 0x34
};

static inline const UnkLandTunables_l482aec& LandTunables() {
    return reinterpret_cast<const UnkLandTunables_l482aec&>(g_unk_71726ce200);
}

// The shock wave description fn_71009f35b0 builds and fn_71003cc474 destroys (0x210 bytes).
struct UnkLandWave_l482aec {
    u8 pad_0x0[0x8];
    u8 request[0xb8];  // 0x8
    CStrId effect;     // 0xc0
    u8 pad_0xc8[0x44];
    s32 field_0x10c;
    bool field_0x110;
    u8 pad_0x111[0x3];
    UnkVec3f position;  // 0x114
    bool field_0x120;
    u8 pad_0x121[0x3];
    f32 size;  // 0x124
    u8 pad_0x128[0x8];
    f32 field_0x130;
    f32 field_0x134;
    bool field_0x138;
    u8 pad_0x139[0x4f];
    UnkWeakHandle* owner;  // 0x188
    u8 pad_0x190[0x20];
    CStrId field_0x1b0;
    u8 pad_0x1b8[0x10];
    f32 speed;      // 0x1c8
    f32 direction;  // 0x1cc
    f32 left_x;     // 0x1d0
    f32 right_x;    // 0x1d4
    u8 pad_0x1d8[0x24];
    bool field_0x1fc;
    u8 pad_0x1fd[0x3];
    f32 time;  // 0x200
    u8 pad_0x204[0xc];
};
static_assert(sizeof(UnkLandWave_l482aec) == 0x210, "UnkLandWave_l482aec layout");
static_assert(__builtin_offsetof(UnkLandWave_l482aec, owner) == 0x188, "UnkLandWave_l482aec layout");
static_assert(__builtin_offsetof(UnkLandWave_l482aec, time) == 0x200, "UnkLandWave_l482aec layout");

struct UnkChozoWarriorAI_b66a2;
f32 fn_710049b2fc(UnkChozoWarriorAI_b66a2* self);
f32 fn_710049b360(UnkChozoWarriorAI_b66a2* self);

// A counted reference to a weak handle.
struct UnkHandleRef_l482aec {
    UnkWeakHandle* handle;

    explicit UnkHandleRef_l482aec(UnkWeakHandle* h) : handle(nullptr) { AssignRef(&handle, h); }
    ~UnkHandleRef_l482aec() {
        if (handle) {
            if (--handle->ref_count == 0)
                handle->vf_0x10();
        }
    }
};

struct UnkWarriorXAI_l482aec {
    void** vtable;

    // The effect of the landing's waves (vtable slot 0x648), or `fallback`.
    CStrId WaveEffect(const CStrId& fallback) {
        return reinterpret_cast<CStrId (*)(UnkWarriorXAI_l482aec*, const CStrId&)>(vtable[0x648 / 8])(this,
                                                                                                      fallback);
    }
};

// The owner as this function reads it: a weak-handle target with its transform at +0x38.
struct UnkLandOwner_l482aec : UnkWeakTarget {
    u8 pad_0x8[0x30];
    UnkXform xform;  // 0x38
    u8 pad_0x94[0x1ac];
    UnkReflObject* ai;  // 0x240
    u8 pad_0x248[0x10];
    void* field_0x258;
};
static_assert(__builtin_offsetof(UnkLandOwner_l482aec, ai) == 0x240, "UnkLandOwner_l482aec layout");

UnkWeakTarget* fn_71009f51f8(UnkLandWave_l482aec* wave, UnkLandOwner_l482aec* owner);
void fn_710048f9d4(UnkWarriorXAI_l482aec* ai, UnkHandleRef_l482aec* left, UnkHandleRef_l482aec* right, f32 t);

struct UnkLandAttack_l482aec {
    u8 pad_0x0[0x8];
    UnkLandOwner_l482aec* owner;  // 0x8
};

static inline bool IsEmpty(const CStrId& id) {
    return &g_unk_7101d47108->empty == id.data;
}

void fn_7100482aec(UnkLandAttack_l482aec* self) {
    UnkReflObject* object = self->owner->ai;
    if (!object)
        return;
    UnkWarriorXAI_l482aec* ai = fn_710009d50c(object->vf_0x0(), fn_710048db44())
                                    ? reinterpret_cast<UnkWarriorXAI_l482aec*>(object)
                                    : nullptr;
    if (!ai)
        return;
    if (!self->owner->field_0x258)
        return;

    f32 left_x = fn_710049b2fc(reinterpret_cast<UnkChozoWarriorAI_b66a2*>(ai));
    f32 right_x = fn_710049b360(reinterpret_cast<UnkChozoWarriorAI_b66a2*>(ai));
    alignas(8) char storage[sizeof(UnkLandWave_l482aec)];
    fn_71009f35b0(storage);
    UnkLandWave_l482aec* wave = reinterpret_cast<UnkLandWave_l482aec*>(storage);
    AssignRef(&wave->owner, GetWeakHandle(self->owner));
    wave->field_0x1b0 = g_unk_71726cf060;
    wave->position = self->owner->xform.GetTranslation();
    wave->field_0x120 = true;
    wave->size = LandTunables().wave_size;
    wave->field_0x130 = 0.1f;
    wave->field_0x134 = -1.0f;
    wave->field_0x10c = 1;
    {
        CStrId effect = ai->WaveEffect(g_unk_71726cf068);
        UnkRequest* request = reinterpret_cast<UnkRequest*>(wave->request);
        wave->effect = IsEmpty(effect) ? g_unk_71726cf068 : effect;
        fn_71008c1538(request, fn_7100789f5c(&wave->effect));
        wave->time = LandTunables().wave_time;
        wave->speed = LandTunables().wave_speed;
        wave->field_0x138 = true;
        wave->field_0x1fc = true;
        wave->left_x = left_x;
        wave->field_0x110 = true;
        wave->direction = 1.0f;
        UnkWeakTarget* left = fn_71009f51f8(wave, self->owner);
        wave->direction = -1.0f;
        wave->right_x = right_x;
        UnkWeakTarget* right = fn_71009f51f8(wave, self->owner);
        f32 t = (left_x - self->owner->xform.m[0][3]) / LandTunables().wave_speed;
        UnkHandleRef_l482aec left_ref(fn_7100d78378(left));
        UnkHandleRef_l482aec right_ref(fn_7100d78378(right));
        fn_710048f9d4(ai, &left_ref, &right_ref, t);
    }
    fn_71003cc474(reinterpret_cast<UnkShockWaveMember*>(storage));
}
