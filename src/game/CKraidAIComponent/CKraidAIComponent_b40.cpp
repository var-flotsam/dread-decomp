// CKraidAIComponent unit (tools/tu_map.py unit 921). fn_7100601cb4: when either of the component's two
// lazily resolved handles (0x14d88 and 0x14dc8: a 0x28-byte source, a state that is 1 until the handle
// takes its value from the source and 2 after, and the handle) names an object whose owner at +0x200
// is `owner`, and `skip` is false, it notifies the owner object (+0x30) and passes the component to
// fn_7101199818. fn_7100600194: a stage step that resets the owner's component at +0x278 (vtable slot
// 0x178), gives the actor five timeline keys, passes every object of two lists (fn_710060694c,
// fn_7100604664) to fn_7100606964 and fn_7100604cc0, and runs the next scenario script. Names and layouts
// are placeholders. fn_71005ff9c0: passes every object of four lists to their functions and hands the
// component, with the callback fn_7100602164, to fn_71011995c0. fn_71005ff92c: whether any object of the
// list fn_71009526a4 is a CKraidSpikeMovablePlatformComponent. fn_71005fbdb0: initialisation; runs the
// base step, keeps the owner's object at 0x14a30 when it is a CCharClassKraidAIComponent, and registers
// eight embedded entries in the list at 0x1618, each initialised from its part of that object.
#include "base/UnkRefHandle.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkTU0.h"
#include "game/UnkTU2730.h"
#include "game/UnkTU341.h"
#include "game/UnkTimelineComponent.h"
#include "types.h"
#include "game/UnkBossAIComponent.h"

// The handle the component refers to (base::UnkRefCounted's layout); its target is at +0x10.
struct UnkCounted : base::UnkRefCounted {
    union {
        void* field_0x10;
        u64 raw_0x10;
    };
};

// A counted reference; the destructor decrements and tests in one step.
struct UnkRef {
    UnkCounted* ptr;
    ~UnkRef() {
        if (ptr && --ptr->ref_count == 0)
            delete ptr;
    }
};

struct UnkFound_kr {
    u8 pad_0x0[0x200];
    void* field_0x200;
};

struct UnkLazyHandle_kr {
    u8 source[0x28];
    u32 state;
    u32 pad_0x2c;
    base::UnkRefHandle handle;
    u8 pad_0x38[0x8];
};

struct UnkSelf_kr {
    u8 pad_0x0[0x30];
    void* field_0x30;
    u8 pad_0x38[0x14d50];
    UnkLazyHandle_kr handles[2];  // 0x14d88
};

UnkRef fn_7100353be8(void* from);
void* fn_71004067a4(void* owner);
extern void* g_unk_7101d47330;

static inline __attribute__((always_inline)) bool Names_kr(UnkLazyHandle_kr* h, void* owner, u32 skip) {
    if (h->state == 1) {
        {
            UnkRef fresh = fn_7100353be8(h->source);
            fn_710031d208(&h->handle, reinterpret_cast<const base::UnkRefHandle*>(&fresh));
        }
        h->state = 2;
    }
    UnkCounted* c = static_cast<UnkCounted*>(h->handle.ptr);
    if (!c || !c->raw_0x10)
        return false;
    asm("" ::: "memory");
    UnkFound_kr* found = static_cast<UnkFound_kr*>(fn_71004067a4(c->field_0x10));
    return !skip && found->field_0x200 == owner;
}

void fn_7100601cb4(UnkSelf_kr* self, void* owner, u32 skip) {
    if (Names_kr(&self->handles[0], owner, skip) || Names_kr(&self->handles[1], owner, skip)) {
        fn_7100232c68(self->field_0x30);
        fn_7101199818(g_unk_7101d47330, self);
    }
}

// fn_7100600194

struct UnkOwner_kr {
    u8 pad_0x0[0x278];
    void** field_0x278;
};

struct UnkList_kr {
    UnkCounted** data;
    u32 count;
};

struct UnkSelf2_kr {
    u8 pad_0x0[0x30];
    UnkOwner_kr* field_0x30;
};

extern const u64 g_unk_710164a258[];
extern const u64 g_unk_710164a260[];
extern const u64 g_unk_710164a268[];
extern const u64 g_unk_710164a270[];
extern const u64 g_unk_710164a278[];
UnkList_kr* fn_710060694c();
UnkList_kr* fn_7100604664();
void fn_7100606964(void* object);
void fn_7100604cc0(void* object);

static inline __attribute__((always_inline)) void ForEach_kr(UnkList_kr* list, void (*fn)(void*)) {
    for (UnkCounted **it = list->data, **end = list->data + list->count; it != end; ++it)
        fn(*it ? (*it)->field_0x10 : nullptr);
}

