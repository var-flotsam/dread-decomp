// nvn_vulkan_check <shader dir>
//
// The shim's shader manifest and binding layer against a real Vulkan implementation (in the port's
// Vulkan image: Mesa's lavapipe, no GPU), with the Khronos validation layers on. For every program
// of the manifest:
//  - a VkShaderModule per stage from the installed SPIR-V;
//  - the stage's VkDescriptorSetLayout from StageLayoutBindings, and the program's pipeline layout
//    (the vertex stage's set and the fragment stage's set, at the manifest's set indices);
//  - one descriptor set per stage, filled by BindingState::BuildWrites (every NVN slot bound, a
//    resolver over real buffers, a 1x1 image and a sampler, placeholders for unused resources)
//    and vkUpdateDescriptorSets;
//  - a graphics pipeline (dynamic rendering; vertex attributes and colour attachments typed from
//    the SPIR-V's own inputs and outputs), and a vertex-only one as the engine's depth-only pass
//    binds (port/nvn/README.md).
// Every validation message is counted; the exit status is 0 only when nothing failed and the
// layers reported no error. Nothing is drawn: image layouts and view types are checked by the
// layers only at draw time, which is the next step's (a 2D view stands in for every sampler).
//
// Reads game-derived SPIR-V from build/; prints only counts, validation message names and the
// names of failing programs.

#include <algorithm>
#include <chrono>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <map>
#include <string>
#include <vector>

#include <vulkan/vulkan.h>

#include "nvn_shim/bindings.h"
#include "nvn_shim/shader_manifest.h"
#include "nvn_shim/spirv_scan.h"

using namespace dread::nvn;

namespace {

int g_failures = 0;
void Fail(const std::string& where, const std::string& what) {
    if (++g_failures <= 20)
        std::fprintf(stderr, "FAIL %s: %s\n", where.c_str(), what.c_str());
}

// Validation messages by name (the layers' VUID or message id), with a first example.
struct Message {
    int count = 0;
    std::string example;
    std::vector<std::string> programs;  // the first few programs that raised it
};
std::map<std::string, Message> g_errors, g_warnings;
std::string g_current;  // the program being checked

VKAPI_ATTR VkBool32 VKAPI_CALL OnMessage(VkDebugUtilsMessageSeverityFlagBitsEXT severity,
                                         VkDebugUtilsMessageTypeFlagsEXT, const VkDebugUtilsMessengerCallbackDataEXT* data,
                                         void*) {
    auto* table = severity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT     ? &g_errors
                  : severity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT ? &g_warnings
                                                                              : nullptr;
    if (table) {
        Message& m = (*table)[data->pMessageIdName ? data->pMessageIdName : "(no id)"];
        if (m.count++ == 0 && data->pMessage) {
            m.example = data->pMessage;
            // Printed as it arrives too, so that a crash later does not lose it.
            std::fprintf(stderr, "%s (%s): %s\n", table == &g_errors ? "validation error" : "validation warning",
                         g_current.c_str(), m.example.c_str());
            std::fflush(stderr);
        }
        if (m.programs.size() < 8 && (m.programs.empty() || m.programs.back() != g_current))
            m.programs.push_back(g_current);
    }
    return VK_FALSE;
}

bool ReadFile(const std::string& path, std::vector<unsigned char>* out) {
    std::FILE* f = std::fopen(path.c_str(), "rb");
    if (!f)
        return false;
    out->clear();
    unsigned char buf[1 << 16];
    std::size_t n;
    while ((n = std::fread(buf, 1, sizeof(buf), f)) > 0)
        out->insert(out->end(), buf, buf + n);
    bool ok = !std::ferror(f);
    std::fclose(f);
    return ok;
}

#define VK_OK(call, what)                                                                            \
    do {                                                                                             \
        VkResult r_ = (call);                                                                        \
        if (r_ != VK_SUCCESS) {                                                                      \
            std::fprintf(stderr, "%s failed: VkResult %d\n", what, static_cast<int>(r_));          \
            return false;                                                                            \
        }                                                                                            \
    } while (0)

VkFormat FormatFor(const InterfaceVar& v) {
    static const VkFormat kFloat[4] = {VK_FORMAT_R32_SFLOAT, VK_FORMAT_R32G32_SFLOAT, VK_FORMAT_R32G32B32_SFLOAT,
                                       VK_FORMAT_R32G32B32A32_SFLOAT};
    static const VkFormat kInt[4] = {VK_FORMAT_R32_SINT, VK_FORMAT_R32G32_SINT, VK_FORMAT_R32G32B32_SINT,
                                     VK_FORMAT_R32G32B32A32_SINT};
    static const VkFormat kUint[4] = {VK_FORMAT_R32_UINT, VK_FORMAT_R32G32_UINT, VK_FORMAT_R32G32B32_UINT,
                                      VK_FORMAT_R32G32B32A32_UINT};
    const std::uint32_t c = (v.components >= 1 && v.components <= 4 ? v.components : 4) - 1;
    switch (v.kind) {
    case ScalarKind::Int:
        return kInt[c];
    case ScalarKind::Uint:
        return kUint[c];
    default:
        return kFloat[c];
    }
}

// Colour attachment formats for fragment outputs: four components of the output's type.
VkFormat AttachmentFor(const InterfaceVar& v) {
    switch (v.kind) {
    case ScalarKind::Int:
        return VK_FORMAT_R32G32B32A32_SINT;
    case ScalarKind::Uint:
        return VK_FORMAT_R32G32B32A32_UINT;
    default:
        return VK_FORMAT_R16G16B16A16_SFLOAT;
    }
}

class Checker : public ResourceResolver {
public:
    bool Init(bool validation);
    void Shutdown();
    void CheckProgram(const ShaderManifest& manifest, const PackageInfo& pkg, const ProgramInfo& prog);

