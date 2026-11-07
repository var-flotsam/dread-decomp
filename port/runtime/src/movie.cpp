// movie for the port (movie.h): a WebM extractor (webm.h) and a VP9 decoder over libvpx.
//
// Built with DREAD_PORT_VPX (tools/port_link.py defines it when tools/build_libvpx.py has built
// libvpx for the target) the movies play; without it every source has no tracks, as before the
// port had a decoder: GetTrackCount answers zero and the player decodes nothing.
//
// How the game drives the decoder (src/game/media/; the stream's decoder is at +0x50 and its
// DecoderEvents, four nn::os events, at +0x60):
//   - RegisterClientEvents gets the events, by kind: 0 input buffers free, 1 output buffers ready,
//     2 the output format is known, 3 an error. The worker fn_7100222ab4 waits on them.
//   - On kind 0 it takes free input indices through the decoder's virtual function at +0x48
//     (fn_7100222f14) and fills each (fn_7100226104): GetInputBuffer, the extractor's Read and
//     sample time, SendInputBufferForDecode with flag 4 when the read failed (the end), Advance.
//   - On kind 2 the stream reads GetOutputFormat ("width", "height") and takes a frame texture of
//     that size from the player's pool (fn_71002268dc), which it registers under the movie's name.
//   - On kind 1 it takes ready outputs through the virtual function at +0x60 (index, time, flags;
//     fn_7100223014) and queues them; its video loop (fn_7100226b70) hands GetOutputBuffer a Buffer
//     whose data is the frame texture's NVN texture and releases the index (flag 4: the end).
// The decoder's other virtual functions are not called on the boot path; each logs once.

#include "movie.h"

#include <condition_variable>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <deque>
#include <map>
#include <memory>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

#include "nn/fs.h"
#include "nn/os.h"
#include "webm.h"

#if defined(DREAD_PORT_VPX)
#include <vpx/vp8dx.h>
#include <vpx/vpx_decoder.h>
#endif

// The NVN shim's texture access for decoded frames (port/nvn/src/texture.cpp). Weak: an executable
// without the shim (no --nvn) decodes and drops the frames.
extern "C" bool dread_nvn_texture_size(const void* texture, std::uint32_t* width, std::uint32_t* height,
                                       std::uint32_t* bytes_per_texel) __attribute__((weak));
extern "C" void dread_nvn_texture_write_rgba(const void* texture, std::uint32_t width, std::uint32_t height,
                                             const void* rgba) __attribute__((weak));

