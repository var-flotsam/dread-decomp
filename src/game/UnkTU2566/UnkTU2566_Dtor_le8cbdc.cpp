// The minimap manager's unit (CMinimapManager, CTunableMinimapManager): the owner's destructor, the
// constructor (fn_7100e8c114) undone member by member in reverse. Names and layouts are placeholders.
#include "base/UnkCountedResource.h"
#include "base/UnkDeferredSet.h"
#include "base/global/CRntSmallDictionary.h"
#include "base/global/CRntString.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "game/CAbilityEnergyWave.h"
#include "game/CAbilityLayout.h"
#include "game/CGunComponent.h"
#include "game/CInputComponent.h"
#include "game/UnkComponent.h"
#include "game/UnkTU2566.h"
#include "game/UnkTU372.h"
#include "game/UnkTU39.h"
#include "game/UnkTU426.h"
#include "types.h"

using base::global::CRntString;
using base::global::CRntVector;
using base::global::CStrId;

extern void* g_unk_7101ca9e38[];

// The clears of the owner's paired arrays, each defined with its own view of them.
struct UnkGroupTable_b0200;
void fn_7100eb3648(UnkGroupTable_b0200* self);
struct UnkReleasedStrId;
struct UnkIdValues;
using UnkIdValuesArrays = base::global::CRntSmallDictionary<UnkReleasedStrId, base::global::CRntVector<UnkIdValues>>;
void fn_7100eb1824(UnkIdValuesArrays* self);
struct UnkResIdTable;
void fn_7100eb346c(UnkResIdTable* self);
void fn_7100e8d784(void* set);  // the pooled set's destructor

// The removals from the sets the owner's callbacks were added to.
struct UnkHintRef_ovb0093;
void fn_7100752ba8(base::UnkDeferredSet<UnkHintRef_ovb0093>* set, UnkHintRef_ovb0093* item);
void fn_7100a73fd0(base::UnkDeferredSet<void>* set, void* item);
void fn_7100eb437c(base::UnkDeferredSet<void>* set, void* item);

// A callback bound to the owner (0x18 bytes): the set it was added to, the object and the function.
struct UnkMinimapCallback_e8cbdc {
    void* set;
    void* object;
    void (*fn)(void*);

    // Leaves the set (`remove` is that set's removal), then forgets the object and the function.
    template <class Remove>
    void Unbind(Remove remove) {
        if (set) {
            remove(this);
            set = nullptr;
        }
        fn = nullptr;
        object = nullptr;
    }
};
static_assert(sizeof(UnkMinimapCallback_e8cbdc) == 0x18, "UnkMinimapCallback_e8cbdc layout");

inline CRntString* AsString(base::UnkNamedEntry& entry) { return reinterpret_cast<CRntString*>(&entry); }

// A modifier (0xd8 bytes): a name, and a label kept in a buffer after it.
struct UnkMinimapModifier_e8cbdc {
    u8 unk_0x0[0x18];
    base::UnkNamedEntry name;  // 0x18
    u8 unk_0x48[0x18];
    base::UnkNamedEntry label;  // 0x60
    u8 unk_0x90[0x48];

    ~UnkMinimapModifier_e8cbdc() {
        fn_710007ed40(AsString(label));
        fn_710007ecd4(AsString(label));
        fn_710007ecd4(AsString(name));
    }
};
static_assert(sizeof(UnkMinimapModifier_e8cbdc) == 0xd8, "UnkMinimapModifier_e8cbdc layout");

// The first 0x50 bytes of a modifier on their own.
struct UnkMinimapModifierHead_e8cbdc {
    u8 unk_0x0[0x18];
    base::UnkNamedEntry name;  // 0x18
    u8 unk_0x48[0x8];

    ~UnkMinimapModifierHead_e8cbdc() { fn_710007ecd4(AsString(name)); }
};
static_assert(sizeof(UnkMinimapModifierHead_e8cbdc) == 0x50, "UnkMinimapModifierHead_e8cbdc layout");

// Two arrays sharing a count and an allocator (0x28 bytes).
struct UnkMinimapArrays_e8cbdc {
    void* first;
    void* second;
    u32 count;
    u32 capacity;
    u32 step;
    u32 flags;
    base::UnkAllocator* allocator;

    void FreeArrays() {
        if (second)
            allocator->vf_0x48(second);
        if (first)
            allocator->vf_0x48(first);
    }
};
static_assert(sizeof(UnkMinimapArrays_e8cbdc) == 0x28, "UnkMinimapArrays_e8cbdc layout");

struct UnkMinimapRecord_e8cbdc {
    u8 unk_0x0[0x118];
};

