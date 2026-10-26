// movie for the port: a complete Buffer, and a source with no tracks until the port has a video
// decoder (movie.h). Every stub names the boot-path function that depends on it in a TODO.

#include "movie.h"

#include <cstdlib>
#include <cstring>

namespace movie {

namespace {

AllocateFunction g_allocate;
FreeFunction g_free;
ReallocateFunction g_reallocate;
void* g_user;

}  // namespace

void SetAllocator(AllocateFunction allocate, FreeFunction free, ReallocateFunction reallocate, void* user) {
    // fn_71002236ac hands the library the engine's allocator. Kept: the port's Buffer uses the
    // host's heap, as the decoder will.
    g_allocate = allocate;
    g_free = free;
    g_reallocate = reallocate;
    g_user = user;
}

s32 GetThreadPriority() {
    // TODO(M1): fn_710022586c, for its Decoder. The port's threads ignore priorities.
    return 0;
}

Buffer::Buffer()
    : m_data(nullptr), m_capacity(0), m_offset(0), m_size(0), m_int32(0), m_uint32(0), m_int64(0),
      m_nvn_device(nullptr), m_nvn_sync(nullptr), m_owned(false) {
    // fn_7100222f14, fn_7100226104, fn_7100226b70: an empty buffer, filled by GetInputBuffer and
    // GetOutputBuffer.
}

Buffer::Buffer(u64 capacity) : Buffer() {
    // TODO(M1): the player fn_71002211b4 and its helpers allocate their own buffers.
    m_data = static_cast<u8*>(std::calloc(capacity ? capacity : 1, 1));
    if (!m_data)
        __builtin_trap();
    m_capacity = capacity;
    m_owned = true;
}

Buffer::Buffer(void* data, u64 capacity) : Buffer() {
    // fn_7100221698, fn_7100221b3c, fn_7100221d48 wrap memory of their own.
    m_data = static_cast<u8*>(data);
    m_capacity = capacity;
}

Buffer::Buffer(const Buffer& other) : Buffer() {
    // fn_7100221698: a copy owns a copy of the bytes if the original owned its own.
    m_offset = other.m_offset;
    m_size = other.m_size;
    m_int32 = other.m_int32;
    m_uint32 = other.m_uint32;
    m_int64 = other.m_int64;
    m_nvn_device = other.m_nvn_device;
    m_nvn_sync = other.m_nvn_sync;
    m_capacity = other.m_capacity;
    if (other.m_owned) {
        m_data = static_cast<u8*>(std::malloc(m_capacity ? m_capacity : 1));
        if (!m_data)
            __builtin_trap();
        std::memcpy(m_data, other.m_data, m_capacity);
        m_owned = true;
    } else {
        m_data = other.m_data;
    }
}

Buffer::~Buffer() {
    // TODO(M1): fn_71002211b4 and the player's destructor fn_7100221534 (arrays of them).
    if (m_owned)
        std::free(m_data);
}

void* Buffer::Base() const {
    return m_data;
}

s32 Buffer::Offset() const {
    return m_offset;
}

s32 Buffer::Size() const {
    return m_size;
}

u64 Buffer::Capacity() const {
    return m_capacity;
}

void Buffer::SetRange(s32 offset, s32 size) {
    m_offset = offset;
    m_size = size;
}

void Buffer::SetDataAndCapacity(void* data, u64 capacity) {
    if (m_owned)
        std::free(m_data);
    m_data = static_cast<u8*>(data);
    m_capacity = capacity;
    m_owned = false;
}

void Buffer::SetInt32Data(s32 value) {
    m_int32 = value;
}

void Buffer::SetInt64Data(s64 value) {
    m_int64 = value;
}

void Buffer::SetUint32Data(u32 value) {
    m_uint32 = value;
}

s64 Buffer::GetInt64Data() const {
    return m_int64;
}

u32 Buffer::GetUint32Data() const {
    return m_uint32;
}

void Buffer::SetNvnDev(void* device) {
    m_nvn_device = device;
}

void Buffer::SetNvnSync(void* sync) {
    m_nvn_sync = sync;
}

MediaData::MediaData() {
    // TODO(M1): fn_71002233c4, per track.
}

MediaData::~MediaData() {}

void MediaData::Clear() {}

bool MediaData::FindString(const char*, const char**) const {
    // TODO(M1): fn_71002233c4 looks up "mime" first; nothing is found without a track.
    return false;
}

bool MediaData::FindInt32(const char*, s32*) const {
    return false;
}

bool MediaData::FindInt64(const char*, s64*) const {
    return false;
}

Extractor::Extractor(ContainerType) {
    // TODO(M1): fn_710022323c and fn_7100223318, which then write their own vptr.
}

Extractor::~Extractor() {
    // fn_710022339c and fn_71002233a0.
}

s32 Extractor::SetDataSource(const char*) {
    // Through the game's vtable (slot 0x10, from fn_710022323c): accepted, and empty.
    return kStatusOk;
}

s32 Extractor::SetDataSource(const void*, s32) {
    return kStatusOk;
}

s32 Extractor::SetDataSource(ClientStreamReader*, const char*) {
    // Slot 0x18, from fn_7100223318.
    return kStatusOk;
}

s32 Extractor::GetTrackCount(s32* out) {
    // Slot 0x20, from fn_71002233c4, which then reports every track index as missing.
    *out = 0;
    return kStatusOk;
}

s32 Extractor::GetTrackConfiguration(s32, MediaData*) {
    return kStatusNoMedia;
}

s32 Extractor::SelectTrack(s32) {
    return kStatusNoMedia;
}

s32 Extractor::UnselectTrack(s32) {
    return kStatusNoMedia;
}

s32 Extractor::GetTrackIndexForAvailableData(u64*) {
    return kStatusNoMedia;
}

s32 Extractor::Read(Buffer*) {
    return kStatusNoMedia;
}

s32 Extractor::Advance() {
    return kStatusNoMedia;
}

s32 Extractor::SeekTo(s64) {
    return kStatusNoMedia;
}

s32 Extractor::GetSampleTime(s64*) {
    return kStatusNoMedia;
}

s32 Extractor::GetSampleMetaData(MediaData*) {
    return kStatusNoMedia;
}

s32 Extractor::GetBufferedRange(s64*, s64*) {
    return kStatusNoMedia;
}

Decoder::Decoder(DecoderType, DecoderOutputFormat, DecoderMode, s32) {
    // TODO(M1): fn_710022586c builds one in its player.
}

Decoder::~Decoder() {
    // fn_710022594c and fn_7100225aac.
}

s32 Decoder::RegisterClientEvents(DecoderEvents*) {
    return kStatusNoMedia;
}

s32 Decoder::Configure(MediaData*) {
    // fn_7100225b84: nothing to configure without a track.
    return kStatusNoMedia;
}

s32 Decoder::Start() {
    return kStatusNoMedia;
}

s32 Decoder::Stop() {
    return kStatusOk;
}

s32 Decoder::Flush() {
    return kStatusOk;
}

s32 Decoder::GetInputBuffer(s32, Buffer*) {
    return kStatusNoMedia;
}

s32 Decoder::SendInputBufferForDecode(s32, s32, s32, s64, u32) {
    return kStatusNoMedia;
}

s32 Decoder::GetOutputBuffer(s32, Buffer*) {
    return kStatusNoMedia;
}

s32 Decoder::ReleaseOutputBufferIndex(s32) {
    return kStatusOk;
}

s32 Decoder::GetOutputFormat(MediaData*) {
    return kStatusNoMedia;
}

s32 Decoder::GetOutputBufferFormat(s32, MediaData*) {
    return kStatusNoMedia;
}

}  // namespace movie
