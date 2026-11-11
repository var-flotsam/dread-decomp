// CAccessPointCommanderComponent's unit (tools/tu_map.py): the destructor (fn_7100a6993c), an event handler
// (fn_7100a6a4bc) and a step that waits for the access point (fn_7100a6a64c). The component keeps a handle
// source at +0x9a0 (a base vtable, an array of callables, a state and the lazily resolved handle at +0x9d8),
// a started flag (+0x9e0) and two registrations (+0x9e8, +0xa00: the registration, this component and a
// callback). Names and layouts are placeholders.
#include "base/UnkDestructors.h"
#include "base/UnkRefHandle.h"
#include "base/UnkVector.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "game/UnkGameManager.h"
#include "game/UnkGuiRoot.h"
#include "game/UnkTU2656.h"
#include "types.h"

using base::global::CRntString;
using base::global::CStrId;

void* fn_71004067a4(void* owner);
void fn_71005c4828(void*, void**);
void fn_7100a6b0f0(void* registration, void** slot);
void fn_7100a6b280(void* registration, void** slot);
void fn_7100a6ce48(void* self);  // the base destructor
extern char g_unk_7101a8fa30[];   // the component's vtable
extern void* g_unk_7101a20998[];  // the handle source's base vtable
extern void* g_unk_7101a33fa0[];  // the handle source's vtable
extern void* g_unk_7101d47360;
void fn_7100ee1f34(void*);
// As src/game/CCreditsMode/CTunableCredits/CTunableCredits_Colors_ovb0070.cpp declares it.
void* fn_7100deab70(UnkGameManager*, const CRntString&);

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

// As src/game/CChozoWarriorXUltimateGrabAttack/fn_710048a194.cpp declares it.
struct UnkVec;

struct UnkGfxFrameSource;
extern UnkGfxFrameSource* g_unk_7101d47328;

static inline f32 FrameTime() {
    return *reinterpret_cast<const f32*>(reinterpret_cast<const u8*>(g_unk_7101d47328) + 0x30);
}

struct UnkAccessPointTarget_ovb0085;
bool fn_71010692a0(UnkAccessPointTarget_ovb0085* self, bool flag);
extern void* g_unk_717274eaa8 __attribute__((visibility("hidden")));  // the event kind handled
extern CStrId g_unk_717274eab8 __attribute__((visibility("hidden")));

struct UnkSubAreaManager_ovb0085 {
    u8 pad_0x0[0xae0];
    bool field_0xae0;
};

static inline UnkSubAreaManager_ovb0085* SubAreaManager() {
    void* manager = g_unk_7101d47360;
    if (!manager) {
        manager = fn_71000839a0(0xae8, 0x3f);
        fn_7100ee1f34(manager);
        g_unk_7101d47360 = manager;
    }
    return static_cast<UnkSubAreaManager_ovb0085*>(manager);
}

// A callable entry of the handle source: its storage and the table of operations (+0x40 destroys it).
struct UnkAccessPointCallable_ovb0085 {
    u64 storage;
    void (**ops)(UnkAccessPointCallable_ovb0085* self);

    ~UnkAccessPointCallable_ovb0085() {
        if (ops) {
            ops[0x40 / 8](this);
            ops = nullptr;
        }
    }
};

struct UnkAccessPointSource_ovb0085 {
    void* vtable;
    base::UnkVector<UnkAccessPointCallable_ovb0085> items;
    u32 state;
    union {
        base::UnkRefPtrMember handle;    // 0x38
        base::UnkRefHandle handle_slot;  // the same pointer, as fn_710031d208 takes it
    };

    __attribute__((always_inline)) void Destroy() {
        vtable = g_unk_7101a33fa0 + 2;
        handle.~UnkRefPtrMember();
        vtable = g_unk_7101a20998 + 2;
        items.Free();
    }
};

struct UnkAccessPointListener_ovb0085 {
    void* registration;
    void* target;
    void (*callback)();
};

struct UnkAccessPointComponent_ovb0085 {
    u8 pad_0x0[0x200];
    UnkAccessPointTarget_ovb0085* target;
};

struct UnkAccessPointEvent_ovb0085 {
    u8 pad_0x0[0x10];
    void* kind;
};

struct UnkVec3_ovb0085 {
    u64 xy;
    u32 z;
};

