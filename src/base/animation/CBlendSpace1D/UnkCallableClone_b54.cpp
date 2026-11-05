// The clone of a callable a std::function wraps in base::animation::CBlendSpace1D's unit (a vtable and
// one captured word, allocated with operator new); a copy of fn_7100a2e0cc
// (src/game/CTimelineComponent/UnkCallableClone.cpp). Names and layouts are placeholders.
#include <new>

#include "types.h"

extern void* g_unk_7101a191e8[];

struct UnkBlendSpace1DCapture {
    void* owner;
};

struct UnkBlendSpace1DCallable {
    char* vtable;
    UnkBlendSpace1DCapture capture;
    explicit UnkBlendSpace1DCallable(const UnkBlendSpace1DCapture& c) : vtable(reinterpret_cast<char*>(g_unk_7101a191e8) + 0x10), capture(c) {}
};

UnkBlendSpace1DCallable* fn_7100008980(const UnkBlendSpace1DCallable* self) {
    return new UnkBlendSpace1DCallable(self->capture);
}
