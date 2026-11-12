// CAccessPointCommanderComponent unit (tools/tu_map.py, unit 1580): the start. Runs fn_7100a700ec, eases the game
// manager's three values over what is left of 13.5 (+0xa1c), then once (+0x9e0) resolves the lazily resolved
// handle at +0x9d8 (as fn_7100897f48), notifies its object, flags the sub-area manager around fn_7100eed85c
// and requests state 1 from the two state machines (+0x5e8, "none" 5; +0x100, "none" 8). Names and layouts are
// placeholders.
#include "../UnkSmRequest_A5_b73.h"
#include "base/UnkRefHandle.h"
#include "game/CCentralUnitAIComponent.h"
#include "game/CCutsceneComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2577.h"
#include "types.h"

void fn_7100a700ec(void* self);
void fn_7100e047f4(UnkGameManager* self, f32 a, f32 b);
void fn_7100e04814(UnkGameManager* self, f32 a, f32 b);
void fn_7100e04834(UnkGameManager* self, f32 a, f32 b);
struct UnkManager;
extern UnkManager* g_unk_7101d47360;
void fn_7100eed85c(void* self, void* key, bool a, bool b);
extern __attribute__((visibility("hidden"))) char g_unk_717274eab0[];

// As src/game/UnkStateMachineSetA5_b61.cpp declares them.
struct UnkCountedA5_b76 : base::UnkRefCounted {
    union {
        void* field_0x10;
        u64 raw_0x10;
    };
};

struct UnkRef {
    UnkCountedA5_b76* ptr;
    ~UnkRef() {
        if (ptr && --ptr->ref_count == 0)
            delete ptr;
    }
};

UnkRef fn_7100353be8(void* from);

struct UnkSubAreaManager_A5_b76 {
    u8 pad_0x0[0xae0];
    bool field_0xae0;
};

static inline UnkSubAreaManager_A5_b76* SubAreaManager_A5_b76() {
    void* manager = g_unk_7101d47360;
    if (!manager) {
        manager = fn_71000839a0(0xae8, 0x3f);
        fn_7100ee1f34(manager);
        g_unk_7101d47360 = static_cast<UnkManager*>(manager);
    }
    return static_cast<UnkSubAreaManager_A5_b76*>(manager);
}

struct UnkCommander_A5_b76 {
    u8 pad_0x0[0x100];
    UnkSmMachine_A5_b73 machine;
    u8 pad_0x1b0[0x5e8 - 0x1b0];
    UnkSmMachine_A5_b73 machine2;
    u8 pad_0x698[0x9a8 - 0x698];
    u8 source[0x28];
    u32 state;
    u32 pad_0x9d4;
    base::UnkRefHandle handle;
    bool started;
    u8 pad_0x9e1[0xa1c - 0x9e1];
    f32 field_0xa1c;
};
static_assert(__builtin_offsetof(UnkCommander_A5_b76, field_0xa1c) == 0xa1c, "layout");

void fn_7100a6ab44(UnkCommander_A5_b76* self) {
    fn_7100a700ec(self);
    f32 left = 13.5f - self->field_0xa1c;
    if (left > 0.0f) {
        fn_7100e047f4(g_unk_7101d47310, left, left);
        fn_7100e04814(g_unk_7101d47310, left, left);
        fn_7100e04834(g_unk_7101d47310, left, left);
        self->field_0xa1c = 13.5f;
    }
    if (self->started)
        return;
    self->started = true;
    if (self->state == 1) {
        {
            UnkRef fresh = fn_7100353be8(self->source);
            fn_710031d208(&self->handle, reinterpret_cast<const base::UnkRefHandle*>(&fresh));
        }
        self->state = 2;
    }
    auto* c = static_cast<UnkCountedA5_b76*>(self->handle.ptr);
    if (c && c->raw_0x10) {
        asm("" ::: "memory");
        if (void* object = fn_71004067a4(c->field_0x10))
            fn_71007a8280(object);
    }
    SubAreaManager_A5_b76()->field_0xae0 = false;
    fn_7100eed85c(SubAreaManager_A5_b76(), g_unk_717274eab0, true, true);
    SubAreaManager_A5_b76()->field_0xae0 = true;
    RequestState_A5_b73<15, 5>(&self->machine2, 1);
    RequestState_A5_b73<15, 8>(&self->machine, 1);
}
