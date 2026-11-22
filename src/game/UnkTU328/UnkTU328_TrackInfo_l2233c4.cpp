// Unit 328: reads one track's format from a movie extractor (its mime type, bit rate, duration,
// language, and the video or audio parameters and codec) into a track description.
// Names and layouts are placeholders.
#include "base/global/CRntString.h"
#include "types.h"

namespace movie {
class MediaData {
public:
    MediaData();
    ~MediaData();
    void Clear();
    bool FindString(const char* key, const char** out) const;
    bool FindInt32(const char* key, s32* out) const;
    bool FindInt64(const char* key, s64* out) const;

private:
    u8 storage[0x8];
};
}  // namespace movie

bool CRntString_EqualsIgnoreCaseN(const base::global::CRntString* self, const char* str, u32 n);

// The extractor the track is read from (placeholder slot names).
class UnkMovieExtractor_l2233c4 {
public:
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual s32 vf_0x20(s32* count);                         // the number of tracks
    virtual s32 vf_0x28(s32 track, movie::MediaData* out);  // the track's format
};

struct UnkTrackInfo_l2233c4 {
    s32 track;      // +0x0
    s32 kind;       // +0x4: 0 video, 1 audio
    s32 codec;      // +0x8: 0 avc, 1 vp8, 2 vp9, 3 mp4a, 4 vorbis
    u8 pad_0xc[0x4];
    s64 duration_us;  // +0x10
    s32 bit_rate;     // +0x18
    u8 pad_0x1c[0xc];
    union {
        struct {
            f32 frame_rate;  // +0x28
            s32 width;       // +0x2c
            s32 height;      // +0x30
        } video;
        struct {
            s32 sample_rate;    // +0x28
            s32 channel_count;  // +0x2c
        } audio;
    };
    u8 pad_0x34[0x4];
    base::global::CRntString language;  // +0x38
};
static_assert(__builtin_offsetof(UnkTrackInfo_l2233c4, language) == 0x38, "layout");

s32 fn_71002233c4(UnkMovieExtractor_l2233c4* extractor, s32 track, UnkTrackInfo_l2233c4* info) {
    s32 count;
    extractor->vf_0x20(&count);
    if (track < 0 || track >= count)
        return 5;

    s32 status;
    {
        movie::MediaData format;
        status = extractor->vf_0x28(track, &format);
        if (status == 0) {
            info->track = track;
            const char* mime_str;
            if (format.FindString("mime", &mime_str)) {
                base::global::CRntString mime(mime_str);
                format.FindInt32("bit-rate", &info->bit_rate);
                format.FindInt64("durationUs", &info->duration_us);
                const char* language;
                const char* name = format.FindString("media-language", &language) ? language : "int";
                CRntString_AssignCString(&info->language, name);
                if (CRntString_EqualsIgnoreCaseN(&mime, "video/", 6)) {
                    info->kind = 0;
                    s32 frame_rate;
                    format.FindInt32("frame-rate", &frame_rate);
                    info->video.frame_rate = frame_rate;
                    format.FindInt32("width", &info->video.width);
                    format.FindInt32("height", &info->video.height);
                    if (CRntString_Contains(&mime, "avc"))
                        info->codec = 0;
                    else if (CRntString_Contains(&mime, "x-vnd.on2.vp8"))
                        info->codec = 1;
                    else if (CRntString_Contains(&mime, "x-vnd.on2.vp9"))
                        info->codec = 2;
                } else if (CRntString_EqualsIgnoreCaseN(&mime, "audio/", 6)) {
                    info->kind = 1;
                    format.FindInt32("channel-count", &info->audio.channel_count);
                    format.FindInt32("sample-rate", &info->audio.sample_rate);
                    if (CRntString_Contains(&mime, "mp4a"))
                        info->codec = 3;
                    else if (CRntString_Contains(&mime, "vorbis"))
                        info->codec = 4;
                }
            }
        }
        format.Clear();
    }
    return status != 0 ? 5 : 1;
}
