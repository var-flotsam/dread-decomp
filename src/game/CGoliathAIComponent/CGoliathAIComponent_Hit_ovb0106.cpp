// CGoliathAIComponent's unit (tools/tu_map.py): the hit check against the unit's static id, the super
// Goliath's environment setup, and tunable values read and written by name through reflection (the
// field whose name hash matches, among the tunable class's fields). Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflAttributes.h"
#include "base/reflection/UnkReflTypedPtr.h"
#include "game/CCharClassTweakPlayRateTrack.h"
#include "game/UnkTU85.h"
#include "types.h"

using base::global::CStrId;
using base::global::UnkStringData;
using base::reflection::CType;
using base::reflection::UnkReflTypedPtr;

struct UnkCollisionContainer;
CStrId* fn_7100864018(UnkCollisionContainer* self, void* object);

extern CStrId g_unk_71726e6748 __attribute__((visibility("hidden")));

struct UnkGoliathOwner2_ovb0106 {
    u8 pad_0x0[0x2a0];
    UnkCollisionContainer* field_0x2a0;
};

struct UnkGoliathHit2_ovb0106 {
    u8 pad_0x0[0x60];
    void* field_0x60;
    u8 pad_0x68[0x4c];
    u32 field_0xb4;
};

struct UnkGoliathAI2_ovb0106 {
    virtual void vf_0x0();
    u8 pad_0x8[0x28];
    UnkGoliathOwner2_ovb0106* owner;  // 0x30

    bool IsTarget(void* object) {
        using Fn = bool (*)(UnkGoliathAI2_ovb0106*, void*);
        return (*reinterpret_cast<Fn**>(this))[0x868 / 8](this, object);
    }
};

u32 fn_71003e60a0(UnkGoliathAI2_ovb0106* self, UnkGoliathHit2_ovb0106* hit);

u32 fn_7100590fc4(UnkGoliathAI2_ovb0106* self, UnkGoliathHit2_ovb0106* hit) {
    u32 result = fn_71003e60a0(self, hit);
    if (result)
        return result;
    if (self->IsTarget(hit->field_0x60) && !(hit->field_0xb4 & 0x2018020))
        return 2;
    UnkCollisionContainer* collision = self->owner->field_0x2a0;
    CStrId id(collision ? *fn_7100864018(collision, hit->field_0x60) : g_unk_7101d470e0);
    if (id.data == g_unk_71726e6748.data)
        return (hit->field_0xb4 & 0x8000000) ? 6 : 0;
    return 0;
}

struct UnkManager;
extern UnkManager* g_unk_7101d47360;
void fn_7100ee1f34(void*);
void fn_7100eea3fc(void*, const base::global::CStrId*, bool, bool);

struct UnkGoliathActor_ovb0106 {
    u8 pad_0x0[0x30];
    UnkStringData* name;  // 0x30
};

struct UnkGoliathAI3_ovb0106 {
    u8 pad_0x0[0x30];
    UnkGoliathActor_ovb0106* actor;  // 0x30
};

// The sub-area manager, created on first use.
static inline void* SubAreaManager() {
    void* manager = reinterpret_cast<void*&>(g_unk_7101d47360);
    if (!manager) {
        manager = fn_71000839a0(0xae8, 0x3f);
        fn_7100ee1f34(manager);
        reinterpret_cast<void*&>(g_unk_7101d47360) = manager;
    }
    return manager;
}

// When the actor is the super Goliath, applies the environment setup.
void fn_7100591968(UnkGoliathAI3_ovb0106* self) {
    static CStrId s_name("SP_Goliath_B_goliath", true);
    if (self->actor->name != s_name.data)
        return;
    static CStrId s_setup("SuperGoliath_CoreX_Env", true);
    fn_7100eea3fc(SubAreaManager(), &s_setup, true, false);
}

struct UnkReflField;
struct UnkReflFieldObject;
struct UnkReflValueArg;

extern base::reflection::UnkTunable g_unk_7172716160;
struct UnkGlobal2;

// A reflected field (0xb0 bytes).
struct UnkGoliathField_ovb0106 {
    u8 pad_0x0[0xb0];
};

