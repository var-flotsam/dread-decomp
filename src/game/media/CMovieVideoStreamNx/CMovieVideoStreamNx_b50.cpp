// The media::CMovieVideoStreamNx unit (tools/tu_map.py: 0x71002264d0-0x7100226fb0). The stream holds
// a movie::Decoder at +0x50 (as media::CMovieStreamNx does). Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "game/UnkGlobal_710b372a98.h"
#include "types.h"

namespace movie {
class MediaData {
public:
    MediaData();
    ~MediaData();
    void Clear();
    bool FindInt32(const char* key, s32* out) const;

private:
    u8 storage[0x8];
};
class Decoder {
public:
    s32 GetOutputFormat(MediaData* out);
};
}  // namespace movie

struct UnkMovieSource_b50 {
    u8 pad_0x0[0x20];
    base::global::CStrId field_0x20;  // the texture's name
};

struct UnkMovieVideoStreamNx_b50 {
    u8 pad_0x0[0x30];
    UnkMovieSource_b50* field_0x30;
    u8 pad_0x38[0x18];
    movie::Decoder field_0x50;
    u8 pad_0x51[0xc28 - 0x51];
    void* field_0xc28;  // the frame texture
    s32 field_0xc30;    // width
    s32 field_0xc34;    // height
};

// The same stream as seen by the decode loop.
struct UnkMovieVideoStreamNx2_b50 {
    u8 pad_0x0[0x30];
    u8 pad_0x30[0x10];
    u8 field_0x40;  // bit 2: paused
    u8 pad_0x41[0xc38 - 0x41];
    s64 field_0xc38;  // the wait between decodes, in microseconds
};

struct UnkGraphicsSystem;
struct UnkCountedResource;
void fn_71001cae0c(UnkGraphicsSystem* self, const base::global::CStrId* name, UnkCountedResource* texture, void* a);
struct UnkPlayer;
extern UnkPlayer* g_unk_716e554378;  // the movie texture owner (as declared in src/game/UnkRenderManager.cpp)
void fn_7100224bcc(UnkPlayer* owner, void* texture);
void* fn_7100224b00(UnkPlayer* owner, s32 width, s32 height);

// Takes the new frame size from `format`; when it changed, recreates the frame texture.
void fn_71002268dc(UnkMovieVideoStreamNx_b50* self, movie::MediaData* format) {
    s32 value = 0;
    s32 oldWidth = self->field_0xc30;
    s32 width = format->FindInt32("width", &value) ? value : oldWidth;
    s32 value2 = 0;
    s32 oldHeight = self->field_0xc34;
    s32 height = format->FindInt32("height", &value2) ? value2 : oldHeight;
    if ((width > 0 && width != self->field_0xc30) || (height > 0 && height != self->field_0xc34)) {
        self->field_0xc30 = width;
        self->field_0xc34 = height;
        if (self->field_0xc28) {
            fn_7100224bcc(g_unk_716e554378, self->field_0xc28);
        }
        void* tex = fn_7100224b00(g_unk_716e554378, self->field_0xc30, self->field_0xc34);
        self->field_0xc28 = tex;
        if (tex)
            fn_71001cae0c(reinterpret_cast<UnkGraphicsSystem*>(g_unk_710b372a98), &self->field_0x30->field_0x20, nullptr, tex);
    }
}
void fn_7100225fb8(UnkMovieVideoStreamNx_b50* self, s32 a, bool b);

// On state 0 with the flag set, reads the decoder's output format and applies it.
void fn_7100226ab0(UnkMovieVideoStreamNx_b50* self, s32 state, bool flags) {
    if (state == 0 && flags) {
        movie::MediaData format;
        if (self->field_0x50.GetOutputFormat(&format) == 0) {
            fn_71002268dc(self, &format);
        }
        format.Clear();
    }
    fn_7100225fb8(self, state, flags);
}

// The decode thread's owner: +0x1e4 is its state (atomic; 3 = stopping), +0x1f0 the video stream.
struct UnkMovieVideoThread_b50 {
    u8 pad_0x0[0x1e4];
    s32 field_0x1e4;
    u8 pad_0x1e8[0x8];
    UnkMovieVideoStreamNx2_b50* field_0x1f0;
};

void fn_7100226b70(UnkMovieVideoStreamNx2_b50* self);
void fn_71000810c0(s64 microseconds);

// The decode loop: until the state is 3, decodes (unless bit 2 of +0x40 is set) and sleeps for the
// stream's wait time, clamped to [2000, 300000] microseconds.
void fn_71002269e0(UnkMovieVideoThread_b50* self) {
    UnkMovieVideoStreamNx2_b50* stream = self->field_0x1f0;
    while (__atomic_load_n(&self->field_0x1e4, __ATOMIC_ACQUIRE) != 3) {
        if (!(stream->field_0x40 & 4))
            fn_7100226b70(stream);
        s64 wait = stream->field_0xc38;
        if (wait > 300000)
            wait = 300000;
        if (wait <= 2000)
            wait = 2000;
        stream->field_0xc38 = wait;
        fn_71000810c0(wait);
    }
}