bool fn_7100600194(UnkSelf2_kr* self) {
    UnkOwner_kr* owner = self->field_0x30;
    if (void** c = owner->field_0x278) {
        reinterpret_cast<void (*)(void*)>((*reinterpret_cast<void***>(c))[0x178 / 8])(c);
        owner = self->field_0x30;
    }
    fn_7100d7c828(reinterpret_cast<UnkActor*>(owner), g_unk_710164a258, false);
    fn_7100d7c828(reinterpret_cast<UnkActor*>(self->field_0x30), g_unk_710164a260, false);
    fn_7100d7c828(reinterpret_cast<UnkActor*>(self->field_0x30), g_unk_710164a268, false);
    fn_7100d7c828(reinterpret_cast<UnkActor*>(self->field_0x30), g_unk_710164a270, false);
    fn_7100d7c828(reinterpret_cast<UnkActor*>(self->field_0x30), g_unk_710164a278, false);
    ForEach_kr(fn_710060694c(), fn_7100606964);
    ForEach_kr(fn_7100604664(), fn_7100604cc0);
    fn_7100000790("CurrentScenario.Kraid_Activation_Stage_02_With_Cut_Scene", "");
    return true;
}


// fn_71005ff9c0

struct UnkCallback_kr {
    void* object;
    void (*fn)();
};

UnkList_kr* fn_71006051d4();
UnkList_kr* fn_7100607130();
UnkList_kr* fn_7100608468();
void fn_7100605388(void* object);
void fn_7100607260(void* object);
void fn_7100608588(void* object);
void fn_7100602164();

bool fn_71005ff9c0(UnkSelf2_kr* self) {
    ForEach_kr(fn_710060694c(), fn_7100606964);
    ForEach_kr(fn_71006051d4(), fn_7100605388);
    ForEach_kr(fn_7100607130(), fn_7100607260);
    ForEach_kr(fn_7100608468(), fn_7100608588);
    fn_71011995c0(g_unk_7101d47330, UnkCallback_kr{self, fn_7100602164});
    return true;
}

// fn_71005ff92c

UnkList_kr* fn_71009526a4();

bool fn_71005ff92c() {
    UnkList_kr* list = fn_71009526a4();
    for (UnkCounted **it = list->data, **end = list->data + list->count; it != end; ++it) {
        base::reflection::UnkReflObject* o =
            static_cast<base::reflection::UnkReflObject*>(*it ? (*it)->field_0x10 : nullptr);
        if (o && fn_710009d50c(o->vf_0x0(), fn_71006081c4()))
            return true;
    }
    return false;
}

// fn_71005fbdb0

typedef u64 u64_a4_kr __attribute__((aligned(4)));

struct UnkSelf;
void fn_71005f662c(void* entry, u64 source);
void fn_71005f2614(void* entry, u64 source);
void fn_71005f4208(void* entry, u64 source);
void fn_71005f122c(void* entry, u64 source);
void fn_71005f50b0(void* entry, u64 source);
void fn_71005f7d00(void* entry, u64 source);
void fn_71005f2c90(void* entry, u64 source);
void fn_71005f5c2c(void* entry, u64 source);

static inline __attribute__((always_inline)) void Register_kr(char* self, u32 offset, void (*init)(void*, u64),
                                                              u32 from) {
    char* entry = self + offset;
    char* list = self + 0x161c;
    u32 index = *reinterpret_cast<u32*>(self + 0x1618);
    *reinterpret_cast<u64_a4_kr*>(list + ((u64)index << 3)) = reinterpret_cast<u64>(entry);
    *reinterpret_cast<u32*>(self + 0x1618) = index + 1;
    init(entry, *reinterpret_cast<u64*>(self + 0x14a30) + from);
}

// `arg` (x1) reaches the base step untouched in the original (tools/forwarded_args.py).
u32 fn_71005fbdb0(char* self, void* arg) {
    u32 result = fn_71003ac8f4(reinterpret_cast<UnkBossAIComponent*>(self), arg);
    base::reflection::UnkReflObject* object = *reinterpret_cast<base::reflection::UnkReflObject**>(self + 0x10);
    base::reflection::UnkReflObject* component = nullptr;
    if (object && fn_710009d50c(object->vf_0x0(), fn_71005f9734()))
        component = object;
    *reinterpret_cast<base::reflection::UnkReflObject**>(self + 0x14a30) = component;
    Register_kr(self, 0x14a38, fn_71005f662c, 0x630);
    Register_kr(self, 0x14aa0, fn_71005f2614, 0x770);
    Register_kr(self, 0x14ad8, fn_71005f4208, 0x860);
    Register_kr(self, 0x14b30, fn_71005f122c, 0x950);
    Register_kr(self, 0x14b80, fn_71005f50b0, 0xa88);
    Register_kr(self, 0x14bc0, fn_71005f7d00, 0xb90);
    Register_kr(self, 0x14c00, fn_71005f2c90, 0xc98);
    Register_kr(self, 0x14c68, fn_71005f5c2c, 0xdb8);
    return result & 1;
}
