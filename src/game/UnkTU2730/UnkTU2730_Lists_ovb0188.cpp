// Unit 2730's functions (tools/tu_map.py; no class hint): the callback registry and cutscene manager
// (g_unk_7101d47330) with its owned-reference list, tracked-object lists and callback list; the
// vector reallocations of the reference lists; the cutscene-seen count read from the blackboard
// sections; and the unit's static initialiser. Names and layouts are placeholders.
#define UNK_VECTOR_GROW_AT_ATTR __attribute__((always_inline))
#include "base/UnkVector.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "base/input/UnkRumbleManager.h"
#include "base/reflection/UnkReflStatics.h"
#include "game/CBlackboard.h"
#include "game/CHeatRoomComponent.h"
#include "game/CInputComponent.h"
#include "game/CNavMeshItemComponent.h"
#include "game/UnkGameBlackboard.h"
#include "game/UnkGameManager.h"
#include "game/UnkRenderManager.h"
#include "game/UnkTU0.h"
#include "game/UnkTU1643.h"
#include "game/UnkTU2553.h"
#include "game/UnkTU2728.h"
#include "game/UnkTU2730.h"
#include "game/UnkTU2732.h"
#include "game/UnkTU315.h"
#include "game/UnkTUInit.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

extern s32 g_unk_7101cb5320 __attribute__((visibility("hidden")));

void fn_71011994e0(void* root, bool value) {
    if (fn_7100decc5c(g_unk_7101d47310) || value)
        return;
    s32 v;
    if (fn_7100219c4c(g_unk_7101d47300)) {
        v = 20;
    } else {
        v = g_unk_7101cb5320 - 1;
        if (v < 0)
            return;
    }
    g_unk_7101cb5320 = v;
}

struct UnkRefObj_b0188 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    s32 ref_count;  // 0x8
};

struct UnkRefList_b0188 {
    u8 pad_0x0[0xe0];
    UnkRefObj_b0188** items;  // 0xe0
    u32 count;                // 0xe8
};

static inline void ReleaseAll(UnkRefList_b0188* self) {
    for (u64 i = 0; i < self->count; i++) {
        UnkRefObj_b0188* o = self->items[i];
        if (!o)
            continue;
        __atomic_fetch_sub(&o->ref_count, 1, __ATOMIC_SEQ_CST);
        if (__atomic_load_n(&o->ref_count, __ATOMIC_SEQ_CST) == 0)
            o->vf_0x10();
    }
    self->count = 0;
}

void fn_7101196f4c(UnkRefList_b0188* self) {
    ReleaseAll(self);
}

extern void* g_unk_7101d47330;
void fn_710106838c(void* p);
void fn_7101196ccc(void* self);

struct UnkOwner_b0188 {
    u8 pad_0x0[0x58];
    void* field_0x58;
};

void fn_7101196bf4(UnkOwner_b0188* self, bool release) {
    if (self->field_0x58)
        fn_710106838c(self->field_0x58);
    fn_7101196ccc(self);
    if (release)
        ReleaseAll(static_cast<UnkRefList_b0188*>(g_unk_7101d47330));
}

// A registered callback; copied and moved with 8-byte copies.
struct UnkCallback_kr {
    void* object;
    void (*fn)();

    UnkCallback_kr() : object(nullptr), fn(nullptr) {}
    UnkCallback_kr(UnkCallback_kr&& o) {
        __builtin_memcpy(&object, &o.object, 8);
        __builtin_memcpy(&fn, &o.fn, 8);
    }
    UnkCallback_kr& operator=(const UnkCallback_kr&) = default;
    ~UnkCallback_kr() {}
};

struct UnkCallbackList_b0188 {
    u8 pad_0x0[0x30];
    base::UnkVector<UnkCallback_kr> callbacks;
};

__attribute__((noinline)) UnkCallback_kr* fn_71011998b4(base::UnkVector<UnkCallback_kr>* v, u32 index) {
    return &v->GrowAt(index);
}

void fn_7101199818(void* manager, void* component) {
    UnkCallbackList_b0188* self = static_cast<UnkCallbackList_b0188*>(manager);
    base::UnkVector<UnkCallback_kr>& list = self->callbacks;
    u32 i = 0;
    while (i < list.count) {
        UnkCallback_kr* e = fn_71011998b4(&list, i);
        if (e->object != component) {
            ++i;
        } else {
            u32 n = list.count;
            if (n > i) {
                --n;
                if (n > i) {
                    UnkCallback_kr* dst = &list.data[i];
                    UnkCallback_kr* src = &list.data[n];
                    *dst = *src;
                }
                list.count = n;
            }
        }
    }
}