    // ResourceResolver over this checker's buffers, image and sampler.
    bool ResolveBuffer(BufferAddress address, std::uint64_t size, VkBuffer* buffer, VkDeviceSize* offset) override {
        if (address < kBase || address + size > kBase + uniform_size_)
            return false;
        *buffer = uniform_;
        *offset = address - kBase;
        return true;
    }
    bool ResolveTexture(std::uint32_t, VkImageView* view, VkImageLayout* layout) override {
        *view = view_;
        *layout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        return true;
    }
    bool ResolveSampler(std::uint32_t, VkSampler* sampler) override {
        *sampler = sampler_;
        return true;
    }
    VkDescriptorBufferInfo PlaceholderBuffer() override { return {placeholder_, 0, VK_WHOLE_SIZE}; }
    VkDescriptorImageInfo PlaceholderImage() override {
        return {sampler_, view_, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL};
    }

    void SizeFor(const ShaderManifest& manifest);
    bool CreateResources();

    int modules = 0, set_layouts = 0, sets = 0, pipelines = 0, vertex_only = 0;

private:
    static constexpr BufferAddress kBase = 0x100000000ull;
    bool Buffer(VkDeviceSize size, VkBuffer* buffer, VkDeviceMemory* memory);
    std::uint32_t MemoryType(std::uint32_t bits, VkMemoryPropertyFlags flags) const;
    BufferAddress SlotAddress(Stage stage, std::uint32_t slot) const {
        return kBase + (static_cast<std::uint64_t>(stage) * kUniformBufferSlots + slot) * stride_;
    }

