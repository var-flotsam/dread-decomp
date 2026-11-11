// The CKraidSpinningNailsAttack unit's start: after the base restart (fn_71005f2018) hooks the callback
// fn_71005f7968 up to the object the owner tracks (as UnkAttackTrackLink_b54.cpp, link at +0x30, handle at
// +0x48), then picks the list index (+0x50: the tunable's value at +0x24 unless it is -1, else a random one
// below the pattern set's (+0x60) count at +0x120 or, when the owner's AI component, cast to
// CKraidAIComponent, has +0x1140 clear, at +0xf8) with slot 0 (+0x54), as fn_71005f2cf0
// (CKraidBouncingCreaturesAttack_Start_b62a4.cpp). Names and layouts are placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CKraidAttack.h"
#include "game/UnkTU2772.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::reflection::UnkReflObject;

void fn_7100316018(UnkWeakTarget* self, UnkWeakHandle** out);
void fn_710117b3e8(UnkWeakTarget* target, void* link);
void fn_71005f7968();

struct UnkTunable;
extern UnkTunable g_unk_71726f0b10;

struct UnkKraidNailsLink_A3_b73 {
    u8 pad_0x0[0x8];
    void* context;
    void (*callback)();
};

struct UnkKraidNailsAttack_A3_b73 {
    u8 pad_0x0[0x8];
    char* owner;
    u8 pad_0x10[0x20];
    UnkKraidNailsLink_A3_b73 link;  // 0x30
    UnkWeakHandle* handle;          // 0x48
    u32 index;
    u32 slot;
    u8 pad_0x58[0x8];
    char* set;  // 0x60
};

void fn_71005f668c(UnkKraidNailsAttack_A3_b73* self) {
    fn_71005f2018(reinterpret_cast<char*>(self));
    UnkKraidNailsLink_A3_b73* link = &self->link;
    link->context = self;
    link->callback = fn_71005f7968;
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
    if (track) {
        asm("" : "+r"(self));
        fn_7100316018(track, &self->handle);
        fn_710117b3e8(self->handle ? self->handle->target : nullptr, link);
    }
    UnkReflObject* ai = *reinterpret_cast<UnkReflObject**>(self->owner + 0x240);
    UnkReflObject* kraid = ai && fn_710009d50c(ai->vf_0x0(), fn_71005fac74()) ? ai : nullptr;
    s32 index = *reinterpret_cast<s32*>(reinterpret_cast<char*>(&g_unk_71726f0b10) + 0x24);
    if (index == -1) {
        bool alt = *reinterpret_cast<u32*>(reinterpret_cast<char*>(kraid) + 0x1140) != 0;
        u32 count = *reinterpret_cast<u32*>(self->set + (alt ? 0x120 : 0xf8));
        index = count != 0 ? (fn_71012395a0() & 0x7fff) % count : 0;
    }
    self->index = index;
    self->slot = 0;
}