// The same, with records (0x118 bytes each) in the first array.
struct UnkMinimapRecords_e8cbdc {
    UnkMinimapRecord_e8cbdc* records;
    void* second;
    u32 count;
    u32 capacity;
    u32 step;
    u32 flags;
    base::UnkAllocator* allocator;

    ~UnkMinimapRecords_e8cbdc() {
        if (records && count)
            for (u64 i = 0; i < count; ++i)
                fn_7100ea7034(reinterpret_cast<UnkRecord118*>(&records[i]));
        count = 0;
        if (second)
            allocator->vf_0x48(second);
        if (records)
            allocator->vf_0x48(records);
    }
};
static_assert(sizeof(UnkMinimapRecords_e8cbdc) == 0x28, "UnkMinimapRecords_e8cbdc layout");

// A counted object released through its vtable.
struct UnkMinimapRef_e8cbdc {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void Release();  // 0x10
    u32 ref_count;           // 0x8
};

struct UnkMinimapInputOwner_e8cbdc {
    u8 unk_0x0[0x1d8];
    void* input;  // 0x1d8
};

// The input component of the component's actor.
inline void* InputOf(UnkBaseComponent* component) {
    return static_cast<UnkMinimapInputOwner_e8cbdc*>(component->field_0x30)->input;
}

// The owner, as its destructor sees it.
struct UnkMinimapManager_e8cbdc {
    void** vtable;
    CRntVector<void*> field_0x8;  // the base class's member
    u8 unk_0x30[0x30];
    UnkCountedResource* field_0x60;
    UnkCountedResource* field_0x68;
    UnkMinimapArrays_e8cbdc field_0x70;
    u8 unk_0x98[0x78];
    CStrId field_0x110;
    u8 cells[0xd63cd8 - 0x118];
    u64 field_0xd63cd8;
    CStrId field_0xd63ce0;
    u8 unk_0xd63ce8[0x20];
    CRntVector<void*> field_0xd63d08;
    u8 unk_0xd63d30[0x60];
    u8 timer_0xd63d90[0xb0];
    u8 timer[0xb0];
    UnkAbilityInputListener input;  // 0xd63ef0, "MINIMAP"
    UnkBaseComponent* input_target;
    UnkMinimapRef_e8cbdc* field_0xd63f40;
    void* camera;
    void* viewport;
    CRntVector<void*> field_0xd63f58;
    u8 unk_0xd63f80[0x58];
    UnkMinimapCallback_e8cbdc callbacks[20];
    u8 unk_0xd641b8[0x18];
    CStrId field_0xd641d0;
    u64 field_0xd641d8;
    CRntVector<void*> markers;
    CStrId field_0xd64208;
    u8 pooled_set[0x90];
    UnkMinimapArrays_e8cbdc field_0xd642a0;
    CRntVector<CStrId> hidden_kinds;
    CRntVector<CStrId> field_0xd642f0;
    CRntVector<void*> pass_items[2];
    UnkMinimapRecords_e8cbdc field_0xd64368;
    CRntVector<CStrId> field_0xd64390;
    UnkMinimapArrays_e8cbdc regions;
    u8 unk_0xd643e0[0x10];
    UnkMinimapRecords_e8cbdc field_0xd643f0;
    u32 field_0xd64418;
    CRntVector<void*> field_0xd64420;
    CRntVector<void*> field_0xd64448;
    UnkMinimapModifier_e8cbdc field_0xd64470;
    UnkMinimapModifier_e8cbdc field_0xd64548;
    UnkMinimapModifierHead_e8cbdc field_0xd64620;
    UnkMinimapModifier_e8cbdc field_0xd64670;
    u64 field_0xd64748;
    u32 field_0xd64750;
    CStrId field_0xd64758;
};
static_assert(__builtin_offsetof(UnkMinimapManager_e8cbdc, field_0x110) == 0x110, "layout");
static_assert(__builtin_offsetof(UnkMinimapManager_e8cbdc, field_0xd63ce0) == 0xd63ce0, "layout");
static_assert(__builtin_offsetof(UnkMinimapManager_e8cbdc, input) == 0xd63ef0, "layout");
static_assert(__builtin_offsetof(UnkMinimapManager_e8cbdc, input_target) == 0xd63f38, "layout");
static_assert(__builtin_offsetof(UnkMinimapManager_e8cbdc, callbacks) == 0xd63fd8, "layout");
static_assert(__builtin_offsetof(UnkMinimapManager_e8cbdc, field_0xd641d0) == 0xd641d0, "layout");
static_assert(__builtin_offsetof(UnkMinimapManager_e8cbdc, field_0xd642a0) == 0xd642a0, "layout");
static_assert(__builtin_offsetof(UnkMinimapManager_e8cbdc, regions) == 0xd643b8, "layout");
static_assert(__builtin_offsetof(UnkMinimapManager_e8cbdc, field_0xd643f0) == 0xd643f0, "layout");
static_assert(__builtin_offsetof(UnkMinimapManager_e8cbdc, field_0xd64470) == 0xd64470, "layout");
static_assert(__builtin_offsetof(UnkMinimapManager_e8cbdc, field_0xd64758) == 0xd64758, "layout");

