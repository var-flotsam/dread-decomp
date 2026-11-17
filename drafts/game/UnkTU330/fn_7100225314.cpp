// Unit 330 of tools/tu_map.py: under the player's mutex, takes each finished track id from the movie
// extractor and hands it to the matching movie stream; when the extractor reports the end (0x2000), every
// movie stream gets one last call. True on success or end. Names and layouts are placeholders.
#include <algorithm>

#include "../../game_s7/UnkIter.h"
#include "base/UnkDefaultAllocator.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkTU324.h"
#include "types.h"

struct UnkMovieStreamNx_ovb0003;
bool fn_7100226068(UnkMovieStreamNx_ovb0003* self);

struct UnkTrackId_l225314 {
    s32 id;
};

class UnkStream_l225314 {
public:
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual const base::reflection::CType* vf_0x18();

    u8 pad_0x8[0x30];
    UnkTrackId_l225314* track;  // 0x38
};

class UnkExtractor_l225314 {
public:
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual void vf_0x40();
    virtual s32 vf_0x48(s64* track);  // the next finished track
};

struct UnkMoviePlayer_l225314 {
    u8 pad_0x0[0x60];
    UnkStream_l225314** streams;  // 0x60
    u32 stream_count;             // 0x68
    u8 pad_0x6c[0x8f4];
    nn::os::MutexType mutex;  // 0x960
    u8 pad_0x980[0x158];
    UnkExtractor_l225314* extractor;  // 0xad8

    UnkStream_l225314** begin() { return streams; }
    UnkStream_l225314** end() { return &streams[stream_count]; }

    // The stream playing `track`, or end().
    UnkStream_l225314** Find(s64 track) {
        return std::find_if(UnkIter<UnkStream_l225314*>{begin()}, UnkIter<UnkStream_l225314*>{end()},
                            [track](UnkStream_l225314* stream) { return track == stream->track->id; })
            .p;
    }
};
static_assert(__builtin_offsetof(UnkMoviePlayer_l225314, mutex) == 0x960, "layout");
static_assert(__builtin_offsetof(UnkMoviePlayer_l225314, extractor) == 0xad8, "layout");

static inline bool IsMovieStream(UnkStream_l225314* stream) {
    return fn_710009d50c(stream->vf_0x18(), fn_7100225620());
}

static inline bool Feed(UnkStream_l225314* stream) {
    return fn_7100226068(reinterpret_cast<UnkMovieStreamNx_ovb0003*>(stream));
}

// Hands each finished track to its stream until one is missing or refuses it; at the end of the movie
// (0x2000) every movie stream gets one last call. The extractor's result, or 0 when a stream stopped it.
static inline s32 Drain(UnkMoviePlayer_l225314* self) {
    s64 track = 0;
    for (;;) {
        s32 result = self->extractor->vf_0x48(&track);
        if (result != 0) {
            if (result != 0x2000)
                return result;
            for (UnkStream_l225314** it = self->begin(); it != self->end(); ++it) {
                UnkStream_l225314* stream = *it;
                if (stream && IsMovieStream(stream))
                    Feed(stream);
            }
            return 0x2000;
        }
        UnkStream_l225314** it = self->Find(track);
        if (it == self->end())
            return 0;
        UnkStream_l225314* stream = *it;
        if (!stream || !IsMovieStream(stream) || !Feed(stream))
            return 0;
    }
}

bool fn_7100225314(UnkMoviePlayer_l225314* self) {
    fn_710006691c(&self->mutex);
    bool ok = false;
    if (!fn_710021eec0(reinterpret_cast<UnkMediaInstance*>(self), 12) && self->extractor) {
        s32 result = Drain(self);
        ok = result == 0 || result == 0x2000;
    }
    fn_7100066920(&self->mutex);
    return ok;
}
