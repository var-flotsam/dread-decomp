// CGrabComponent unit (tools/tu_map.py unit 2709, the name from reflection): the component's
// constructor (fn_710113adb4) and the constructor of its grab state (fn_710113af2c), the grab
// description's copy (fn_710113ab6c), the registry of grabbing components (g_unk_7172a090b8:
// fn_710113b72c adds, fn_710113b7d0 removes), the grab checks (fn_710113d120, fn_710113d25c,
// fn_710113d378: whether another actor can be grabbed and which of the requests listed at +0x1c8
// accepts it), taking a grab (fn_710113d940), the weak handle to a target (fn_710113b6ac), the release
// message (fn_710113ee18) and a pass over all entities (fn_710113bf5c). Names and layouts are
// placeholders.
#include <new>

#include "../../game_s2/UnkSpitSend.h"
#include "base/UnkSList.h"
#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CActorComponent.h"
#include "game/CComponent.h"
#include "game/CTeamManager/CTunableTeamManager.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkSubAreaGuiCache.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::global::CStrId;
using base::global::UnkStringData;
using base::reflection::UnkReflObject;

struct UnkStateObject;
struct UnkKraidGrab_b57a4;

UnkWeakHandle* fn_7100d78378(UnkWeakTarget* self);
void fn_71010ff850(void* target, bool flag, void* arg);
bool fn_710119dc88(UnkSubAreaGuiCache* self, void* actor, bool flag);

extern void* g_unk_7101cb3230[];  // the component's vtable
extern char* g_unk_71726b2398;
extern UnkSubAreaGuiCache* g_unk_71727254f8;
extern u8 g_unk_7172a09100[] __attribute__((visibility("hidden")));

// As in src/game/CCharClassToStateTrack/CCharClassToStateTrack_Fill.cpp.
struct __attribute__((packed, aligned(4))) UnkTrackBlend {
    u64 kind;
    f32 value;
};
extern UnkTrackBlend g_unk_7172769758;

// A zero whose constructor isn't constexpr, so the static is guarded (as
// src/game/CUsableComponent/fn_7100a9e244.cpp's).
struct UnkGrabStaticZero_ovb0057 {
    u32 value;
    UnkGrabStaticZero_ovb0057() : value(0) {}
};

inline const UnkGrabStaticZero_ovb0057& UnkGrabStaticZero() {
    static UnkGrabStaticZero_ovb0057 s_value asm("g_unk_7172716dcc");
    return s_value;
}

static constexpr f32 kMax = 3.40282347e+38f;
// FLT_MAX's bits: some fields are written as words (the original's constant stores of them are
// integer stores).
static constexpr u32 kMaxBits = 0x7f7fffff;

struct UnkGrabComponent_ovb0057;

struct UnkGrabState_ovb0057 {
    u8 pad_0x0[0x70];
    s32 field_0x70;
};

struct UnkGrabRequest_ovb0057 {
    u8 pad_0x0[0x8];
    u64 id;  // 0x8: the character class it applies to (its +0x98)
    u8 pad_0x10[0x19];
    bool field_0x29;
    u8 pad_0x2a[0xa];
    u8 field_0x34[0x28];
    u8 field_0x5c[0x2c];
    UnkGrabRequest_ovb0057* next;  // 0x88
};
static_assert(__builtin_offsetof(UnkGrabRequest_ovb0057, next) == 0x88, "UnkGrabRequest_ovb0057 layout");

struct UnkGrabRequestSet_ovb0057 {
    u8 pad_0x0[0x68];
    UnkGrabRequest_ovb0057* first;
};

struct UnkGrabCharClass_ovb0057 : UnkReflObject {
    u8 pad_0x8[0x90];
    u64 field_0x98;
};

struct UnkGrabActor_ovb0057;

