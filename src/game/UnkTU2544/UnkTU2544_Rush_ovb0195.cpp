// CBossRushSurvivalConfiguration's unit (no reliable name): helpers of one boss-rush survival object
// (levels, a small state machine at +0x8, a score). Names and layouts are placeholders.
#include <new>

#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflCast.h"
#include "base/reflection/UnkReflAttributes.h"
#include "base/snd/CSoundSystem.h"
#include "game/CBlackboard.h"
#include "game/UnkGameBlackboard.h"
#include "game/UnkGuiRoot.h"
#include "game/UnkItemProperties.h"
#include "game/UnkTU2544.h"
#include "game/UnkTU2656.h"
#include "game/UnkTU2768.h"
#include "game/UnkTUProperty.h"
#include "game/UnkGameManager.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::global::CStrId;

struct UnkRushSink_b0195 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual void vf_0x40();
    virtual void vf_0x48(void* item);
};

struct UnkRushItem_b0195 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual CStrId* vf_0x28();  // the item's id
};

struct UnkRushItemEntry_b0195 {
    UnkRushItem_b0195* item;
    u32 value;
    u32 pad_0xc;
};

struct UnkRushItems_b0195 {
    UnkRushItemEntry_b0195* data;
    u32 count;
};

struct UnkRushLevel_b0195 {
    u8 pad_0x0[0x140];  // levels start at +0x140 + 0xa8 * level; viewed from the object start
    CStrId first;   // 0x0
    CStrId second;  // 0x8
    u8 pad_0x10[8];
    void* field_0x18;
    s32 field_0x20;
    u8 pad_0x24[0x50 - 0x24];
    UnkRushItems_b0195 items;  // 0x50
};

struct UnkRushMessage_b0195 {
    u8 pad_0x0[0x18];
    base::global::UnkStringData* field_0x18;
};

// The small state machine at +0x8 (the template of src/game/UnkStateMachineSetA5_b61.cpp).
struct UnkSmState_b0195 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10(s32 previous);  // entered
    virtual void vf_0x18(s32 next);      // left
    u64 field_0x8;
    u32 id;
};

typedef UnkSmState_b0195* __attribute__((aligned(4))) UnkSmStatePtr_b0195;

struct UnkSmMachine_b0195 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20(UnkSmState_b0195* previous, UnkSmState_b0195* next);
    virtual void vf_0x28(UnkSmState_b0195* current, UnkSmState_b0195* next);

    u64 field_0x8;
    UnkSmState_b0195* current;
    UnkSmState_b0195* previous;
    u32 count;
    UnkSmStatePtr_b0195 states[16];
    u32 pending;
    bool busy;
};

__attribute__((always_inline)) static inline bool FindState_b0195(UnkSmMachine_b0195* self, u32 id,
                                                               UnkSmState_b0195*& out) {
    u64 n = (u64)self->count * sizeof(UnkSmStatePtr_b0195);
    // Keeps the byte count as one register value, so the search loop is not strength-reduced.
    asm("" : "+r"(n));
    for (UnkSmStatePtr_b0195* p = self->states; n != 0; ++p, n -= sizeof(UnkSmStatePtr_b0195)) {
        if ((*p)->id == id) {
            out = *p;
            return out != nullptr;
        }
    }
    out = nullptr;
    return false;
}

// Switches to state `id` (the b59 "apply pending" body) and clears the pending id.
template <u32 None>
__attribute__((always_inline)) static inline void SwitchState_b0195(UnkSmMachine_b0195* self, u32 id,
                                                                 UnkSmState_b0195* current) {
    if (!current || current->id != id) {
        UnkSmState_b0195* found;
        UnkSmState_b0195* next;
        u32 enter;
        if (FindState_b0195(self, id, found)) {
            next = found;
            enter = true;
        } else {
            if (id != 0)
                goto done;
            enter = false;
            // Keeps the miss-path flag from folding into the later tests.
            asm("" : "+r"(enter));
            next = nullptr;
            // Keeps the miss-path state pointer from folding into the later tests.
            asm("" : "+r"(next));
        }
        if (current) {
            current->vf_0x18(id);
            self->vf_0x28(self->current, next);
        }
        self->previous = self->current;
        self->current = found;
        if (enter) {
            typedef void (*EnteredFn)(UnkSmState_b0195*, s32);
            s32 previous_id = self->previous ? self->previous->id : None;
            EnteredFn entered = (*reinterpret_cast<EnteredFn**>(found))[2];
            entered(next, previous_id);
            self->vf_0x20(self->previous, next);
        }
    }
done:
    self->pending = None;
}

