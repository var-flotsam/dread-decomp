// CChozoWarriorGlaiveWalljumpAttack's unit (tools/tu_map.py): the attack's start. After the base start
// (fn_71003608ac), when the owner's (+0x8) AI (+0x240) is a CChozoWarriorAIComponent and the owner has an
// effect sink (+0x218), hands the sink a kind-3 parameter block and has the AI pick a string id for the
// negated facing x (fn_710049c2f4; the id is dropped). Then hooks the callback fn_7100496570 (link at +0x48,
// handle at +0x60) up to the object the owner tracks (+0x220), as
// src/game/CChozoWarriorXGlaiveSpinAttack/UnkAttackTrackLink_b54.cpp. Names and layouts are placeholders.
#include "base/UnkMatrix44.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::reflection::UnkReflObject;

struct UnkAttackBase;
struct UnkEffectSink;
void fn_71003608ac(UnkAttackBase* self);
void fn_71010c7e6c(UnkEffectSink* sink, void* params, u32 flags);
base::global::CStrId fn_710049c2f4(UnkReflObject* ai, f32 facing);
void fn_7100316018(UnkWeakTarget* self, UnkWeakHandle** out);
void fn_710117b3e8(UnkWeakTarget* target, void* link);
void fn_7100496570();

extern void* g_unk_7101a2e208[];

struct UnkWalljumpParams_A2_b73 {
    void** vtable;
    u8 pad_0x8[4];
    u8 field_0xc;
    u8 pad_0xd[7];
    u8 field_0x14;
    u8 pad_0x15[3];
    u8 field_0x18[13];
    u8 pad_0x25[3];
    u8 field_0x28[13];
    u8 pad_0x35[3];
    u32 field_0x38;
    u8 pad_0x3c;
    u8 field_0x3d;
};

struct UnkWalljumpLink_A2_b73 {
    u8 pad_0x0[0x8];
    void* context;
    void (*callback)();
};

struct UnkWalljumpStart_A2_b73 {
    u8 pad_0x0[0x8];
    char* owner;
    u8 pad_0x10[0x38];
    UnkWalljumpLink_A2_b73 link;  // 0x48
    UnkWeakHandle* handle;        // 0x60
};

void fn_71004954d4(UnkWalljumpStart_A2_b73* self) {
    fn_71003608ac(reinterpret_cast<UnkAttackBase*>(self));
    UnkReflObject* ai = *reinterpret_cast<UnkReflObject**>(self->owner + 0x240);
    if (ai) {
        UnkEffectSink* sink = *reinterpret_cast<UnkEffectSink**>(self->owner + 0x218);
        if (fn_710009d50c(ai->vf_0x0(), fn_7100497f2c()) && sink) {
            UnkWalljumpParams_A2_b73 params;
            params.field_0xc = 0;
            params.field_0x14 = 0;
            __builtin_memset(params.field_0x18, 0, 13);
            __builtin_memset(params.field_0x28, 0, 13);
            params.vtable = g_unk_7101a2e208 + 2;
            params.field_0x3d = 0;
            params.field_0x38 = 3;
            fn_71010c7e6c(sink, &params, 1);
            const UnkMatrix44& m = fn_710002614c(reinterpret_cast<const UnkMatrix44*>(self->owner + 0x38));
            fn_710049c2f4(ai, -m.m[2]);
        }
    }
    UnkWalljumpLink_A2_b73* link = &self->link;
    link->context = self;
    link->callback = fn_7100496570;
    UnkWeakTarget* track = *reinterpret_cast<UnkWeakTarget**>(self->owner + 0x220);
    if (UnkWeakHandle* handle = self->handle) {
        if (UnkWeakTarget* target = handle->target)
            fn_710117b3f0(target, link);
        if (UnkWeakHandle* p = self->handle) {
            if (--p->ref_count == 0)
                p->vf_0x10();
            self->handle = nullptr;
        }
    }
    if (!track)
        return;
    asm("" : "+r"(self));
    fn_7100316018(track, &self->handle);
    fn_710117b3e8(self->handle ? self->handle->target : nullptr, link);
}