struct UnkSoundMgr;
extern UnkSoundMgr* g_unk_7101d4d820;

struct UnkEmitterOwner_b0188 {
    u8 pad_0x0[0x90];
    void* field_0x90;
};

struct UnkEmitterHolder_b0188 {
    UnkEmitterOwner_b0188* owner;
};

struct UnkEmitterRef_b0188 {
    UnkEmitterHolder_b0188* holder;
    u64 pad_0x8;
};

struct __attribute__((packed, aligned(4))) UnkEmitterList_b0188 {
    u32 count;
    UnkEmitterRef_b0188 items[16];
};

struct UnkEmitterObj_b0188 {
    u8 pad_0x0[0x318];
    void* field_0x318;
    u8 pad_0x320[0x190];
    u32 state;  // 0x4b0
};

struct UnkRoot_b0188 {
    u8 pad_0x0[0x58];
    UnkEmitterObj_b0188* field_0x58;
    u8 pad_0x60[0xab2];
    bool field_0xb12;
};

void fn_7100108a7c(UnkSoundMgr* self, u32 kind, UnkEmitterList_b0188* out);

bool fn_710119a320(void* root) {
    UnkRoot_b0188* self = static_cast<UnkRoot_b0188*>(root);
    if (!self->field_0xb12 || !self->field_0x58 || self->field_0x58->state != 2)
        return false;
    UnkEmitterList_b0188 list;
    list.count = 0;
    fn_7100108a7c(g_unk_7101d4d820, 7, &list);
    if (list.count == 0)
        return false;
    bool found;
    u64 i = 0;
    do {
        found = list.items[i].holder->owner->field_0x90 == self->field_0x58->field_0x318;
    } while (++i < list.count && !found);
    return found;
}

using base::global::CStrId;

extern CStrId g_unk_7172a0bc98;

struct UnkIdHolder_b0188 {
    u8 pad_0x0[0xae8];
    CStrId* ids;  // 0xae8
    u32 count;    // 0xaf0
};

static inline void ClearSection(UnkIdHolder_b0188* self) {
    CBlackboard::CSection* section = fn_71010472e0(&g_unk_7101d47310->field_0x968, g_unk_7172a0bc98);
    if (section->data.field_0x38)
        section->data.field_0x38 = false;
    for (u64 i = 0; i < self->count; ++i)
        self->ids[i].Reset();
    self->count = 0;
}

void fn_710119a50c(UnkIdHolder_b0188* self) {
    ClearSection(self);
}

void fn_7101198e88(UnkIdHolder_b0188* self, bool a, bool b, bool c, bool d);

void fn_71011990e8(UnkIdHolder_b0188* self) {
    ClearSection(self);
    // The callee's first argument is not set here; whatever x0 holds goes through.
    UnkIdHolder_b0188* unset;
    fn_7101198e88(unset, true, true, false, false);
}

using base::global::CFilePathStrId;
using base::global::CStrId;

extern CStrId g_unk_7172a0bc98;

struct UnkSubId_7100fc4b34 {
    CStrId id;
    u64 field_0x8;
};
struct UnkGameManagerNamedDef : UnkIdSource {
    u64 field_0x8;
    UnkQualifiedName name;
};

const UnkSubId_7100fc4b34& fn_710119a610(UnkQualifiedName* self, const void* path);
bool fn_7100a68fb0(CBlackboard::CSection* section, const void* name, u32* out);

// A path's string data: a use counter at +0x44 and the text from +0x10.
struct UnkPathData_b0188 {
    u8 pad_0x0[0x10];
    u8 text[0x34];
    union {
        u32 field_0x44;
    };
};

struct UnkCutsceneMgr_b0188 {
    u8 pad_0x0[0xb18];
    CStrId section;  // 0xb18
};

static inline __attribute__((always_inline)) void ReadSub(CBlackboard::CSection* section, const void* key, u32* count) {
    UnkGameManagerNamedDef& def = g_unk_7172a0bb10;
    if (!def.name.str || !def.name.str[0])
        fn_7100ae9d7c(&def.name, &def, g_unk_7101d470e0);
    UnkSubId_7100fc4b34 id = fn_710119a610(&def.name, key);
    fn_7100a68fb0(section, &id, count);
}