template <u32 Last, u32 None>
__attribute__((always_inline)) static inline void RequestState_b0195(UnkSmMachine_b0195* self, u32 id) {
    // Stored through a plain u32 pointer: the original rereads a state id the caller tested after this.
    u32* pending = &self->pending;
    *pending = id;
    if (id > Last)
        return;
    if (self->busy)
        return;
    SwitchState_b0195<None>(self, id, self->current);
}

struct UnkRushSub_b0195;

struct UnkRushLevelB_b0195 {
    u8 pad_0x0[0x20];
    s32 field_0x20;
    u8 pad_0x24[0xa8 - 0x24];
};

struct UnkRush_b0195 {
    union {
        void** vtable;
        u8 pad_0x0[0x8];
    };
    union {
        void** machine_vtable;
        UnkSmMachine_b0195 machine;
    };
    bool flag_0xb8;
    bool flag_0xb9;
    bool flag_0xba;
    bool flag_0xbb;
    f32 field_0xbc;
    s32 field_0xc0;
    f32 field_0xc4;
    f32 field_0xc8;
    f32 field_0xcc;
    s32 field_0xd0;
    u8 pad_0xd4[4];
    UnkRushItems_b0195 items_0xd8;
    u8 pad_0xe8[0xf8 - 0xe8];
    UnkRushSink_b0195* sink_0xf8;
    u32 level;
    u8 pad_0x104[4];
    UnkRefObject* ref_0x108;
    u8 pad_0x110[0x998 - 0x110];
    u8 pad_0x998[0xa38 - 0x998];
    f64 field_0xa38;
    s32 field_0xa40;
    union {
        u16 clear_0xa44;
        struct {
            u8 pad_0xa44;
            bool flag_0xa45;
        };
    };
    bool flag_0xa46;
    u8 pad_0xa47;
    void* field_0xa48;
    s32 field_0xa50;
    f32 field_0xa54;
    f32 field_0xa58;
    f32 field_0xa5c;
    u8 pad_0xa60[0xc60 - 0xa60];
    s32 survival_record_0xc60;
    f32 survival_time_0xc64;
    bool new_record_0xc68;
    u8 pad_0xc69[3];
    s32 best_0xc6c;
    u8 pad_0xc70[4];
    s32 count_0xc74;
    u8 pad_0xc78[0xc80 - 0xc78];
    u8 link_0xc80[0x18];
    UnkWeakHandle* handle_0xc98;
    u8 pad_0xca0[0xdf0 - 0xca0];
    f32 timer_0xdf0;
    bool flag_0xdf4;
    u8 pad_0xdf5[3];
    u8 sub_0xdf8[1];
    u8 pad_0xdf9[0x13ec - 0xdf9];
    f32 field_0x13ec;
};

static inline UnkRushLevel_b0195* LevelAt_b0195(UnkRush_b0195* self, u32 index) {
    return reinterpret_cast<UnkRushLevel_b0195*>(reinterpret_cast<char*>(self) + 0xa8ull * index);
}

struct UnkKeyedFloats_b0195 {
    f32* values;
    u32* keys;
    u32 count;
};

__attribute__((always_inline)) static inline bool KeyedEqual_b0195(UnkKeyedFloats_b0195* a, UnkKeyedFloats_b0195* b) {
    u64 n = a->count;
    if (n != b->count)
        return false;
    if (n == 0)
        return true;
    f32* b_values = b->values;
    u32* b_keys = b->keys;
    u32* a_keys = a->keys;
    u64 i = 0;
    do {
        u64 j = 0;
        do {
            if (b_keys[j] == a_keys[i])
                break;
            ++j;
            if (j == n)
                return false;
        } while (true);
        u32 found = static_cast<u32>(j);
        if (found == n)
            return false;
        if (b_values[found] != a->values[i])
            return false;
        ++i;
    } while (i != n);
    return true;
}

