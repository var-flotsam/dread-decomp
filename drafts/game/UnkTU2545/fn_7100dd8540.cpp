// Unit 2545 (tools/tu_map.py): adds the elapsed time to the flow's timer and, until it has answered once,
// polls for accept (state 1) or back (a confirmation pop-up, or state 7 when nothing is at stake), else
// pulses the two prompt widgets. Names and layouts are placeholders.
#include <math.h>

#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "game/GUI/CPopUpComposition.h"
#include "game/UnkGameManager.h"
#include "game/UnkGuiDisplayObject.h"
#include "game/UnkStateMachineFind.h"
#include "game/UnkTU0.h"
#include "game/UnkTU32.h"
#include "game/nn/time.h"
#include "game/sound/CSoundManager.h"
#include "types.h"

using base::global::CStrId;
using UnkGuiObj = UnkGuiDisplayObject;

struct UnkStrIdPathRecord_b48 {
    u8 data[0x18];
};
struct UnkIdPair;
struct UnkStrIdPathPair;

void fn_71011cfba8(UnkGuiObj* object, f32 alpha);

struct UnkFlow_ldd8540;
template <u32 Offset>
struct UnkSmOwner_b61;
void fn_7100dd905c(UnkSmOwner_b61<0>* self);
void fn_7100dd9150(UnkFlow_ldd8540* self);

// Set once the prompt has been answered.
extern bool g_unk_71727bdf80 __attribute__((visibility("hidden")));
// The accept and back sounds.
extern UnkStrIdPathRecord_b48 g_unk_71727bdfb8 __attribute__((visibility("hidden")));
extern UnkStrIdPathRecord_b48 g_unk_71727bdfc0 __attribute__((visibility("hidden")));
// The pop-up's accept, back and question texts.
extern CStrId g_unk_71727bdfc8 __attribute__((visibility("hidden")));
extern CStrId g_unk_71727bdfd0 __attribute__((visibility("hidden")));
extern CStrId g_unk_71727bdfd8 __attribute__((visibility("hidden")));

struct UnkCopiedIds_ldd8540 {
    UnkStrIdPathRecord_b48 record;
    UnkCopiedIds_ldd8540(const UnkStrIdPathRecord_b48* src) { fn_7100027464(&record, src); }
    ~UnkCopiedIds_ldd8540() { fn_7100026f70(reinterpret_cast<UnkIdPair*>(this)); }
    const UnkStrIdPathPair* Pair() const { return reinterpret_cast<const UnkStrIdPathPair*>(this); }
};

static inline void PlaySound(const UnkStrIdPathRecord_b48* sound) {
    fn_7101024e28(g_unk_7101d47318, UnkCopiedIds_ldd8540(sound).Pair(), nullptr, false);
}

struct UnkInput_ldd8540 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual bool IsPressed(u32 button, u32 mask);
};

static inline UnkSoundHolder* SoundHolder() {
    if (!g_unk_7101d47340) {
        auto* h = static_cast<UnkSoundHolder*>(fn_71000839a0(0x48, 0x3f));
        fn_710006cd9c(h);
        g_unk_7101d47340 = h;
    }
    return g_unk_7101d47340;
}

struct UnkFlowState_ldd8540 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10(s32 previous);  // entered
    virtual void vf_0x18(s32 next);      // left
    u64 field_0x8;
    s32 id;
};

// The states are only 4-aligned after the count.
typedef UnkFlowState_ldd8540* __attribute__((aligned(4))) UnkFlowStatePtr_ldd8540;

struct UnkFlowMachine_ldd8540 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20(UnkFlowState_ldd8540* previous, UnkFlowState_ldd8540* next);  // after entering
    virtual void vf_0x28(UnkFlowState_ldd8540* current, UnkFlowState_ldd8540* next);   // after leaving

    u64 field_0x8;
    UnkFlowState_ldd8540* current;
    UnkFlowState_ldd8540* previous;
    u32 count;
    UnkFlowStatePtr_ldd8540 states[16];
    s32 pending;
    bool busy;

    static const s32 kNoState = 9;

    UnkFlowState_ldd8540* FindState(s32 id) { return FindStateById(states, count, id); }

    void RequestState(s32 id) {
        pending = id;
        if (busy)
            return;
        UnkFlowState_ldd8540* cur = current;
        if (!cur || cur->id != id) {
            if (UnkFlowState_ldd8540* next = FindState(id)) {
                if (cur) {
                    cur->vf_0x18(id);
                    vf_0x28(current, next);
                }
                previous = current;
                current = next;
                next->vf_0x10(previous ? previous->id : kNoState);
                vf_0x20(previous, next);
            }
        }
        pending = kNoState;
    }
};