// The actor's object at +0x240.
struct UnkGrabListener_ovb0057 {
    void** vtable;
    bool vf_0x338(UnkGrabRequest_ovb0057* req, UnkGrabActor_ovb0057* other, void* arg) {
        return reinterpret_cast<bool (*)(UnkGrabListener_ovb0057*, UnkGrabRequest_ovb0057*, UnkGrabActor_ovb0057*,
                                         void*)>(vtable[0x338 / 8])(this, req, other, arg);
    }
    bool vf_0x340(UnkGrabRequest_ovb0057* req, UnkGrabActor_ovb0057* other) {
        return reinterpret_cast<bool (*)(UnkGrabListener_ovb0057*, UnkGrabRequest_ovb0057*, UnkGrabActor_ovb0057*)>(
            vtable[0x340 / 8])(this, req, other);
    }
    void vf_0x350() { reinterpret_cast<void (*)(UnkGrabListener_ovb0057*)>(vtable[0x350 / 8])(this); }
};

// The actor's object at +0x2e0.
struct UnkGrabNotified_ovb0057 {
    void** vtable;
    void vf_0x180() { reinterpret_cast<void (*)(UnkGrabNotified_ovb0057*)>(vtable[0x180 / 8])(this); }
};

struct UnkGrabActor_ovb0057 : UnkReflObject {
    u8 pad_0x8[0x20];
    UnkReflObject* character;  // 0x28
    u8 pad_0x30[0x188];
    UnkGrabState_ovb0057* field_0x1b8;
    u8 pad_0x1c0[0x60];
    UnkAnimationComponent* field_0x220;
    void* field_0x228;
    u8 pad_0x230[0x10];
    UnkGrabListener_ovb0057* field_0x240;
    u8 pad_0x248[0x18];
    UnkGrabComponent_ovb0057* grab;  // 0x260
    u8 pad_0x268[0x78];
    UnkGrabNotified_ovb0057* field_0x2e0;
    u8 pad_0x2e8[0xc4];
    f32 field_0x3ac;
};
static_assert(__builtin_offsetof(UnkGrabActor_ovb0057, field_0x2e0) == 0x2e0, "UnkGrabActor_ovb0057 layout");
static_assert(__builtin_offsetof(UnkGrabActor_ovb0057, field_0x3ac) == 0x3ac, "UnkGrabActor_ovb0057 layout");

// A counted link to a grabbed or grabbing actor.
struct UnkGrabLink_ovb0057 : UnkRefObject {
    UnkGrabActor_ovb0057* target;  // 0x10
};

struct UnkGrabFlags_ovb0057 {
    bool enabled;
    u32 field_0x4;
    bool field_0x8;
    bool field_0x9;
    bool field_0xa;
    bool field_0xb;
    bool field_0xc;
    f32 field_0x10;
};

struct UnkGrabPair_ovb0057 {
    f32 x, y;
};

// What a grab is set up with (copied in by fn_710113ab6c).
struct UnkGrabInfo_ovb0057 {
    s32 mode;
    CStrId field_0x8;
    CStrId field_0x10;
    CStrId field_0x18;
    UnkGrabFlags_ovb0057 flags;  // 0x20
    f32 field_0x34;
    f32 field_0x38;
    f32 field_0x3c;
    f32 field_0x40;
    f32 field_0x44;
    u32 field_0x48;
    UnkGrabPair_ovb0057 field_0x4c;
    u32 field_0x54;
    u32 field_0x58;
    f32 field_0x5c;
    f32 field_0x60;
    f32 field_0x64;
    u32 field_0x68;
    u32 field_0x6c;
    u32 field_0x70;
    UnkGrabPair_ovb0057 field_0x74;
    u32 field_0x7c;
    u32 field_0x80;

    UnkGrabInfo_ovb0057& operator=(const UnkGrabInfo_ovb0057& other) asm("fn_710113ab6c");
};
static_assert(__builtin_offsetof(UnkGrabInfo_ovb0057, field_0x80) == 0x80, "UnkGrabInfo_ovb0057 layout");

struct UnkGrabParams_ovb0057 {
    s32 state;
    UnkWeakHandle* grabbed;  // 0x8
    UnkGrabInfo_ovb0057 info;  // 0x10
    u64 field_0x98;
    u32 field_0xa0;
};
static_assert(sizeof(UnkGrabParams_ovb0057) == 0xa8, "UnkGrabParams_ovb0057 layout");