void fn_7100dd5ec0(UnkRushSub_b0195* sub);
void fn_7100dcb178(UnkRush_b0195* self, u32 level, bool flag);
void fn_7100dcbd88(UnkRush_b0195* self);

void fn_7100dd11c0(UnkRush_b0195* self, s32 mode) {
    fn_7100dd5ec0(reinterpret_cast<UnkRushSub_b0195*>(self->sub_0xdf8));
    self->timer_0xdf0 = 0;
    if (mode != 1) {
        if (self->flag_0xdf4)
            fn_7100dcb178(self, self->level, true);
        else if (self->flag_0xba)
            fn_7100dcb178(self, self->level, false);
        else
            fn_7100dcbd88(self);
    }
    self->flag_0xdf4 = false;
}

struct UnkRushEvent3_b0195 {
    u8 pad_0x0[0x10];
    struct UnkRushEvent2_b0195* field_0x10;
};
struct UnkRushEvent2_b0195 {
    u8 pad_0x0[0x38];
    struct UnkRushEvent1_b0195* field_0x38;
};
struct UnkRushEvent1_b0195 {
    u8 pad_0x0[0x30];
    void* field_0x30;
};
struct UnkRushEvent_b0195 {
    s32 kind;
    UnkRushEvent3_b0195* field_0x8;
};

bool fn_7100dd13f4(UnkRush_b0195* self, UnkRushEvent_b0195* event) {
    if (event->kind == 3) {
        UnkRushEvent3_b0195* e = event->field_0x8;
        u32 level = self->level;
        UnkRushLevel_b0195* l = LevelAt_b0195(self, (u32)level);
        UnkRushEvent1_b0195* inner = e->field_0x10->field_0x38;
        void* wanted = l->field_0x18;
        if (inner->field_0x30 == wanted) {
            // The original addresses the level a second time here.
            self->field_0xa40 = reinterpret_cast<UnkRushLevelB_b0195*>(reinterpret_cast<char*>(self) + 0x140)[level].field_0x20;
            if (self->handle_0xc98) {
                if (self->handle_0xc98->target)
                    fn_710117b3f0(self->handle_0xc98->target, self->link_0xc80);
                ResetRef(&self->handle_0xc98);
            }
        }
    }
    return true;
}

extern char g_unk_71727bd078[];  // a tunable of the unit

struct UnkRushTunables_b0195 {
    u8 pad_0x0[0x24];
    f32 field_0x24;
};

struct UnkVec {
    u64 xy;
    u32 z;
};

void fn_7100dcdc2c(UnkRush_b0195* self) {
    self->flag_0xa45 = true;
    UnkGameManager* manager = g_unk_7101d47310;
    void* effects = &manager->field_0x1040;
    f32 amount;
    if (self->flag_0xbb) {
        if (self->level == 12 && self->field_0xd0 >= *reinterpret_cast<s32*>(g_unk_71727bd078 + 0x34) * 12 - 1)
            amount = 1.0f;
        else
            amount = 0.0f;
    } else if (!self->flag_0xba && self->level == 12) {
        amount = 1.0f;
    } else {
        amount = 0.0f;
    }
    alignas(8) f32 v[3] = {amount, amount, amount};
    fn_7101053850(effects, reinterpret_cast<UnkVec*>(v), 1, 0.3f, 1.0f);
}

struct UnkRushScoreRow_b0195 {
    u8 pad_0x0[0x998];  // rows of 12 bytes start at +0x998, viewed from the object start
    f32 field_0x998;
    f32 field_0x99c;
    f32 field_0x9a0;
};
static inline UnkRushScoreRow_b0195* ScoreRowAt_b0195(UnkRush_b0195* self, s32 index) {
    return reinterpret_cast<UnkRushScoreRow_b0195*>(reinterpret_cast<char*>(self) + 0xc * (s64)index);
}

