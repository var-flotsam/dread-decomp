// Unit 2545: a GUI screen's update. It runs its small state machine (states 0-8, "none" 9; the update of
// src/game/UnkStateMachineUpdateA5_b62.cpp), places its display objects for the current layout and
// language, and reports a mode from the current state. Names and layouts are placeholders.
#include "types.h"
#include "base/UnkVector.h"
#include "base/math/CVector.h"
#include "game/UnkGuiDisplayObject.h"

struct UnkSmState_ldd2ca8 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10(s32 previous);  // entered
    virtual void vf_0x18(s32 next);      // left
    virtual void vf_0x20(f32 dt);        // updated
    u64 field_0x8;
    u32 id;
};

typedef UnkSmState_ldd2ca8* __attribute__((aligned(4))) UnkSmStatePtr_ldd2ca8;

struct UnkMinimapVec3 {
    f32 x;
    f32 y;
    f32 z;
    UnkMinimapVec3() {}
    UnkMinimapVec3(f32 x_, f32 y_, f32 z_) : x(x_), y(y_), z(z_) {}
};
void fn_710020a4c4(void* node, const UnkMinimapVec3& pos);

struct UnkNodeOwner_ldd2ca8 {
    u8 pad_0x0[0x18];
    u8 node;
};

struct UnkScreen_ldd2ca8 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20(UnkSmState_ldd2ca8* previous, UnkSmState_ldd2ca8* next);
    virtual void vf_0x28(UnkSmState_ldd2ca8* current, UnkSmState_ldd2ca8* next);

    u64 field_0x8;
    UnkSmState_ldd2ca8* current;
    UnkSmState_ldd2ca8* previous;
    u32 count;
    UnkSmStatePtr_ldd2ca8 states[16];
    u32 pending;
    bool busy;
    u8 pad_0xa9[0x13];
    u8 field_0xbc;
    u8 pad_0xbd[0x23];
    u8 field_0xe0;
    u8 pad_0xe1[0x47];
    UnkGuiDisplayObject* field_0x128;
    UnkGuiDisplayObject* field_0x130;
    UnkGuiDisplayObject* field_0x138;
    UnkGuiDisplayObject* field_0x140;
    UnkGuiDisplayObject* field_0x148;
    UnkGuiDisplayObject* field_0x150;
    UnkNodeOwner_ldd2ca8* field_0x158;
    UnkGuiDisplayObject* field_0x160;
    UnkGuiDisplayObject* field_0x168;
    UnkGuiDisplayObject* field_0x170;
    UnkGuiDisplayObject* field_0x178;
    UnkGuiDisplayObject* field_0x180;
    UnkGuiDisplayObject* field_0x188;
    UnkGuiDisplayObject* field_0x190;
};
static_assert(__builtin_offsetof(UnkScreen_ldd2ca8, pending) == 0xa4, "pending");
static_assert(__builtin_offsetof(UnkScreen_ldd2ca8, field_0xe0) == 0xe0, "0xe0");
static_assert(__builtin_offsetof(UnkScreen_ldd2ca8, field_0x190) == 0x190, "0x190");

struct UnkLanguage;
extern UnkLanguage* g_unk_7101d47358;
void fn_710119c020(UnkLanguage* language);
s32 fn_710119c9f4(UnkLanguage* language);

typedef base::math::CVector2D UnkPos_ldd2ca8;
__attribute__((visibility("hidden"))) extern const UnkPos_ldd2ca8* const g_unk_7101aa9840[5];
__attribute__((visibility("hidden"))) extern const UnkPos_ldd2ca8 g_unk_7101aa9868[4];
__attribute__((visibility("hidden"))) extern const UnkPos_ldd2ca8 g_unk_7101aa9888[5];
__attribute__((visibility("hidden"))) extern const UnkPos_ldd2ca8 g_unk_7101aa98b0;
__attribute__((visibility("hidden"))) extern const UnkPos_ldd2ca8 g_unk_7101aa98b8[3];
__attribute__((visibility("hidden"))) extern const UnkPos_ldd2ca8 g_unk_7101aa98d0[2];
__attribute__((visibility("hidden"))) extern bool g_unk_71727bdf80;

__attribute__((always_inline)) static inline bool FindState_ldd2ca8(UnkScreen_ldd2ca8* self, u32 id,
                                                                  UnkSmState_ldd2ca8*& out) {
    u64 n = (u64)self->count * sizeof(UnkSmStatePtr_ldd2ca8);
    asm("" : "+r"(n));
    for (UnkSmStatePtr_ldd2ca8* p = self->states; n != 0; ++p, n -= sizeof(UnkSmStatePtr_ldd2ca8)) {
        if ((*p)->id == id) {
            out = *p;
            return out != nullptr;
        }
    }
    out = nullptr;
    return false;
}

