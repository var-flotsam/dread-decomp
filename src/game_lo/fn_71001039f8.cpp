// Returns the lazily constructed sound system (include/game/UnkAtkSystem.h; its constructor is
// fn_7101213858). The getter is a template in the original: the guard and the object are weak
// and reached through the GOT.
#include "game/UnkAtkSystem.h"
#include "types.h"

template <class G>
UnkAtkSystem_b0150* refl_GetSingleton() {
    static UnkAtkSystem_b0150 s_instance;
    return &s_instance;
}

struct UnkTag_fn_71001039f8;
template UnkAtkSystem_b0150* refl_GetSingleton<UnkTag_fn_71001039f8>();
