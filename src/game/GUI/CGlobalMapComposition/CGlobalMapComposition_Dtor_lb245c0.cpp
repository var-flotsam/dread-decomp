// GUI::CGlobalMapComposition's unit (tools/tu_map.py unit 1680): the composition's destructor. Its members go
// last to first: the string ids, the two set registrations, the strings, the regions by id, the six value
// holders and the state machine, then the display object container's destructor.
// Names and layouts are placeholders.
#include "base/UnkCloneHelpers.h"
#include "base/UnkDeferredSet.h"
#include "base/UnkDestructors.h"
#include "base/global/CRntSmallDictionary.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkGuiDisplayObject.h"
#include "types.h"

using base::UnkStrIdMember;
using base::global::CRntString;
using base::global::CStrId;

extern void* g_unk_7101a9b310[];
extern char g_unk_7101a9b5d8[];
extern char g_unk_7101a9b630[];

struct UnkMapRegion_ovb0109;
void CGlobalMapComposition_FreeRegionAreaLists(UnkMapRegion_ovb0109* self);

// A region of the map (0x58 bytes); its destructor is fn_7100b262f4.
struct UnkMapRegion_lb245c0 {
    u8 data[0x58];

    ~UnkMapRegion_lb245c0() { CGlobalMapComposition_FreeRegionAreaLists(reinterpret_cast<UnkMapRegion_ovb0109*>(this)); }
};

// The composition's value holders.
struct UnkMapHolderTag_lb245c0;
template <>
struct base::UnkValueHolderTraits<UnkMapHolderTag_lb245c0> {
    static char* vtable() { return g_unk_7101a9b5d8; }
};
using UnkMapHolder_lb245c0 = base::UnkValueHolder<UnkMapHolderTag_lb245c0>;
static_assert(sizeof(UnkMapHolder_lb245c0) == 0x70, "UnkMapHolder_lb245c0 size");

// A registration in a deferred set: it takes itself out of the set when destroyed.
struct UnkMapRegistration_lb245c0 {
    base::UnkDeferredSet<void>* set;
    void* field_0x8;
    void* field_0x10;

    ~UnkMapRegistration_lb245c0() {
        if (set) {
            fn_71004f3ea4(set, this);
            set = nullptr;
        }
        field_0x10 = nullptr;
        field_0x8 = nullptr;
    }
};

// A string with inline storage: cleared, then destroyed.
struct UnkMapInlineString_lb245c0 {
    CRntString str;
    u8 storage[0x48];

    ~UnkMapInlineString_lb245c0() { CRntString_Clear(&str); }
};

// The state machine: its states are not owned.
struct UnkMapStateMachine_lb245c0 {
    char* vtable;
    u8 pad_0x8[0x8];
    void* current;   // 0x10
    void* previous;  // 0x18
    u32 count;       // 0x20
    u8 pad_0x24[0xac - 0x24];

    ~UnkMapStateMachine_lb245c0() {
        vtable = g_unk_7101a9b630 + 0x10;
        current = nullptr;
        previous = nullptr;
        count = 0;
    }
};

struct UnkGlobalMapComposition_lb245c0 {
    void** vtable;  // the display object container's base
    u8 pad_0x8[0x238 - 0x8];
    UnkMapStateMachine_lb245c0 states;  // 0x238
    UnkMapHolder_lb245c0 holders[6];  // 0x2e8
    u8 pad_0x588[0x5a0 - 0x588];
    CStrId field_0x5a0;
    u8 pad_0x5a8[0x8];
    base::global::CRntSmallDictionary<UnkStrIdMember, UnkMapRegion_lb245c0> regions;  // 0x5b0
    u8 pad_0x5d8[0x638 - 0x5d8];
    CRntString field_0x638;
    u8 pad_0x668[0x680 - 0x668];
    UnkMapInlineString_lb245c0 field_0x680;
    CStrId field_0x6f8;
    CStrId field_0x700;
    CStrId field_0x708;
    u8 pad_0x710[0x740 - 0x710];
    UnkMapRegistration_lb245c0 field_0x740;
    UnkMapRegistration_lb245c0 field_0x758;
    u8 pad_0x770[0x780 - 0x770];
    CStrId field_0x780;
    CStrId field_0x788;
    CStrId field_0x790;
    CStrId field_0x798;
};
static_assert(__builtin_offsetof(UnkGlobalMapComposition_lb245c0, regions) == 0x5b0, "layout");
static_assert(__builtin_offsetof(UnkGlobalMapComposition_lb245c0, field_0x6f8) == 0x6f8, "layout");
static_assert(__builtin_offsetof(UnkGlobalMapComposition_lb245c0, field_0x798) == 0x798, "layout");

void fn_7100b245c0(UnkGlobalMapComposition_lb245c0* self) {
    self->vtable = g_unk_7101a9b310 + 2;
    self->field_0x798.~CStrId();
    self->field_0x790.~CStrId();
    self->field_0x788.~CStrId();
    self->field_0x780.~CStrId();
    self->field_0x758.~UnkMapRegistration_lb245c0();
    self->field_0x740.~UnkMapRegistration_lb245c0();
    self->field_0x708.~CStrId();
    self->field_0x700.~CStrId();
    self->field_0x6f8.~CStrId();
    self->field_0x680.~UnkMapInlineString_lb245c0();
    self->field_0x638.~CRntString();

    base::global::CRntSmallDictionary<UnkStrIdMember, UnkMapRegion_lb245c0>& regions = self->regions;
    regions.Clear();
    if (regions.keys)
        regions.allocator->vf_0x48(regions.keys);
    if (regions.values)
        regions.allocator->vf_0x48(regions.values);

    self->field_0x5a0.~CStrId();
    for (int i = 5; i >= 0; --i)
        base::refl_ValueHolderDtor(&self->holders[i]);

    self->states.~UnkMapStateMachine_lb245c0();
    fn_71011d8e90(reinterpret_cast<UnkGuiDisplayObject*>(self));
}
