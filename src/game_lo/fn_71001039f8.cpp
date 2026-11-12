// Returns a lazily constructed singleton (a large object holding mutexes; its constructor is
// fn_7101213858). The getter is a template in the original: the guard and the object are weak
// and reached through the GOT.
#include "types.h"

// Layout unknown beyond its size (the constructor writes up to +0x960 and beyond).
struct UnkSingleton_71001039f8 {
    UnkSingleton_71001039f8() asm("fn_7101213858");  // fn_7101213858
    ~UnkSingleton_71001039f8();
    u8 field_0x0[0x970];
};

template <class G>
UnkSingleton_71001039f8* refl_GetSingleton() {
    static UnkSingleton_71001039f8 s_instance;
    return &s_instance;
}

struct UnkTag_fn_71001039f8;
template UnkSingleton_71001039f8* refl_GetSingleton<UnkTag_fn_71001039f8>();