// A class as the lookup sees it: its fields and their name hashes.
struct UnkGoliathClass_ovb0106 {
    u8 pad_0x0[0xe8];
    UnkGoliathField_ovb0106* fields;  // 0xe8
    u64* keys;                        // 0xf0
    u32 count;                        // 0xf8

    UnkGoliathField_ovb0106* End() { return fields + count; }
    UnkGoliathField_ovb0106* Find(u64 key) {
        for (u64 i = 0; i < count; ++i)
            if (keys[i] == key)
                return &fields[i];
        return End();
    }
    // The field with that name hash, null when there is none.
    UnkGoliathField_ovb0106* FindField(u64 key) {
        UnkGoliathField_ovb0106* it = Find(key);
        return it != End() ? it : nullptr;
    }
};

// A reflected object: its class is vtable slot 0.
struct UnkGoliathObject_ovb0106 {
    virtual UnkGoliathClass_ovb0106* GetClass();
};

// A string's data: its key (CRC-64) at +0x30.
struct UnkGoliathStrData_ovb0106 {
    u8 pad_0x0[0x30];
    u64 key;
};

struct UnkGoliathValue_ovb0106 {
    const CType* type;
    void* value;
};

static inline bool Get(UnkGoliathObject_ovb0106* obj, u64 key, f32* out) {
    UnkGoliathClass_ovb0106* cls = obj->GetClass();
    UnkReflTypedPtr typed;
    typed.type = reinterpret_cast<char*>(obj->GetClass());
    typed.ptr = obj;
    UnkGoliathValue_ovb0106 value = {fn_710000abbc(), out};
    if (UnkGoliathField_ovb0106* field = cls->FindField(key)) {
        fn_71000e78e0(reinterpret_cast<UnkReflField*>(field), reinterpret_cast<const UnkReflFieldObject*>(&typed),
                      reinterpret_cast<UnkReflValueArg*>(&value));
        return true;
    }
    return false;
}

static inline void Set(UnkGoliathObject_ovb0106* obj, u64 key, f32* in) {
    UnkGoliathClass_ovb0106* cls = obj->GetClass();
    UnkReflTypedPtr typed;
    typed.type = reinterpret_cast<char*>(obj->GetClass());
    typed.ptr = obj;
    UnkGoliathValue_ovb0106 value = {fn_710000abbc(), in};
    if (UnkGoliathField_ovb0106* field = cls->FindField(key))
        fn_71000e7a80(reinterpret_cast<UnkReflField*>(field), reinterpret_cast<const UnkReflFieldObject*>(&typed),
                      reinterpret_cast<UnkReflValueArg*>(&value));
}

struct UnkGoliathNamed_ovb0106 {
    UnkGoliathStrData_ovb0106* name;
};

struct UnkGoliathAI4_ovb0106 {
    virtual void vf_0x0();

    UnkGoliathNamed_ovb0106* Named() {
        using Fn = UnkGoliathNamed_ovb0106* (*)(UnkGoliathAI4_ovb0106*);
        return (*reinterpret_cast<Fn**>(this))[0x858 / 8](this);
    }
};

f32 fn_7100591b54(UnkGoliathAI4_ovb0106* self) {
    f32 v = 0.0f;
    u64 key = self->Named()->name->key;
    UnkGoliathObject_ovb0106* tunable = reinterpret_cast<UnkGoliathObject_ovb0106*>(&g_unk_7172716160);
    if (!Get(tunable, key, &v))
        return 0.0f;
    return v;
}

extern UnkGoliathNamed_ovb0106 g_unk_71726e6750 __attribute__((visibility("hidden")));

struct UnkGoliathSource_ovb0106 {
    u8 pad_0x0[0x620];
    f32 field_0x620;
};

struct UnkGoliathAI5_ovb0106 {
    u8 pad_0x0[0x14a30];
    UnkGoliathSource_ovb0106* field_0x14a30;
};

void fn_71003ad278(UnkGoliathAI5_ovb0106* self);

void fn_7100590e68(UnkGoliathAI5_ovb0106* self) {
    fn_71003ad278(self);
    if (UnkGoliathSource_ovb0106* source = self->field_0x14a30) {
        u64 key = g_unk_71726e6750.name->key;
        f32 v = source->field_0x620;
        Set(reinterpret_cast<UnkGoliathObject_ovb0106*>(&g_unk_7172790a08), key, &v);
    }
}
