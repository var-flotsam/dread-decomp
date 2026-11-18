// behaviortree::CHyperDashAttackAction's unit (tools/tu_map.py, unit 662): the node's start. Picks the dash
// target with the commander's flag at +0x15ed0 lowered, then either signals the owner's slot at +0x298 or
// starts the dash. Names and layouts are placeholders.
#include "../../../game_aca57c/UnkBufferString.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/behaviortree/CBehavior.h"
#include "game_s2/UnkParams.h"
#include "types.h"

// Matches when built without strict aliasing (the original reloads +0x70 after the commander's store);
// unit 662 is not in data/unit_flags.csv.

using base::global::CRntString;
using base::global::CStrId;
using base::reflection::CClass;

using UnkDashName_l4496a8 = UnkBufferString<0x100, g_unk_7101a194b8>;

// Signatures as other files declare them.
struct UnkStruct_0x68;
struct UnkStringEqualsCondition_A2_b72;
struct UnkInlineString_A2_b72;

struct UnkDashAI_l4496a8 {
    void** vtable;
    u8 pad_0x8[0x15ed0 - 0x8];
    bool field_0x15ed0;

    const CClass* vf_0x0() { return reinterpret_cast<const CClass* (*)(UnkDashAI_l4496a8*)>(vtable[0])(this); }
};

struct UnkDashActor_l4496a8 {
    u8 pad_0x0[0x240];
    UnkDashAI_l4496a8* ai;  // 0x240
    u8 pad_0x248[0x298 - 0x248];
    void* field_0x298;
};

struct UnkHyperDashAttackAction_l4496a8 {
    u8 pad_0x0[0x18];
    UnkDashActor_l4496a8* actor;  // 0x18
    u8 pad_0x20[0x10];
    CRntString name;  // 0x30
    CStrId name_property;  // 0x60
    u8 pad_0x68;
    bool started;  // 0x69
    u8 pad_0x6a[6];
    bool field_0x70;
};

namespace base {
namespace reflection {
struct UnkReflObject;
}  // namespace reflection
}  // namespace base
using base::reflection::UnkReflObject;
struct UnkHyperDashNode_ovb0095;
UnkReflObject* fn_7100449868(UnkHyperDashNode_ovb0095* self);
bool fn_7100449944(UnkHyperDashAttackAction_l4496a8* self, void* target);
bool fn_7100449bdc(UnkHyperDashAttackAction_l4496a8* self, void* target, bool flag);

void fn_71004496a8(UnkHyperDashAttackAction_l4496a8* self) {
    fn_7100394bd0(reinterpret_cast<UnkStruct_0x68*>(self));
    self->started = false;
    UnkDashAI_l4496a8* commander = nullptr;
    if (self->actor) {
        UnkDashAI_l4496a8* ai = self->actor->ai;
        if (ai && fn_710009d50c(ai->vf_0x0(), fn_710044e708()))
            commander = ai;
    }
    if (commander)
        commander->field_0x15ed0 = false;
    UnkDashName_l4496a8 name(*reinterpret_cast<const UnkDashName_l4496a8*>(&self->name));
    if (self->name_property.data != &g_unk_7101d47108->empty)
        fn_7100395044(reinterpret_cast<UnkStringEqualsCondition_A2_b72*>(self), &self->name_property,
                      reinterpret_cast<UnkInlineString_A2_b72*>(&name));
    UnkReflObject* target = fn_7100449868(reinterpret_cast<UnkHyperDashNode_ovb0095*>(self));
    self->field_0x70 = fn_7100449944(self, target);
    if (commander)
        commander->field_0x15ed0 = true;
    if (self->field_0x70) {
        void* sink = self->actor->field_0x298;
        if (sink) {
            UnkParams params;
            __builtin_memset(params.field_0x8, 0, 13);
            params.field_0x30 = 0;
            __builtin_memset(params.field_0x20, 0, 16);
            params.vtable = g_unk_7101a25b10 + 2;
            params.field_0x3b = 0;
            params.field_0x3d = 0;
            params.field_0x34 = 5;
            params.pad_0x38 = 1;
            params.field_0x39 = 1;
            params.field_0x18 = 0;
            params.field_0x1c = 1;
            fn_71006f4b0c(sink, &params, 1);
        }
    } else {
        self->started = fn_7100449bdc(self, target, false);
    }
}