static inline __attribute__((always_inline)) u32 ReadCount(CBlackboard* blackboard, UnkCutsceneMgr_b0188* mgr, const CFilePathStrId* path) {
    if (path->data == &g_unk_7101d47200->empty)
        return 0;
    CBlackboard::CSection* section = fn_7101047484(blackboard, mgr->section);
    if (!section)
        return 0;
    u32 count = 0;
    UnkPathData_b0188* data = reinterpret_cast<UnkPathData_b0188*>(path->data);
    ++data->field_0x44;
    ReadSub(section, reinterpret_cast<UnkPathData_b0188*>(path->data)->text, &count);
    return count;
}

u32 fn_71011986e0(void* manager, const CFilePathStrId* path) {
    CBlackboard* blackboard = reinterpret_cast<CBlackboard*>(reinterpret_cast<u8*>(g_unk_7101d47310) + 0xab0);
    return ReadCount(blackboard, static_cast<UnkCutsceneMgr_b0188*>(manager), path);
}

u32 fn_710119882c(void* manager, const CFilePathStrId* path) {
    return ReadCount(&g_unk_7101d47310->field_0x968, static_cast<UnkCutsceneMgr_b0188*>(manager), path);
}

struct UnkEventTable_b0188;

u32 fn_7100f0cc48(UnkEventTable_b0188* table, u32 value);

struct UnkEventEntry_b0188 {
    u32 id;
    u32 kind;
    u8 pad_0x8[0xc];
};

struct UnkEventSource_b0188 {
    u8 pad_0x0[0x110];
    u32 pending;  // 0x110
    u32 pad_0x114;
    const char* name;  // 0x118
    u8 pad_0x120[0x28];
    u8 item[0x38];  // 0x148
    UnkEventEntry_b0188 entries[1];  // 0x180
};

struct UnkEventSourceCount_b0188 {
    u8 pad_0x0[0xae0];
    u32 count;
};

void fn_7101198348(UnkEventSource_b0188* self) {
    if (!self->pending)
        return;
    u32 id = fn_7100f0cc48(reinterpret_cast<UnkEventTable_b0188*>(reinterpret_cast<u8*>(g_unk_7101d47310) + 0x38), self->pending);
    s32 count = reinterpret_cast<UnkEventSourceCount_b0188*>(self)->count;
    for (s32 i = 0; count != i; ++i) {
        // keeps the entry index in a register instead of a stepped pointer, as the original does
        asm("" : "+r"(i));
        if (self->entries[i].kind != 1 || self->entries[i].id != id)
            continue;
        fn_7100000790(self->name, g_unk_710162e2ed);
        if (!self->pending)
            continue;
        void* mgr = g_unk_7172729910;
        if (!mgr) {
            mgr = fn_71000839a0(0x2208, 0x3f);
            fn_710119aee8(mgr);
            g_unk_7172729910 = mgr;
        }
        fn_710119ae24(mgr, self->item, true);
        self->pending = 0;
    }
    reinterpret_cast<UnkEventSourceCount_b0188*>(self)->count = 0;
}

struct UnkCounted_b0188 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    union {
        s32 ref_count;  // 0x8
    };
};

struct UnkRefElem_b0188 {
    UnkCounted_b0188* ref;
    u64 fields[6];

    UnkRefElem_b0188() : ref(nullptr) {}
    UnkRefElem_b0188(UnkRefElem_b0188&& o) : ref(nullptr) {
        if (UnkCounted_b0188* p = o.ref) {
            ++p->ref_count;
            if (UnkCounted_b0188* old = ref)
                if (--old->ref_count == 0)
                    old->vf_0x10();
            ref = p;
        }
        __builtin_memcpy(fields, o.fields, sizeof(fields));
    }
    ~UnkRefElem_b0188() {
        if (UnkCounted_b0188* p = ref)
            if (--p->ref_count == 0)
                p->vf_0x10();
    }
};

void fn_710119a874(base::UnkVector<UnkRefElem_b0188>* v, u32 n) {
    if (v->capacity < n) {
        UnkRefElem_b0188* p = static_cast<UnkRefElem_b0188*>(v->allocator->vf_0x40((u64)n * sizeof(UnkRefElem_b0188), 8, v->flags));
        if (v->data) {
            UnkRefElem_b0188* src = v->data;
            UnkRefElem_b0188* dst = p;
            for (u32 i = 0; i < v->count; ++i, ++dst, ++src) {
                new (dst) UnkRefElem_b0188(static_cast<UnkRefElem_b0188&&>(*src));
                src->~UnkRefElem_b0188();
            }
            v->allocator->vf_0x48(v->data);
        }
        v->data = p;
        v->capacity = n;
    }
}

static inline void Release(UnkRefObj_b0188* o) {
    __atomic_fetch_sub(&o->ref_count, 1, __ATOMIC_SEQ_CST);
    if (__atomic_load_n(&o->ref_count, __ATOMIC_SEQ_CST) == 0)
        o->vf_0x10();
}