struct UnkGrabEntry_ovb0057;

struct UnkGrabComponent_ovb0057 {
    void** vtable;
    u8 pad_0x8[0x28];
    UnkGrabActor_ovb0057* owner;  // 0x30
    u8 pad_0x38[0x8];
    base::UnkVector<UnkGrabEntry_ovb0057*> field_0x40;
    base::UnkVector<UnkGrabEntry_ovb0057*> field_0x68;
    base::UnkVector<UnkGrabEntry_ovb0057*> field_0x90;
    bool field_0xb8;
    u8 pad_0xb9[7];
    f32 field_0xc0;
    u32 field_0xc4;
    UnkGrabLink_ovb0057* field_0xc8;
    UnkGrabLink_ovb0057* field_0xd0;
    UnkWeakHandle* target;  // 0xd8
    bool target_set;  // 0xe0
    UnkGrabParams_ovb0057 params;  // 0xe8: state 0xe8, grabbed 0xf0, info 0xf8
    u64 field_0x190;
    u64 field_0x198;
    u64 field_0x1a0;
    u32 field_0x1a8;
    f32 field_0x1ac;
    f32 field_0x1b0;
    f32 field_0x1b4;
    f32 field_0x1b8;
    f32 field_0x1bc;
    f32 field_0x1c0;
    f32 field_0x1c4;
    UnkGrabRequestSet_ovb0057* requests;  // 0x1c8
    CStrId field_0x1d0;
    bool field_0x1d8;
    u64 field_0x1e0;
    UnkGrabComponent_ovb0057* field_0x1e8;
    void (*field_0x1f0)();
};
static_assert(__builtin_offsetof(UnkGrabComponent_ovb0057, field_0x1f0) == 0x1f0, "UnkGrabComponent_ovb0057 layout");

void fn_710113b8cc(UnkGrabComponent_ovb0057* self);
void fn_710113c060(UnkGrabComponent_ovb0057* self);
void fn_710113f6d8();
bool fn_710113d4f4(UnkGrabComponent_ovb0057* self, UnkGrabRequest_ovb0057* req, void* part,
                   UnkGrabActor_ovb0057* other);

// The registry of grabbing components.
struct UnkGrabListTag_ovb0057;
using UnkGrabList_ovb0057 = base::UnkSList<UnkGrabListTag_ovb0057>;
extern UnkGrabList_ovb0057 g_unk_7172a090b8;

static inline s32 IndexOf(UnkGrabList_ovb0057* list, u64 v) {
    u32 i = 0;
    for (UnkGrabList_ovb0057::Node* n = list->head; n; n = n->next, ++i)
        if (n->value == v)
            return static_cast<s32>(i);
    return -1;
}

static inline UnkGrabList_ovb0057::Node* Find(UnkGrabList_ovb0057* list, u64 v) {
    for (UnkGrabList_ovb0057::Node* n = list->head; n; n = n->next)
        if (n->value == v)
            return n;
    return nullptr;
}

static inline void Erase(UnkGrabList_ovb0057* list, UnkGrabList_ovb0057::Node* pos) {
    UnkGrabList_ovb0057::Node* prev = nullptr;
    for (UnkGrabList_ovb0057::Node* it = list->head; it;) {
        UnkGrabList_ovb0057::Node* next = it->next;
        if (it == pos) {
            if (prev) {
                prev->next = next;
                if (list->tail == pos)
                    list->tail = prev;
            } else {
                list->head = next;
            }
            if (--list->count == 0) {
                list->head = nullptr;
                list->tail = nullptr;
            }
            list->allocator->vf_0x48(pos);
            return;
        }
        prev = it;
        it = next;
    }
}

// The sub-area GUI cache, created on first use.
static inline UnkSubAreaGuiCache* GuiCache() {
    UnkSubAreaGuiCache* cache = g_unk_71727254f8;
    if (!cache) {
        cache = static_cast<UnkSubAreaGuiCache*>(fn_71000839a0(0x48, 0x3f));
        fn_710119d484(cache);
        g_unk_71727254f8 = cache;
    }
    return cache;
}

