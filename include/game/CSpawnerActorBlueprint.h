#pragma once

#include "base/UnkHashStream.h"
#include "base/global/CRntVector.h"
#include "base/reflection.h"
#include "base/reflection/UnkReflCast.h"
#include "base/reflection/UnkReflValue.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU341.h"
#include "types.h"

// The element of the component's voActorBlueprint (reflection: CRntVector<CSpawnerActorBlueprint>; the
// destructor fn_7101125c90 frees that vector with fn_71011266cc): a value, destroyed through the inline
// sequence below.
struct CSpawnerActorBlueprint {
    base::reflection::UnkReflValue value;
    CSpawnerActorBlueprint() {
        value.field_0x0 = nullptr;
        value.type = nullptr;
        value.kind = 2;
        value.field_0x14 = 1;
    }
    CSpawnerActorBlueprint(CSpawnerActorBlueprint&& other) { fn_71000a179c(&value, &other.value); }
    ~CSpawnerActorBlueprint() {
        if (value.type) {
            bool released = fn_7100deabe4(g_unk_7101d47310);
            void* payload = nullptr;
            if (value.type)
                payload = (value.field_0x14 == 1 || *reinterpret_cast<const u64*>(value.type + 0x18) > 8)
                              ? value.field_0x0
                              : &value;
            if (released) {
                fn_7100232124(static_cast<UnkBoundObj*>(payload));
            } else if (payload) {
                fn_710007c0e4(static_cast<UnkHashStream_0x38*>(payload));
                fn_7100083c00(payload);
            }
        }
        fn_71000a1c60(&value);
    }
    // Reflection's value operations (CType+0x28..+0x58), defined out of line.
    static void refl_Destroy(void* obj);
    static void* refl_MoveAssign(void* dst, const void* src);
};
static_assert(sizeof(CSpawnerActorBlueprint) == 0x18, "element size");
