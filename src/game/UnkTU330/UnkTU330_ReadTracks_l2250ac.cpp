// Unit 330 of tools/tu_map.py: under the player's mutex, asks its movie extractor for each track's
// description (fn_71002233c4) and appends the ones with a known kind and codec to its track list.
// Names and layouts are placeholders.
#include <new>

#include "../../game_aca57c/UnkBufferString.h"
#include "base/UnkDefaultAllocator.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "game/UnkBufferStringPool.h"
#include "types.h"

using base::global::UnkStringData;
using base::global::UnkStringPool;

class UnkMovieExtractor_l2233c4;
struct UnkTrackInfo_l2233c4;
s32 fn_71002233c4(UnkMovieExtractor_l2233c4* extractor, s32 track, UnkTrackInfo_l2233c4* info);

// The extractor's slots as used here.
class UnkExtractor_l2250ac {
public:
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual s32 vf_0x20(s32* count);  // the number of tracks
};

// A file-path id that starts as a copy of the shared one and is released in two steps.
struct UnkPathId_l2250ac {
    UnkStringData* data;

    UnkPathId_l2250ac() : data(g_unk_7101d470e8.data) {
        if (&g_unk_7101d47200->empty != data)
            ++data->ref_count;
    }
    UnkPathId_l2250ac(const UnkPathId_l2250ac& other) : data(other.data) {
        if (&g_unk_7101d47200->empty != data)
            ++data->ref_count;
    }
    ~UnkPathId_l2250ac() {
        if (UnkStringPool* pool = g_unk_7101d47200) {
            if (data && data != &pool->empty) {
                --data->ref_count;
                if (data->ref_count == 0) {
                    StringPool_Release(pool, data);
                    pool = g_unk_7101d47200;
                }
            }
            data = &pool->empty;
        }
    }
};

// One track's description, as fn_71002233c4 fills it.
struct UnkTrack_l2250ac {
    s32 track = -1;
    s32 kind = 0x7fffffff;
    s32 codec = 0x7fffffff;
    s64 duration_us;
    s32 bit_rate;
    UnkPathId_l2250ac path;  // 0x20
    s32 field_0x28;
    s32 field_0x2c;
    s32 field_0x30;
    UnkBufferString<0x20, g_unk_7101a1b5a0> language;  // 0x38
};
static_assert(__builtin_offsetof(UnkTrack_l2250ac, field_0x28) == 0x28, "layout");
static_assert(sizeof(UnkTrack_l2250ac) == 0xa0, "layout");

struct UnkMoviePlayer_l2250ac {
    u8 pad_0x0[0x88];
    u32 track_count;
    u8 tracks[14 * sizeof(UnkTrack_l2250ac)];  // 0x8c: the tracks, unaligned after the count
    u8 pad_0x94c[0x14];
    nn::os::MutexType mutex;  // 0x960
    u8 pad_0x980[0x158];
    UnkExtractor_l2250ac* extractor;  // 0xad8

    UnkTrack_l2250ac* Tracks() { return reinterpret_cast<UnkTrack_l2250ac*>(tracks); }
};
static_assert(__builtin_offsetof(UnkMoviePlayer_l2250ac, mutex) == 0x960, "layout");
static_assert(__builtin_offsetof(UnkMoviePlayer_l2250ac, extractor) == 0xad8, "layout");

void fn_71002250ac(UnkMoviePlayer_l2250ac* self) {
    fn_710006691c(&self->mutex);
    s32 count = 0;
    self->extractor->vf_0x20(&count);
    for (s32 i = 0; i < count; ++i) {
        UnkTrack_l2250ac track;
        fn_71002233c4(reinterpret_cast<UnkMovieExtractor_l2233c4*>(self->extractor), i,
                      reinterpret_cast<UnkTrackInfo_l2233c4*>(&track));
        if (track.kind != 0x7fffffff && track.codec != 0x7fffffff) {
            new (&self->Tracks()[self->track_count]) UnkTrack_l2250ac(track);
            ++self->track_count;
        }
    }
    fn_7100066920(&self->mutex);
}
