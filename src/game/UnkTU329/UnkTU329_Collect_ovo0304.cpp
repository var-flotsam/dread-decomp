// Unit 329: a collector of media streams; for a stream list of kind 7, adds every stream whose class
// (vf_0x18) derives from the stream class (fn_7100225620) to the collector's list at +0x68.
// Names are placeholders.
#include "base/UnkDefaultAllocator.h"
#include "base/global/CRntVector.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkMediaCollector.h"
#include "game/UnkMediaStream.h"
#include "game/UnkMediaStreamList.h"
#include "game/UnkTU327.h"
#include "types.h"

// Declared as in src/game/media/UnkMovieUnit327/UnkMovieUnit327_b50.cpp; here it gets the collector's
// list in x0 and the stream in x1, so it is called through a cast.
struct UnkMovieEmpty_b50;
struct UnkMovieEventOwner_b50;

// Declared with this item type in src/base/UnkUnit322/UnkUnit322_Dispatch.cpp.
struct UnkItem322;

void fn_71002247c4(char* collector, UnkItem322* item) {
    UnkMediaCollector* self = reinterpret_cast<UnkMediaCollector*>(collector);
    UnkMediaStreamList* src = reinterpret_cast<UnkMediaStreamList*>(item);
    if (src->kind != 7)
        return;
    for (UnkMediaStream** it = src->items, **end = it + src->count; it != end; ++it) {
        UnkMediaStream* stream = *it;
        if (!stream)
            continue;
        if (fn_710009d50c(stream->vf_0x18(), fn_7100225620()))
            reinterpret_cast<s32 (*)(void*, UnkMediaStream*)>(fn_7100222f14)(self->field_0x68, stream);
    }
}
