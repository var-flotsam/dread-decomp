// In unit 437 (CAimComponent's neighbourhood): the aim component's constructor. Builds the state
// machine at +0x40 with four states (ids 1 to 4, each with its callbacks), the "AIM_INPUT" input
// listener, two listeners, the string ids and the reference list, then requests state 1.
// Names and layouts are placeholders.
#include <new>
#include "base/UnkNamedRegistry.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflValue.h"
#include "game/CAbilityEnergyWave.h"
#include "game/CAbilityGhostAura.h"
#include "game/CAbilityLayout.h"
#include "game/CAbilityOpticCamouflage.h"
#include "game/CComponent.h"
#include "game/UnkComponent.h"
#include "game/UnkTU39.h"
#include "types.h"

using base::global::CStrId;

extern void* g_unk_7101a23ab0[];
extern void* g_unk_7101a23d08[];
extern void* g_unk_7101a23db8[];

struct UnkStateObject;
struct UnkMoveSlot_0xb0;
void fn_71000309ac(UnkMoveSlot_0xb0* self);

// The state callbacks (this unit's thunks into the component).
void fn_7100354634(void* self);
void fn_71003546d8(void* self);
void fn_7100354754(void* self);
void fn_7100354778(void* self);
void fn_710035477c(void* self);
void fn_7100354788(void* self);
void fn_71003547f0(void* self);
void fn_71003547f4(void* self);
void fn_71003547f8(void* self);
void fn_71003547fc(void* self);
void fn_7100354800(void* self);
void fn_7100354804(void* self);
void fn_7100354808(void* self);

struct UnkAimDelegate_l343054 {
    void* object;
    void (*function)(void* object);

    void Set(void* o, void (*f)(void*)) {
        object = o;
        function = f;
    }
};

struct UnkAimMachine_l343054;

// A state of the machine (0x70 bytes): its id and four callbacks.
struct UnkAimState_l343054 {
    void** vtable;
    UnkAimMachine_l343054* machine;  // 0x8
    u32 id;                          // 0x10
    base::reflection::UnkReflValue value;  // 0x18
    UnkAimDelegate_l343054 on_enter;   // 0x30
    UnkAimDelegate_l343054 on_leave;   // 0x40
    UnkAimDelegate_l343054 on_update;  // 0x50
    UnkAimDelegate_l343054 on_event;   // 0x60

    void Init(u32 state_id) {
        vtable = g_unk_7101a23d08 + 2;
        machine = nullptr;
        id = state_id;
        value.field_0x0 = nullptr;
        value.type = nullptr;
        value.kind = 2;
        value.field_0x14 = 1;
        on_enter.Set(nullptr, nullptr);
        on_leave.Set(nullptr, nullptr);
        on_update.Set(nullptr, nullptr);
        on_event.Set(nullptr, nullptr);
    }

    void Entered(s32 previous) {
        typedef void (*Fn)(UnkAimState_l343054*, s32);
        reinterpret_cast<Fn>(vtable[2])(this, previous);
    }
    void Left(s32 next) {
        typedef void (*Fn)(UnkAimState_l343054*, s32);
        reinterpret_cast<Fn>(vtable[3])(this, next);
    }
};

typedef UnkAimState_l343054* __attribute__((aligned(4))) UnkAimStatePtr_l343054;

struct UnkAimMachine_l343054 {
    void** vtable;
    void* owner;                    // 0x8
    UnkAimState_l343054* current;   // 0x10
    UnkAimState_l343054* previous;  // 0x18
    u32 count;                      // 0x20
    UnkAimStatePtr_l343054 states[16];
    s32 pending;  // 0xa4
    bool busy;    // 0xa8
    bool field_0xa9;

    void AfterEnter(UnkAimState_l343054* prev, UnkAimState_l343054* next) {
        typedef void (*Fn)(UnkAimMachine_l343054*, UnkAimState_l343054*, UnkAimState_l343054*);
        reinterpret_cast<Fn>(vtable[4])(this, prev, next);
    }
    void AfterLeave(UnkAimState_l343054* cur, UnkAimState_l343054* next) {
        typedef void (*Fn)(UnkAimMachine_l343054*, UnkAimState_l343054*, UnkAimState_l343054*);
        reinterpret_cast<Fn>(vtable[5])(this, cur, next);
    }
};

