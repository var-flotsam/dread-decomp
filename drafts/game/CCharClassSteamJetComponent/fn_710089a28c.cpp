// CCharClassSteamJetComponent unit (tools/tu_map.py): the update. While the jet runs (+0x1fc) it advances its
// timer and passes the progress to its own vtable slot 0x170; then it either sets a level from two actors'
// heights or eases the current level (+0xe0) towards a target and hands it to the jet actor's component.
// Names and layouts are placeholders.
#include "base/UnkRefHandle.h"
#include "game/CComponent.h"
#include "game/CPullOffGrapplePointComponent.h"
#include "game/UnkTU437.h"
#include "types.h"

struct UnkParent;

struct UnkSteamJetActor_l89a28c {
    u8 pad_0x0[0x54];
    f32 height;  // 0x54
};

struct UnkCountedActor_l89a28c : base::UnkRefCounted {
    base::UnkRefHandle object;  // 0x10
    UnkSteamJetActor_l89a28c* Actor() const { return reinterpret_cast<UnkSteamJetActor_l89a28c*>(object.ptr); }
};

// A handle resolved from its source on first use.
struct UnkLazyHandle_l89a28c {
    u8 source[0x28];            // 0x0
    u32 state;                  // 0x28
    u32 pad_0x2c;
    base::UnkRefHandle handle;  // 0x30
};

// A component that shows a level (vtable slot 0x170).
struct UnkLevelView_l89a28c {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual void vf_0x40();
    virtual void vf_0x48();
    virtual void vf_0x50();
    virtual void vf_0x58();
    virtual void vf_0x60();
    virtual void vf_0x68();
    virtual void vf_0x70();
    virtual void vf_0x78();
    virtual void vf_0x80();
    virtual void vf_0x88();
    virtual void vf_0x90();
    virtual void vf_0x98();
    virtual void vf_0xa0();
    virtual void vf_0xa8();
    virtual void vf_0xb0();
    virtual void vf_0xb8();
    virtual void vf_0xc0();
    virtual void vf_0xc8();
    virtual void vf_0xd0();
    virtual void vf_0xd8();
    virtual void vf_0xe0();
    virtual void vf_0xe8();
    virtual void vf_0xf0();
    virtual void vf_0xf8();
    virtual void vf_0x100();
    virtual void vf_0x108();
    virtual void vf_0x110();
    virtual void vf_0x118();
    virtual void vf_0x120();
    virtual void vf_0x128();
    virtual void vf_0x130();
    virtual void vf_0x138();
    virtual void vf_0x140();
    virtual void vf_0x148();
    virtual void vf_0x150();
    virtual void vf_0x158();
    virtual void vf_0x160();
    virtual void vf_0x168();
    virtual void SetLevel(f32 value);  // 0x170
};

struct UnkSteamJet_l89a28c : UnkLevelView_l89a28c {
    u8 pad_0x8[0xe0 - 0x8];
    f32 level;        // 0xe0
    f32 start;        // 0xe4
    f32 target;       // 0xe8
    f32 duration;     // 0xec
    f32 elapsed;      // 0xf0
    u8 pad_0xf4[0x100 - 0xf4];
    UnkLazyHandle_l89a28c base_actor;  // 0x100
    u8 pad_0x138[0x140 - 0x138];
    UnkLazyHandle_l89a28c jet_actor;   // 0x140
    u8 pad_0x178[0x1f4 - 0x178];
    f32 scale;        // 0x1f4
    u8 pad_0x1f8[0x1fc - 0x1f8];
    bool running;     // 0x1fc
    f32 total;        // 0x200
    f32 time;         // 0x204
    bool from_actors; // 0x208
};
static_assert(__builtin_offsetof(UnkSteamJet_l89a28c, jet_actor) == 0x140, "layout");
static_assert(__builtin_offsetof(UnkSteamJet_l89a28c, from_actors) == 0x208, "layout");

static inline void ResolveBase(UnkSteamJet_l89a28c* self) {
    if (self->base_actor.state == 1) {
        {
            base::UnkRefPtrMember fresh = fn_7100353be8(self->base_actor.source);
            fn_710031d208(&self->base_actor.handle, reinterpret_cast<const base::UnkRefHandle*>(&fresh));
        }
        self->base_actor.state = 2;
    }
}

static inline void ResolveJet(UnkSteamJet_l89a28c* self) {
    if (self->jet_actor.state == 1) {
        {
            base::UnkRefPtrMember fresh = fn_7100353be8(self->jet_actor.source);
            fn_710031d208(&self->jet_actor.handle, reinterpret_cast<const base::UnkRefHandle*>(&fresh));
        }
        self->jet_actor.state = 2;
    }
}

static inline UnkSteamJetActor_l89a28c* JetActor(UnkSteamJet_l89a28c* self) {
    ResolveJet(self);
    auto* c = static_cast<UnkCountedActor_l89a28c*>(self->jet_actor.handle.ptr);
    return c ? c->Actor() : nullptr;
}

static inline UnkCountedActor_l89a28c* JetHandle(UnkSteamJet_l89a28c* self) {
    return static_cast<UnkCountedActor_l89a28c*>(self->jet_actor.handle.ptr);
}

static inline UnkLevelView_l89a28c* ViewOf(UnkSteamJetActor_l89a28c* actor) {
    return static_cast<UnkLevelView_l89a28c*>(fn_710082e510(actor));
}

static inline void SetJetLevel(UnkSteamJet_l89a28c* self, f32 value) {
    ResolveJet(self);
    if (JetHandle(self) && JetHandle(self)->Actor()) {
        if (UnkLevelView_l89a28c* view = ViewOf(JetHandle(self)->Actor()))
            view->SetLevel(value);
    }
}

void fn_710089a28c(UnkSteamJet_l89a28c* self, f32 dt) {
    fn_71010c424c(reinterpret_cast<UnkParent*>(self));
    if (!self->running)
        return;
    self->time += dt;
    self->SetLevel(self->time / self->total);
    if (self->from_actors) {
        ResolveJet(self);
        if (!JetHandle(self) || !JetHandle(self)->Actor())
            return;
        f32 top = JetHandle(self)->Actor()->height;
        ResolveBase(self);
        f32 bottom = static_cast<UnkCountedActor_l89a28c*>(self->base_actor.handle.ptr)->Actor()->height;
        f32 level = (top - bottom) / self->scale;
        self->start = level;
        self->target = level;
        self->level = level;
        if (UnkLevelView_l89a28c* view = ViewOf(JetActor(self)))
            view->SetLevel(self->level);
        return;
    }
    f32 level = self->level;
    f32 target = self->target;
    if (level - 1e-5f <= target && level + 1e-5f >= target) {
        SetJetLevel(self, 0.0f);
        self->SetLevel(1.0f);
        self->running = false;
        return;
    }
    self->elapsed += dt;
    f32 value = target;
    if (self->elapsed < self->duration) {
        f32 t = self->elapsed / self->duration;
        t = !(t >= 1.0f) ? t : 1.0f;
        value = self->start + t * (target - self->start);
    }
    self->level = value;
    SetJetLevel(self, value);
}