void fn_7100dcc110(UnkRush_b0195* self) {
    ++self->count_0xc74;
    if (self->flag_0xb8 && self->flag_0xbb) {
        bool flag = self->flag_0xb9;
        fn_7100dcc2cc(self);
        fn_7100dcc404(self, flag, true, false);
        return;
    }
    UnkRushScoreRow_b0195* row = ScoreRowAt_b0195(self, (s32)self->level);
    // An untyped copy: the original reloads the flag after it.
    __builtin_memcpy(&row->field_0x9a0, &row->field_0x99c, sizeof(f32));
    if (self->flag_0xb8) {
        bool flag = self->flag_0xba;
        if (flag)
            self->field_0xa38 = 0;
        self->flag_0xdf4 = true;
        if (!(self->flag_0xbb | flag)) {
            f64 step = reinterpret_cast<UnkRushTunables_b0195*>(g_unk_71727bd078)->field_0x24;
            self->field_0xa38 = self->field_0xa38 + step;
        }
    } else {
        self->flag_0xdf4 = true;
        f64 step = reinterpret_cast<UnkRushTunables_b0195*>(g_unk_71727bd078)->field_0x24;
        self->field_0xa38 = self->field_0xa38 + step;
    }
    RequestState_b0195<4, 4>(&self->machine, 3);
}

struct UnkRushBoard_b0195 {
    u64 field_0x0;
    u64 field_0x8;
    union {
        u64 field_0x10;
        struct {
            u32 step;
            bool field_0x14;
        };
    };
    u64 field_0x18;
    base::UnkAllocator* allocator;  // 0x20
    union {
        u64 field_0x28;
        bool flag_0x28;
    };

    __attribute__((always_inline)) UnkRushBoard_b0195()
        : field_0x0(0), field_0x8(0), field_0x10(0), field_0x18(0xf), allocator(nullptr), field_0x28(0) {
        step = 10;
        field_0x14 = true;
        allocator = fn_7100083990();
        flag_0x28 = true;
    }
};
struct UnkSettings_b0158;
extern UnkSettings_b0158* g_unk_71726bc568;

void fn_7100e8f9a0(char* world, bool remove, base::global::CStrId* key);
extern CStrId g_unk_71727bdb88 __attribute__((visibility("hidden")));
struct UnkSoundMgr;
extern UnkSoundMgr* g_unk_7101d4d820;

void fn_7100dcc2cc(UnkRush_b0195* self) {
    fn_7100e8f9a0(static_cast<char*>(g_unk_7101d47310->field_0x2228), true, &g_unk_71727bdb88);
    if (self->flag_0xb8) {
        UnkRushBoard_b0195* board = reinterpret_cast<UnkRushBoard_b0195*>(g_unk_71726bc568);
        if (!board) {
            board = new (fn_71000839a0(0x30, 0x3f)) UnkRushBoard_b0195;
            g_unk_71726bc568 = reinterpret_cast<UnkSettings_b0158*>(board);
        }
        board->flag_0x28 = true;
    }
    self->flag_0xb8 = false;
    self->flag_0xba = false;
    if (self->handle_0xc98) {
        if (self->handle_0xc98->target)
            fn_710117b3f0(self->handle_0xc98->target, self->link_0xc80);
        ResetRef(&self->handle_0xc98);
    }
    if (self->field_0xa48) {
        fn_71011b57c4(g_unk_717271e8f8, self->field_0xa48);
        self->field_0xa48 = nullptr;
    }
    if (self->flag_0xb8 && self->field_0x13ec >= 0.0f) {
        fn_710010806c(g_unk_7101d4d820, self->field_0x13ec);
        self->field_0x13ec = -1.0f;
    }
}

// A record of six string ids, and two lists (+0x58, +0x80) of items handed back to their sinks.
struct UnkRushIds_b0195 {
    CStrId id_0x0;
    CStrId id_0x8;
    CStrId id_0x10;
    CStrId id_0x18;
    u8 pad_0x20[8];
    CStrId id_0x28;
    CStrId id_0x30;
    CStrId id_0x38;
    u8 pad_0x40[0x58 - 0x40];
    void* item_0x58;
    u32 field_0x60;
    u32 field_0x64;
    u8 pad_0x68[0x78 - 0x68];
    UnkRushSink_b0195* sink_0x78;
    void* item_0x80;
    u32 field_0x88;
    u32 field_0x8c;
    u8 pad_0x90[0xa0 - 0x90];
    UnkRushSink_b0195* sink_0xa0;
};