static inline UnkAimState_l343054* FindState(UnkAimMachine_l343054* self, u32 id) {
    u64 bytes = (u64)self->count * sizeof(UnkAimStatePtr_l343054);
    for (UnkAimStatePtr_l343054* p = self->states; bytes != 0; ++p, bytes -= sizeof(UnkAimStatePtr_l343054)) {
        if ((*p)->id == id)
            return *p;
    }
    return nullptr;
}

static inline void AddState(UnkAimMachine_l343054* self, UnkAimState_l343054* state) {
    if (FindState(self, state->id))
        return;
    state->machine = self;
    self->states[self->count] = state;
    ++self->count;
}

static inline void RequestState(UnkAimMachine_l343054* self, s32 id) {
    self->pending = id;
    if (self->busy)
        return;
    UnkAimState_l343054* cur = self->current;
    if (!cur || cur->id != (u32)id) {
        if (UnkAimState_l343054* next = FindState(self, id)) {
            if (cur) {
                cur->Left(id);
                self->AfterLeave(self->current, next);
            }
            self->previous = self->current;
            self->current = next;
            next->Entered(self->previous ? self->previous->id : 5);
            self->AfterEnter(self->previous, next);
        }
    }
    self->pending = 5;
}

// A listener with a bound callback and the counted link it registers through (+0x18).
struct UnkAimListener_l343054 {
    void** vtable;
    UnkAimDelegate_l343054 callback;  // 0x8
    void* link;                       // 0x18
};

struct UnkAimTarget_l343054;
struct UnkAimRef_l343054;
struct UnkAimRefPtr_l343054 {
    UnkAimRef_l343054* ptr;
};

