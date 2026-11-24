// UnkTU811 (CEmmyAIComponent's unit): reads the chase target's name from the E.M.M.I.'s blackboard (taking a
// pending request first) and, when it names one, decides whether to keep chasing it through fn_7100546074,
// clearing the name when that says so. Answers false while the chase timer has run out. Names and layouts
// are placeholders.
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/ReflectedAttributes.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CAbilityShinespark.h"
#include "game/CAnimationComponent.h"
#include "game/CBlackboard.h"
#include "game/CEnemyMovement.h"
#include "game/UnkActor.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkTU0.h"
#include "game/UnkTypedValue.h"
#include "game/behaviortree/CBehavior.h"
#include "game/behaviortree/CEmmyChaseAction.h"
#include "game/behaviortree/CEmmySearchingAction.h"
#include "types.h"

using base::global::CRntString;
using base::global::CStrId;

// CEmmyAIComponent's class, which its getter (fn_710053b934, in this unit) builds and returns.
extern base::reflection::CClass g_unk_71726e2950 __attribute__((visibility("hidden")));

// A string with 0x100 bytes of inline storage.
struct UnkInlineString_l545c8c {
    u8 str[0x30];
    char* storage_vtable;  // +0x30
    u8 buffer[0x100];
    u64 field_0x138;
    u32 field_0x140;

    CRntString* Str() { return reinterpret_cast<CRntString*>(str); }
    UnkInlineString_l545c8c() {
        CRntString_InitWithAllocator(Str(), reinterpret_cast<base::IAllocator*>(&storage_vtable));
        storage_vtable = reinterpret_cast<char*>(g_unk_7101a194b8 + 2);
        field_0x138 = 0;
        field_0x140 = 0;
    }
    ~UnkInlineString_l545c8c() {
        CRntString_Clear(Str());
        CRntString_Destroy(Str());
    }
};

struct UnkFrameTimer_l545c8c {
    u8 pad_0x0[0x2c];
    f32 time;  // 0x2c
};

// How long the E.M.M.I. keeps chasing once it lost its target.
struct UnkChaseTimer_l545c8c {
    u32 kind;  // 0x0
    u8 pad_0x4[0x8];
    f32 phases[4];  // 0xc
    u8 pad_0x1c[0x4c - 0x1c];
    u32 mode;  // 0x4c
    f32 start;  // 0x50
    u8 pad_0x54[0x139 - 0x54];
    bool enabled;  // 0x139

    f32 Duration() const { return kind == 1 ? phases[0] + phases[1] + phases[2] + phases[3] : 0.0f; }
};

struct UnkEmmyAI_l545c8c {
    u8 pad_0x0[0x1260];
    CBlackboard blackboard;  // 0x1260
    u8 pad_after[0x14afc - 0x1260 - sizeof(CBlackboard)];
    UnkChaseTimer_l545c8c timer;  // 0x14afc
    u8 pad_0x14c38[0x27720 - 0x14afc - sizeof(UnkChaseTimer_l545c8c)];
    bool chasing;  // 0x27720
};
static_assert(__builtin_offsetof(UnkEmmyAI_l545c8c, timer) == 0x14afc, "UnkEmmyAI_l545c8c layout");

struct UnkEmmyOwner_l545c8c {
    u8 pad_0x0[0x1f0];
    void* movement;  // 0x1f0
    u8 pad_0x1f8[0x220 - 0x1f8];
    UnkAnimationComponent* animation;  // 0x220
    u8 pad_0x228[0x240 - 0x228];
    void* ai;  // 0x240
};

struct UnkEmmyComponent_l545c8c {
    u8 pad_0x0[0x30];
    UnkEmmyOwner_l545c8c* owner;  // 0x30
    u8 pad_0x38[0x2a60c - 0x38];
    bool requested;  // 0x2a60c
    bool ignoring;  // 0x2a60d
};

bool fn_7100546074(UnkEmmyComponent_l545c8c* self, UnkInlineString_l545c8c* name, bool* chase);