// Whether `other` has a grab component that holds nothing and is in a state to be grabbed.
static inline bool CanBeGrabbed(UnkGrabActor_ovb0057* other) {
    UnkGrabComponent_ovb0057* grab = other->grab;
    if (!grab)
        return false;
    if (grab->owner->field_0x3ac >= 0.0f)
        return false;
    if (fn_710119dc88(GuiCache(), grab->owner, true))
        return false;
    UnkGrabState_ovb0057* state = grab->owner->field_0x1b8;
    if (state && state->field_0x70 == 2)
        return false;
    if (grab->field_0xc8 && grab->field_0xc8->target)
        return false;
    if (grab->field_0xd0 && grab->field_0xd0->target)
        return false;
    return true;
}

// Whether `req` applies to `other`'s character class and the owner's listener and fn_710113d4f4
// accept it.
static inline bool Accepts(UnkGrabComponent_ovb0057* self, UnkGrabRequest_ovb0057& req, UnkGrabActor_ovb0057* other) {
    UnkReflObject* character = other->character;
    UnkGrabCharClass_ovb0057* cls = static_cast<UnkGrabCharClass_ovb0057*>(
        fn_710009d50c(character->vf_0x0(), fn_71011abbd4()) ? character : nullptr);
    bool same = req.id == cls->field_0x98 && !req.field_0x29;
    if (!same)
        return false;
    UnkGrabListener_ovb0057* listener = self->owner->field_0x240;
    if (listener && !listener->vf_0x340(&req, other))
        return false;
    return fn_710113d4f4(self, &req, req.field_0x34, other);
}

static inline u64 SingletonHash(u32 offset) {
    const CStrId* id = reinterpret_cast<const CStrId*>(
        reinterpret_cast<const char*>(&base::reflection::refl_GetSingleton()) + offset);
    return *reinterpret_cast<const u64*>(reinterpret_cast<const char*>(id->data) + 0x30);
}

__attribute__((noinline)) void fn_710113af2c(UnkGrabParams_ovb0057* self) {
    UnkGrabInfo_ovb0057& info = self->info;
    self->state = 0;
    self->grabbed = nullptr;
    info.mode = 1;
    new (&info.field_0x8) CStrId("", true);
    new (&info.field_0x10) CStrId("", true);
    info.field_0x18.data = nullptr;
    if (!g_unk_7101d47108)
        base::global::UnkEnsureStringPool();
    info.field_0x18.data = &g_unk_7101d47108->empty;
    info.flags.enabled = false;
    info.flags.field_0x4 = 0;
    info.flags.field_0x8 = false;
    info.flags.field_0x9 = false;
    info.flags.field_0xa = true;
    info.flags.field_0xb = true;
    info.flags.field_0xc = true;
    info.flags.field_0x10 = 0.0f;
    info.field_0x34 = -kMax;
    info.field_0x38 = -kMax;
    info.field_0x3c = -kMax;
    info.field_0x40 = kMax;
    info.field_0x44 = kMax;
    info.field_0x48 = kMaxBits;
    info.field_0x54 = UnkGrabStaticZero().value;
    info.field_0x58 = UnkGrabStaticZero().value;
    info.field_0x5c = -kMax;
    info.field_0x60 = -kMax;
    info.field_0x64 = -kMax;
    info.field_0x68 = kMaxBits;
    info.field_0x6c = kMaxBits;
    info.field_0x70 = kMaxBits;
    info.field_0x7c = UnkGrabStaticZero().value;
    info.field_0x80 = UnkGrabStaticZero().value;
    self->field_0x98 = 0;
    self->field_0xa0 = 0;
}