struct UnkAim_l343054 : UnkBaseComponent {
    UnkAimMachine_l343054 machine;  // 0x40
    UnkAimState_l343054 state_1;    // 0xf0
    UnkAimState_l343054 state_2;    // 0x160
    UnkAimState_l343054 state_3;    // 0x1d0
    UnkAimState_l343054 state_4;    // 0x240
    u64 field_0x2b0;
    u64 field_0x2b8;
    f32 field_0x2c0;
    f32 field_0x2c4;
    bool field_0x2c8;
    f32 field_0x2cc;
    u8 pad_0x2d0[4];
    bool field_0x2d4;
    u32 field_0x2d8;
    UnkAbilityInputListener input;  // 0x2e0
    UnkAimTarget_l343054* target;   // 0x328
    u64 field_0x330;
    u64 field_0x338;
    u32 field_0x340;
    u16 field_0x344;
    u64 field_0x348;
    u64 field_0x350;
    u32 field_0x358;
    u32 field_0x35c;
    bool field_0x360;
    bool field_0x361;
    bool field_0x362;
    u8 pad_0x363[5];
    u8 field_0x368[0xb0];
    u8 field_0x418[0xb0];
    bool field_0x4c8;
    f32 field_0x4cc;
    f32 field_0x4d0;
    u32 field_0x4d4;
    bool field_0x4d8;
    CStrId field_0x4e0;
    CStrId field_0x4e8;
    u64 field_0x4f0[7];
    bool field_0x528;
    UnkAimListener_l343054 listener_a;  // 0x530
    UnkAimListener_l343054 listener_b;  // 0x550
    bool field_0x570;
    u32 field_0x574;
    u32 field_0x578;
    u64 field_0x580[7];
    u32 field_0x5b8;
    CStrId field_0x5c0;
    u64 field_0x5c8[11];
    u32 field_0x620;
    u32 field_0x628;
    bool field_0x62c;
    void* field_0x630;
    void* field_0x638;
    bool field_0x640;
    bool field_0x641;
    base::global::CRntVector<UnkAimRefPtr_l343054> refs;  // 0x648
    u64 field_0x670;
    bool field_0x678;
    u32 field_0x67c;
    u64 field_0x680;
    u32 field_0x688;
    u32 field_0x68c;
    bool field_0x690;
    bool field_0x691;
    bool field_0x692;
    u32 field_0x694;
    u64 field_0x698;
    u32 field_0x6a0;
    u32 field_0x6a4[16];
    u8 pad_0x6e4[4];
    u8 field_0x6e8[0xb0];
    u8 field_0x798[0xb0];
    u32 field_0x848;
    u8 pad_0x84c[0x950 - 0x84c];
    u64 field_0x950;
    f32 field_0x958[4];
    bool field_0x968;
    bool field_0x969;
    UnkAimRef_l343054* field_0x970;
    u64 name;  // 0x978
    u64 field_0x980;
    u64 field_0x988;
    bool field_0x990;
};
static_assert(__builtin_offsetof(UnkAim_l343054, state_1) == 0xf0, "layout");
static_assert(__builtin_offsetof(UnkAim_l343054, input) == 0x2e0, "layout");
static_assert(__builtin_offsetof(UnkAim_l343054, field_0x35c) == 0x35c, "layout");
static_assert(__builtin_offsetof(UnkAim_l343054, field_0x368) == 0x368, "layout");
static_assert(__builtin_offsetof(UnkAim_l343054, field_0x4e0) == 0x4e0, "layout");
static_assert(__builtin_offsetof(UnkAim_l343054, listener_a) == 0x530, "layout");
static_assert(__builtin_offsetof(UnkAim_l343054, field_0x574) == 0x574, "layout");
static_assert(__builtin_offsetof(UnkAim_l343054, field_0x5c0) == 0x5c0, "layout");
static_assert(__builtin_offsetof(UnkAim_l343054, refs) == 0x648, "layout");
static_assert(__builtin_offsetof(UnkAim_l343054, field_0x694) == 0x694, "layout");
static_assert(__builtin_offsetof(UnkAim_l343054, field_0x6a4) == 0x6a4, "layout");
static_assert(__builtin_offsetof(UnkAim_l343054, field_0x6e8) == 0x6e8, "layout");
static_assert(__builtin_offsetof(UnkAim_l343054, field_0x950) == 0x950, "layout");
static_assert(__builtin_offsetof(UnkAim_l343054, field_0x990) == 0x990, "layout");

static inline void InitListener(UnkAimListener_l343054* listener, void** vtable) {
    listener->vtable = vtable;
    listener->callback.Set(nullptr, nullptr);
    listener->link = nullptr;
}