void fn_7100db8468(UnkRushIds_b0195* self) {
    if (self->item_0x80) {
        self->field_0x88 = 0;
        self->sink_0xa0->vf_0x48(self->item_0x80);
        self->field_0x8c = 0;
    }
    if (self->item_0x58) {
        self->field_0x60 = 0;
        self->sink_0x78->vf_0x48(self->item_0x58);
        self->field_0x64 = 0;
    }
    self->id_0x38.Reset();
    self->id_0x30.Reset();
    self->id_0x28.Reset();
    self->id_0x18.Reset();
    self->id_0x10.Reset();
    self->id_0x8.Reset();
    self->id_0x0.Reset();
}

// Three parameter records of 0x70 bytes at +0xca0, each with a reflected value at +0x18 and eight
// words from +0x30.
struct UnkRushParam_b0195 {
    void** vtable;
    u8 pad_0x8[0x10];
    u8 value_0x18[0x18];
    u64 field_0x30;
    u64 field_0x38;
    u64 field_0x40;
    u64 field_0x48;
    u64 field_0x50;
    u64 field_0x58;
    u64 field_0x60;
    u64 field_0x68;
};
extern void* g_unk_7101aa9608[];

__attribute__((always_inline)) static inline void DestroyParam_b0195(UnkRushParam_b0195* p) {
    p->field_0x68 = 0;
    p->field_0x60 = 0;
    p->field_0x58 = 0;
    p->field_0x50 = 0;
    p->field_0x48 = 0;
    p->field_0x40 = 0;
    p->field_0x38 = 0;
    p->field_0x30 = 0;
    p->vtable = &g_unk_7101aa9608[2];
    fn_71000a1c60(reinterpret_cast<base::reflection::UnkReflValue*>(p->value_0x18));
}

extern void* g_unk_7101aa94b8[];
extern void* g_unk_7101aa9660[];
extern void* g_unk_7101a22d48[];
void fn_7100dd27ec(UnkRushSub_b0195* sub);
struct UnkRushRecord_b0195;
void fn_7100db8374(UnkRushRecord_b0195* record);

void fn_7100db8154(UnkRush_b0195* self) {
    char* raw = reinterpret_cast<char*>(self);
    self->vtable = &g_unk_7101aa94b8[2];
    fn_7100dd27ec(reinterpret_cast<UnkRushSub_b0195*>(self->sub_0xdf8));
    DestroyParam_b0195(reinterpret_cast<UnkRushParam_b0195*>(raw + 0xd80));
    DestroyParam_b0195(reinterpret_cast<UnkRushParam_b0195*>(raw + 0xd10));
    DestroyParam_b0195(reinterpret_cast<UnkRushParam_b0195*>(raw + 0xca0));
    *reinterpret_cast<void***>(self->link_0xc80) = &g_unk_7101a22d48[2];
    if (self->handle_0xc98) {
        if (self->handle_0xc98->target)
            fn_710117b3f0(self->handle_0xc98->target, self->link_0xc80);
        if (UnkWeakHandle* handle = self->handle_0xc98) {
            if (--handle->ref_count == 0)
                handle->vf_0x10();
        }
    }
    fn_7100db8374(reinterpret_cast<UnkRushRecord_b0195*>(raw + 0xbb8));
    fn_7100db8374(reinterpret_cast<UnkRushRecord_b0195*>(raw + 0xb10));
    fn_7100db8374(reinterpret_cast<UnkRushRecord_b0195*>(raw + 0xa68));
#pragma clang loop unroll(full)
    for (int i = 12; i >= 0; --i)
        fn_7100db8468(reinterpret_cast<UnkRushIds_b0195*>(raw + 0x110 + 0xa8 * i));
    if (UnkRefObject* ref = self->ref_0x108) {
        if (--ref->ref_count == 0)
            ref->vf_0x10();
    }
    if (self->items_0xd8.data) {
        self->items_0xd8.count = 0;
        self->sink_0xf8->vf_0x48(self->items_0xd8.data);
        __builtin_memset(reinterpret_cast<char*>(self) + 0xe4, 0, sizeof(u32));
    }
    void** machine_vtable = &g_unk_7101aa9660[2];
    // Keeps the vtable address loaded ahead of the stores that follow.
    asm("" : "+r"(machine_vtable));
    self->machine.count = 0;
    self->machine.current = nullptr;
    self->machine.previous = nullptr;
    self->machine_vtable = machine_vtable;
}

// The user data's owner, created on first use (as src/game/CBossRushManager_SurvivalReset_b49.cpp has it).
struct UnkUserDataOwner;
extern UnkUserDataOwner* g_unk_7101d47308;
CBlackboard* fn_710120384c(UnkUserDataOwner* self);