struct UnkRefPtr_b0188 {
    UnkRefObj_b0188* ref;

    UnkRefPtr_b0188(UnkRefPtr_b0188&& o) {
        __builtin_memset(&ref, 0, sizeof(ref));
        if (UnkRefObj_b0188* p = o.ref) {
            __atomic_fetch_add(&p->ref_count, 1, __ATOMIC_SEQ_CST);
            if (UnkRefObj_b0188* old = ref)
                Release(old);
            ref = p;
        }
    }
    ~UnkRefPtr_b0188() {
        if (UnkRefObj_b0188* p = ref)
            Release(p);
    }
};

void fn_710119a9bc(base::UnkVector<UnkRefPtr_b0188>* v, u32 n) {
    if (v->capacity < n) {
        UnkRefPtr_b0188* p = static_cast<UnkRefPtr_b0188*>(v->allocator->vf_0x40((u64)n * sizeof(UnkRefPtr_b0188), 8, v->flags));
        if (v->data) {
            UnkRefPtr_b0188* src = v->data;
            UnkRefPtr_b0188* dst = p;
            for (u32 i = 0; i < v->count; ++i, ++dst, ++src) {
                new (dst) UnkRefPtr_b0188(static_cast<UnkRefPtr_b0188&&>(*src));
                src->~UnkRefPtr_b0188();
            }
            v->allocator->vf_0x48(v->data);
        }
        v->data = p;
        v->capacity = n;
    }
}

struct UnkGameManagerNamedDef;
extern CStrId g_unk_7172a0bc98;

// The unit's header data, then two floats and a flag the unit's statics read.
struct UnkCutsceneUnitHeader {
    UnkTUHeader header;
    f32 field_0x18;
    f32 field_0x1c;
    bool field_0x20;
};
extern UnkCutsceneUnitHeader g_unk_7172a0bcb8 __attribute__((visibility("hidden")));

void fn_710119ab20_init();

void fn_710119ab20() {
    InitHeader(g_unk_7172a0bcb8.header);
    g_unk_7172a0bcb8.field_0x18 = -1.0f;
    g_unk_7172a0bcb8.field_0x1c = -1.0f;
    g_unk_7172a0bcb8.field_0x20 = true;
    UnkPropertyEx<0x160>* property = reinterpret_cast<UnkPropertyEx<0x160>*>(&g_unk_7172a0bb10);
    InitPropertyExVtableFirst(property, g_unk_7101a889c8, g_unk_71726db788, g_unk_7101a8d830, fn_7100ae9cc8,
                              "CutScenePlayed", refl_TypeOf<fn_7100005178>(), fn_71004fd2cc, fn_7100a24a50);
    new (&g_unk_7172a0bc98) CStrId("PlayedCutscenes", true);
    fn_7100000250(fn_710000051c, &g_unk_7172a0bc98, &g_unk_7101a19000);
}

using base::UnkAllocator;

struct UnkTracked_b0188 : UnkRefObject {
    u64 field_0x10;
};

struct UnkTrackedNode_b0188 {
    union {
        UnkTracked_b0188* item;
    };
    UnkTrackedNode_b0188* next;

    ~UnkTrackedNode_b0188() {
        if (UnkTracked_b0188* p = item)
            if (--p->ref_count == 0)
                p->vf_0x10();
    }
};

struct UnkTrackedList_b0188 {
    u32 count;
    UnkTrackedNode_b0188* head;
    UnkTrackedNode_b0188* tail;
    UnkAllocator* allocator;
    u32 flags;

    UnkTrackedNode_b0188* Find(u64 key) {
        for (UnkTrackedNode_b0188* n = head; n; n = n->next)
            if (n->item && n->item->field_0x10 == key)
                return n;
        return nullptr;
    }
    void Erase(UnkTrackedNode_b0188* node) {
        UnkTrackedNode_b0188* cur = nullptr;
        for (UnkTrackedNode_b0188* it = head; it;) {
            UnkTrackedNode_b0188* prev = cur;
            cur = it;
            it = it->next;
            if (cur == node) {
                if (prev) {
                    prev->next = it;
                    if (tail == node)
                        tail = prev;
                } else {
                    head = it;
                }
                if (--count == 0) {
                    head = nullptr;
                    tail = nullptr;
                }
                node->~UnkTrackedNode_b0188();
                allocator->vf_0x48(node);
                return;
            }
        }
    }
};

struct UnkTracker_b0188 {
    u8 pad_0x0[0x68];
    UnkTrackedList_b0188 field_0x68;
    u8 pad_0x90[0x8];
    UnkTrackedList_b0188 field_0x98;
};

