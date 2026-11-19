// CSwarmControllerComponent unit (tools/tu_map.py unit 1163): the controller's destructor. It destroys
// the members in reverse (dictionaries, arrays of arrays and named arrays, the refcounted object at
// +0xe8, the callback set at +0x40), then runs the component base's destructor (fn_71010c4194).
// Names and layouts are placeholders.
#include "base/global/CRntSmallDictionary.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "game/CSwarmControllerComponent.h"
#include "game/UnkCallbackSet_7101a20950.h"
#include "game/UnkComponent.h"
#include "game/UnkTU341.h"
#include "game/UnkTU883.h"
#include "types.h"

using base::global::CRntSmallDictionary;
using base::global::CRntVector;
using base::global::CStrId;

struct UnkSwarmCtrl_ovb0063;

// An array that owns its buffer: the elements are destroyed and the buffer freed with it.
template <class T>
struct UnkSwarmArray_l7479e4 {
    CRntVector<T> v;

    ~UnkSwarmArray_l7479e4() {
        if (v.data) {
            for (u64 i = 0; i < v.count; ++i)
                v.data[i].~T();
            v.count = 0;
            v.allocator->vf_0x48(v.data);
            v.capacity = 0;
        }
    }
};

// A dictionary that owns its two arrays.
template <class K, class V>
struct UnkSwarmDictionary_l7479e4 {
    CRntSmallDictionary<K, V> d;

    ~UnkSwarmDictionary_l7479e4() {
        d.Clear();
        if (d.keys)
            d.allocator->vf_0x48(d.keys);
        if (d.values)
            d.allocator->vf_0x48(d.values);
    }
};

struct UnkSwarmWord_l7479e4 {
    u32 value;
};

// An array and its name (0x38 bytes).
struct UnkSwarmNamedList_l7479e4 {
    UnkSwarmArray_l7479e4<void*> list;
    u64 field_0x28;
    CStrId name;
};
static_assert(sizeof(UnkSwarmNamedList_l7479e4) == 0x38, "layout");

// The named arrays' own release (the controller's only user).
static void FreeNamedLists(CRntVector<UnkSwarmNamedList_l7479e4>& v) {
    if (v.data) {
        for (u64 i = 0; i < v.count; ++i)
            v.data[i].~UnkSwarmNamedList_l7479e4();
        v.count = 0;
        v.allocator->vf_0x48(v.data);
        v.capacity = 0;
    }
}

// The base of the dictionary's values: only its vtable is reset when it goes.
struct UnkSwarmEntryBase_l7479e4 {
    char* vtable;
    u8 pad_0x8[0x10];

    ~UnkSwarmEntryBase_l7479e4() { vtable = g_unk_7101a20a08 + 0x10; }
};

struct UnkSwarmEntry_l7479e4 : UnkSwarmEntryBase_l7479e4 {
    UnkSwarmArray_l7479e4<void*> list;  // 0x18
};
static_assert(sizeof(UnkSwarmEntry_l7479e4) == 0x40, "layout");

struct UnkSwarmController_l7479e4 {
    void** vtable;
    u8 pad_0x8[0x38];
    UnkCallbackSet_7101a20950 callbacks;  // 0x40
    u64 field_0x80;
    u8 pad_0x88[0x8];
    UnkSwarmArray_l7479e4<void*> field_0x90;
    UnkSwarmArray_l7479e4<void*> field_0xb8;
    f32 field_0xe0;
    f32 field_0xe4;
    UnkRefCounted* field_0xe8;
    UnkSwarmDictionary_l7479e4<void*, void*> field_0xf0;
    UnkSwarmArray_l7479e4<UnkSwarmArray_l7479e4<void*>> field_0x118;
    UnkSwarmArray_l7479e4<UnkSwarmArray_l7479e4<void*>> field_0x140;
    UnkSwarmArray_l7479e4<void*> field_0x168;
    u8 pad_0x190[0x18];
    CRntVector<UnkSwarmNamedList_l7479e4> field_0x1a8;
    u8 pad_0x1d0[0x38];
    UnkSwarmArray_l7479e4<UnkSwarmWord_l7479e4> field_0x208;
    UnkSwarmArray_l7479e4<void*> sounds;  // 0x230
    u8 pad_0x258[0x8];
    void* field_0x260;
    void* handler_owner;  // 0x268
    void* handler;
    UnkSwarmDictionary_l7479e4<void*, UnkSwarmEntry_l7479e4> field_0x278;
};
static_assert(__builtin_offsetof(UnkSwarmController_l7479e4, field_0x90) == 0x90, "layout");
static_assert(__builtin_offsetof(UnkSwarmController_l7479e4, field_0x1a8) == 0x1a8, "layout");
static_assert(__builtin_offsetof(UnkSwarmController_l7479e4, field_0x208) == 0x208, "layout");
static_assert(__builtin_offsetof(UnkSwarmController_l7479e4, field_0x278) == 0x278, "layout");

void fn_71007479e4(UnkSwarmCtrl_ovb0063* object) {
    auto* self = reinterpret_cast<UnkSwarmController_l7479e4*>(object);
    self->vtable = g_unk_7101a6dc58 + 2;
    self->field_0x278.~UnkSwarmDictionary_l7479e4();
    if (self->field_0x260) {
        fn_71005c4828(self->field_0x260, &self->field_0x260);
        self->field_0x260 = nullptr;
    }
    self->handler = nullptr;
    self->handler_owner = nullptr;
    self->sounds.~UnkSwarmArray_l7479e4();
    self->field_0x208.~UnkSwarmArray_l7479e4();
    FreeNamedLists(self->field_0x1a8);
    self->field_0x168.~UnkSwarmArray_l7479e4();
    self->field_0x140.~UnkSwarmArray_l7479e4();
    self->field_0x118.~UnkSwarmArray_l7479e4();
    self->field_0xf0.~UnkSwarmDictionary_l7479e4();
    if (UnkRefCounted* p = self->field_0xe8) {
        if (--p->ref_count == 0)
            p->Delete();
    }
    self->field_0xb8.~UnkSwarmArray_l7479e4();
    self->field_0x90.~UnkSwarmArray_l7479e4();
    self->callbacks.~UnkCallbackSet_7101a20950();
    fn_71010c4194(reinterpret_cast<UnkBaseComponent*>(self));
}