void fn_7100343054(UnkAim_l343054* self) {
    fn_71010c4148(reinterpret_cast<UnkStateObject*>(self));
    self->vtable = g_unk_7101a23ab0 + 2;

    UnkAimMachine_l343054* machine = &self->machine;
    machine->vtable = g_unk_7101a23db8 + 2;
    machine->owner = nullptr;
    machine->current = nullptr;
    machine->previous = nullptr;
    machine->count = 0;
    machine->pending = 5;
    machine->busy = false;
    machine->field_0xa9 = false;
    self->state_1.Init(1);
    self->state_2.Init(2);
    self->state_3.Init(3);
    self->state_4.Init(4);
    self->field_0x2b0 = 0;
    self->field_0x2b8 = 0;
    self->field_0x2c0 = -1.0f;
    self->field_0x2c4 = -1.0f;
    self->field_0x2c8 = false;
    self->field_0x2cc = 1.5707964f;
    self->field_0x2d4 = false;
    self->field_0x2d8 = 0;

    self->input.vtable = g_unk_7101a22878 + 2;
    CRntString_ConstructFromCString(&self->input.name, "AIM_INPUT");
    self->input.capacity = 20;
    // An array of 20 0x1c-byte entries, after its element count.
    u64* block = static_cast<u64*>(fn_7100083ba0(0x238, 5));
    *block = 20;
    self->input.entries = block + 1;
    self->input.vtable = g_unk_7101a22858 + 2;
    self->target = nullptr;
    self->field_0x330 = 0;
    self->field_0x338 = 0;
    self->field_0x340 = 0;
    self->field_0x344 = 0;
    self->field_0x348 = 0;
    self->field_0x350 = 0;
    self->field_0x358 = 0;
    self->field_0x35c = 0;
    self->field_0x360 = false;
    self->field_0x361 = false;
    self->field_0x362 = false;
    fn_71000309ac(reinterpret_cast<UnkMoveSlot_0xb0*>(self->field_0x368));
    fn_71000309ac(reinterpret_cast<UnkMoveSlot_0xb0*>(self->field_0x418));
    self->field_0x4c8 = false;
    self->field_0x4cc = -1.0f;
    self->field_0x4d0 = -1.0f;
    self->field_0x4d4 = 0;
    self->field_0x4d8 = false;
    new (&self->field_0x4e0) CStrId(base::global::UnkStrIdInline(), "", true);
    new (&self->field_0x4e8) CStrId(base::global::UnkStrIdInline(), "", true);
    for (u64& v : self->field_0x4f0)
        v = 0;
    self->field_0x528 = false;
    InitListener(&self->listener_a, g_unk_7101a23280 + 2);
    InitListener(&self->listener_b, g_unk_7101a22d48 + 2);
    self->field_0x570 = false;
    self->field_0x574 = 0;
    self->field_0x578 = 0;
    for (u64& v : self->field_0x580)
        v = 0;
    self->field_0x5b8 = 0;
    new (&self->field_0x5c0) CStrId(g_unk_710162e2ed, true);
    for (u64& v : self->field_0x5c8)
        v = 0;
    self->field_0x620 = 0;
    self->field_0x628 = 0;
    self->field_0x62c = false;
    self->field_0x630 = nullptr;
    self->field_0x638 = nullptr;
    self->field_0x640 = false;
    self->field_0x641 = false;
    new (&self->refs) base::global::CRntVector<UnkAimRefPtr_l343054>(0xf);
    self->field_0x670 = 0;
    self->field_0x678 = false;
    self->field_0x67c = 0;
    self->field_0x680 = 0;
    self->field_0x688 = 0;
    self->field_0x68c = 0;
    self->field_0x690 = false;
    self->field_0x691 = false;
    self->field_0x692 = true;
    self->field_0x694 = 0;
    self->field_0x698 = 0;
    self->field_0x6a0 = 0;
    for (u32& v : self->field_0x6a4)
        v = 0;
    fn_710002f4e4(self->field_0x6e8);
    fn_710002f4e4(self->field_0x798);
    self->field_0x848 = 0;
    self->field_0x950 = 0;
    self->field_0x958[0] = 1.0f;
    self->field_0x958[1] = 1.0f;
    self->field_0x958[2] = 1.0f;
    self->field_0x958[3] = 1.0f;
    self->field_0x968 = false;
    self->field_0x969 = false;
    self->field_0x970 = nullptr;
    self->name = fn_7100001570(g_unk_710162e2ed, 0);
    self->field_0x980 = 0;
    self->field_0x988 = 0;
    self->field_0x990 = false;

    self->listener_a.callback.Set(self, fn_7100354634);
    self->listener_b.callback.Set(self, fn_71003546d8);
    machine->owner = self;
    AddState(machine, &self->state_1);
    self->state_1.on_update.Set(self, fn_7100354754);
    self->state_1.on_event.Set(self, fn_7100354778);
    AddState(machine, &self->state_2);
    self->state_2.on_enter.Set(self, fn_710035477c);
    self->state_2.on_leave.Set(self, fn_7100354788);
    self->state_2.on_update.Set(self, fn_71003547f0);
    self->state_2.on_event.Set(self, fn_71003547f4);
    AddState(machine, &self->state_3);
    self->state_3.on_enter.Set(self, fn_71003547f8);
    self->state_3.on_leave.Set(self, fn_71003547fc);
    self->state_3.on_update.Set(self, fn_7100354800);
    self->state_3.on_event.Set(self, fn_7100354804);
    AddState(machine, &self->state_4);
    self->state_4.on_update.Set(self, fn_7100354808);
    RequestState(machine, 1);
}
