// best draft of fn_7100dcafbc for batch b0195 (differs by one or two scheduling instructions); not under src/
// CBossRushSurvivalConfiguration's unit (no reliable name): helpers of one boss-rush survival object
// (levels, a small state machine at +0x8, a score). Names and layouts are placeholders.
#include <new>

#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflCast.h"
#include "base/reflection/UnkReflAttributes.h"
#include "game/CBlackboard.h"
#include "game/UnkGameBlackboard.h"
#include "game/UnkGuiRoot.h"
#include "game/UnkItemProperties.h"
#include "game/UnkTUProperty.h"
#include "game/UnkManager_71726af468.h"
#include "game/UnkActor.h"
#include "game/UnkTimelineCalls.h"
#include "game/UnkTimelineComponent.h"
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
            asm("" : "+r"(enter));
            next = nullptr;
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

extern f32 g_unk_7101aa94a4;
extern f32 g_unk_7101aa94a8;
extern f32 g_unk_7101aa949c;
extern f32 g_unk_7101aa94a0;

extern char g_unk_71727bd078[];  // a tunable of the unit

struct UnkRushTunables_b0195 {
    u8 pad_0x0[0x24];
    f32 field_0x24;
};

struct UnkRushVec3_b0195 {
    u64 xy;
    u32 z;
};
void fn_7101053850(void* effects, UnkRushVec3_b0195* position, u32 flag, f32 duration, f32 scale);

struct UnkRushScoreRow_b0195 {
    u8 pad_0x0[0x998];  // rows of 12 bytes start at +0x998, viewed from the object start
    f32 field_0x998;
    f32 field_0x99c;
    f32 field_0x9a0;
};
static inline UnkRushScoreRow_b0195* ScoreRowAt_b0195(UnkRush_b0195* self, s32 index) {
    return reinterpret_cast<UnkRushScoreRow_b0195*>(reinterpret_cast<char*>(self) + 0xc * (s64)index);
}

void fn_7100dcca90(UnkRush_b0195* self);
void fn_7100dccd90(UnkRush_b0195* self);

s32 fn_7100dd2ca8(UnkRushSub_b0195* sub);

void fn_7100dcafbc(UnkRush_b0195* self, f32 dt) {
    self->timer_0xdf0 = self->timer_0xdf0 + dt;
    switch (fn_7100dd2ca8(reinterpret_cast<UnkRushSub_b0195*>(self->sub_0xdf8))) {
    case 1:
        RequestState_b0195<4, 4>(&self->machine, 2);
        break;
    case 2:
        RequestState_b0195<4, 4>(&self->machine, 1);
        break;
    }
}

void fn_7100dcc2cc(UnkRush_b0195* self);
void fn_7100dcc404(UnkRush_b0195* self, bool a, bool b, bool c);

struct UnkRushStageParams_b0195 {
    u8 pad_0x0[8];
    u32 field_0x8;
    u32 field_0xc;
    u32 field_0x10;
};
extern UnkRushStageParams_b0195* g_unk_7101aa96a0[] __attribute__((visibility("hidden")));

struct UnkRushTunableParams_b0195 {
    u8 pad_0x0[0x98];
    UnkRushStageParams_b0195 field_0x98;
};

struct UnkRushSetup_b0195 {
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
    virtual bool vf_0x68();
    virtual void vf_0x70();
};

void fn_7100dca630(UnkRush_b0195* self, bool flag);
void fn_7100dd3520(UnkRushSub_b0195* sub);

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
extern UnkRushBoard_b0195* g_unk_71726bc568;

void fn_7100e8f9a0(char* world, bool remove, CStrId* key);
extern CStrId g_unk_71727bdb88 __attribute__((visibility("hidden")));
struct UnkSoundMgr;
extern UnkSoundMgr* g_unk_7101d4d820;
void fn_710010806c(UnkSoundMgr* self);

struct UnkRushBehavior_b0195 {
    u8 pad_0x0[0x100];
    struct UnkRushBehaviorSlot_b0195* field_0x100;
};
struct UnkRushBehaviorSlot_b0195 {
    u8 pad_0x0[0x10];
    struct UnkRushActor_b0195* field_0x10;
};
struct UnkRushActorInfo_b0195 {
    u8 pad_0x0[0x70];
    s32 field_0x70;
};
struct UnkRushActor_b0195 {
    u8 pad_0x0[0x1b8];
    UnkRushActorInfo_b0195* field_0x1b8;
};
struct UnkRushNodeSlot_b0195 {
    u8 pad_0x0[0x30];
    UnkRushActor_b0195* field_0x30;
};
struct UnkNodeSet695_b43a1;
base::reflection::UnkReflObject* fn_7100475564(UnkNodeSet695_b43a1* self);
void fn_71011a664c(char* instance);
extern char* g_unk_71726b2398;
struct UnkDoorRegistry_b61;
void fn_7100ddb6dc(UnkDoorRegistry_b61* self);
extern UnkDoorRegistry_b61* g_unk_71726bee10;
struct UnkUnit2547;
struct UnkCentralUnit;
UnkCentralUnit* fn_7100ddb768(UnkUnit2547* self);
struct UnkZiplineLink_b49;
struct UnkZiplineLinkTarget_b49;
UnkZiplineLinkTarget_b49** fn_71003b0c4c(UnkZiplineLink_b49* source);

struct UnkRushLinkNode_b0195 {
    u8 pad_0x0[0x10];
    UnkRushActor_b0195* field_0x10;
};

struct UnkPlayer;
UnkPlayer* fn_7100dec788(UnkGameManager*);
struct UnkRushPlayerStats_b0195 {
    u8 pad_0x0[0x54];
    f32 field_0x54;
};
struct UnkRushPlayer_b0195 {
    u8 pad_0x0[0x1b8];
    UnkRushPlayerStats_b0195* field_0x1b8;
};

struct UnkRushStaticZero_b0195 {
    u32 value;
    UnkRushStaticZero_b0195() : value(0) {}
};

inline const UnkRushStaticZero_b0195& UnkRushStaticZero() {
    static UnkRushStaticZero_b0195 s_value asm("g_unk_7101d57cc8");
    return s_value;
}

inline u32 UnkRushDefaultMask() {
    static u32 s_mask asm("g_unk_7101d57cb8") = ~UnkRushStaticZero().value & 0xff;
    return s_mask;
}

struct UnkOwner;
UnkOwner* fn_7100defcd4(UnkGameManager*);
extern u64 g_unk_710164bcb0;

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

// The user data's owner, created on first use (as src/game/CBossRushManager_SurvivalReset_b49.cpp has it).
struct UnkUserDataOwner;
extern UnkUserDataOwner* g_unk_7101d47308;
void fn_710120320c(UnkUserDataOwner* self);  // constructor
CBlackboard* fn_710120384c(UnkUserDataOwner* self);
void fn_7101203854(UnkUserDataOwner* self);

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