struct UnkAccessPointCommander_ovb0085 {
    void* vtable;
    u8 pad_0x8[0x9a0 - 0x8];
    UnkAccessPointSource_ovb0085 source;  // 0x9a0
    bool started;                        // 0x9e0
    bool field_0x9e1;
    UnkAccessPointListener_ovb0085 first;   // 0x9e8
    UnkAccessPointListener_ovb0085 second;  // 0xa00
    f32 field_0xa18;
    f32 field_0xa1c;

    // The handle, resolved from the source once (state 1 becomes 2), as fn_7100a6ab44 does.
    __attribute__((always_inline)) void Resolve() {
        if (source.state == 1) {
            {
                UnkRef fresh = fn_7100353be8(&source.items);
                fn_710031d208(&source.handle_slot, reinterpret_cast<const base::UnkRefHandle*>(&fresh));
            }
            source.state = 2;
        }
    }
};
static_assert(__builtin_offsetof(UnkAccessPointCommander_ovb0085, source.handle) == 0x9d8, "layout");
static_assert(__builtin_offsetof(UnkAccessPointCommander_ovb0085, field_0xa1c) == 0xa1c, "layout");

void fn_7100a6993c(UnkAccessPointCommander_ovb0085* self) {
    self->vtable = g_unk_7101a8fa30 + 0x10;
    if (self->second.registration) {
        fn_7100a6b0f0(self->second.registration, &self->second.registration);
        self->second.registration = nullptr;
    }
    self->second.callback = nullptr;
    self->second.target = nullptr;
    if (self->first.registration) {
        fn_71005c4828(self->first.registration, &self->first.registration);
        self->first.registration = nullptr;
    }
    self->first.callback = nullptr;
    self->first.target = nullptr;
    self->source.Destroy();
    fn_7100a6ce48(self);
}

// Only for the event kind g_unk_717274eaa8 (false otherwise): clears the sub-area manager's flag, and when
// the handle's object has the component fn_71004067a4 looks for and that component's object at +0x200
// accepts (fn_71010692a0), moves the registration at +0xa00 onto the "INGAME" GUI object's +0x1d0 and adds
// the key g_unk_717274eab8 to its list at +0x108.
bool fn_7100a6a4bc(UnkAccessPointCommander_ovb0085* self, UnkAccessPointEvent_ovb0085* event) {
    if (event->kind != g_unk_717274eaa8)
        return false;
    SubAreaManager()->field_0xae0 = false;
    self->Resolve();
    auto* c = static_cast<UnkCountedA5_b76*>(self->source.handle.ptr);
    if (c && c->raw_0x10) {
        asm("" ::: "memory");
        auto* component = static_cast<UnkAccessPointComponent_ovb0085*>(fn_71004067a4(c->field_0x10));
        if (component && component->target && fn_71010692a0(component->target, true)) {
            u8* gui = static_cast<u8*>(fn_7100deab70(g_unk_7101d47310, CRntString("INGAME")));
            if (gui) {
                void** slot = &self->second.registration;
                void* registration = gui + 0x1d0;
                if (*slot) {
                    fn_7100a6b0f0(*slot, slot);
                    *slot = nullptr;
                }
                *slot = registration;
                fn_7100a6b280(registration, slot);
                fn_710002fd10(reinterpret_cast<UnkGuiKeyList*>(gui + 0x108), &g_unk_717274eab8);
            }
        }
    }
    return true;
}

// Until it has started: when the handle's object has the component fn_71004067a4 looks for, sets +0xa18
// to 0.3, starts the game manager's effect at the origin (its duration 0.3 less three frames) and marks
// itself started. Always true.
bool fn_7100a6a64c(UnkAccessPointCommander_ovb0085* self) {
    if (self->started)
        return true;
    self->Resolve();
    auto* c = static_cast<UnkCountedA5_b76*>(self->source.handle.ptr);
    if (c && c->raw_0x10) {
        asm("" ::: "memory");
        if (fn_71004067a4(c->field_0x10)) {
            const f32 duration = 0.3f;
            self->field_0xa18 = duration;
            void* effects = g_unk_7101d47310->field_0x1040;
            f32 time = FrameTime() * -3.0f + duration;
            UnkVec3_ovb0085 origin;
            origin.xy = 0;
            origin.z = 0;
            fn_7101053850(effects, reinterpret_cast<UnkVec*>(&origin), 1, time, 1.0f);
            self->started = true;
        }
    }
    return true;
}
