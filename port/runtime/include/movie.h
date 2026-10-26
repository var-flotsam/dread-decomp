#pragma once

// movie: the SDK's video player library (an extractor for the container, a decoder, buffers and
// media-format dictionaries). The game's player (src/game_71001e2acc/: fn_71002211b4 builds the
// player, fn_710022586c its decoder, fn_710022323c and fn_7100223318 its extractor over a
// "<mount>:/<path>" file or a stream; fn_71002233c4 reads each track's format: mime type,
// bit rate, duration, language, frame rate and size or channels and sample rate) subclasses
// Extractor and Decoder: it writes its own vptr over theirs, and its vtables in .data name the
// library's functions, which the data loader resolves by mangled name to these.
//
// On PC, until the port has a video decoder: Buffer is complete (a byte container the player uses
// on its own), and every source has no tracks: SetDataSource accepts any file or stream,
// GetTrackCount answers zero, so fn_71002233c4 reports each track as missing (its code 5) and
// the player has nothing to decode; the other Extractor and Decoder calls return kStatusNoMedia.
//
// `main` imports (data/functions/; parameter lists from the mangled names): SetAllocator,
// GetThreadPriority, Buffer (17), Extractor (16), MediaData (6), Decoder (16). All declared below.
// On an LLP64 host the 64-bit parameters keep their width (s64) and lose the mangled `long` of the
// console's names, so the loader cannot resolve those few by name there (they trap if called).

#include "types.h"

namespace movie {

// The library's status codes: zero is success; the game tests for non-zero only
// (fn_71002233c4). The port's one failure:
constexpr s32 kStatusOk = 0;
constexpr s32 kStatusNoMedia = -1;

using AllocateFunction = void* (*)(u64 size, u64 alignment, void* user);
using FreeFunction = void (*)(void* pointer, void* user);
using ReallocateFunction = void* (*)(void* pointer, u64 size, void* user);
void SetAllocator(AllocateFunction allocate, FreeFunction free, ReallocateFunction reallocate, void* user);
// The decoder thread's priority (fn_710022586c passes it to the Decoder).
s32 GetThreadPriority();

// A byte range in memory, owned or borrowed, with three words of side data and the NVN objects
// a decoded frame needs. 0x40 bytes: fn_7100221534 destroys arrays of them at that stride.
class Buffer {
public:
    Buffer();
    explicit Buffer(u64 capacity);
    Buffer(void* data, u64 capacity);
    Buffer(const Buffer& other);
    ~Buffer();

    void* Base() const;
    s32 Offset() const;
    s32 Size() const;
    u64 Capacity() const;
    void SetRange(s32 offset, s32 size);
    void SetDataAndCapacity(void* data, u64 capacity);
    void SetInt32Data(s32 value);
    void SetInt64Data(s64 value);
    void SetUint32Data(u32 value);
    s64 GetInt64Data() const;
    u32 GetUint32Data() const;
    void SetNvnDev(void* device);
    void SetNvnSync(void* sync);

private:
    u8* m_data;
    u64 m_capacity;
    s32 m_offset;
    s32 m_size;
    s32 m_int32;
    u32 m_uint32;
    s64 m_int64;
    void* m_nvn_device;
    void* m_nvn_sync;
    bool m_owned;
};
static_assert(sizeof(Buffer) <= 0x40, "fn_7100221534 steps 0x40 bytes");

// A media-format dictionary (keys such as "mime", "width", "durationUs"). fn_71002233c4 keeps one
// in 12 bytes of its frame, so the port's has no state: nothing is ever found.
class MediaData {
public:
    MediaData();
    ~MediaData();
    void Clear();
    bool FindString(const char* key, const char** out) const;
    bool FindInt32(const char* key, s32* out) const;
    bool FindInt64(const char* key, s64* out) const;
};

enum ContainerType : s32 {};
class ClientStreamReader;

// The game's extractor subclass keeps its vptr at +0; the port's base has no state.
class Extractor {
public:
    explicit Extractor(ContainerType type);
    ~Extractor();
    s32 SetDataSource(const char* path);
    s32 SetDataSource(const void* data, s32 size);
    s32 SetDataSource(ClientStreamReader* reader, const char* name);
    s32 GetTrackCount(s32* out);
    s32 GetTrackConfiguration(s32 track, MediaData* out);
    s32 SelectTrack(s32 track);
    s32 UnselectTrack(s32 track);
    s32 GetTrackIndexForAvailableData(u64* out);
    s32 Read(Buffer* out);
    s32 Advance();
    s32 SeekTo(s64 time_us);
    s32 GetSampleTime(s64* out);
    s32 GetSampleMetaData(MediaData* out);
    s32 GetBufferedRange(s64* begin, s64* end);

    void* vtable;
};

enum DecoderType : s32 {};
enum DecoderOutputFormat : s32 {};
enum DecoderMode : s32 {};
class DecoderEvents;

// fn_710022586c builds one at +0x50 of its player; the next member is at +0xe8.
class Decoder {
public:
    Decoder(DecoderType type, DecoderOutputFormat format, DecoderMode mode, s32 priority);
    ~Decoder();
    s32 RegisterClientEvents(DecoderEvents* events);
    s32 Configure(MediaData* format);
    s32 Start();
    s32 Stop();
    s32 Flush();
    s32 GetInputBuffer(s32 index, Buffer* out);
    s32 SendInputBufferForDecode(s32 index, s32 offset, s32 size, s64 time_us, u32 flags);
    s32 GetOutputBuffer(s32 index, Buffer* out);
    s32 ReleaseOutputBufferIndex(s32 index);
    s32 GetOutputFormat(MediaData* out);
    s32 GetOutputBufferFormat(s32 index, MediaData* out);

    void* vtable;
};

}  // namespace movie
