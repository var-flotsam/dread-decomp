// CCentralUnitAIComponent's unit (tools/tu_map.py unit 614): for the link `index` of the component's
// links at +0x146d0, takes a resolved copy of it; when the linked object is registered, has a Rinka unit
// (fn_7100405ba0) that passes its slot 0x168 check, and either has no part or the tunable allows
// simultaneous Rinkas, the unit gets the value at +0x14730 (fn_710065ff7c). Otherwise the part's AI
// component (fn_7100405c7c) is asked (fn_71003fc314). Names and layouts are placeholders.
#include "base/UnkErasedVector.h"
#include "base/UnkRefHandle.h"
#include "base/reflection/ReflectedAttributes.h"
#include "base/global/CRntVector.h"
#include "game/CAISmartObjectComponent.h"
#include "game/UnkTU341.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

struct UnkCentralUnitPart_l405854;
struct UnkRinkaUnit_b61;
void* fn_7100405ba0(void* object);
void* fn_7100405c7c(void* object);
void CRinkaUnitComponent_RequestState2WithOwnRinkaType(UnkRinkaUnit_b61* self, u32 a);
template <u32 Offset>
struct UnkSmOwner_A2_b63;
bool fn_71003fc314(UnkSmOwner_A2_b63<0x14978>* self);

// The central unit's tunable (CTunableCharClassCentralUnitAIComponent).
extern base::reflection::UnkTunable g_unk_71726c42a0;
struct UnkCentralUnitTunable_l405854 {
    u8 pad_0x0[0x21];
    bool bAllowSimultaneousRinkas;  // 0x21
};

struct UnkCentralUnitPartHolder_l405854 {
    u8 pad_0x0[0x10];
    UnkCentralUnitPart_l405854* part;  // 0x10
};

struct UnkCentralUnitRegistered_l405854 {
    void** vtable;
    u8 pad_0x8[0x38];
    UnkCentralUnitPartHolder_l405854* holder;  // 0x40

    bool vf_0x168(f32 value) {
        return reinterpret_cast<bool (*)(UnkCentralUnitRegistered_l405854*, f32)>(vtable[0x168 / 8])(this, value);
    }
};

// A link to an actor (the 0x40-byte callback set of vtable g_unk_7101a20950): the key the lookup
// reads, a state that is 1 until the link is resolved, then 2, and the handle of the target.
struct UnkCentralUnitActor_l405854;

// The weak handle a resolved link holds: its actor at +0x10.
struct UnkCentralUnitLinkTarget_l405854 : base::UnkRefCounted {
    UnkCentralUnitActor_l405854* actor;  // 0x10
};

struct UnkCentralUnitLinkHandle_l405854 : base::UnkRefHandle {
    ~UnkCentralUnitLinkHandle_l405854() {
        if (base::UnkRefCounted* p = ptr) {
            if (--p->ref_count == 0)
                delete p;
        }
    }
};

struct UnkCentralUnitLinkBase_l405854 {
    char* vtable;
    UnkOwnedErasedVector key;  // 0x8
    u32 state;                 // 0x30

    UnkCentralUnitLinkBase_l405854() : state(0) {}
    ~UnkCentralUnitLinkBase_l405854() { vtable = reinterpret_cast<char*>(g_unk_7101a20998 + 2); }
};

struct UnkCentralUnitLink_l405854 : UnkCentralUnitLinkBase_l405854 {
    UnkCentralUnitLinkHandle_l405854 handle;  // 0x38

    UnkCentralUnitLink_l405854(const UnkCentralUnitLink_l405854& other) {
        vtable = reinterpret_cast<char*>(g_unk_7101a20950 + 2);
        handle.ptr = nullptr;
        handle.Set(other.handle.ptr);
        state = other.state;
        fn_710023b29c(&key, &other.key);
    }
    ~UnkCentralUnitLink_l405854() { vtable = reinterpret_cast<char*>(g_unk_7101a20950 + 2); }

    // Resolves the link once and returns its target, or null.
    UnkCentralUnitActor_l405854* Target() {
        if (state == 1) {
            {
                UnkRefPtr ref = fn_710023b580(&key);
                fn_7100234f78(&handle, reinterpret_cast<const base::UnkRefHandle*>(&ref));
            }
            state = 2;
        }
        UnkCentralUnitLinkTarget_l405854* target = static_cast<UnkCentralUnitLinkTarget_l405854*>(handle.ptr);
        return target ? target->actor : nullptr;
    }
};
static_assert(sizeof(UnkCentralUnitLink_l405854) == 0x40, "layout");

struct UnkCentralUnitAI_l405854 {
    u8 pad_0x0[0x146d0];
    base::global::CRntVector<UnkCentralUnitLink_l405854> links;  // 0x146d0
    u8 pad_0x146f8[0x38];
    u32 field_0x14730;
};
static_assert(__builtin_offsetof(UnkCentralUnitAI_l405854, field_0x14730) == 0x14730, "layout");

bool fn_7100405854(UnkCentralUnitAI_l405854* self, u32 index) {
    if (self->links.count <= index)
        return false;
    UnkCentralUnitLink_l405854 link(*reinterpret_cast<UnkCentralUnitLink_l405854*>(
        fn_7100247340(reinterpret_cast<UnkCaterzillaLinks_ovb0054*>(&self->links), index)));
    UnkCentralUnitRegistered_l405854* registered = reinterpret_cast<UnkCentralUnitRegistered_l405854*>(
        fn_7100360044(link.Target()));
    if (!registered)
        return false;
    UnkCentralUnitPart_l405854* part = registered->holder ? registered->holder->part : nullptr;
    void* rinka = fn_7100405ba0(link.Target());
    if (rinka && registered->vf_0x168(0.0f)) {
        const UnkCentralUnitTunable_l405854* tunable =
            reinterpret_cast<const UnkCentralUnitTunable_l405854*>(&g_unk_71726c42a0);
        if (!part || tunable->bAllowSimultaneousRinkas) {
            CRinkaUnitComponent_RequestState2WithOwnRinkaType(static_cast<UnkRinkaUnit_b61*>(rinka), self->field_0x14730);
            return true;
        }
    }
    if (part) {
        if (void* ai = fn_7100405c7c(part))
            return fn_71003fc314(static_cast<UnkSmOwner_A2_b63<0x14978>*>(ai));
    }
    return false;
}
