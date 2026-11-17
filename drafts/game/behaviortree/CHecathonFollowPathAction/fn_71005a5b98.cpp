// behaviortree::CHecathonFollowPathAction's unit: the action's start. After the base step it builds a
// path request (the parameter block with vtable g_unk_7101a264d0, kind 2) towards the target held by the
// Hecathon AI's path state (+0x14718), hands it to the owner's component at +0x298 and marks the state
// started (+0xb2). Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/behaviortree/CBehavior.h"
#include "game/behaviortree/CFlyingFollowPathAction.h"
#include "game_s2/UnkParams.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::reflection::UnkReflObject;

// Keeps a counted reference for as long as it lives.
struct UnkHecathonHeldRef_l5a5b98 {
    UnkWeakHandle* handle;

    explicit UnkHecathonHeldRef_l5a5b98(UnkWeakHandle* h) : handle(h) {
        if (handle)
            ++handle->ref_count;
    }
    ~UnkHecathonHeldRef_l5a5b98() { ResetRef(&handle); }
};

struct UnkHecathonPathTarget_l5a5b98 {
    u8 pad_0x0[0x30];
    u8 field_0x30[0x10];
};

struct UnkHecathonSlot_l5a5b98 {
    u32 field_0x0 = 0;
    u32 field_0x4 = 0;
    u32 field_0x8 = 0;
    u8 field_0xc = 0;
};

struct UnkHecathonFlagged_l5a5b98 {
    u8 flag = 0;
    u64 field_0x8 = 0;
    u64 field_0x10 = 0;
};

// A value with a flag saying whether it is set.
template <class T>
struct UnkHecathonOptional_l5a5b98 {
    T value;
    bool has = false;

    void Set(T v) {
        value = v;
        has = true;
    }
};

// The parameter block of a path request (0x108 bytes).
struct UnkHecathonPathParams_l5a5b98 {
    void* vtable = g_unk_7101a264d0 + 0x10;                // 0x0
    UnkHecathonSlot_l5a5b98 field_0x8;                     // 0x8
    UnkHecathonOptional_l5a5b98<f32> field_0x18;           // 0x18
    u64 field_0x20 = 0;                                    // 0x20
    u64 field_0x28 = 0;                                    // 0x28
    u8 field_0x30 = 0;                                     // 0x30
    u32 kind = 0;                                          // 0x34
    void* target;                                          // 0x38
    bool has_target = false;                               // 0x40
    UnkHecathonOptional_l5a5b98<u32> field_0x44;           // 0x44
    UnkHecathonOptional_l5a5b98<f32> field_0x4c[6];        // 0x4c
    UnkHecathonSlot_l5a5b98 field_0x7c;                    // 0x7c
    u8 pad_0x8c[4];
    u8 field_0x90 = 0;                                     // 0x90
    bool field_0x91;                                       // 0x91
    u8 field_0x92 = 0;                                     // 0x92
    bool field_0x93;                                       // 0x93
    u8 field_0x94 = 0;                                     // 0x94
    u8 pad_0x95[7];
    u8 field_0x9c = 0;                                     // 0x9c
    u8 pad_0x9d;
    u8 field_0x9e = 0;                                     // 0x9e
    UnkHecathonFlagged_l5a5b98 field_0xa0[2];              // 0xa0
    u8 field_0xd0 = 0;                                     // 0xd0
    base::global::CRntVector<u8> points{15};               // 0xd8
    u8 field_0x100 = 0;                                    // 0x100
    bool field_0x101;                                      // 0x101
    u8 field_0x102 = 0;                                    // 0x102
    u8 pad_0x103;
    u8 field_0x104 = 0;                                    // 0x104
    u8 pad_0x105;
    u8 field_0x106 = 0;                                    // 0x106
    u8 pad_0x107;

    ~UnkHecathonPathParams_l5a5b98() {
        vtable = g_unk_7101a264d0 + 0x10;
        points.Free();
    }
};
static_assert(__builtin_offsetof(UnkHecathonPathParams_l5a5b98, points) == 0xd8, "");
static_assert(sizeof(UnkHecathonPathParams_l5a5b98) == 0x108, "");

// The Hecathon AI's path state.
struct UnkHecathonPathState_l5a5b98 {
    u32 field_0x0;
    u8 pad_0x4[0xae];
    u16 field_0xb2;                // 0xb2
    u8 pad_0xb4[0xc];
    UnkWeakHandle* target;         // 0xc0
};

struct UnkHecathonAI_l5a5b98 {
    u8 pad_0x0[0x14718];
    UnkHecathonPathState_l5a5b98 path;  // 0x14718
};

struct UnkHecathonOwner_l5a5b98 {
    u8 pad_0x0[0x240];
    UnkReflObject* ai;  // 0x240
    u8 pad_0x248[0x50];
    void* field_0x298;  // 0x298
};

struct UnkHecathonFollowPath_l5a5b98 {
    u8 pad_0x0[0x18];
    UnkHecathonOwner_l5a5b98* owner;  // 0x18
};

void fn_71005a5b98(UnkHecathonFollowPath_l5a5b98* self) {
    fn_7100394bd0(reinterpret_cast<UnkStruct_0x68*>(self));
    UnkReflObject* o = self->owner->ai;
    bool is_ai = o && fn_710009d50c(o->vf_0x0(), fn_71005a7e74());
    UnkHecathonAI_l5a5b98* ai = is_ai ? reinterpret_cast<UnkHecathonAI_l5a5b98*>(o) : nullptr;
    if (!is_ai)
        return;
    UnkHecathonPathState_l5a5b98* state = &ai->path;
    void* receiver = self->owner->field_0x298;
    if (!receiver)
        return;
    UnkHecathonPathParams_l5a5b98 params;
    params.kind = 2;
    UnkWeakTarget* target;
    {
        UnkHecathonHeldRef_l5a5b98 ref(state->target);
        target = ref.handle ? ref.handle->target : nullptr;
    }
    if (target) {
        params.target = reinterpret_cast<UnkHecathonPathTarget_l5a5b98*>(target)->field_0x30;
        params.has_target = true;
        params.field_0x44.Set(state->field_0x0);
        params.field_0x18.Set(1.0f);
        params.field_0x91 = false;
        params.field_0x92 = 1;
        params.field_0x93 = true;
        params.field_0x94 = 1;
        params.field_0x101 = true;
        params.field_0x102 = 1;
        params.field_0x4c[1].Set(300.0f);
        params.field_0x4c[2].Set(9999.0f);
        fn_71006f4b0c(receiver, &params, 1);
    }
    state->field_0xb2 = 1;
}
