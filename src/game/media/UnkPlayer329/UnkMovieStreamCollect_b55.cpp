// Unit 329 (tools/tu_map.py; next to media::CMovieStreamNx): two twins that walk another object's
// list of streams (+0x60, count at +0x68) and pass each media::CMovieStreamNx among them (its
// part at +0x60) to fn_7100222cc8 with the list at +0x68 of their own object (flag set in the first, clear in
// the second).
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "types.h"

void fn_7100222cc8(void* list, void* item, bool flag);

struct UnkMediaStream {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual const base::reflection::CType* vf_0x18();
};

struct UnkMediaStreamList {
    u8 pad_0x0[0x60];
    UnkMediaStream** items;
    u32 count;  // +0x68
};

struct UnkMediaCollector {
    u8 pad_0x0[0x68];
    u8 field_0x68[1];
};

template <bool kFlag>
static __attribute__((always_inline)) void CollectMovieStreams(UnkMediaCollector* self, UnkMediaStreamList* src) {
    for (UnkMediaStream** it = src->items, **end = it + src->count; it != end; ++it) {
        UnkMediaStream* stream = *it;
        if (!stream)
            continue;
        if (fn_710009d50c(stream->vf_0x18(), fn_7100225620()))
            fn_7100222cc8(self->field_0x68, reinterpret_cast<char*>(stream) + 0x60, kFlag);
    }
}

void fn_7100224728(UnkMediaCollector* self, UnkMediaStreamList* src) {
    CollectMovieStreams<true>(self, src);
}

void fn_7100224868(UnkMediaCollector* self, UnkMediaStreamList* src) {
    CollectMovieStreams<false>(self, src);
}