static inline UnkEmmyAI_l545c8c* AsEmmyAI(void* component) {
    base::reflection::UnkReflObject* object = static_cast<base::reflection::UnkReflObject*>(component);
    if (!object)
        return nullptr;
    const base::reflection::CType* type = object->vf_0x0();
    // Faithful to the original: the getter is in this unit, so its result (the class) is known and
    // the call stays only for building the class on first use.
    fn_710053b934();
    return fn_710009d50c(type, &g_unk_71726e2950) ? reinterpret_cast<UnkEmmyAI_l545c8c*>(object) : nullptr;
}

static inline void* AsMovement(void* component) {
    base::reflection::UnkReflObject* object = static_cast<base::reflection::UnkReflObject*>(component);
    if (!object)
        return nullptr;
    return fn_710009d50c(object->vf_0x0(), fn_710057134c()) ? object : nullptr;
}

template <class T>
static inline void SetValue(CBlackboard* blackboard, UnkPropertyName* name, T* value) {
    if (CBlackboard::CSection* s = fn_71010472e0(blackboard, g_unk_71726b7cb8)) {
        if (!blackboard->field_0xe8) {
            UnkTypedValue v;
            v.type = base::reflection::UnkReflTypeOf<T>::Get();
            v.value = value;
            fn_7100332c80(s, name, &v);
        }
    }
}

// Reads the flag `name` of the section into `out`, and answers it.
static inline bool GetFlag(CBlackboard::CSection* section, UnkPropertyName* name, bool* out) {
    fn_710006d11c(section, reinterpret_cast<const CStrId*>(name), out);
    return *out;
}

static inline void ReadName(CBlackboard* blackboard, UnkInlineString_l545c8c* name) {
    if (CBlackboard::CSection* s = fn_7101047270(blackboard, g_unk_71726b7cb8))
        fn_7100395c38(s, &g_unk_71726debd8, name->Str());
}

bool fn_7100545c8c(UnkEmmyComponent_l545c8c* self, bool* chase) {
    UnkEmmyAI_l545c8c* ai = AsEmmyAI(self->owner->ai);
    UnkAnimationComponent* anim = self->owner->animation;
    if (ai->timer.mode != 2) {
        UnkChaseTimer_l545c8c* timer = &ai->timer;
        if (!timer->enabled)
            return false;
        f32 elapsed = reinterpret_cast<UnkFrameTimer_l545c8c*>(g_unk_7101d47328)->time - timer->start;
        if (!(elapsed <= timer->Duration() * 1.5f))
            return false;
    }

    UnkInlineString_l545c8c name;
    if (ai->chasing || self->requested) {
        ReadName(&ai->blackboard, &name);
    } else {
        bool request = false;
        CBlackboard::CSection* s = fn_7101047270(&ai->blackboard, g_unk_71726b7cb8);
        if (s && GetFlag(s, &g_unk_71726df1a8, &request)) {
            self->requested = true;
            bool off = false;
            SetValue(&ai->blackboard, &g_unk_71726df1a8, &off);
            ReadName(&ai->blackboard, &name);
        } else {
            SetValue(&ai->blackboard, &g_unk_71726debd8, &g_unk_7101d470e0);
        }
    }

    CRntString* text = name.Str();
    if (!text->data || !*text->data || fn_7101178e6c(reinterpret_cast<UnkTakumakuComponent*>(anim), 10)) {
        *chase = false;
    } else {
        void* movement = AsMovement(self->owner->movement);
        if (fn_710117b348(anim, fn_7100c35eb8()) || fn_71010cc3a4(movement) == 4)
            self->ignoring = true;
        if (fn_71010cc3a4(movement) == 1 || *chase) {
            if (fn_71011782ec(anim, 0xf4061ff25a2163d6) ||
                (!fn_710117b348(anim, fn_7100d2b3a8()) && !fn_710117b348(anim, fn_7100c35eb8()))) {
                if (fn_7100546074(self, &name, chase))
                    SetValue(&ai->blackboard, &g_unk_71726debd8, &g_unk_7101d470e0);
            }
        }
    }
    return true;
}
