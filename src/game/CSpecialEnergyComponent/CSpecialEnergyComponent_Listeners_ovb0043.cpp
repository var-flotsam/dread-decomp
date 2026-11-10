// CSpecialEnergyComponent's unit (tools/tu_map.py): fn_7100a20438 tells the energy listeners (a deferred
// callback set) about a change, through base::refl_DeferredInvoke. Names and layouts are placeholders.
#include "base/UnkCloneHelpers.h"
#include "base/UnkDeferredSet.h"
#include "base/UnkVector.h"
#include "types.h"

using base::UnkDeferredCallback;
using base::UnkDeferredSet;
using base::UnkVector;

// What the energy listeners receive: the energy, its maximum and a flag.
struct UnkSpecialEnergyEvent_ovb0043 {
    f32 energy;
    f32 max;
    bool field_0x8;
};

struct UnkDeferredInvoke_7100a20438;

UnkDeferredCallback<UnkSpecialEnergyEvent_ovb0043>** fn_7100a20654(
    UnkVector<UnkDeferredCallback<UnkSpecialEnergyEvent_ovb0043>*>* v, u32 i);

template <>
struct base::UnkDeferredInvokeTraits<UnkDeferredInvoke_7100a20438> {
    static UnkDeferredCallback<UnkSpecialEnergyEvent_ovb0043>** at(
        UnkVector<UnkDeferredCallback<UnkSpecialEnergyEvent_ovb0043>*>* v, u32 i) {
        return fn_7100a20654(v, i);
    }
};

// Sends the event to the energy listeners (base::refl_DeferredInvoke).
__attribute__((flatten)) void fn_7100a20438(UnkDeferredSet<UnkDeferredCallback<UnkSpecialEnergyEvent_ovb0043>>* s,
                                            const UnkSpecialEnergyEvent_ovb0043* event) {
    base::refl_DeferredInvoke<UnkDeferredInvoke_7100a20438, UnkSpecialEnergyEvent_ovb0043>(s, event);
}
