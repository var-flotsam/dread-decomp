// CCharClassAttackComponent (unit 1185): a float tunable read by the name of the component's id through
// reflection, rounded down; zero when the tunable class has no such field. Names and layouts are placeholders.
#include "base/reflection/ReflectedAttributes.h"
#include "base/reflection/UnkReflTypedPtr.h"
#include "base/global/CStrId.h"
#include "game/CCharClassAttackComponent.h"
#include "game/UnkTU85.h"
#include "types.h"

using base::reflection::CType;
using base::reflection::UnkReflTypedPtr;

struct UnkReflField;
struct UnkReflFieldObject;
struct UnkReflValueArg;

// A reflected field (0xb0 bytes).
struct UnkAttackField_l789f5c {
    u8 pad_0x0[0xb0];
};

// A class as the lookup sees it: its fields and their name hashes.
struct UnkAttackClass_l789f5c {
    u8 pad_0x0[0xe8];
    UnkAttackField_l789f5c* fields;  // 0xe8
    u64* keys;                       // 0xf0
    u32 count;                       // 0xf8

    UnkAttackField_l789f5c* End() { return fields + count; }
    UnkAttackField_l789f5c* Find(u64 key) {
        for (u64 i = 0; i < count; ++i)
            if (keys[i] == key)
                return &fields[i];
        return End();
    }
    UnkAttackField_l789f5c* FindField(u64 key) {
        UnkAttackField_l789f5c* it = Find(key);
        return it != End() ? it : nullptr;
    }
};

struct UnkAttackObject_l789f5c {
    virtual UnkAttackClass_l789f5c* GetClass();
};

struct UnkAttackStrData_l789f5c {
    u8 pad_0x0[0x30];
    u64 key;
};

struct UnkAttackValue_l789f5c {
    const CType* type;
    void* value;
};

static inline bool Get(UnkAttackObject_l789f5c* obj, u64 key, f32* out) {
    UnkAttackClass_l789f5c* cls = obj->GetClass();
    UnkReflTypedPtr typed;
    typed.type = reinterpret_cast<char*>(obj->GetClass());
    typed.ptr = obj;
    UnkAttackValue_l789f5c value = {fn_710000abbc(), out};
    if (UnkAttackField_l789f5c* field = cls->FindField(key)) {
        fn_71000e78e0(reinterpret_cast<UnkReflField*>(field), reinterpret_cast<const UnkReflFieldObject*>(&typed),
                      reinterpret_cast<UnkReflValueArg*>(&value));
        return true;
    }
    return false;
}

f32 fn_7100789f5c(const base::global::CStrId* id) {
    f32 v = 0.0f;
    u64 key = reinterpret_cast<UnkAttackStrData_l789f5c*>(id->data)->key;
    UnkAttackObject_l789f5c* tunable = reinterpret_cast<UnkAttackObject_l789f5c*>(&g_unk_7172716160);
    return __builtin_floorf(Get(tunable, key, &v) ? v : 0.0f);
}
