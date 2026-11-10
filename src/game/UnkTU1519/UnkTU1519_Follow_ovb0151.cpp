// Unit 1519: helpers of an object that follows a target (placeholders for names and layouts): the
// detach of its effect and the constructor holding weak handles of two targets.
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

struct UnkFollowOwner_b0151 {
    u8 pad_0x0[0x228];
    struct {
        u8 pad_0x0[0x50];
        void* effect;  // 0x50
    }* field_0x228;
};

struct UnkFollowParent_b0151 {
    u8 pad_0x0[0x10];
    UnkFollowOwner_b0151* owner;  // 0x10
};

struct UnkFollowEffect_b0151 {
    u8 pad_0x0[0x8];
    UnkFollowParent_b0151* parent;  // 0x8
    u8 pad_0x10[0x8];
    void* scene;  // 0x18
};

void fn_710014580c(void* effect);

void fn_7100a0f1f0(UnkFollowEffect_b0151* self) {
    reinterpret_cast<void (*)(void*, void*)>(fn_710014580c)(self->scene, self->parent->owner->field_0x228->effect);
}

extern char g_unk_7101a8b8a0[];

struct UnkFollowPair_b0151 {
    void* vtable;
    UnkWeakHandle* first;  // 0x8
    UnkWeakHandle* second;  // 0x10
    u64 field_0x18;
};

// Takes a counted reference into an empty slot.
static inline void SetRef_b0151(UnkWeakHandle** slot, UnkWeakHandle* p) {
    if (p) {
        ++p->ref_count;
        if (UnkWeakHandle* old = *slot) {
            if (--old->ref_count == 0)
                old->vf_0x10();
        }
        *slot = p;
    }
}

// Holds the weak handles of two targets (and a word).
void fn_7100a0ed5c(UnkFollowPair_b0151* self, UnkWeakTarget* first, UnkWeakTarget* second, u64 value) {
    self->vtable = g_unk_7101a8b8a0 + 0x10;
    UnkWeakHandle* handle = GetWeakHandle(first);
    __builtin_memset(&self->first, 0, sizeof(self->first));
    SetRef_b0151(&self->first, handle);
    handle = GetWeakHandle(second);
    __builtin_memset(&self->second, 0, sizeof(self->second));
    SetRef_b0151(&self->second, handle);
    self->field_0x18 = value;
}