struct UnkFlow_ldd8540 : UnkFlowMachine_ldd8540 {
    u32 field_0xac;
    f32 time;
    s32 field_0xb4;
    f32 field_0xb8;
    bool field_0xbc;
    u8 pad_0xbd[0x178 - 0xbd];
    UnkGuiObj* field_0x178;
    UnkGuiObj* field_0x180;
    u8 pad_0x188[0x269 - 0x188];
    bool skip;
};
static_assert(__builtin_offsetof(UnkFlow_ldd8540, time) == 0xb0, "layout");
static_assert(__builtin_offsetof(UnkFlow_ldd8540, skip) == 0x269, "layout");
static_assert(__builtin_offsetof(UnkFlowMachine_ldd8540, pending) == 0xa4, "layout");

// A callback: an object and a function to call on it.
struct UnkFlowCallback_ldd8540 {
    UnkFlow_ldd8540* object;
    void (*function)(UnkFlow_ldd8540*);
};

struct UnkGuiRef_ldd8540 {
    u8 pad_0x0[0x10];
    UnkGuiObj* composition;  // 0x10
};

static inline f32 Clamp01(f32 v) {
    return v <= 0.0f ? 0.0f : (!(v >= 1.0f) ? v : 1.0f);
}

void fn_7100dd8540(UnkFlow_ldd8540* self, f32 dt) {
    self->time += dt;
    if (g_unk_71727bdf80)
        return;
    bool idle = self->field_0xb8 < 0.001f && self->field_0xb4 == 0;
    fn_71011cdc30(self->field_0x178, true);
    fn_71011cdc30(self->field_0x180, true);
    auto* input = *reinterpret_cast<UnkInput_ldd8540**>(SoundHolder());
    bool accept = input->IsPressed(1, 0xfd);
    bool back = input->IsPressed(2, 0xfd);
    if ((accept || back) && !self->skip) {
        if (accept) {
            PlaySound(&g_unk_71727bdfb8);
            self->RequestState(1);
            return;
        }
        if (!self->field_0xbc && !idle) {
            UnkGuiRef_ldd8540* ref = static_cast<UnkGuiRef_ldd8540*>(g_unk_7101d47310->field_0x2258);
            UnkGuiObj* gui = ref ? ref->composition : nullptr;
            fn_71011cdce8(gui, true);
            UnkFlowCallback_ldd8540 accept_callback = {self, reinterpret_cast<void (*)(UnkFlow_ldd8540*)>(fn_7100dd905c)};
            UnkFlowCallback_ldd8540 back_callback = {self, fn_7100dd9150};
            fn_7100b5fde0(reinterpret_cast<UnkGuiComposition_ovb0090*>(gui), &g_unk_71727bdfd8, &g_unk_71727bdfc8,
                          &g_unk_71727bdfd0, reinterpret_cast<const UnkMapCallback_ovb0090*>(&accept_callback),
                          reinterpret_cast<const UnkMapCallback_ovb0090*>(&back_callback), 0, 1.0f);
        } else {
            self->RequestState(7);
            PlaySound(&g_unk_71727bdfc0);
        }
        g_unk_71727bdf80 = true;
        return;
    }

    f32 t = self->time * 2.0f;
    f32 alpha = Clamp01(t);
    if (t >= 1.0f)
        alpha -= Clamp01(fabsf(sinf(t - 1.0f))) * 0.5f;
    fn_71011cfba8(self->field_0x178, alpha);
    fn_71011cfba8(self->field_0x180, alpha);
    self->skip = false;
}