static inline UnkUserDataOwner* GetOwner() {
    if (!g_unk_7101d47308) {
        UnkUserDataOwner* owner = static_cast<UnkUserDataOwner*>(fn_71000839a0(0x2b8, 0x3f));
        fn_710120320c(owner);
        g_unk_7101d47308 = owner;
    }
    return g_unk_7101d47308;
}

// A typed value as the section setter takes it (as in src/game/CDoorLifeComponent.cpp).
struct UnkTypedValue {
    const base::reflection::CType* type;
    void* value;
};
void fn_7100332c80(CBlackboard::CSection* section, UnkPropertyName* name, const UnkTypedValue* value);

// "SurvivalRushRecord", "SurvivalRushTime" and the section's id (as CBossRushManager.cpp has them).
extern UnkPropertyEx<> g_unk_71727bcac0;
extern UnkPropertyEx<> g_unk_71727bcb00;
extern const CStrId g_unk_71727bdb80 __attribute__((visibility("hidden")));

// The property's qualified id, built on first use.
static inline UnkQualifiedId& Qid(UnkPropertyEx<>& item) {
    UnkQualifiedId& qid = ItemQualifiedId(item);
    if (qid.id.data == &g_unk_7101d47108->empty)
        fn_7100ae9a10(&qid, ItemSource(item), g_unk_7101d470e0);
    return qid;
}

template <const base::reflection::CType* (*Type)()>
__attribute__((always_inline)) static inline void Store(CBlackboard* data, UnkPropertyEx<>& item, void* value) {
    UnkQualifiedId& name = Qid(item);
    if (CBlackboard::CSection* section = fn_71010472e0(data, g_unk_71727bdb80)) {
        if (!data->field_0xe8) {
            UnkTypedValue v;
            v.type = Type();
            v.value = value;
            fn_7100332c80(section, reinterpret_cast<UnkPropertyName*>(&name), &v);
        }
    }
}

void fn_7100dcd070(UnkRush_b0195* self) {
    if (!self->flag_0xb8 || !self->flag_0xbb)
        return;
    s32 count = self->field_0xd0;
    s32 best = self->best_0xc6c;
    bool is_new = count > best;
    self->new_record_0xc68 = is_new;
    if (is_new)
        self->survival_record_0xc60 = count;
    bool faster = false;
    if (count == best) {
        f32 t = (f32)self->field_0xa38;
        f32 time = t > 0.0f ? __builtin_floorf(t) : __builtin_ceilf(t);
        faster = time > self->survival_time_0xc64;
    }
    if (!(count > best) && !faster)
        return;
    f32 t = (f32)self->field_0xa38;
    self->survival_time_0xc64 = t > 0.0f ? __builtin_floorf(t) : __builtin_ceilf(t);
    CBlackboard* data = fn_710120384c(GetOwner());
    Store<fn_7100014768>(data, g_unk_71727bcac0, &self->survival_record_0xc60);
    Store<fn_710000abbc>(data, g_unk_71727bcb00, &self->survival_time_0xc64);
    fn_7101203854(GetOwner());
}

extern UnkPropertyName g_unk_71727bd270;
extern UnkPropertyName g_unk_71727bd288;
extern UnkPropertyName g_unk_71727bd2a0;

template <UnkPropertyName* Name>
__attribute__((always_inline)) static inline void SetFlag_b0195(CBlackboard* data) {
    UnkTypedValue v;
    CStrId section_name("USER_DATA", true);
    bool value = true;
    CBlackboard::CSection* section = fn_71010472e0(data, section_name);
    if (section && !data->field_0xe8) {
        v.type = fn_710000d228();
        v.value = &value;
        fn_7100332c80(section, Name, &v);
    }
}

// Sets the three "USER_DATA" flags of src/game_aca57c/fn_7100dc8828.cpp in one go.
void fn_7100dc85e0() {
    CBlackboard* data = fn_710120384c(GetOwner());
    SetFlag_b0195<&g_unk_71727bd270>(data);
    SetFlag_b0195<&g_unk_71727bd288>(data);
    SetFlag_b0195<&g_unk_71727bd2a0>(data);
    fn_7101203854(GetOwner());
}