    VkInstance instance_ = VK_NULL_HANDLE;
    VkDebugUtilsMessengerEXT messenger_ = VK_NULL_HANDLE;
    VkPhysicalDevice physical_ = VK_NULL_HANDLE;
    VkPhysicalDeviceProperties props_{};
    VkPhysicalDeviceMemoryProperties memory_props_{};
    VkDevice device_ = VK_NULL_HANDLE;
    VkBuffer uniform_ = VK_NULL_HANDLE, placeholder_ = VK_NULL_HANDLE;
    VkDeviceMemory uniform_memory_ = VK_NULL_HANDLE, placeholder_memory_ = VK_NULL_HANDLE, image_memory_ = VK_NULL_HANDLE;
    VkImage image_ = VK_NULL_HANDLE;
    VkImageView view_ = VK_NULL_HANDLE;
    VkSampler sampler_ = VK_NULL_HANDLE;
    VkDeviceSize max_block_ = 16, stride_ = 256, uniform_size_ = 0;
};

std::uint32_t Checker::MemoryType(std::uint32_t bits, VkMemoryPropertyFlags flags) const {
    for (std::uint32_t i = 0; i < memory_props_.memoryTypeCount; ++i)
        if ((bits & (1u << i)) && (memory_props_.memoryTypes[i].propertyFlags & flags) == flags)
            return i;
    return UINT32_MAX;
}

bool Checker::Init(bool validation) {
    VkApplicationInfo app{VK_STRUCTURE_TYPE_APPLICATION_INFO};
    app.pApplicationName = "nvn_vulkan_check";
    app.apiVersion = VK_API_VERSION_1_3;
    const char* layers[] = {"VK_LAYER_KHRONOS_validation"};
    const char* extensions[] = {VK_EXT_DEBUG_UTILS_EXTENSION_NAME};
    VkDebugUtilsMessengerCreateInfoEXT messenger{VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT};
    messenger.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    messenger.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
                            VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    messenger.pfnUserCallback = OnMessage;
    VkInstanceCreateInfo ici{VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO};
    ici.pNext = &messenger;
    ici.pApplicationInfo = &app;
    ici.enabledLayerCount = validation ? 1 : 0;
    ici.ppEnabledLayerNames = layers;
    ici.enabledExtensionCount = 1;
    ici.ppEnabledExtensionNames = extensions;
    VK_OK(vkCreateInstance(&ici, nullptr, &instance_), "vkCreateInstance (are the validation layers installed?)");
    auto create_messenger = reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT>(
        vkGetInstanceProcAddr(instance_, "vkCreateDebugUtilsMessengerEXT"));
    if (!create_messenger)
        return false;
    VK_OK(create_messenger(instance_, &messenger, nullptr, &messenger_), "vkCreateDebugUtilsMessengerEXT");

    std::uint32_t count = 1;
    VkResult r = vkEnumeratePhysicalDevices(instance_, &count, &physical_);
    if ((r != VK_SUCCESS && r != VK_INCOMPLETE) || count == 0) {
        std::fprintf(stderr, "no Vulkan device\n");
        return false;
    }
    vkGetPhysicalDeviceProperties(physical_, &props_);
    vkGetPhysicalDeviceMemoryProperties(physical_, &memory_props_);
    std::uint32_t families = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(physical_, &families, nullptr);
    std::vector<VkQueueFamilyProperties> fam(families);
    vkGetPhysicalDeviceQueueFamilyProperties(physical_, &families, fam.data());
    std::uint32_t graphics = UINT32_MAX;
    for (std::uint32_t i = 0; i < families; ++i)
        if (fam[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            graphics = i;
            break;
        }
    if (graphics == UINT32_MAX)
        return false;
    // The features the shim's device needs (port/nvn/README.md): queried, then enabled.
    VkPhysicalDeviceVulkan13Features have13{VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_FEATURES};
    VkPhysicalDeviceFeatures2 have{VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2};
    have.pNext = &have13;
    if (props_.apiVersion < VK_API_VERSION_1_3) {
        std::fprintf(stderr, "%s supports Vulkan %u.%u; the checks need 1.3\n", props_.deviceName,
                     VK_API_VERSION_MAJOR(props_.apiVersion), VK_API_VERSION_MINOR(props_.apiVersion));
        return false;
    }
    vkGetPhysicalDeviceFeatures2(physical_, &have);
    if (!have13.dynamicRendering || !have13.maintenance4) {
        std::fprintf(stderr, "%s lacks dynamicRendering or maintenance4\n", props_.deviceName);
        return false;
    }
    const float priority = 1.0f;
    VkDeviceQueueCreateInfo qci{VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO};
    qci.queueFamilyIndex = graphics;
    qci.queueCount = 1;
    qci.pQueuePriorities = &priority;
    VkPhysicalDeviceVulkan13Features v13{VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_FEATURES};
    v13.dynamicRendering = VK_TRUE;
    // A vertex stage may write a wider vector than the fragment stage reads (GL allows it; 2 programs
    // do it, pp_fxaa_nx among them); Vulkan needs maintenance4 for that, core since 1.3. The shim's
    // device needs it too (port/nvn/README.md).
    v13.maintenance4 = VK_TRUE;
    VkDeviceCreateInfo dci{VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO};
    dci.pNext = &v13;
    dci.queueCreateInfoCount = 1;
    dci.pQueueCreateInfos = &qci;
    VK_OK(vkCreateDevice(physical_, &dci, nullptr, &device_), "vkCreateDevice");
    std::printf("device: %s, Vulkan %u.%u.%u, validation %s\n", props_.deviceName, VK_API_VERSION_MAJOR(props_.apiVersion),
                VK_API_VERSION_MINOR(props_.apiVersion), VK_API_VERSION_PATCH(props_.apiVersion), validation ? "on" : "off");
    return true;
}

void Checker::SizeFor(const ShaderManifest& manifest) {
    for (const PackageInfo& p : manifest.packages())
        for (const ProgramInfo& q : p.programs)
            for (const StageInfo& s : q.stages)
                for (const StageResource& b : s.blocks)
                    if (b.size_or_count > max_block_)
                        max_block_ = b.size_or_count;
    const VkDeviceSize align = props_.limits.minUniformBufferOffsetAlignment ? props_.limits.minUniformBufferOffsetAlignment : 256;
    stride_ = (max_block_ + align - 1) / align * align;
    uniform_size_ = stride_ * kUniformBufferSlots * 3;
}

bool Checker::Buffer(VkDeviceSize size, VkBuffer* buffer, VkDeviceMemory* memory) {
    VkBufferCreateInfo bci{VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO};
    bci.size = size;
    bci.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
    VK_OK(vkCreateBuffer(device_, &bci, nullptr, buffer), "vkCreateBuffer");
    VkMemoryRequirements req;
    vkGetBufferMemoryRequirements(device_, *buffer, &req);
    VkMemoryAllocateInfo mai{VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO};
    mai.allocationSize = req.size;
    mai.memoryTypeIndex = MemoryType(req.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
    VK_OK(vkAllocateMemory(device_, &mai, nullptr, memory), "vkAllocateMemory");
    VK_OK(vkBindBufferMemory(device_, *buffer, *memory, 0), "vkBindBufferMemory");
    return true;
}

bool Checker::CreateResources() {
    if (max_block_ > props_.limits.maxUniformBufferRange) {
        std::fprintf(stderr, "largest block %llu bytes > maxUniformBufferRange %u\n",
                     static_cast<unsigned long long>(max_block_), props_.limits.maxUniformBufferRange);
        return false;
    }
    if (!Buffer(uniform_size_, &uniform_, &uniform_memory_) || !Buffer(max_block_, &placeholder_, &placeholder_memory_))
        return false;
    VkImageCreateInfo ici{VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO};
    ici.imageType = VK_IMAGE_TYPE_2D;
    ici.format = VK_FORMAT_R8G8B8A8_UNORM;
    ici.extent = {1, 1, 1};
    ici.mipLevels = 1;
    ici.arrayLayers = 1;
    ici.samples = VK_SAMPLE_COUNT_1_BIT;
    ici.tiling = VK_IMAGE_TILING_OPTIMAL;
    ici.usage = VK_IMAGE_USAGE_SAMPLED_BIT;
    VK_OK(vkCreateImage(device_, &ici, nullptr, &image_), "vkCreateImage");
    VkMemoryRequirements req;
    vkGetImageMemoryRequirements(device_, image_, &req);
    VkMemoryAllocateInfo mai{VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO};
    mai.allocationSize = req.size;
    mai.memoryTypeIndex = MemoryType(req.memoryTypeBits, 0);
    VK_OK(vkAllocateMemory(device_, &mai, nullptr, &image_memory_), "vkAllocateMemory (image)");
    VK_OK(vkBindImageMemory(device_, image_, image_memory_, 0), "vkBindImageMemory");
    VkImageViewCreateInfo vci{VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO};
    vci.image = image_;
    vci.viewType = VK_IMAGE_VIEW_TYPE_2D;
    vci.format = ici.format;
    vci.subresourceRange = {VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1};
    VK_OK(vkCreateImageView(device_, &vci, nullptr, &view_), "vkCreateImageView");
    VkSamplerCreateInfo sci{VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO};
    sci.magFilter = sci.minFilter = VK_FILTER_LINEAR;
    sci.maxLod = 1.0f;
    VK_OK(vkCreateSampler(device_, &sci, nullptr, &sampler_), "vkCreateSampler");
    return true;
}

void Checker::CheckProgram(const ShaderManifest& manifest, const PackageInfo& pkg, const ProgramInfo& prog) {
    const std::string where = pkg.name + "/p" + std::to_string(prog.key);
    g_current = where;
    std::vector<VkShaderModule> mods;
    std::vector<VkDescriptorSetLayout> layouts;
    std::vector<SpirvScan> scans;
    VkPipelineLayout pipeline_layout = VK_NULL_HANDLE;
    VkDescriptorPool pool = VK_NULL_HANDLE;
    const StageInfo* stages[2] = {nullptr, nullptr};  // vertex, fragment
    for (const StageInfo& s : prog.stages)
        stages[s.stage == Stage::Fragment ? 1 : 0] = &s;
    if (!stages[0] || !stages[1]) {
        Fail(where, "a program without both stages");
        return;
    }
    std::vector<unsigned char> spv;
    std::uint32_t uniform_count = 0, sampler_count = 0;
    for (const StageInfo* s : stages) {
        SpirvScan scan;
        if (!ReadFile(manifest.dir() + "/" + s->spirv_path, &spv) || !ScanSpirv(spv, &scan)) {
            Fail(where, "cannot read " + s->spirv_path);
            break;
        }
        scans.push_back(scan);
        VkShaderModuleCreateInfo mci{VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO};
        mci.codeSize = spv.size();
        mci.pCode = reinterpret_cast<const std::uint32_t*>(spv.data());
        VkShaderModule m;
        if (vkCreateShaderModule(device_, &mci, nullptr, &m) != VK_SUCCESS) {
            Fail(where, "vkCreateShaderModule");
            break;
        }
        mods.push_back(m);
        ++modules;
        const std::vector<VkDescriptorSetLayoutBinding> bindings = StageLayoutBindings(*s);
        for (const VkDescriptorSetLayoutBinding& b : bindings)
            (b.descriptorType == VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER ? uniform_count : sampler_count) += b.descriptorCount;
        VkDescriptorSetLayoutCreateInfo lci{VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO};
        lci.bindingCount = static_cast<std::uint32_t>(bindings.size());
        lci.pBindings = bindings.data();
        VkDescriptorSetLayout l;
        if (vkCreateDescriptorSetLayout(device_, &lci, nullptr, &l) != VK_SUCCESS) {
            Fail(where, "vkCreateDescriptorSetLayout");
            break;
        }
        layouts.push_back(l);
        ++set_layouts;
    }
    if (layouts.size() == 2 && DescriptorSetIndex(manifest, Stage::Vertex) == 0 &&
        DescriptorSetIndex(manifest, Stage::Fragment) == 1) {
        VkPipelineLayoutCreateInfo pci{VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO};
        pci.setLayoutCount = 2;
        pci.pSetLayouts = layouts.data();
        if (vkCreatePipelineLayout(device_, &pci, nullptr, &pipeline_layout) != VK_SUCCESS)
            Fail(where, "vkCreatePipelineLayout");
    } else if (layouts.size() == 2) {
        Fail(where, "the manifest's set indices are not vertex 0, fragment 1");
    }

    // Descriptor sets filled through the binding layer, every NVN slot bound.
    if (pipeline_layout) {
        VkDescriptorPoolSize sizes[2] = {{VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, uniform_count ? uniform_count : 1},
                                         {VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, sampler_count ? sampler_count : 1}};
        VkDescriptorPoolCreateInfo dpi{VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO};
        dpi.maxSets = 2;
        dpi.poolSizeCount = 2;
        dpi.pPoolSizes = sizes;
        VkDescriptorSet sets_[2];
        VkDescriptorSetAllocateInfo dai{VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO};
        dai.descriptorSetCount = 2;
        dai.pSetLayouts = layouts.data();
        if (vkCreateDescriptorPool(device_, &dpi, nullptr, &pool) != VK_SUCCESS ||
            (dai.descriptorPool = pool, vkAllocateDescriptorSets(device_, &dai, sets_) != VK_SUCCESS)) {
            Fail(where, "descriptor pool or sets");
        } else {
            for (int i = 0; i < 2; ++i) {
                const StageInfo& s = *stages[i];
                BindingState state;
                for (const StageResource& b : s.blocks)
                    if (b.nvn_slot >= 0)
                        state.BindUniformBuffer(s.stage, static_cast<std::uint32_t>(b.nvn_slot),
                                                SlotAddress(s.stage, static_cast<std::uint32_t>(b.nvn_slot)), b.size_or_count);
                for (std::uint32_t t = 0; t < kTextureSlots; ++t)
                    state.BindTexture(s.stage, t, MakeTextureHandle(t, t));
                DescriptorWrites out;
                state.BuildWrites(s, sets_[i], *this, &out);
                if (!out.warnings.empty())
                    Fail(where, "binding layer: " + out.warnings[0]);
                vkUpdateDescriptorSets(device_, static_cast<std::uint32_t>(out.writes.size()), out.writes.data(), 0, nullptr);
                ++sets;
            }
        }
    }

    // Pipelines: the full one and the depth-only pass's vertex-only one.
    if (pipeline_layout && mods.size() == 2 && scans.size() == 2) {
        std::vector<VkVertexInputBindingDescription> vbind;
        std::vector<VkVertexInputAttributeDescription> vattr;
        for (const InterfaceVar& v : scans[0].inputs)
            for (std::uint32_t l = 0; l < v.locations; ++l) {
                const std::uint32_t b = static_cast<std::uint32_t>(vbind.size());
                vbind.push_back({b, 64, VK_VERTEX_INPUT_RATE_VERTEX});
                vattr.push_back({v.location + l, b, FormatFor(v), 0});
            }
        std::uint32_t attachments = 0;
        for (const InterfaceVar& v : scans[1].outputs)
            attachments = std::max(attachments, v.location + v.locations);
        std::vector<VkFormat> formats(attachments, VK_FORMAT_UNDEFINED);
        for (const InterfaceVar& v : scans[1].outputs)
            for (std::uint32_t l = 0; l < v.locations; ++l)
                formats[v.location + l] = AttachmentFor(v);
        std::vector<VkPipelineColorBlendAttachmentState> blend(attachments);
        for (auto& b : blend)
            b.colorWriteMask = 0xF;

        VkPipelineShaderStageCreateInfo ssi[2]{};
        for (int i = 0; i < 2; ++i) {
            ssi[i].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
            ssi[i].stage = i == 0 ? VK_SHADER_STAGE_VERTEX_BIT : VK_SHADER_STAGE_FRAGMENT_BIT;
            ssi[i].module = mods[static_cast<std::size_t>(i)];
            ssi[i].pName = "main";
        }
        VkPipelineVertexInputStateCreateInfo vis{VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO};
        vis.vertexBindingDescriptionCount = static_cast<std::uint32_t>(vbind.size());
        vis.pVertexBindingDescriptions = vbind.data();
        vis.vertexAttributeDescriptionCount = static_cast<std::uint32_t>(vattr.size());
        vis.pVertexAttributeDescriptions = vattr.data();
        VkPipelineInputAssemblyStateCreateInfo ias{VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO};
        ias.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
        VkPipelineViewportStateCreateInfo vps{VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO};
        vps.viewportCount = vps.scissorCount = 1;
        VkPipelineRasterizationStateCreateInfo rs{VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO};
        rs.polygonMode = VK_POLYGON_MODE_FILL;
        rs.cullMode = VK_CULL_MODE_NONE;
        rs.lineWidth = 1.0f;
        VkPipelineMultisampleStateCreateInfo ms{VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO};
        ms.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
        VkPipelineDepthStencilStateCreateInfo ds{VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO};
        ds.depthTestEnable = ds.depthWriteEnable = VK_TRUE;
        ds.depthCompareOp = VK_COMPARE_OP_LESS_OR_EQUAL;
        VkPipelineColorBlendStateCreateInfo cbs{VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO};
        cbs.attachmentCount = attachments;
        cbs.pAttachments = blend.data();
        const VkDynamicState dyn[2] = {VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR};
        VkPipelineDynamicStateCreateInfo dys{VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO};
        dys.dynamicStateCount = 2;
        dys.pDynamicStates = dyn;
        VkPipelineRenderingCreateInfo rci{VK_STRUCTURE_TYPE_PIPELINE_RENDERING_CREATE_INFO};
        rci.colorAttachmentCount = attachments;
        rci.pColorAttachmentFormats = formats.data();
        rci.depthAttachmentFormat = VK_FORMAT_D32_SFLOAT;
        VkGraphicsPipelineCreateInfo gci{VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO};
        gci.pNext = &rci;
        gci.stageCount = 2;
        gci.pStages = ssi;
        gci.pVertexInputState = &vis;
        gci.pInputAssemblyState = &ias;
        gci.pViewportState = &vps;
        gci.pRasterizationState = &rs;
        gci.pMultisampleState = &ms;
        gci.pDepthStencilState = &ds;
        gci.pColorBlendState = &cbs;
        gci.pDynamicState = &dys;
        gci.layout = pipeline_layout;
        VkPipeline pipeline;
        if (vkCreateGraphicsPipelines(device_, VK_NULL_HANDLE, 1, &gci, nullptr, &pipeline) != VK_SUCCESS) {
            Fail(where, "vkCreateGraphicsPipelines");
        } else {
            ++pipelines;
            vkDestroyPipeline(device_, pipeline, nullptr);
        }
        // Depth only: the vertex stage, no colour attachments, the same layout.
        gci.stageCount = 1;
        rci.colorAttachmentCount = 0;
        cbs.attachmentCount = 0;
        if (vkCreateGraphicsPipelines(device_, VK_NULL_HANDLE, 1, &gci, nullptr, &pipeline) != VK_SUCCESS) {
            Fail(where, "vkCreateGraphicsPipelines (vertex only)");
        } else {
            ++vertex_only;
            vkDestroyPipeline(device_, pipeline, nullptr);
        }
    }

    if (pool)
        vkDestroyDescriptorPool(device_, pool, nullptr);
    if (pipeline_layout)
        vkDestroyPipelineLayout(device_, pipeline_layout, nullptr);
    for (VkDescriptorSetLayout l : layouts)
        vkDestroyDescriptorSetLayout(device_, l, nullptr);
    for (VkShaderModule m : mods)
        vkDestroyShaderModule(device_, m, nullptr);
}

void Checker::Shutdown() {
    if (device_) {
        vkDestroySampler(device_, sampler_, nullptr);
        vkDestroyImageView(device_, view_, nullptr);
        vkDestroyImage(device_, image_, nullptr);
        vkFreeMemory(device_, image_memory_, nullptr);
        vkDestroyBuffer(device_, placeholder_, nullptr);
        vkFreeMemory(device_, placeholder_memory_, nullptr);
        vkDestroyBuffer(device_, uniform_, nullptr);
        vkFreeMemory(device_, uniform_memory_, nullptr);
        vkDestroyDevice(device_, nullptr);
    }
    if (messenger_) {
        auto destroy = reinterpret_cast<PFN_vkDestroyDebugUtilsMessengerEXT>(
            vkGetInstanceProcAddr(instance_, "vkDestroyDebugUtilsMessengerEXT"));
        if (destroy)
            destroy(instance_, messenger_, nullptr);
    }
    if (instance_)
        vkDestroyInstance(instance_, nullptr);
}

void Report(const char* what, const std::map<std::string, Message>& table) {
    int total = 0;
    for (const auto& kv : table)
        total += kv.second.count;
    std::printf("validation %s: %d (%zu kinds)\n", what, total, table.size());
    for (const auto& kv : table)
    {
        std::string progs;
        for (const std::string& q : kv.second.programs)
            progs += (progs.empty() ? "" : ", ") + q;
        std::printf("  %6d  %s (%s%s)\n          %s\n", kv.second.count, kv.first.c_str(), progs.c_str(),
                    kv.second.programs.size() == 8 ? ", ..." : "", kv.second.example.c_str());
    }
}

}  // namespace

int main(int argc, char** argv) {
    bool validation = true;
    std::string dir;
    for (int i = 1; i < argc; ++i) {
        if (std::strcmp(argv[i], "--no-validation") == 0)
            validation = false;
        else
            dir = argv[i];
    }
    if (dir.empty()) {
        std::fprintf(stderr, "usage: %s [--no-validation] <shader dir>\n", argv[0]);
        return 2;
    }
    ShaderManifest manifest;
    std::string error;
    if (!manifest.Load(dir, &error)) {
        std::fprintf(stderr, "%s\n", error.c_str());
        return 1;
    }
    Checker checker;
    if (!checker.Init(validation)) {
        checker.Shutdown();
        return 1;
    }
    checker.SizeFor(manifest);
    if (!checker.CreateResources()) {
        checker.Shutdown();
        return 1;
    }
    const auto start = std::chrono::steady_clock::now();
    std::size_t programs = 0;
    for (const PackageInfo& p : manifest.packages())
        for (const ProgramInfo& q : p.programs) {
            checker.CheckProgram(manifest, p, q);
            ++programs;
            std::fflush(stdout);
        }
    checker.Shutdown();
    const double seconds = std::chrono::duration<double>(std::chrono::steady_clock::now() - start).count();
    std::printf("%zu programs: %d shader modules, %d set layouts, %d descriptor sets written, %d pipelines, "
                "%d vertex-only pipelines in %.0f s; %d failures\n",
                programs, checker.modules, checker.set_layouts, checker.sets, checker.pipelines, checker.vertex_only,
                seconds, g_failures);
    Report("errors", g_errors);
    Report("warnings", g_warnings);
    return g_failures || !g_errors.empty() ? 1 : 0;
}
