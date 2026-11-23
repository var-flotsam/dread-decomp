// behaviortree::CTunableBoolCondition's unit (tools/tu_map.py unit 537): the condition's value, a constant
// that a tunable (a group and a field name) overrides when both are set; the group is found by name in the
// global tunable registry and the field read through reflection. Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "base/reflection/ReflectedAttributes.h"
#include "base/reflection/UnkReflTypedPtr.h"
#include "base/reflection/UnkTunableRegistry.h"
#include "game/UnkTU57.h"
#include "game/UnkTU85.h"
#include "types.h"

using base::global::CStrId;
using base::reflection::CType;
using base::reflection::UnkReflTypedPtr;
using base::reflection::UnkTunableRegistry;

struct UnkReflField;
struct UnkReflFieldObject;
struct UnkReflValueArg;

// A reflected field (0xb0 bytes).
struct UnkTunableField_l3a45a8 {
    u8 pad_0x0[0xb0];
};

// A class as the lookup sees it: its fields and their name hashes.
struct UnkTunableClass_l3a45a8 {
    u8 pad_0x0[0xe8];
    UnkTunableField_l3a45a8* fields;  // 0xe8
    u64* keys;                        // 0xf0
    u32 count;                        // 0xf8

    UnkTunableField_l3a45a8* End() { return fields + count; }
    UnkTunableField_l3a45a8* Find(u64 key) {
        for (u64 i = 0; i < count; ++i)
            if (keys[i] == key)
                return &fields[i];
        return End();
    }
    // The field with that name hash, null when there is none.
    UnkTunableField_l3a45a8* FindField(u64 key) {
        UnkTunableField_l3a45a8* it = Find(key);
        return it != End() ? it : nullptr;
    }
};

// A reflected object: its class is vtable slot 0.
struct UnkTunableObject_l3a45a8 {
    virtual UnkTunableClass_l3a45a8* GetClass();
};

struct UnkTunableValue_l3a45a8 {
    const CType* type;
    void* value;
};

// The registry as the lookup sees it: the tunables and their group names, side by side.
struct UnkTunableKey_l3a45a8 {
    u8 pad_0x0[0x30];
};

struct UnkTunableTable_l3a45a8 {
    UnkTunableObject_l3a45a8** values;  // 0x0
    UnkTunableKey_l3a45a8* keys;        // 0x8
    u32 count;                          // 0x10

    // The tunable registered under the group's name (the slot past the last one when there is none).
    UnkTunableObject_l3a45a8* Find(const CStrId& group) {
        const UnkCompared_0x30* name = reinterpret_cast<const UnkCompared_0x30*>(&group.data->String());
        for (u64 i = 0; i < count; ++i)
            if (fn_710007f120(reinterpret_cast<const UnkCompared_0x30*>(&keys[i]), name))
                return values[i];
        return values[count];
    }
};

struct UnkTunableBoolCondition_l3a45a8 {
    u8 pad_0x0[0x38];
    bool value;                      // 0x38
    u8 pad_0x39[0x7];
    CStrId group;                    // 0x40
    CStrId name;                     // 0x48
};

static inline UnkTunableRegistry* Registry() {
    UnkTunableRegistry* registry = g_unk_7101d47350;
    if (!registry) {
        registry = static_cast<UnkTunableRegistry*>(fn_71000839a0(0x58, 0x3f));
        fn_710013ac14(registry);
        g_unk_7101d47350 = registry;
    }
    return registry;
}

static inline bool Get(UnkTunableObject_l3a45a8* obj, u64 key, bool* out) {
    UnkTunableClass_l3a45a8* cls = obj->GetClass();
    UnkReflTypedPtr typed;
    typed.type = reinterpret_cast<char*>(obj->GetClass());
    typed.ptr = obj;
    UnkTunableValue_l3a45a8 value = {fn_710000d228(), out};
    if (UnkTunableField_l3a45a8* field = cls->FindField(key)) {
        fn_71000e78e0(reinterpret_cast<UnkReflField*>(field), reinterpret_cast<const UnkReflFieldObject*>(&typed),
                      reinterpret_cast<UnkReflValueArg*>(&value));
        return true;
    }
    return false;
}

bool fn_71003a45a8(UnkTunableBoolCondition_l3a45a8* self) {
    bool value = self->value;
    if (self->group.data != &g_unk_7101d47108->empty && self->name.data != &g_unk_7101d47108->empty) {
        UnkTunableTable_l3a45a8* table = reinterpret_cast<UnkTunableTable_l3a45a8*>(Registry());
        ++self->group.data->reads;
        if (UnkTunableObject_l3a45a8* tunable = table->Find(self->group))
            Get(tunable, self->name.data->Key(), &value);
    }
    return value;
}