// Switches to the pending state (ids above 8 are ignored) and clears the pending id.
__attribute__((always_inline)) static inline void Apply_ldd2ca8(UnkScreen_ldd2ca8* self) {
    u32 id = self->pending;
    if (id > 8)
        return;
    UnkSmState_ldd2ca8* current = self->current;
    if (!current || current->id != id) {
        UnkSmState_ldd2ca8* found;
        UnkSmState_ldd2ca8* next;
        u32 enter;
        if (FindState_ldd2ca8(self, id, found)) {
            next = found;
            enter = true;
        } else {
            if (id != 0)
                goto done;
            enter = false;
            asm("" : "+r"(enter));
            next = nullptr;
            asm("" : "+r"(next));
        }
        if (current) {
            current->vf_0x18(id);
            self->vf_0x28(self->current, next);
        }
        self->previous = self->current;
        self->current = found;
        if (enter) {
            typedef void (*EnteredFn)(UnkSmState_ldd2ca8*, s32);
            s32 previous_id = self->previous ? self->previous->id : 9;
            EnteredFn entered = (*reinterpret_cast<EnteredFn**>(found))[2];
            entered(next, previous_id);
            self->vf_0x20(self->previous, next);
        }
    }
done:
    self->pending = 9;
}

static inline UnkLanguage* Language_ldd2ca8() {
    UnkLanguage* language = g_unk_7101d47358;
    if (!language) {
        language = static_cast<UnkLanguage*>(fn_71000839a0(0x2068, 0x3f));
        fn_710119c020(language);
        g_unk_7101d47358 = language;
    }
    return language;
}

s32 fn_7100dd2ca8(UnkScreen_ldd2ca8* self, f32 dt) {
    self->busy = true;
    if (self->pending != 9)
        Apply_ldd2ca8(self);
    u32 i = 0;
    while (UnkSmState_ldd2ca8* current = self->current) {
        current->vf_0x20(dt);
        if (self->pending == 9)
            break;
        Apply_ldd2ca8(self);
        if (i++ > 8)
            break;
    }
    self->busy = false;

    const UnkPos_ldd2ca8* first = &g_unk_7101aa9868[0];
    const UnkPos_ldd2ca8* second = &g_unk_7101aa9868[1];
    if (!self->field_0xbc && !self->field_0xe0) {
        first = &g_unk_7101aa9868[2];
        second = &g_unk_7101aa9868[3];
    }
    fn_71011cde64(self->field_0x128, first, 1, 1);
    fn_71011cde64(self->field_0x130, second, 1, 1);
    fn_71011cde64(self->field_0x138, &g_unk_7101aa9888[0], 1, 1);
    fn_71011cde64(self->field_0x140, &g_unk_7101aa9888[1], 1, 1);
    fn_71011cde64(self->field_0x150, &g_unk_7101aa9888[2], 1, 1);
    fn_71011cde64(self->field_0x148, &g_unk_7101aa9888[3], 1, 1);
    fn_71011cde64(self->field_0x190, &g_unk_7101aa9888[4], 1, 1);

    u32 language = fn_710119c9f4(Language_ldd2ca8()) - 10;
    const UnkPos_ldd2ca8* text_pos = language <= 4 ? g_unk_7101aa9840[(s32)language] : &g_unk_7101aa98b0;
    fn_71011cde64(self->field_0x188, text_pos, 1, 1);

    if (self->field_0xbc) {
        fn_710020a4c4(&self->field_0x158->node, UnkMinimapVec3(-0.9f, -0.337f, 0.0f));
        const f32 dx = 0.0f;
        const f32 dy = -0.478f;
        UnkPos_ldd2ca8 pos;
        pos.x = g_unk_7101aa98b8[0].x + dx;
        pos.y = g_unk_7101aa98b8[0].y + dy;
        fn_71011cde64(self->field_0x170, &pos, 1, 1);
        pos.x = g_unk_7101aa98b8[1].x + dx;
        pos.y = g_unk_7101aa98b8[1].y + dy;
        fn_71011cde64(self->field_0x160, &pos, 1, 1);
        pos.x = g_unk_7101aa98b8[2].x + dx;
        pos.y = g_unk_7101aa98b8[2].y + dy;
        fn_71011cde64(self->field_0x168, &pos, 1, 1);
    } else {
        fn_710020a4c4(&self->field_0x158->node, UnkMinimapVec3(-0.9f, 0.2f, 0.0f));
        fn_71011cde64(self->field_0x170, &g_unk_7101aa98b8[0], 1, 1);
        fn_71011cde64(self->field_0x160, &g_unk_7101aa98b8[1], 1, 1);
        fn_71011cde64(self->field_0x168, &g_unk_7101aa98b8[2], 1, 1);
    }
    fn_71011cde64(self->field_0x178, &g_unk_7101aa98d0[0], 1, 1);
    fn_71011cde64(self->field_0x180, &g_unk_7101aa98d0[1], 1, 1);

    UnkSmState_ldd2ca8* current = self->current;
    if (!current)
        return 0;
    return current->id == 1 ? (g_unk_71727bdf80 ? 2 : 1) : 0;
}
