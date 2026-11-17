// CWaterNozzleComponent's unit (tools/tu_map.py): its destructor (fn_71008a1148): the callback list at
// +0x2b0 drops its counted reference and frees its callbacks, four value holders from +0x240 down to
// +0xf0 and the member at +0x40 are destroyed, then the component's base destructor runs
// (fn_71010c4194). Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "base/reflection/UnkReflCast.h"
#include "base/reflection/UnkReflValue.h"
#include "game/CCentralUnitAIComponent.h"
#include "game/UnkComponent.h"
#include "game/UnkTU341.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

extern char g_unk_7101a7cf10[];  // the component's vtable
extern char g_unk_7101a7d110[];
extern char g_unk_7101a7d168[];

// A type-erased callback: storage and the manager that destroys it.
struct UnkCallbackManager_l8a1148 {
    u8 pad_0x0[0x40];
    void (*destroy)(void* callback);  // 0x40
};

struct UnkCallback_l8a1148 {
    u64 storage;
    UnkCallbackManager_l8a1148* manager;

    ~UnkCallback_l8a1148() {
        if (manager) {
            manager->destroy(this);
            manager = nullptr;
        }
    }
};

struct UnkCallbackList_l8a1148 {
    void** vtable;
    base::global::CRntVector<UnkCallback_l8a1148> callbacks;  // +0x8
    u8 pad_0x30[0x8];
    UnkRefObject* ref;  // +0x38
};

struct UnkNozzlePair_l8a1148 {
    u64 a, b;
    ~UnkNozzlePair_l8a1148() {
        a = 0;
        b = 0;
    }
};

struct UnkNozzleValueHolderBase_l8a1148 {
    char* vtable;
    u64 field_0x8;
    u64 field_0x10;
    base::reflection::UnkReflValue value;
    ~UnkNozzleValueHolderBase_l8a1148() {
        vtable = g_unk_7101a7d110 + 0x10;
        fn_71000a1c60(&value);
    }
};

struct UnkNozzleValueHolder_l8a1148 : UnkNozzleValueHolderBase_l8a1148 {
    UnkNozzlePair_l8a1148 pairs[4];
};
static_assert(sizeof(UnkNozzleValueHolder_l8a1148) == 0x70, "value holder layout");

struct UnkNozzleMember_l8a1148 {
    char* vtable;
    u64 field_0x8;
    u64 field_0x10;
    u64 field_0x18;
    u32 field_0x20;
    ~UnkNozzleMember_l8a1148() {
        vtable = g_unk_7101a7d168 + 0x10;
        field_0x10 = 0;
        field_0x18 = 0;
        field_0x20 = 0;
    }
};

struct UnkWaterNozzleComponent_l8a1148 {
    char* vtable;
    u8 pad_0x8[0x38];
    UnkNozzleMember_l8a1148 field_0x40;
    u8 pad_0x68[0x88];
    UnkNozzleValueHolder_l8a1148 field_0xf0;
    UnkNozzleValueHolder_l8a1148 field_0x160;
    UnkNozzleValueHolder_l8a1148 field_0x1d0;
    UnkNozzleValueHolder_l8a1148 field_0x240;
    UnkCallbackList_l8a1148 list;  // 0x2b0
};
static_assert(__builtin_offsetof(UnkWaterNozzleComponent_l8a1148, list) == 0x2b0, "layout");

void fn_71008a1148(UnkWaterNozzleComponent_l8a1148* self) {
    self->vtable = g_unk_7101a7cf10 + 0x10;
    UnkCallbackList_l8a1148* list = &self->list;
    list->vtable = &g_unk_7101a33fa0[2];
    if (UnkRefObject* ref = list->ref) {
        if (--ref->ref_count == 0)
            ref->vf_0x10();
    }
    list->vtable = &g_unk_7101a20998[2];
    list->callbacks.Free();
    self->field_0x240.~UnkNozzleValueHolder_l8a1148();
    self->field_0x1d0.~UnkNozzleValueHolder_l8a1148();
    self->field_0x160.~UnkNozzleValueHolder_l8a1148();
    self->field_0xf0.~UnkNozzleValueHolder_l8a1148();
    self->field_0x40.~UnkNozzleMember_l8a1148();
    fn_71010c4194(reinterpret_cast<UnkBaseComponent*>(self));
}