namespace movie {

namespace webm = dread::webm;

namespace {

AllocateFunction g_allocate;
FreeFunction g_free;
ReallocateFunction g_reallocate;
void* g_user;

#if defined(DREAD_PORT_VPX)
constexpr bool kHaveDecoder = true;
#else
constexpr bool kHaveDecoder = false;
#endif

// A status other than success: the game tests for non-zero.
constexpr s32 kStatusEnd = -2;
constexpr s32 kStatusTryAgain = -3;
[[maybe_unused]] constexpr u32 kFlagEndOfStream = 4;

void LogOnce(const char* what) {
    static std::mutex m;
    static std::vector<std::string> seen;
    std::lock_guard<std::mutex> hold(m);
    for (const std::string& s : seen)
        if (s == what)
            return;
    seen.emplace_back(what);
    std::fprintf(stderr, "movie: %s\n", what);
}

// ---- MediaData: the game keeps one in 8 bytes, so the dictionary is behind a pointer.

struct MediaDict {
    std::map<std::string, std::string> strings;
    std::map<std::string, s64> ints;
};

MediaDict*& Dict(const MediaData* md) {
    return *reinterpret_cast<MediaDict**>(&const_cast<MediaData*>(md)->m_dict);
}

MediaDict* EnsureDict(MediaData* md) {
    MediaDict*& d = Dict(md);
    if (!d)
        d = new MediaDict;
    return d;
}

// ---- Extractor: the game's subclass writes its vptr over the object's first word, so the state
// lives in a table keyed by the object.

struct ExtractorState {
    webm::File file;
    s32 track = -1;          // the selected track
    std::size_t cursor = 0;  // its next frame
};

std::mutex g_extractors_lock;
std::map<const Extractor*, std::unique_ptr<ExtractorState>> g_extractors;

ExtractorState* StateOf(const Extractor* e, bool create) {
    std::lock_guard<std::mutex> hold(g_extractors_lock);
    auto it = g_extractors.find(e);
    if (it != g_extractors.end())
        return it->second.get();
    if (!create)
        return nullptr;
    return (g_extractors[e] = std::make_unique<ExtractorState>()).get();
}

const char* MimeOf(const std::string& codec) {
    if (codec == "V_VP9")
        return "video/x-vnd.on2.vp9";
    if (codec == "V_VP8")
        return "video/x-vnd.on2.vp8";
    if (codec == "A_VORBIS")
        return "audio/vorbis";
    if (codec == "A_OPUS")
        return "audio/opus";
    return "application/octet-stream";
}

s32 Load(Extractor* e, std::vector<std::uint8_t> bytes, const char* name) {
    ExtractorState* s = StateOf(e, true);
    std::string error;
    if (!webm::Parse(std::move(bytes), &s->file, &error)) {
        std::fprintf(stderr, "movie: %s: %s\n", name, error.c_str());
        return kStatusNoMedia;
    }
    s->track = -1;
    s->cursor = 0;
    for (std::size_t i = 0; i < s->file.tracks.size(); ++i) {
        const webm::Track& t = s->file.tracks[i];
        std::fprintf(stderr, "movie: %s: track %zu %s %ux%u, %zu frames, %.2f s\n", name, i, t.codec.c_str(),
                     t.width, t.height, s->file.frames[i].size(), s->file.DurationUs() / 1e6);
    }
    return kStatusOk;
}

const webm::Frame* Current(ExtractorState* s) {
    if (!s || s->track < 0)
        return nullptr;
    const auto& frames = s->file.frames[static_cast<std::size_t>(s->track)];
    return s->cursor < frames.size() ? &frames[s->cursor] : nullptr;
}

// ---- Decoder: 0x10 bytes in the game's stream (its events follow at +0x10): a vtable the game
// calls two functions of, and the state.

#if defined(DREAD_PORT_VPX)
struct DecodedFrame {
    std::vector<std::uint8_t> rgba;
    u32 width = 0, height = 0;
};
#endif

struct DecoderState {
    std::mutex lock;
    std::condition_variable wake;
    std::thread worker;
    bool quit = false;
    bool running = false;
    nn::os::EventType* events[4] = {};
    u32 width = 0, height = 0;
    bool format_reported = false;

    static constexpr int kInputs = 4;
    static constexpr int kOutputs = 4;
    std::vector<std::uint8_t> input_storage[kInputs];
    std::deque<s32> free_inputs;
    struct Pending {
        s32 index;
        s32 offset, size;
        s64 time_us;
        u32 flags;
    };
    std::deque<Pending> pending;
    struct Ready {
        s32 index;
        s64 time_us;
        u32 flags;
    };
    std::deque<Ready> ready;  // decoded, not yet acquired
    bool output_busy[kOutputs] = {};
#if defined(DREAD_PORT_VPX)
    DecodedFrame outputs[kOutputs];
    vpx_codec_ctx_t codec{};
    bool codec_open = false;
#endif

