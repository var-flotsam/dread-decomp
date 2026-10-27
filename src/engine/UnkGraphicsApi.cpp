// The graphics system's entry points (tools/tu_map.py unit 167, 0x7100150b50-0x710015142c): one-line
// wrappers over the singleton g_unk_710b372a98 (engine/UnkGraphicsSystem.h) and factories of render
// objects that the widget constructors in src/game_s1/ store at their +0x108. The other functions of
// the unit are lifted (src/lifted/lifted_015.cpp). These are written by hand because their lifted
// copies name fewer arguments than the callees read (the AArch64 bytes are the same; an x86-64
// build would lose them): the creator passed the constructor no object, the scale setter no value,
// each factory its constructor no object. Names are placeholders.
#include "base/UnkVector.h"
#include "engine/UnkGraphicsSystem.h"
#include "game/UnkGlobal_710b372a98.h"
#include "types.h"

static inline UnkGraphicsSystem*& Graphics() {
    return reinterpret_cast<UnkGraphicsSystem*&>(g_unk_710b372a98);
}

// Creates the graphics system on first use, then sets it up.
void fn_7100150b70(UnkGfxSettings* settings, void* unused) {
    UnkGraphicsSystem*& slot = Graphics();
    UnkGraphicsSystem* system = slot;
    if (!system) {
        system = static_cast<UnkGraphicsSystem*>(fn_71000839a0(0x3183718, 0x3f));
        fn_71001c8294(system);
        slot = system;
    }
    fn_71001c9718(system, settings, unused);
}

// Sets scale `index` of the graphics system (fn_71001d2dcc).
bool fn_7100150f90(u32 index, f32 value) {
    return fn_71001d2dcc(Graphics(), index, value);
}

// What a factory returns: the new object, written through the indirect result (x8; the callers'
// view, the same in each widget constructor). The bytes past the pointer are the callers'.
struct UnkWidgetArg;
struct UnkWidgetMember {
    u8 data[0x38];
    UnkWidgetMember() {}
    UnkWidgetMember(const UnkWidgetMember&) {}
};

// The objects' constructors: the billboard (src/base/spatial/UnkBillboard.cpp), the cull view
// (src/base/spatial/UnkFrustumCull.cpp) and three without a source yet.
struct UnkBillboard;
struct UnkCullView;
void fn_71001b4aac(UnkBillboard* self);
void fn_71001b76ec(UnkCullView* self);
void fn_71001c071c(void* self);
void fn_71001bc2dc(void* self);
void fn_71001bdeac(void* self);

template <class T, u64 Size, void (*Construct)(T*)>
static inline __attribute__((always_inline)) UnkWidgetMember Create() {
    T* object = static_cast<T*>(fn_71000839a0(Size, 8));
    Construct(object);
    UnkWidgetMember result;
    *reinterpret_cast<T**>(result.data) = object;
    return result;
}

UnkWidgetMember fn_7100151110(UnkWidgetArg*) {
    return Create<UnkBillboard, 0x378, fn_71001b4aac>();
}
UnkWidgetMember fn_7100151150(UnkWidgetArg*) {
    return Create<void, 0x368, fn_71001c071c>();
}
UnkWidgetMember fn_7100151190(UnkWidgetArg*) {
    return Create<UnkCullView, 0x4e8, fn_71001b76ec>();
}
UnkWidgetMember fn_7100151220(UnkWidgetArg*) {
    return Create<void, 0xe8, fn_71001bc2dc>();
}
UnkWidgetMember fn_7100151360(UnkWidgetArg*) {
    return Create<void, 0x1c0, fn_71001bdeac>();
}