void fn_710113adb4(UnkGrabComponent_ovb0057* self) {
    fn_71010c4148(reinterpret_cast<UnkStateObject*>(self));
    self->vtable = &g_unk_7101cb3230[2];
    new (&self->field_0x40) base::UnkVector<UnkGrabEntry_ovb0057*>(0xf);
    new (&self->field_0x68) base::UnkVector<UnkGrabEntry_ovb0057*>(0xf);
    new (&self->field_0x90) base::UnkVector<UnkGrabEntry_ovb0057*>(0xf);
    self->field_0xb8 = false;
    self->field_0xc0 = -1.0f;
    self->field_0xc4 = 1;
    self->field_0xc8 = nullptr;
    self->field_0xd0 = nullptr;
    self->target = nullptr;
    self->target_set = false;
    fn_710113af2c(&self->params);
    self->field_0x190 = 0;
    self->field_0x198 = 0;
    self->field_0x1a0 = 0;
    self->field_0x1a8 = kMaxBits;
    self->field_0x1ac = 0.0f;
    self->field_0x1b0 = 0.0f;
    self->field_0x1b4 = 0.0f;
    self->field_0x1b8 = 1.0f;
    self->field_0x1c4 = 0.0f;
    self->field_0x1bc = 1.0f;
    self->field_0x1c0 = 1.0f;
    self->requests = nullptr;
    new (&self->field_0x1d0) CStrId(g_unk_7101d470e0);
    self->field_0x1d8 = false;
    self->field_0x1e0 = 0;
    self->field_0x1e8 = self;
    self->field_0x1f0 = fn_710113f6d8;
    ResetRef(&self->field_0xd0);
    ResetRef(&self->field_0xc8);
}

__attribute__((noinline)) UnkGrabInfo_ovb0057& UnkGrabInfo_ovb0057::operator=(const UnkGrabInfo_ovb0057& other) {
    mode = other.mode;
    field_0x8 = other.field_0x8;
    field_0x10 = other.field_0x10;
    field_0x18 = other.field_0x18;
    flags = other.flags;
    field_0x34 = other.field_0x34;
    field_0x38 = other.field_0x38;
    field_0x3c = other.field_0x3c;
    field_0x40 = other.field_0x40;
    field_0x44 = other.field_0x44;
    field_0x48 = other.field_0x48;
    field_0x4c = other.field_0x4c;
    field_0x54 = other.field_0x54;
    field_0x58 = other.field_0x58;
    field_0x5c = other.field_0x5c;
    field_0x60 = other.field_0x60;
    field_0x64 = other.field_0x64;
    field_0x68 = other.field_0x68;
    field_0x6c = other.field_0x6c;
    field_0x70 = other.field_0x70;
    field_0x74 = other.field_0x74;
    field_0x7c = other.field_0x7c;
    field_0x80 = other.field_0x80;
    return *this;
}

void fn_710113b6ac(UnkGrabComponent_ovb0057* self, UnkWeakTarget* target) {
    AssignRef(&self->target, fn_7100d78378(target));
    if (self->target_set != true)
        self->target_set = true;
}

void fn_710113b72c(UnkGrabComponent_ovb0057* self) {
    u64 v = reinterpret_cast<u64>(self);
    if (IndexOf(&g_unk_7172a090b8, v) >= 0)
        return;
    g_unk_7172a090b8.PushBack(v);
}

void fn_710113b7d0(UnkGrabComponent_ovb0057* self) {
    if (self->params.state == 2)
        fn_710113b8cc(self);
    u64 v = reinterpret_cast<u64>(self);
    if (IndexOf(&g_unk_7172a090b8, v) < 0)
        return;
    if (UnkGrabList_ovb0057::Node* n = Find(&g_unk_7172a090b8, v))
        Erase(&g_unk_7172a090b8, n);
}

// The game manager's list of entities (its +0x2638).
struct UnkGrabEntityNode_ovb0057 {
    void* field_0x0;
    UnkGrabActor_ovb0057* entity;
    UnkGrabEntityNode_ovb0057* next;
};

