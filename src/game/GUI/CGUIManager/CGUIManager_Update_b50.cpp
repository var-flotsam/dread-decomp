// From unit 2744 of tools/tu_map.py (gui/scripts/guimanager.lua): the GUI manager's per-frame update.
// The step is scaled by the game manager's time scale; the input deltas are cleared, read again
// (fn_71011b3990) unless input is blocked, and kept as the previous frame's. Names and layouts are
// placeholders.
#include "base/UnkVector.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2768.h"
#include "game/UnkTU39.h"
#include "types.h"

struct UnkUserDataOwner;
extern UnkUserDataOwner* g_unk_7101d47308;

static inline UnkUserDataOwner* GetOwner() {
    if (!g_unk_7101d47308) {
        UnkUserDataOwner* owner = static_cast<UnkUserDataOwner*>(fn_71000839a0(0x2b8, 0x3f));
        fn_710120320c(owner);
        g_unk_7101d47308 = owner;
    }
    return g_unk_7101d47308;
}

struct UnkGuiDelta_71011b3840 {
    u32 x;
    u32 y;
    UnkGuiDelta_71011b3840& operator=(const UnkGuiDelta_71011b3840& o) {
        x = o.x;
        y = o.y;
        return *this;
    }
};

struct UnkGuiResource_71011b3840 {
    u8 pad_0x0[0x10];
    void* field_0x10;
};

struct UnkGuiManager_71011b3840 {
    // Offsets checked below.
    u8 pad_0x0[0x68];
    void** field_0x68;  // an object; its slot 0xb0 takes a word
    u8 pad_0x70[0x80];
    bool field_0xf0;
    u8 pad_0xf1[0xcf];
    u32 field_0x1c0;
    u8 pad_0x1c4[0x6c];
    UnkGuiResource_71011b3840* field_0x230;
    u8 pad_0x238[0xa38];
    u32 field_0xc70;
    u8 pad_0xc74[0x4];
    u8 field_0xc78;
    u8 pad_0xc79[0x3];
    UnkGuiDelta_71011b3840 prev_a;  // +0xc7c
    UnkGuiDelta_71011b3840 cur_a;   // +0xc84
    u32 field_0xc8c;
    UnkGuiDelta_71011b3840 prev_b;  // +0xc90
    UnkGuiDelta_71011b3840 cur_b;   // +0xc98
    u8 pad_0xca0[0x8];
    u8 field_0xca8[0xb0];
    bool field_0xd58;
    u8 pad_0xd59[0xd7];
    bool field_0xe30;
};

static_assert(__builtin_offsetof(UnkGuiManager_71011b3840, field_0xe30) == 0xe30, "");

void fn_71011b3990(UnkGuiManager_71011b3840* self, f32 dt);
void fn_71011b5050(UnkGuiManager_71011b3840* self);
void fn_71011b51c4(UnkGuiManager_71011b3840* self, f32 dt);

void fn_71011b3840(UnkGuiManager_71011b3840* self, f32 dt) {
    char* gm = reinterpret_cast<char*>(g_unk_7101d47310);
    f32 scale = *reinterpret_cast<f32*>(gm + 0x6a4);
    if (scale > 0)
        dt = dt / scale;
    *reinterpret_cast<u64*>(&self->cur_a) = 0;
    *reinterpret_cast<u64*>(&self->cur_b) = 0;
    if (!self->field_0xc78) {
        if (!self->field_0xe30) {
            if (fn_710002fb70(self->field_0xca8) && !self->field_0xd58)
                goto done;
            if (fn_71012037dc(GetOwner()) == 1)
                goto done;
            const char* p = *reinterpret_cast<const char**>(reinterpret_cast<char*>(g_unk_7101d47310) + 0x5d0);
            if (p && *p)
                goto done;
        }
        fn_71011b3990(self, dt);
    }
done:
    self->prev_a = self->cur_a;
    self->prev_b = self->cur_b;
    self->field_0xc78 = 0;
    self->field_0xc70 = 0;
    if (!self->field_0x230 || !self->field_0x230->field_0x10) {
        if (self->field_0x1c0)
            fn_71011b5050(self);
    }
    if (self->field_0xf0) {
        u32 tmp = 0;
        reinterpret_cast<void (*)(void**, u32*)>((*reinterpret_cast<void***>(self->field_0x68))[0xb0 / 8])(
            self->field_0x68, &tmp);
        self->field_0xf0 = false;
    }
    fn_71011b51c4(self, dt);
}
