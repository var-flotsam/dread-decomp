// The CKraidBouncingCreaturesAttack unit's start (fn_71005f2cf0): after the base restart
// (fn_71005f2018) it hooks the callback fn_71005f3e4c up to the object the owner tracks (as
// UnkAttackTrackLink_b54.cpp, link at +0x48), caches whether the owner's AI component (+0x240),
// cast to CKraidAIComponent, passes fn_7100600c90 (+0x44), picks the list index (+0x38: the tunable's
// value at +0x24 unless it is -1, else a random one below the pattern set's count at +0x100) with
// slot 0 (+0x3c), and, when that list's first entry has a non-zero time at +0x14, sends the owner's
// +0x220 sub-object 0xa5133ff8f4e5d6c8 with mode 2 (UnkSpitSend.h) before keeping the time at +0x40.
// Names and layouts are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::reflection::UnkReflObject;

void fn_7100316018(UnkWeakTarget* self, UnkWeakHandle** out);
void fn_710117b3e8(UnkWeakTarget* target, void* link);
void fn_71005f2018(char* self);
void fn_71005f3e4c();
bool fn_7100600c90(UnkReflObject* ai);
int fn_71012395a0();

struct UnkTunable;
extern UnkTunable g_unk_71726ef3f0;

struct UnkKraidBouncingList_b62a4 {
    u8 data[0x28];
};
struct UnkKraidBouncingEntry_b62a4 {
    u8 pad_0x0[0x14];
    f32 time;
};

struct UnkKraidBouncingSet_b62a4 {
    u8 pad_0x0[0xf8];
    UnkKraidBouncingList_b62a4* lists;
    u32 count;
};

// Typed as CKraidBouncingCreaturesAttack_Update_b57.cpp declares it (one signature per symbol).
struct UnkKraidBouncingList_b57a4;
void** fn_71005e2c08(UnkKraidBouncingList_b57a4* list, u32 index);

struct UnkKraidBouncingLink_b62a4 {
    u8 pad_0x0[0x8];
    void* context;
    void (*callback)();
};

struct UnkKraidBouncingAttack_b62a4 {
    u8 pad_0x0[0x8];
    char* owner;
    u8 pad_0x10[0x20];
    UnkKraidBouncingSet_b62a4* set;
    u32 index;
    u32 slot;
    f32 time;
    bool field_0x44;
    UnkKraidBouncingLink_b62a4 link;
    UnkWeakHandle* handle;
};

static inline __attribute__((always_inline)) void Send_b62a4(void* target, u64 hash, u64 kind, u32 m) {
    base::reflection::UnkReflTypeIdArg mode;
    mode.id = m;
    base::reflection::UnkReflTypeIdArg mask;
    mask.id = UnkAll_SpitSend().value;
    fn_7101176690(target, hash, kind, &mode, &g_unk_710164b958, &g_unk_7172769740, &mask);
}

void fn_71005f2cf0(UnkKraidBouncingAttack_b62a4* self) {
    fn_71005f2018(reinterpret_cast<char*>(self));
    UnkKraidBouncingLink_b62a4* link = &self->link;
    link->context = self;
    link->callback = fn_71005f3e4c;
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
    self->field_0x44 = fn_7100600c90(kraid);
    s32 index = *reinterpret_cast<s32*>(reinterpret_cast<char*>(&g_unk_71726ef3f0) + 0x24);
    if (index == -1) {
        u32 count = self->set->count;
        index = count != 0 ? (fn_71012395a0() & 0x7fff) % count : 0;
    }
    self->index = index;
    self->slot = 0;
    void* target = *reinterpret_cast<void**>(self->owner + 0x220);
    UnkKraidBouncingEntry_b62a4* entry = *reinterpret_cast<UnkKraidBouncingEntry_b62a4**>(fn_71005e2c08(reinterpret_cast<UnkKraidBouncingList_b57a4*>(&self->set->lists[(u32)index]), 0));
    if (!entry)
        return;
    if (entry->time != 0.0f)
        Send_b62a4(target, 0xa5133ff8f4e5d6c8, 0xf4061ff25a2163d6, 2);
    self->time = entry->time;
}