void fn_710113bf5c(UnkGrabComponent_ovb0057* self, bool flag) {
    for (UnkGrabEntityNode_ovb0057* n = *reinterpret_cast<UnkGrabEntityNode_ovb0057**>(
             reinterpret_cast<char*>(g_unk_7101d47310) + 0x2638);
         n; n = n->next) {
        UnkGrabActor_ovb0057* e = n->entity;
        if (!e)
            continue;
        if (!fn_710009d50c(e->vf_0x0(), fn_7100d70c20()))
            continue;
        if (e->field_0x228 && !fn_710024fbc8(e->field_0x228))
            continue;
        if (self->owner == e)
            continue;
        if (self->field_0xd0 && self->field_0xd0->target == e)
            continue;
        if (!e->field_0x240)
            continue;
        fn_71010ff850(e->field_0x240, flag, g_unk_7172a09100);
    }
    char* instance = g_unk_71726b2398;
    if (!instance) {
        instance = static_cast<char*>(fn_71000839a0(0x108, 0x3f));
        fn_71011a664c(instance);
        g_unk_71726b2398 = instance;
    }
    fn_71011a710c(instance);
}

bool fn_710113d120(UnkGrabComponent_ovb0057* self, UnkGrabActor_ovb0057* other) {
    if (!CanBeGrabbed(other))
        return false;
    if (self->owner->field_0x3ac >= 0.0f)
        return false;
    if (fn_710119dc88(GuiCache(), self->owner, true))
        return false;
    UnkAnimationComponent* anim = self->owner->field_0x220;
    if (anim && !fn_710117b348(anim, fn_7100cef3e8()))
        return false;
    return true;
}

bool fn_710113d25c(UnkGrabComponent_ovb0057* self, UnkGrabRequest_ovb0057& req, UnkGrabActor_ovb0057* other,
                   void* arg) {
    if (!Accepts(self, req, other))
        return false;
    if (!self->target_set || req.field_0x29)
        return false;
    UnkGrabListener_ovb0057* listener = self->owner->field_0x240;
    if (listener && !listener->vf_0x338(&req, other, arg))
        return false;
    return fn_710113d4f4(self, &req, req.field_0x5c, other);
}

UnkGrabRequest_ovb0057* fn_710113d378(UnkGrabComponent_ovb0057* self, UnkGrabActor_ovb0057* other) {
    if (!CanBeGrabbed(other))
        return nullptr;
    for (UnkGrabRequest_ovb0057* req = self->requests->first; req; req = req->next)
        if (Accepts(self, *req, other))
            return req;
    return nullptr;
}

void fn_710113d940(void* grabber, UnkKraidGrab_b57a4* grab, void* owner) {
    UnkGrabComponent_ovb0057* self = static_cast<UnkGrabComponent_ovb0057*>(grabber);
    UnkGrabActor_ovb0057* actor = static_cast<UnkGrabActor_ovb0057*>(owner);
    if (self->params.state == 2)
        fn_710113b8cc(self);
    if (actor->field_0x240)
        actor->field_0x240->vf_0x350();
    UnkGrabNotified_ovb0057* notified = self->owner->field_0x2e0;
    if (!notified)
        notified = actor->field_0x2e0;
    if (notified)
        notified->vf_0x180();
    self->params.state = 1;
    AssignRef(&self->params.grabbed, fn_7100d78378(reinterpret_cast<UnkWeakTarget*>(actor)));
    self->params.info = *reinterpret_cast<const UnkGrabInfo_ovb0057*>(grab);
    if (self->params.info.flags.enabled)
        fn_710113c060(self);
}

void fn_710113ee18(UnkGrabComponent_ovb0057* self) {
    if (UnkAnimationComponent* anim = self->owner->field_0x220) {
        if (fn_710117ae38(anim, SingletonHash(0x848), false)) {
            u64 hash = SingletonHash(0x850);
            base::reflection::UnkReflTypeIdArg mode;
            mode.id = 1;
            base::reflection::UnkReflTypeIdArg mask;
            mask.id = UnkAll_SpitSend().value;
            fn_7101176690(anim, hash, 0xf4061ff25a2163d6, &mode, &g_unk_710164b958,
                          reinterpret_cast<UnkDesc_SpitSend*>(&g_unk_7172769758), &mask);
        }
    }
    ResetRef(&self->target);
}