namespace base::reflection {
struct UnkReflObject;
}
using base::reflection::UnkReflObject;

void fn_7101198450(void* gui, UnkReflObject* entity) {
    UnkTracker_b0188* self = static_cast<UnkTracker_b0188*>(gui);
    u64 key = reinterpret_cast<u64>(entity);
    if (UnkTrackedNode_b0188* n = self->field_0x98.Find(key))
        self->field_0x98.Erase(n);
    if (UnkTrackedNode_b0188* n = self->field_0x68.Find(key))
        self->field_0x68.Erase(n);
}

using base::input::UnkRumbleManager;

struct UnkPlayer;
struct UnkComponentOwner;

struct UnkDropTarget;
struct UnkSubAreaDropTargetView {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38(u32 a, u32 b);
};

extern UnkRumbleManager* g_unk_7101d47348;
void fn_7100073f48(UnkRumbleManager* self, u32 a, u32 b);
void fn_7100d7ae80(UnkComponentOwner* self);
void fn_7101192a4c(void* self);
void fn_7100219c44(UnkRenderManager* self, u32 value);
void fn_710021a09c(UnkRenderManager* self, u32 value);
void fn_7101196ccc(void* self);


struct UnkCallback_b0188 {
    void* object;
    void (*fn)(void* object, void* arg, u32 mode);
};

struct UnkHudView_b0188 {
    u8 pad_0x0[0x4a0];
    u32 field_0x4a0;
    bool field_0x4a4;
};
struct UnkMenuView_b0188 {
    u8 pad_0x0[0x2d0];
    UnkHudView_b0188* hud;
};

struct UnkScene_b0188 {
    u8 pad_0x0[0x30];
    base::UnkVector<UnkCallback_b0188> callbacks;  // 0x30
    UnkRefObj_b0188* field_0x58;
    u8 pad_0x60[0xb0];
    u32 pending;  // 0x110
    u8 pad_0x114[0x34];
    u8 item[0x38];  // 0x148
    u8 pad_0x180[0x992];
    bool field_0xb12;
};
static_assert(__builtin_offsetof(UnkScene_b0188, field_0xb12) == 0xb12, "layout");

void fn_7101197ae0(UnkScene_b0188* self, UnkRefObj_b0188* arg) {
    self->field_0xb12 = true;
    reinterpret_cast<UnkSubAreaDropTargetView*>(g_unk_7101d47318)->vf_0x38(0, 0);
    UnkRumbleManager* rumble = g_unk_7101d47348;
    if (!rumble) {
        rumble = static_cast<UnkRumbleManager*>(fn_71000839a0(sizeof(UnkRumbleManager), 0x3f));
        fn_71000733e4(rumble);
        g_unk_7101d47348 = rumble;
    }
    fn_7100073f48(rumble, 0, 0);
    if (UnkPlayer* player = fn_7100dec788(g_unk_7101d47310))
        fn_7100d7ae80(reinterpret_cast<UnkComponentOwner*>(player));
    if (self->pending) {
        void* mgr = g_unk_7172729910;
        if (!mgr) {
            mgr = fn_71000839a0(0x2208, 0x3f);
            fn_710119aee8(mgr);
            g_unk_7172729910 = mgr;
        }
        fn_710119ae24(mgr, self->item, true);
        self->pending = 0;
    }
    void* queue = g_unk_7172758b20;
    if (!queue) {
        queue = fn_71000839a0(0x40, 0x3f);
        fn_7101192628(queue);
        g_unk_7172758b20 = queue;
    }
    fn_7101192a4c(queue);
    UnkHudView_b0188* hud = reinterpret_cast<UnkMenuView_b0188*>(fn_7100deac84(g_unk_7101d47310))->hud;
    hud->field_0x4a4 = true;
    hud->field_0x4a0 = 0;
    fn_7100219c44(g_unk_7101d47300, 0);
    fn_710021a09c(g_unk_7101d47300, 0);
    for (UnkCallback_b0188 *it = self->callbacks.data, *end = it + self->callbacks.count; it != end; ++it)
        it->fn(it->object, arg, 2);
    if (self->field_0x58 == arg) {
        if (arg) {
            __atomic_fetch_sub(&arg->ref_count, 1, __ATOMIC_SEQ_CST);
            if (__atomic_load_n(&arg->ref_count, __ATOMIC_SEQ_CST) == 0)
                arg->vf_0x10();
            self->field_0x58 = nullptr;
        }
        fn_7101196ccc(self);
    }
}