    void Signal(int kind) {
        if (kind >= 0 && kind < 4 && events[kind])
            nn::os::SignalEvent(events[kind]);
    }
};

DecoderState*& StateOf(const Decoder* d) {
    return *reinterpret_cast<DecoderState**>(&const_cast<Decoder*>(d)->state);
}

#if defined(DREAD_PORT_VPX)
// I420 to RGBA8 (BT.709, limited range: the movies are HD video).
void ToRgba(const vpx_image_t* img, std::vector<std::uint8_t>* out) {
    const u32 w = img->d_w, h = img->d_h;
    out->resize(static_cast<std::size_t>(w) * h * 4);
    std::uint8_t* dst = out->data();
    auto clamp = [](int v) { return static_cast<std::uint8_t>(v < 0 ? 0 : v > 255 ? 255 : v); };
    for (u32 y = 0; y < h; ++y) {
        const std::uint8_t* py = img->planes[VPX_PLANE_Y] + y * img->stride[VPX_PLANE_Y];
        const std::uint8_t* pu = img->planes[VPX_PLANE_U] + (y >> img->y_chroma_shift) * img->stride[VPX_PLANE_U];
        const std::uint8_t* pv = img->planes[VPX_PLANE_V] + (y >> img->y_chroma_shift) * img->stride[VPX_PLANE_V];
        for (u32 x = 0; x < w; ++x) {
            const int c = (static_cast<int>(py[x]) - 16) * 298;
            const int d = static_cast<int>(pu[x >> img->x_chroma_shift]) - 128;
            const int e = static_cast<int>(pv[x >> img->x_chroma_shift]) - 128;
            std::uint8_t* p = dst + (static_cast<std::size_t>(y) * w + x) * 4;
            p[0] = clamp((c + 459 * e + 128) >> 8);
            p[1] = clamp((c - 55 * d - 136 * e + 128) >> 8);
            p[2] = clamp((c + 541 * d + 128) >> 8);
            p[3] = 255;
        }
    }
}

void OpenCodec(DecoderState* s) {
    if (s->codec_open)
        vpx_codec_destroy(&s->codec);
    vpx_codec_dec_cfg_t cfg{};
    cfg.threads = 2;
    s->codec_open = vpx_codec_dec_init(&s->codec, vpx_codec_vp9_dx(), &cfg, 0) == VPX_CODEC_OK;
    if (!s->codec_open)
        LogOnce("libvpx: the VP9 decoder did not initialise");
}

int FreeOutput(DecoderState* s) {
    for (int i = 0; i < DecoderState::kOutputs; ++i)
        if (!s->output_busy[i])
            return i;
    return -1;
}

// The worker: decodes the queued inputs in order while an output slot is free.
void Work(DecoderState* s) {
    std::unique_lock<std::mutex> hold(s->lock);
    for (;;) {
        s->wake.wait(hold, [s] { return s->quit || (s->running && !s->pending.empty() && FreeOutput(s) >= 0); });
        if (s->quit)
            return;
        DecoderState::Pending in = s->pending.front();
        s->pending.pop_front();
        const int out = FreeOutput(s);
        bool produced = false, format_now = false;
        if (in.size > 0 && s->codec_open) {
            const std::uint8_t* data = s->input_storage[in.index].data() + in.offset;
            if (vpx_codec_decode(&s->codec, data, static_cast<unsigned>(in.size), nullptr, 0) != VPX_CODEC_OK) {
                LogOnce("libvpx: a frame did not decode");
            } else {
                vpx_codec_iter_t it = nullptr;
                if (const vpx_image_t* img = vpx_codec_get_frame(&s->codec, &it)) {
                    if (img->fmt == VPX_IMG_FMT_I420) {
                        ToRgba(img, &s->outputs[out].rgba);
                        s->outputs[out].width = img->d_w;
                        s->outputs[out].height = img->d_h;
                        if (img->d_w != s->width || img->d_h != s->height || !s->format_reported) {
                            s->width = img->d_w;
                            s->height = img->d_h;
                            s->format_reported = true;
                            format_now = true;
                        }
                        produced = true;
                    } else {
                        LogOnce("libvpx: a frame not in I420 (dropped)");
                    }
                }
            }
        }
        s->free_inputs.push_back(in.index);
        const bool end = (in.flags & kFlagEndOfStream) != 0;
        if (produced || end) {
            s->output_busy[out] = true;
            if (!produced)
                s->outputs[out].width = 0;  // the end alone: no frame
            s->ready.push_back({out, in.time_us, end && !produced ? kFlagEndOfStream : 0u});
            if (end && produced) {
                // The end comes as an output of its own after the last frame.
                const int last = FreeOutput(s);
                if (last >= 0) {
                    s->output_busy[last] = true;
                    s->outputs[last].width = 0;
                    s->ready.push_back({last, in.time_us, kFlagEndOfStream});
                }
            }
        }
        hold.unlock();
        if (format_now)
            s->Signal(2);
        s->Signal(0);
        if (produced || end)
            s->Signal(1);
        hold.lock();
    }
}
#endif

// The decoder's virtual functions (the game calls +0x48 and +0x60).
s32 AcquireInputIndex(Decoder* self, s32* index, s32* remaining) {
    DecoderState* s = StateOf(self);
    if (!s)
        return kStatusNoMedia;
    std::lock_guard<std::mutex> hold(s->lock);
    if (s->free_inputs.empty()) {
        *remaining = 0;
        return kStatusTryAgain;
    }
    *index = s->free_inputs.front();
    s->free_inputs.pop_front();
    *remaining = static_cast<s32>(s->free_inputs.size());
    return kStatusOk;
}

s32 AcquireOutputIndex(Decoder* self, s32* index, s64* time_us, u32* flags, s32* remaining) {
    DecoderState* s = StateOf(self);
    if (!s)
        return kStatusNoMedia;
    std::lock_guard<std::mutex> hold(s->lock);
    if (s->ready.empty()) {
        *remaining = 0;
        return kStatusTryAgain;
    }
    const DecoderState::Ready r = s->ready.front();
    s->ready.pop_front();
    *index = r.index;
    *time_us = r.time_us;
    *flags = r.flags;
    *remaining = static_cast<s32>(s->ready.size());
    return kStatusOk;
}

template <int Slot>
s32 UnknownSlot(Decoder*) {
    static const std::string what = "Decoder virtual function +0x" + [] {
        char b[8];
        std::snprintf(b, sizeof b, "%x", Slot * 8);
        return std::string(b);
    }() + " called (not implemented)";
    LogOnce(what.c_str());
    return kStatusNoMedia;
}

using Slot = void*;
const Slot kDecoderVtable[] = {
    reinterpret_cast<Slot>(&UnknownSlot<0>),  reinterpret_cast<Slot>(&UnknownSlot<1>),
    reinterpret_cast<Slot>(&UnknownSlot<2>),  reinterpret_cast<Slot>(&UnknownSlot<3>),
    reinterpret_cast<Slot>(&UnknownSlot<4>),  reinterpret_cast<Slot>(&UnknownSlot<5>),
    reinterpret_cast<Slot>(&UnknownSlot<6>),  reinterpret_cast<Slot>(&UnknownSlot<7>),
    reinterpret_cast<Slot>(&UnknownSlot<8>),  reinterpret_cast<Slot>(&AcquireInputIndex),
    reinterpret_cast<Slot>(&UnknownSlot<10>), reinterpret_cast<Slot>(&UnknownSlot<11>),
    reinterpret_cast<Slot>(&AcquireOutputIndex), reinterpret_cast<Slot>(&UnknownSlot<13>),
    reinterpret_cast<Slot>(&UnknownSlot<14>), reinterpret_cast<Slot>(&UnknownSlot<15>),
};

}  // namespace

void SetAllocator(AllocateFunction allocate, FreeFunction free, ReallocateFunction reallocate, void* user) {
    // fn_71002236ac hands the library the engine's allocator. Kept; the runtime's own state uses the
    // host's heap.
    g_allocate = allocate;
    g_free = free;
    g_reallocate = reallocate;
    g_user = user;
}

s32 GetThreadPriority() {
    // fn_710022586c, for its Decoder. The port's threads ignore priorities.
    return 0;
}

// ---- Buffer

Buffer::Buffer()
    : m_data(nullptr), m_capacity(0), m_offset(0), m_size(0), m_int32(0), m_uint32(0), m_int64(0),
      m_nvn_device(nullptr), m_nvn_sync(nullptr), m_owned(false) {}

Buffer::Buffer(u64 capacity) : Buffer() {
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
    if (m_owned)
        std::free(m_data);
}

void* Buffer::Base() const { return m_data; }
s32 Buffer::Offset() const { return m_offset; }
s32 Buffer::Size() const { return m_size; }
u64 Buffer::Capacity() const { return m_capacity; }

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

void Buffer::SetInt32Data(s32 value) { m_int32 = value; }
void Buffer::SetInt64Data(s64 value) { m_int64 = value; }
void Buffer::SetUint32Data(u32 value) { m_uint32 = value; }
s64 Buffer::GetInt64Data() const { return m_int64; }
u32 Buffer::GetUint32Data() const { return m_uint32; }
void Buffer::SetNvnDev(void* device) { m_nvn_device = device; }
void Buffer::SetNvnSync(void* sync) { m_nvn_sync = sync; }

// ---- MediaData

MediaData::MediaData() {
    Dict(this) = nullptr;
}

MediaData::~MediaData() {
    delete Dict(this);
    Dict(this) = nullptr;
}

void MediaData::Clear() {
    if (MediaDict* d = Dict(this)) {
        d->strings.clear();
        d->ints.clear();
    }
}

bool MediaData::FindString(const char* key, const char** out) const {
    const MediaDict* d = Dict(this);
    if (!d || !key)
        return false;
    auto it = d->strings.find(key);
    if (it == d->strings.end())
        return false;
    *out = it->second.c_str();
    return true;
}

bool MediaData::FindInt32(const char* key, s32* out) const {
    s64 v;
    if (!FindInt64(key, &v))
        return false;
    *out = static_cast<s32>(v);
    return true;
}

bool MediaData::FindInt64(const char* key, s64* out) const {
    const MediaDict* d = Dict(this);
    if (!d || !key)
        return false;
    auto it = d->ints.find(key);
    if (it == d->ints.end())
        return false;
    *out = it->second;
    return true;
}

// ---- Extractor

Extractor::Extractor(ContainerType) {
    // fn_710022323c and fn_7100223318, which then write their own vptr.
}

Extractor::~Extractor() {
    std::lock_guard<std::mutex> hold(g_extractors_lock);
    g_extractors.erase(this);
}

s32 Extractor::SetDataSource(const char* path) {
    // Through the game's vtable (slot 0x10, from fn_710022323c): a "<mount>:/<path>" file.
    nn::fs::FileHandle file;
    if (!path || nn::fs::OpenFile(&file, path, nn::fs::OpenMode_Read).IsFailure()) {
        std::fprintf(stderr, "movie: cannot open %s\n", path ? path : "(null)");
        return kStatusNoMedia;
    }
    s64 size = 0;
    nn::fs::GetFileSize(&size, file);
    std::vector<std::uint8_t> bytes(static_cast<std::size_t>(size > 0 ? size : 0));
    u64 read = 0;
    const bool ok = size > 0 && nn::fs::ReadFile(&read, file, 0, bytes.data(), bytes.size()).IsSuccess() &&
                    read == bytes.size();
    nn::fs::CloseFile(file);
    if (!ok) {
        std::fprintf(stderr, "movie: cannot read %s\n", path);
        return kStatusNoMedia;
    }
    return Load(this, std::move(bytes), path);
}

s32 Extractor::SetDataSource(const void* data, s32 size) {
    if (!data || size <= 0)
        return kStatusNoMedia;
    const auto* p = static_cast<const std::uint8_t*>(data);
    return Load(this, std::vector<std::uint8_t>(p, p + size), "(memory)");
}

s32 Extractor::SetDataSource(ClientStreamReader*, const char* name) {
    // Slot 0x18, from fn_7100223318: a stream the game reads for the library. Not on the boot path.
    std::fprintf(stderr, "movie: %s: stream sources are not supported\n", name ? name : "(null)");
    return kStatusNoMedia;
}

s32 Extractor::GetTrackCount(s32* out) {
    // Slot 0x20, from fn_71002233c4. Without a decoder no track is offered.
    const ExtractorState* s = StateOf(this, false);
    *out = kHaveDecoder && s ? static_cast<s32>(s->file.tracks.size()) : 0;
    return kStatusOk;
}

s32 Extractor::GetTrackConfiguration(s32 track, MediaData* out) {
    // fn_71002233c4 reads mime, bit-rate, durationUs, media-language, then frame-rate, width,
    // height for a video track.
    const ExtractorState* s = StateOf(this, false);
    if (!s || track < 0 || static_cast<std::size_t>(track) >= s->file.tracks.size() || !out)
        return kStatusNoMedia;
    const webm::Track& t = s->file.tracks[static_cast<std::size_t>(track)];
    const auto& frames = s->file.frames[static_cast<std::size_t>(track)];
    MediaDict* d = EnsureDict(out);
    d->strings["mime"] = MimeOf(t.codec);
    const s64 duration = s->file.DurationUs();
    d->ints["durationUs"] = duration;
    std::size_t bytes = 0;
    for (const webm::Frame& f : frames)
        bytes += f.size;
    d->ints["bit-rate"] = duration > 0 ? static_cast<s64>(bytes * 8 * 1000000.0 / duration) : 0;
    if (t.type == 1) {
        d->ints["width"] = t.width;
        d->ints["height"] = t.height;
        s64 rate = 30;
        if (t.default_duration_ns)
            rate = static_cast<s64>(1e9 / static_cast<double>(t.default_duration_ns) + 0.5);
        else if (frames.size() > 1 && duration > 0)
            rate = static_cast<s64>(frames.size() * 1000000.0 / duration + 0.5);
        d->ints["frame-rate"] = rate;
    }
    return kStatusOk;
}

s32 Extractor::SelectTrack(s32 track) {
    ExtractorState* s = StateOf(this, false);
    if (!s || track < 0 || static_cast<std::size_t>(track) >= s->file.tracks.size())
        return kStatusNoMedia;
    s->track = track;
    s->cursor = 0;
    return kStatusOk;
}

s32 Extractor::UnselectTrack(s32 track) {
    ExtractorState* s = StateOf(this, false);
    if (s && s->track == track)
        s->track = -1;
    return kStatusOk;
}

s32 Extractor::GetTrackIndexForAvailableData(u64* out) {
    ExtractorState* s = StateOf(this, false);
    if (!Current(s))
        return kStatusEnd;
    *out = static_cast<u64>(s->track);
    return kStatusOk;
}

s32 Extractor::Read(Buffer* out) {
    // fn_7100226104, through the player: the frame at the buffer's start; a failure is the end.
    ExtractorState* s = StateOf(this, false);
    const webm::Frame* f = Current(s);
    if (!f || !out)
        return kStatusEnd;
    if (f->size > out->Capacity() || !out->Base()) {
        LogOnce("Extractor::Read: a frame larger than the input buffer");
        return kStatusNoMedia;
    }
    std::memcpy(out->Base(), s->file.bytes.data() + f->offset, f->size);
    out->SetRange(0, static_cast<s32>(f->size));
    out->SetInt64Data(f->time_us);
    return kStatusOk;
}

s32 Extractor::Advance() {
    ExtractorState* s = StateOf(this, false);
    if (!Current(s))
        return kStatusEnd;
    ++s->cursor;
    return kStatusOk;
}

s32 Extractor::SeekTo(s64 time_us) {
    // To the last key frame at or before `time_us` (a loop seeks to 0).
    ExtractorState* s = StateOf(this, false);
    if (!s || s->track < 0)
        return kStatusNoMedia;
    const auto& frames = s->file.frames[static_cast<std::size_t>(s->track)];
    std::size_t at = 0;
    for (std::size_t i = 0; i < frames.size() && frames[i].time_us <= time_us; ++i)
        if (frames[i].key)
            at = i;
    s->cursor = at;
    return kStatusOk;
}

s32 Extractor::GetSampleTime(s64* out) {
    const webm::Frame* f = Current(StateOf(this, false));
    if (!f)
        return kStatusEnd;
    *out = f->time_us;
    return kStatusOk;
}

s32 Extractor::GetSampleMetaData(MediaData* out) {
    const webm::Frame* f = Current(StateOf(this, false));
    if (!f)
        return kStatusEnd;
    if (out)
        EnsureDict(out)->ints["is-sync-frame"] = f->key ? 1 : 0;
    return kStatusOk;
}

s32 Extractor::GetBufferedRange(s64* begin, s64* end) {
    // The whole file is in memory.
    const ExtractorState* s = StateOf(this, false);
    if (!s)
        return kStatusNoMedia;
    *begin = 0;
    *end = s->file.DurationUs();
    return kStatusOk;
}

// ---- Decoder

Decoder::Decoder(DecoderType, DecoderOutputFormat, DecoderMode, s32) {
    // fn_710022586c builds one at +0x50 of its stream.
    vtable = const_cast<Slot*>(kDecoderVtable);  // the game calls +0x48 and +0x60
    auto* s = new DecoderState;
    for (int i = 0; i < DecoderState::kInputs; ++i)
        s->free_inputs.push_back(i);
    StateOf(this) = s;
#if defined(DREAD_PORT_VPX)
    s->worker = std::thread(Work, s);
#endif
}

Decoder::~Decoder() {
    // fn_710022594c and fn_7100225aac.
    DecoderState* s = StateOf(this);
    if (!s)
        return;
    {
        std::lock_guard<std::mutex> hold(s->lock);
        s->quit = true;
    }
    s->wake.notify_all();
    if (s->worker.joinable())
        s->worker.join();
#if defined(DREAD_PORT_VPX)
    if (s->codec_open)
        vpx_codec_destroy(&s->codec);
#endif
    delete s;
    StateOf(this) = nullptr;
}

s32 Decoder::RegisterClientEvents(DecoderEvents* events) {
    // fn_7100225b84: four nn::os events, by kind.
    DecoderState* s = StateOf(this);
    if (!s || !events)
        return kStatusNoMedia;
    std::lock_guard<std::mutex> hold(s->lock);
    std::memcpy(s->events, events, sizeof s->events);
    return kStatusOk;
}

s32 Decoder::Configure(MediaData* format) {
    // fn_7100225b84, with the track's format from the extractor.
    DecoderState* s = StateOf(this);
    if (!s || !kHaveDecoder)
        return kStatusNoMedia;
    const char* mime = nullptr;
    if (!format || !format->FindString("mime", &mime) || std::strcmp(mime, "video/x-vnd.on2.vp9") != 0) {
        std::fprintf(stderr, "movie: Decoder::Configure: %s is not supported\n", mime ? mime : "(no mime)");
        return kStatusNoMedia;
    }
    std::lock_guard<std::mutex> hold(s->lock);
    s32 w = 0, h = 0;
    format->FindInt32("width", &w);
    format->FindInt32("height", &h);
    s->width = static_cast<u32>(w);
    s->height = static_cast<u32>(h);
    s->format_reported = false;
#if defined(DREAD_PORT_VPX)
    OpenCodec(s);
    return s->codec_open ? kStatusOk : kStatusNoMedia;
#else
    return kStatusNoMedia;
#endif
}

s32 Decoder::Start() {
    // The stream's fn_7100225fb8 (state 1). Every free input is offered.
    DecoderState* s = StateOf(this);
    if (!s || !kHaveDecoder)
        return kStatusNoMedia;
    {
        std::lock_guard<std::mutex> hold(s->lock);
        s->running = true;
    }
    s->wake.notify_all();
    s->Signal(0);
    return kStatusOk;
}

s32 Decoder::Stop() {
    DecoderState* s = StateOf(this);
    if (!s)
        return kStatusOk;
    std::lock_guard<std::mutex> hold(s->lock);
    s->running = false;
    return kStatusOk;
}

s32 Decoder::Flush() {
    // Drops queued inputs and undelivered outputs and restarts the codec (a seek or a loop follows,
    // to a key frame).
    DecoderState* s = StateOf(this);
    if (!s)
        return kStatusOk;
    {
        std::lock_guard<std::mutex> hold(s->lock);
        s->pending.clear();
        s->ready.clear();
        for (bool& b : s->output_busy)
            b = false;
        s->free_inputs.clear();
        for (int i = 0; i < DecoderState::kInputs; ++i)
            s->free_inputs.push_back(i);
#if defined(DREAD_PORT_VPX)
        if (s->codec_open)
            OpenCodec(s);
#endif
    }
    if (s->running)
        s->Signal(0);
    return kStatusOk;
}

s32 Decoder::GetInputBuffer(s32 index, Buffer* out) {
    DecoderState* s = StateOf(this);
    if (!s || !out || index < 0 || index >= DecoderState::kInputs)
        return kStatusNoMedia;
    std::lock_guard<std::mutex> hold(s->lock);
    std::vector<std::uint8_t>& storage = s->input_storage[index];
    if (storage.empty())
        storage.resize(4u << 20);  // the largest 720p frame of the game's movies is far smaller
    out->SetDataAndCapacity(storage.data(), storage.size());
    out->SetRange(0, 0);
    return kStatusOk;
}

s32 Decoder::SendInputBufferForDecode(s32 index, s32 offset, s32 size, s64 time_us, u32 flags) {
    // fn_7100226104: flag 4 marks the end (the read failed; the size may be zero).
    DecoderState* s = StateOf(this);
    if (!s || index < 0 || index >= DecoderState::kInputs)
        return kStatusNoMedia;
    {
        std::lock_guard<std::mutex> hold(s->lock);
        if (offset < 0 || size < 0 || static_cast<std::size_t>(offset) + size > s->input_storage[index].size())
            size = 0;
        s->pending.push_back({index, offset, size, time_us, flags});
    }
    s->wake.notify_all();
    return kStatusOk;
}

s32 Decoder::GetOutputBuffer(s32 index, Buffer* out) {
    // fn_7100226b70: `out` holds the frame texture's NVN texture; the frame goes into it.
    DecoderState* s = StateOf(this);
    if (!s || !out || index < 0 || index >= 4)
        return kStatusNoMedia;
#if defined(DREAD_PORT_VPX)
    std::lock_guard<std::mutex> hold(s->lock);
    const DecodedFrame& f = s->outputs[index];
    if (!s->output_busy[index] || f.width == 0)
        return kStatusNoMedia;
    const void* texture = out->Base();
    std::uint32_t tw = 0, th = 0, bpt = 0;
    if (!dread_nvn_texture_size || !dread_nvn_texture_write_rgba) {
        LogOnce("Decoder::GetOutputBuffer: no NVN shim in this executable (frames dropped)");
        return kStatusOk;
    }
    if (!dread_nvn_texture_size(texture, &tw, &th, &bpt)) {
        LogOnce("Decoder::GetOutputBuffer: the buffer is not an NVN texture the shim knows (frames dropped)");
        return kStatusOk;
    }
    if (tw != f.width || th != f.height || bpt != 4) {
        LogOnce("Decoder::GetOutputBuffer: the frame texture's size or format does not fit the frame (dropped)");
        return kStatusOk;
    }
    dread_nvn_texture_write_rgba(texture, f.width, f.height, f.rgba.data());
    LogOnce("Decoder::GetOutputBuffer: the first frame is in its texture");
    return kStatusOk;
#else
    return kStatusNoMedia;
#endif
}

s32 Decoder::ReleaseOutputBufferIndex(s32 index) {
    DecoderState* s = StateOf(this);
    if (!s || index < 0 || index >= DecoderState::kOutputs)
        return kStatusOk;
    {
        std::lock_guard<std::mutex> hold(s->lock);
        s->output_busy[index] = false;
    }
    s->wake.notify_all();
    return kStatusOk;
}

s32 Decoder::GetOutputFormat(MediaData* out) {
    // fn_7100226ab0 on kind 2: the frame size, for the frame texture.
    DecoderState* s = StateOf(this);
    if (!s || !out)
        return kStatusNoMedia;
    std::lock_guard<std::mutex> hold(s->lock);
    if (!s->width || !s->height)
        return kStatusNoMedia;
    MediaDict* d = EnsureDict(out);
    d->ints["width"] = s->width;
    d->ints["height"] = s->height;
    d->strings["mime"] = "video/raw";
    return kStatusOk;
}

s32 Decoder::GetOutputBufferFormat(s32 index, MediaData* out) {
    // fn_7100226b70 before each frame: the frame's size.
    DecoderState* s = StateOf(this);
    if (!s || !out || index < 0 || index >= DecoderState::kOutputs)
        return kStatusNoMedia;
#if defined(DREAD_PORT_VPX)
    std::lock_guard<std::mutex> hold(s->lock);
    const DecodedFrame& f = s->outputs[index];
    if (!f.width)
        return kStatusNoMedia;
    MediaDict* d = EnsureDict(out);
    d->ints["width"] = f.width;
    d->ints["height"] = f.height;
    return kStatusOk;
#else
    return kStatusNoMedia;
#endif
}

}  // namespace movie