void fn_7100e8cbdc(UnkMinimapManager_e8cbdc* self) {
    self->vtable = g_unk_7101ca9e38 + 2;
    self->field_0xd64758.~CStrId();
    self->field_0xd64670.~UnkMinimapModifier_e8cbdc();
    self->field_0xd64620.~UnkMinimapModifierHead_e8cbdc();
    self->field_0xd64548.~UnkMinimapModifier_e8cbdc();
    self->field_0xd64470.~UnkMinimapModifier_e8cbdc();
    self->field_0xd64448.Free();
    self->field_0xd64420.Free();
    self->field_0xd643f0.~UnkMinimapRecords_e8cbdc();
    fn_7100eb3648(reinterpret_cast<UnkGroupTable_b0200*>(&self->regions));
    self->regions.FreeArrays();
    self->field_0xd64390.Free();
    self->field_0xd64368.~UnkMinimapRecords_e8cbdc();
    self->pass_items[1].Free();
    self->pass_items[0].Free();
    self->field_0xd642f0.Free();
    self->hidden_kinds.Free();
    fn_7100eb1824(reinterpret_cast<UnkIdValuesArrays*>(&self->field_0xd642a0));
    self->field_0xd642a0.FreeArrays();
    fn_7100e8d784(self->pooled_set);
    self->field_0xd64208.~CStrId();
    self->markers.Free();
    self->field_0xd641d0.~CStrId();

    UnkMinimapCallback_e8cbdc* callbacks = self->callbacks;
    callbacks[19].Unbind([](UnkMinimapCallback_e8cbdc* cb) {
        fn_71002b46ec(static_cast<UnkLinkList*>(cb->set), reinterpret_cast<UnkLinkList**>(&cb->set));
    });
    callbacks[18].Unbind([](UnkMinimapCallback_e8cbdc* cb) { fn_7100840f50(cb->set, &cb->set); });
    callbacks[17].Unbind([](UnkMinimapCallback_e8cbdc* cb) {
        fn_7100a73fd0(static_cast<base::UnkDeferredSet<void>*>(cb->set), cb);
    });
    callbacks[16].Unbind([](UnkMinimapCallback_e8cbdc* cb) {
        fn_7100eb437c(static_cast<base::UnkDeferredSet<void>*>(cb->set), cb);
    });
    callbacks[15].Unbind([](UnkMinimapCallback_e8cbdc* cb) {
        fn_71004f3ea4(static_cast<base::UnkDeferredSet<void>*>(cb->set), cb);
    });
    for (s32 i = 14; i >= 0; --i)
        callbacks[i].Unbind([](UnkMinimapCallback_e8cbdc* cb) {
            fn_7100752ba8(static_cast<base::UnkDeferredSet<UnkHintRef_ovb0093>*>(cb->set),
                          reinterpret_cast<UnkHintRef_ovb0093*>(cb));
        });

    self->field_0xd63f58.Free();
    if (UnkMinimapRef_e8cbdc* ref = self->field_0xd63f40)
        if (--ref->ref_count == 0)
            ref->Release();

    UnkAbilityInputListener& input = self->input;
    input.vtable = g_unk_7101a22858 + 2;
    if (self->input_target && self->input_target->field_0x30 && InputOf(self->input_target))
        fn_7100872908(InputOf(self->input_target), &input);
    self->input_target = nullptr;
    input.vtable = g_unk_7101a22878 + 2;
    if (input.entries) {
        fn_7100083c30(static_cast<u64*>(input.entries) - 1);
        input.entries = nullptr;
    }
    fn_710007ecd4(AsString(input.name));

    fn_710002f67c(self->timer);
    fn_710002f67c(self->timer_0xd63d90);
    self->field_0xd63d08.Free();
    self->field_0xd63ce0.~CStrId();
    self->field_0x110.~CStrId();
    fn_7100eb346c(reinterpret_cast<UnkResIdTable*>(&self->field_0x70));
    self->field_0x70.FreeArrays();
    if (UnkCountedResource* p = self->field_0x68)
        if (--p->ref_count == 0)
            fn_710001737c(p);
    if (UnkCountedResource* p = self->field_0x60)
        if (--p->ref_count == 0)
            fn_710001737c(p);

    self->vtable = g_unk_7101a22398 + 2;
    fn_71002f6ac0(&self->field_0x8);
    self->field_0x8.Free();
}
