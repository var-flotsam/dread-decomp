// Three setters that store `true` under a property of the "USER_DATA" section of the
// singleton's data (unless that data is locked at 0xe8), then tell the singleton (created on
// first use) through fn_7101203854. Three getters read the same properties (false when the
// section is missing).
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflAttributes.h"
#include "types.h"

using base::global::CStrId;

struct UnkUserDataOwner;
struct UnkUserData {
    u8 unk_0x0[0xe8];
    bool locked;
};
struct UnkSection;
struct UnkPropertyName;

struct UnkTypedValue {
    const base::reflection::CType* type;
    void* value;
};

extern UnkUserDataOwner* g_unk_7101d47308;
void* fn_71000839a0(u64 size, u32 flags);
void fn_710120320c(UnkUserDataOwner* self);  // constructor
UnkUserData* fn_710120384c(UnkUserDataOwner* self);
void fn_7101203854(UnkUserDataOwner* self);
UnkSection* fn_71010472e0(UnkUserData* data, const CStrId& name);
void fn_7100332c80(UnkSection* section, UnkPropertyName* name, const UnkTypedValue* value);

static inline UnkUserDataOwner* GetOwner() {
    if (!g_unk_7101d47308) {
        UnkUserDataOwner* owner = static_cast<UnkUserDataOwner*>(fn_71000839a0(0x2b8, 0x3f));
        fn_710120320c(owner);
        g_unk_7101d47308 = owner;
    }
    return g_unk_7101d47308;
}

template <UnkPropertyName* Name>
__attribute__((always_inline)) static inline void SetFlag() {
    UnkUserData* data = fn_710120384c(GetOwner());
    {
        UnkTypedValue v;
        CStrId section_name("USER_DATA", true);
        bool value = true;
        UnkSection* section = fn_71010472e0(data, section_name);
        if (section && !data->locked) {
            v.type = fn_710000d228();
            v.value = &value;
            fn_7100332c80(section, Name, &v);
        }
    }
    fn_7101203854(GetOwner());
}

extern UnkPropertyName g_unk_71727bd270;
extern UnkPropertyName g_unk_71727bd288;
extern UnkPropertyName g_unk_71727bd2a0;

void fn_7100dc8828() { SetFlag<&g_unk_71727bd270>(); }
void fn_7100dc8948() { SetFlag<&g_unk_71727bd288>(); }
void fn_7100dc8a68() { SetFlag<&g_unk_71727bd2a0>(); }

bool fn_710006d11c(UnkSection* section, UnkPropertyName* name, bool* out);  // reads a bool
UnkSection* fn_7101047270(UnkUserData* data, const CStrId& name);

template <UnkPropertyName* Name>
__attribute__((always_inline)) static inline bool GetFlag() {
    UnkUserData* data = fn_710120384c(GetOwner());
    bool value = false;
    {
        CStrId section_name("USER_DATA", true);
        if (UnkSection* section = fn_7101047270(data, section_name))
            fn_710006d11c(section, Name, &value);
    }
    return value;
}

bool fn_7100dc8b88() { return GetFlag<&g_unk_71727bd270>(); }
bool fn_7100dc8c5c() { return GetFlag<&g_unk_71727bd288>(); }
bool fn_7100dc8d30() { return GetFlag<&g_unk_71727bd2a0>(); }
