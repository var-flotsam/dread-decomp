// The clone of a callable a std::function wraps in the CTimelineComponent unit (a vtable and one
// captured word, allocated with operator new); a copy of fn_7100014174
// (src/base/animation/CSkeletalAnimation/UnkCallableClone.cpp). Names and layouts are placeholders.
#include <new>

#include "types.h"

extern char g_unk_7101a8deb8[];

struct UnkTimelineCompCapture {
    void* owner;
};

struct UnkTimelineCompCallable {
    char* vtable;
    UnkTimelineCompCapture capture;
    explicit UnkTimelineCompCallable(const UnkTimelineCompCapture& c) : vtable(g_unk_7101a8deb8 + 0x10), capture(c) {}
};

UnkTimelineCompCallable* fn_7100a2e0cc(const UnkTimelineCompCallable* self) {
    return new UnkTimelineCompCallable(self->capture);
}
