// CAbilityEnergyWave's unit (tools/tu_map.py): the reflected wrapper of fn_71003010e0 (select an ability by
// name; src/game/CAbilityEnergyWave/CAbilityEnergyWave_Select_ovb0022.cpp), answering whether it was
// selected. Names and layouts are placeholders.
#include <new>
#include "base/UnkNamedRegistry.h"
#include "base/UnkVector.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflAttributes.h"
#include "base/reflection/UnkReflCast.h"
#include "base/reflection/UnkReflValue.h"
#include "game/UnkComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkSwitchObject.h"
#include "game/UnkTimelineComponent.h"
#include "types.h"

using base::global::CStrId;
using base::reflection::CClass;
using base::reflection::UnkReflValue;

struct UnkSelf;
struct UnkOwner;
struct UnkObject;
struct UnkComponent;
struct UnkPlayer;
struct UnkWorld;
struct UnkEntry;
struct UnkLifeComponent;
struct UnkSamusGunComponent;
struct UnkScriptCaller;
struct UnkSmLockon_A1_b64;
struct UnkSmAbility3_A2_b63;
struct UnkAbilityC_A2_b69;
struct UnkStruct_71002fda50;
struct UnkWaveChild_A2_b72;
struct UnkStateObject;
struct UnkEnergyWaveCompA_b49;
struct UnkEnergyWaveCompB_b49;
struct UnkEnergyWaveObject_b49;
struct UnkEnergyWaveFxComp_b49;

UnkOwner* fn_7100defcd4(UnkGameManager*);
UnkPlayer* fn_7100dec788(UnkGameManager*);
void fn_710113f270(UnkEnergyWaveCompA_b49* comp, void* object);
void fn_710094f8f0(UnkEnergyWaveCompB_b49* comp, UnkEnergyWaveObject_b49* object);
void fn_71002fda50(UnkStruct_71002fda50* self, bool all, bool force_off);
void fn_71002f8848(UnkWaveChild_A2_b72* child);
void fn_710115372c(UnkWorld* world);
void fn_71011738f8(UnkEnergyWaveFxComp_b49* comp, void* id, u32 flags);
extern char g_unk_7101648d28[];
extern const u64 g_unk_7101648d18;

// The id table (fn_7100031894 builds it; as src/game/CEmmyAIComponent/CEmmyAIComponent_IsStateId_b56.cpp).
struct UnkGameManagerIdTable {
    u8 pad_0x0[0xf18];
    CStrId field_0xf18;
    u8 pad_0xf20[0x2578 - 0xf20];
    UnkGameManagerIdTable() asm("fn_7100031894");
    ~UnkGameManagerIdTable() asm("fn_71000407f0");
};
inline UnkGameManagerIdTable& GameManagerIdTable() {
    static UnkGameManagerIdTable s_table;
    return s_table;
}

struct UnkAbilitySet_ovb22;
bool fn_71003010e0(UnkAbilitySet_ovb22* self, const CStrId* id, bool notify, bool);

// A pointer argument's value: null without a type.
template <class T>
static inline const T* ArgPtr(UnkReflValue* arg) {
    return arg->type ? base::reflection::refl_ArgValue<T>(arg) : nullptr;
}

static inline UnkReflValue BoolValue(bool value) {
    const base::reflection::CType* type = fn_710000d228();
    UnkReflValue r;
    r.kind = 2;
    r.field_0x0 = nullptr;
    r.type = nullptr;
    fn_71000a18d4(&r, type, &value, false);
    return r;
}

UnkReflValue fn_7100302c14(void*, char* obj, UnkReflValue* const* args) {
    UnkReflValue* a = args[0];
    UnkAbilitySet_ovb22* self = *reinterpret_cast<UnkAbilitySet_ovb22**>(obj + 8);
    return BoolValue(fn_71003010e0(self, ArgPtr<CStrId>(&a[0]), *base::reflection::refl_ArgValue<u8>(&a[1]) != 0,
                                   *base::reflection::refl_ArgValue<bool>(&a[2])));
}

