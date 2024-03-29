#include "Graphics/Renderer/Renderer.h"
#include "RuntimeReflection/Recorder.h"
#include "RuntimeReflection/GeneratedMeta.h"


DUCKVIL_RUNTIME_REFLECTION_RECORD(3)
{
using namespace Duckvil::RuntimeReflection;
using namespace Duckvil;
DUCKVIL_RESOURCE(type_t) _type;
DUCKVIL_RESOURCE(property_t) _property;
DUCKVIL_RESOURCE(constructor_t) _constructor;
DUCKVIL_RESOURCE(destructor_t) _destructor;
DUCKVIL_RESOURCE(function_t) _function;
DUCKVIL_RESOURCE(enum_t) _enum;
DUCKVIL_RESOURCE(enum_element_t) _enumElement;
DUCKVIL_RESOURCE(ntype_t) _ntype;
std::vector<Duckvil::RuntimeReflection::__duckvil_resource_type_t> _recordedTypes;
std::vector<Duckvil::RuntimeReflection::__duckvil_resource_ntype_t> _recordedNTypes;
std::vector<const char*> _namespaces;
{
_namespaces.push_back("Duckvil");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
{
_namespaces.push_back("Graphics");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
{
_namespaces.push_back("Renderer");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
_enum = record_enum<Duckvil::Graphics::Renderer::renderer_op_code>(_data, _ntype, "renderer_op_code");
_enumElement = record_enum_element(_data, _ntype, _enum, Duckvil::Graphics::Renderer::renderer_op_code::renderer_op_code_bind_shader, "renderer_op_code_bind_shader");
_enumElement = record_enum_element(_data, _ntype, _enum, Duckvil::Graphics::Renderer::renderer_op_code::renderer_op_code_bind_texture, "renderer_op_code_bind_texture");
_enumElement = record_enum_element(_data, _ntype, _enum, Duckvil::Graphics::Renderer::renderer_op_code::renderer_op_code_bind_texture_object, "renderer_op_code_bind_texture_object");
_enumElement = record_enum_element(_data, _ntype, _enum, Duckvil::Graphics::Renderer::renderer_op_code::renderer_op_code_bind_framebuffer, "renderer_op_code_bind_framebuffer");
_enumElement = record_enum_element(_data, _ntype, _enum, Duckvil::Graphics::Renderer::renderer_op_code::renderer_op_code_draw, "renderer_op_code_draw");
_enumElement = record_enum_element(_data, _ntype, _enum, Duckvil::Graphics::Renderer::renderer_op_code::renderer_op_code_set_uniform, "renderer_op_code_set_uniform");
_enumElement = record_enum_element(_data, _ntype, _enum, Duckvil::Graphics::Renderer::renderer_op_code::renderer_op_code_clear_color, "renderer_op_code_clear_color");
_enumElement = record_enum_element(_data, _ntype, _enum, Duckvil::Graphics::Renderer::renderer_op_code::renderer_op_code_clear, "renderer_op_code_clear");
_enumElement = record_enum_element(_data, _ntype, _enum, Duckvil::Graphics::Renderer::renderer_op_code::renderer_op_code_clear_attachment, "renderer_op_code_clear_attachment");
_enumElement = record_enum_element(_data, _ntype, _enum, Duckvil::Graphics::Renderer::renderer_op_code::renderer_op_code_viewport, "renderer_op_code_viewport");
_enumElement = record_enum_element(_data, _ntype, _enum, Duckvil::Graphics::Renderer::renderer_op_code::renderer_op_code_read_pixel, "renderer_op_code_read_pixel");
_enumElement = record_enum_element(_data, _ntype, _enum, Duckvil::Graphics::Renderer::renderer_op_code::renderer_op_code_fbo_read_pixel, "renderer_op_code_fbo_read_pixel");
_enumElement = record_enum_element(_data, _ntype, _enum, Duckvil::Graphics::Renderer::renderer_op_code::renderer_op_code_frame_buffer_reader_read, "renderer_op_code_frame_buffer_reader_read");
_enumElement = record_enum_element(_data, _ntype, _enum, Duckvil::Graphics::Renderer::renderer_op_code::renderer_op_code_frame_buffer_reader_update, "renderer_op_code_frame_buffer_reader_update");
_enumElement = record_enum_element(_data, _ntype, _enum, Duckvil::Graphics::Renderer::renderer_op_code::renderer_op_code_none, "renderer_op_code_none");
_enum = record_enum<Duckvil::Graphics::Renderer::uniform_type>(_data, _ntype, "uniform_type");
_enumElement = record_enum_element(_data, _ntype, _enum, Duckvil::Graphics::Renderer::uniform_type::uniform_type_int, "uniform_type_int");
_enumElement = record_enum_element(_data, _ntype, _enum, Duckvil::Graphics::Renderer::uniform_type::uniform_type_float, "uniform_type_float");
_enumElement = record_enum_element(_data, _ntype, _enum, Duckvil::Graphics::Renderer::uniform_type::uniform_type_vec2, "uniform_type_vec2");
_enumElement = record_enum_element(_data, _ntype, _enum, Duckvil::Graphics::Renderer::uniform_type::uniform_type_vec3, "uniform_type_vec3");
_enumElement = record_enum_element(_data, _ntype, _enum, Duckvil::Graphics::Renderer::uniform_type::uniform_type_vec4, "uniform_type_vec4");
_enumElement = record_enum_element(_data, _ntype, _enum, Duckvil::Graphics::Renderer::uniform_type::uniform_type_mat4, "uniform_type_mat4");
_namespaces.push_back("texture_descriptor");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
using namespace Duckvil::Graphics::Renderer;
_type = record_type<Duckvil::Graphics::Renderer::texture_descriptor>(_data, "texture_descriptor");
_recordedTypes.push_back(_type);
record_namespace(_data, _type, "Duckvil");
record_namespace(_data, _type, "Graphics");
record_namespace(_data, _type, "Renderer");
record_meta(_data, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 3);
_property = record_property<GLenum>(_data, _type, offsetof(Duckvil::Graphics::Renderer::texture_descriptor, m_target), "m_target");
_property = record_property<GLfloat>(_data, _type, offsetof(Duckvil::Graphics::Renderer::texture_descriptor, m_filter), "m_filter");
_property = record_property<GLsizei>(_data, _type, offsetof(Duckvil::Graphics::Renderer::texture_descriptor, m_width), "m_width");
_property = record_property<GLsizei>(_data, _type, offsetof(Duckvil::Graphics::Renderer::texture_descriptor, m_height), "m_height");
_property = record_property<void*>(_data, _type, offsetof(Duckvil::Graphics::Renderer::texture_descriptor, m_pData), "m_pData");
_namespaces.pop_back();
_namespaces.push_back("texture_object_descriptor");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
_type = record_type<Duckvil::Graphics::Renderer::texture_object_descriptor>(_data, "texture_object_descriptor");
_recordedTypes.push_back(_type);
record_namespace(_data, _type, "Duckvil");
record_namespace(_data, _type, "Graphics");
record_namespace(_data, _type, "Renderer");
record_meta(_data, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 3);
_property = record_property<GLenum>(_data, _type, offsetof(Duckvil::Graphics::Renderer::texture_object_descriptor, m_target), "m_target");
_property = record_property<GLfloat*>(_data, _type, offsetof(Duckvil::Graphics::Renderer::texture_object_descriptor, m_filter), "m_filter");
_property = record_property<GLsizei>(_data, _type, offsetof(Duckvil::Graphics::Renderer::texture_object_descriptor, m_width), "m_width");
_property = record_property<GLsizei>(_data, _type, offsetof(Duckvil::Graphics::Renderer::texture_object_descriptor, m_height), "m_height");
_property = record_property<void**>(_data, _type, offsetof(Duckvil::Graphics::Renderer::texture_object_descriptor, m_pData), "m_pData");
_property = record_property<uint32_t>(_data, _type, offsetof(Duckvil::Graphics::Renderer::texture_object_descriptor, m_uiCount), "m_uiCount");
_property = record_property<GLint*>(_data, _type, offsetof(Duckvil::Graphics::Renderer::texture_object_descriptor, m_internalFormats), "m_internalFormats");
_property = record_property<GLenum*>(_data, _type, offsetof(Duckvil::Graphics::Renderer::texture_object_descriptor, m_formats), "m_formats");
_property = record_property<GLenum*>(_data, _type, offsetof(Duckvil::Graphics::Renderer::texture_object_descriptor, m_types), "m_types");
_namespaces.pop_back();
_namespaces.push_back("framebuffer_descriptor");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
_type = record_type<Duckvil::Graphics::Renderer::framebuffer_descriptor>(_data, "framebuffer_descriptor");
_recordedTypes.push_back(_type);
record_namespace(_data, _type, "Duckvil");
record_namespace(_data, _type, "Graphics");
record_namespace(_data, _type, "Renderer");
record_meta(_data, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 3);
_property = record_property<GLenum>(_data, _type, offsetof(Duckvil::Graphics::Renderer::framebuffer_descriptor, m_target), "m_target");
_property = record_property<GLenum*>(_data, _type, offsetof(Duckvil::Graphics::Renderer::framebuffer_descriptor, m_aAttachments), "m_aAttachments");
_property = record_property<uint32_t>(_data, _type, offsetof(Duckvil::Graphics::Renderer::framebuffer_descriptor, m_uiCount), "m_uiCount");
_property = record_property<GLuint*>(_data, _type, offsetof(Duckvil::Graphics::Renderer::framebuffer_descriptor, m_aTextures), "m_aTextures");
_property = record_property<GLenum>(_data, _type, offsetof(Duckvil::Graphics::Renderer::framebuffer_descriptor, m_textureTarget), "m_textureTarget");
_property = record_property<GLsizei>(_data, _type, offsetof(Duckvil::Graphics::Renderer::framebuffer_descriptor, m_width), "m_width");
_property = record_property<GLsizei>(_data, _type, offsetof(Duckvil::Graphics::Renderer::framebuffer_descriptor, m_height), "m_height");
_namespaces.pop_back();
_namespaces.push_back("vertex_buffer_object_descriptor");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
_type = record_type<Duckvil::Graphics::Renderer::vertex_buffer_object_descriptor>(_data, "vertex_buffer_object_descriptor");
_recordedTypes.push_back(_type);
record_namespace(_data, _type, "Duckvil");
record_namespace(_data, _type, "Graphics");
record_namespace(_data, _type, "Renderer");
record_meta(_data, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 3);
_property = record_property<uint32_t>(_data, _type, offsetof(Duckvil::Graphics::Renderer::vertex_buffer_object_descriptor, m_uiTypeSize), "m_uiTypeSize");
_property = record_property<void const*>(_data, _type, offsetof(Duckvil::Graphics::Renderer::vertex_buffer_object_descriptor, m_pData), "m_pData");
_property = record_property<uint16_t>(_data, _type, offsetof(Duckvil::Graphics::Renderer::vertex_buffer_object_descriptor, m_usNumber), "m_usNumber");
_property = record_property<GLenum>(_data, _type, offsetof(Duckvil::Graphics::Renderer::vertex_buffer_object_descriptor, m_target), "m_target");
_property = record_property<uint32_t>(_data, _type, offsetof(Duckvil::Graphics::Renderer::vertex_buffer_object_descriptor, m_uiCount), "m_uiCount");
_property = record_property<GLenum>(_data, _type, offsetof(Duckvil::Graphics::Renderer::vertex_buffer_object_descriptor, m_type), "m_type");
_constructor = record_constructor<Duckvil::Graphics::Renderer::vertex_buffer_object_descriptor, GLenum, uint32_t, void const*, uint32_t, uint16_t>(_data, _type);
_namespaces.pop_back();
_namespaces.push_back("vertex_array_object_descriptor");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
_type = record_type<Duckvil::Graphics::Renderer::vertex_array_object_descriptor>(_data, "vertex_array_object_descriptor");
_recordedTypes.push_back(_type);
record_namespace(_data, _type, "Duckvil");
record_namespace(_data, _type, "Graphics");
record_namespace(_data, _type, "Renderer");
record_meta(_data, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 3);
_property = record_property<uint32_t>(_data, _type, offsetof(Duckvil::Graphics::Renderer::vertex_array_object_descriptor, m_uiVBO_Count), "m_uiVBO_Count");
_property = record_property<vertex_buffer_object_descriptor*>(_data, _type, offsetof(Duckvil::Graphics::Renderer::vertex_array_object_descriptor, m_aVBO), "m_aVBO");
_namespaces.pop_back();
_namespaces.push_back("texture_object");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
_type = record_type<Duckvil::Graphics::Renderer::texture_object>(_data, "texture_object");
_recordedTypes.push_back(_type);
record_namespace(_data, _type, "Duckvil");
record_namespace(_data, _type, "Graphics");
record_namespace(_data, _type, "Renderer");
record_meta(_data, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 3);
_property = record_property<GLuint*>(_data, _type, offsetof(Duckvil::Graphics::Renderer::texture_object, m_pTexture), "m_pTexture");
_property = record_property<uint32_t>(_data, _type, offsetof(Duckvil::Graphics::Renderer::texture_object, m_uiTextureCount), "m_uiTextureCount");
_namespaces.pop_back();
_namespaces.push_back("framebuffer");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
_type = record_type<Duckvil::Graphics::Renderer::framebuffer>(_data, "framebuffer");
_recordedTypes.push_back(_type);
record_namespace(_data, _type, "Duckvil");
record_namespace(_data, _type, "Graphics");
record_namespace(_data, _type, "Renderer");
record_meta(_data, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 3);
_property = record_property<GLuint>(_data, _type, offsetof(Duckvil::Graphics::Renderer::framebuffer, m_FBO), "m_FBO");
_namespaces.pop_back();
_namespaces.push_back("vertex_array_object");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
_type = record_type<Duckvil::Graphics::Renderer::vertex_array_object>(_data, "vertex_array_object");
_recordedTypes.push_back(_type);
record_namespace(_data, _type, "Duckvil");
record_namespace(_data, _type, "Graphics");
record_namespace(_data, _type, "Renderer");
record_meta(_data, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 3);
_property = record_property<GLuint>(_data, _type, offsetof(Duckvil::Graphics::Renderer::vertex_array_object, m_vao), "m_vao");
_property = record_property<uint32_t>(_data, _type, offsetof(Duckvil::Graphics::Renderer::vertex_array_object, m_uiDrawCount), "m_uiDrawCount");
_namespaces.pop_back();
_namespaces.push_back("__duckvil_slot_array_shader");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
_type = record_type<Duckvil::Graphics::Renderer::__duckvil_slot_array_shader>(_data, "__duckvil_slot_array_shader");
_recordedTypes.push_back(_type);
record_namespace(_data, _type, "Duckvil");
record_namespace(_data, _type, "Graphics");
record_namespace(_data, _type, "Renderer");
record_meta(_data, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 3);
_property = record_property<uint32_t*>(_data, _type, offsetof(Duckvil::Graphics::Renderer::__duckvil_slot_array_shader, m_indices), "m_indices");
_property = record_property<shader*>(_data, _type, offsetof(Duckvil::Graphics::Renderer::__duckvil_slot_array_shader, m_data), "m_data");
_namespaces.pop_back();
_namespaces.push_back("__duckvil_slot_array_texture");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
_type = record_type<Duckvil::Graphics::Renderer::__duckvil_slot_array_texture>(_data, "__duckvil_slot_array_texture");
_recordedTypes.push_back(_type);
record_namespace(_data, _type, "Duckvil");
record_namespace(_data, _type, "Graphics");
record_namespace(_data, _type, "Renderer");
record_meta(_data, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 3);
_property = record_property<uint32_t*>(_data, _type, offsetof(Duckvil::Graphics::Renderer::__duckvil_slot_array_texture, m_indices), "m_indices");
_property = record_property<texture*>(_data, _type, offsetof(Duckvil::Graphics::Renderer::__duckvil_slot_array_texture, m_data), "m_data");
_namespaces.pop_back();
_namespaces.push_back("__duckvil_slot_array_texture_object");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
_type = record_type<Duckvil::Graphics::Renderer::__duckvil_slot_array_texture_object>(_data, "__duckvil_slot_array_texture_object");
_recordedTypes.push_back(_type);
record_namespace(_data, _type, "Duckvil");
record_namespace(_data, _type, "Graphics");
record_namespace(_data, _type, "Renderer");
record_meta(_data, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 3);
_property = record_property<uint32_t*>(_data, _type, offsetof(Duckvil::Graphics::Renderer::__duckvil_slot_array_texture_object, m_indices), "m_indices");
_property = record_property<texture_object*>(_data, _type, offsetof(Duckvil::Graphics::Renderer::__duckvil_slot_array_texture_object, m_data), "m_data");
_namespaces.pop_back();
_namespaces.push_back("__duckvil_slot_array_framebuffer");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
_type = record_type<Duckvil::Graphics::Renderer::__duckvil_slot_array_framebuffer>(_data, "__duckvil_slot_array_framebuffer");
_recordedTypes.push_back(_type);
record_namespace(_data, _type, "Duckvil");
record_namespace(_data, _type, "Graphics");
record_namespace(_data, _type, "Renderer");
record_meta(_data, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 3);
_property = record_property<uint32_t*>(_data, _type, offsetof(Duckvil::Graphics::Renderer::__duckvil_slot_array_framebuffer, m_indices), "m_indices");
_property = record_property<framebuffer*>(_data, _type, offsetof(Duckvil::Graphics::Renderer::__duckvil_slot_array_framebuffer, m_data), "m_data");
_namespaces.pop_back();
_namespaces.push_back("__duckvil_slot_array_vertex_array_object");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
_type = record_type<Duckvil::Graphics::Renderer::__duckvil_slot_array_vertex_array_object>(_data, "__duckvil_slot_array_vertex_array_object");
_recordedTypes.push_back(_type);
record_namespace(_data, _type, "Duckvil");
record_namespace(_data, _type, "Graphics");
record_namespace(_data, _type, "Renderer");
record_meta(_data, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 3);
_property = record_property<uint32_t*>(_data, _type, offsetof(Duckvil::Graphics::Renderer::__duckvil_slot_array_vertex_array_object, m_indices), "m_indices");
_property = record_property<vertex_array_object*>(_data, _type, offsetof(Duckvil::Graphics::Renderer::__duckvil_slot_array_vertex_array_object, m_data), "m_data");
_namespaces.pop_back();
_namespaces.push_back("__duckvil_slot_array_uniform");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
_type = record_type<Duckvil::Graphics::Renderer::__duckvil_slot_array_uniform>(_data, "__duckvil_slot_array_uniform");
_recordedTypes.push_back(_type);
record_namespace(_data, _type, "Duckvil");
record_namespace(_data, _type, "Graphics");
record_namespace(_data, _type, "Renderer");
record_meta(_data, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 3);
_property = record_property<uint32_t*>(_data, _type, offsetof(Duckvil::Graphics::Renderer::__duckvil_slot_array_uniform, m_indices), "m_indices");
_property = record_property<uniform*>(_data, _type, offsetof(Duckvil::Graphics::Renderer::__duckvil_slot_array_uniform, m_data), "m_data");
_namespaces.pop_back();
_namespaces.push_back("__duckvil_slot_array_frame_buffer_reader");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
_type = record_type<Duckvil::Graphics::Renderer::__duckvil_slot_array_frame_buffer_reader>(_data, "__duckvil_slot_array_frame_buffer_reader");
_recordedTypes.push_back(_type);
record_namespace(_data, _type, "Duckvil");
record_namespace(_data, _type, "Graphics");
record_namespace(_data, _type, "Renderer");
record_meta(_data, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 3);
_property = record_property<uint32_t*>(_data, _type, offsetof(Duckvil::Graphics::Renderer::__duckvil_slot_array_frame_buffer_reader, m_indices), "m_indices");
_property = record_property<frame_buffer_reader*>(_data, _type, offsetof(Duckvil::Graphics::Renderer::__duckvil_slot_array_frame_buffer_reader, m_data), "m_data");
_namespaces.pop_back();
_namespaces.push_back("renderer_data");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
_type = record_type<Duckvil::Graphics::Renderer::renderer_data>(_data, "renderer_data");
_recordedTypes.push_back(_type);
record_namespace(_data, _type, "Duckvil");
record_namespace(_data, _type, "Graphics");
record_namespace(_data, _type, "Renderer");
record_meta(_data, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 3);
_property = record_property<Duckvil::Graphics::Renderer::__duckvil_slot_array_shader>(_data, _type, offsetof(Duckvil::Graphics::Renderer::renderer_data, m_shader), "m_shader");
_property = record_property<Duckvil::Graphics::Renderer::__duckvil_slot_array_texture>(_data, _type, offsetof(Duckvil::Graphics::Renderer::renderer_data, m_texture), "m_texture");
_property = record_property<Duckvil::Graphics::Renderer::__duckvil_slot_array_texture_object>(_data, _type, offsetof(Duckvil::Graphics::Renderer::renderer_data, m_textureObject), "m_textureObject");
_property = record_property<Duckvil::Graphics::Renderer::__duckvil_slot_array_framebuffer>(_data, _type, offsetof(Duckvil::Graphics::Renderer::renderer_data, m_fbo), "m_fbo");
_property = record_property<Duckvil::Graphics::Renderer::__duckvil_slot_array_vertex_array_object>(_data, _type, offsetof(Duckvil::Graphics::Renderer::renderer_data, m_vao), "m_vao");
_property = record_property<Duckvil::Graphics::Renderer::__duckvil_slot_array_uniform>(_data, _type, offsetof(Duckvil::Graphics::Renderer::renderer_data, m_uniform), "m_uniform");
_property = record_property<Duckvil::Graphics::Renderer::__duckvil_slot_array_frame_buffer_reader>(_data, _type, offsetof(Duckvil::Graphics::Renderer::renderer_data, m_frameBufferReader), "m_frameBufferReader");
_property = record_property<command_buffer>(_data, _type, offsetof(Duckvil::Graphics::Renderer::renderer_data, m_pCommandBuffer), "m_pCommandBuffer");
_property = record_property<Memory::free_list_allocator*>(_data, _type, offsetof(Duckvil::Graphics::Renderer::renderer_data, m_pAllocator), "m_pAllocator");
_property = record_property<flecs::world*>(_data, _type, offsetof(Duckvil::Graphics::Renderer::renderer_data, m_ecs), "m_ecs");
_namespaces.pop_back();
_namespaces.push_back("renderer_ftable");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
_type = record_type<Duckvil::Graphics::Renderer::renderer_ftable>(_data, "renderer_ftable");
_recordedTypes.push_back(_type);
record_namespace(_data, _type, "Duckvil");
record_namespace(_data, _type, "Graphics");
record_namespace(_data, _type, "Renderer");
record_meta(_data, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 3);
_property = record_property<bool(*)(Memory::ftable const*,Memory::free_list_allocator*,Window::IWindow*,renderer_data*)>(_data, _type, offsetof(Duckvil::Graphics::Renderer::renderer_ftable, m_fnInit), "m_fnInit");
_property = record_property<void(*)(Memory::ftable const*,renderer_data*)>(_data, _type, offsetof(Duckvil::Graphics::Renderer::renderer_ftable, m_fnUpdate), "m_fnUpdate");
_property = record_property<void(*)(Memory::ftable const*,renderer_data*)>(_data, _type, offsetof(Duckvil::Graphics::Renderer::renderer_ftable, m_fnSubmitCommandBuffer), "m_fnSubmitCommandBuffer");
_property = record_property<void(*)()>(_data, _type, offsetof(Duckvil::Graphics::Renderer::renderer_ftable, m_fnBindAsRenderTarget), "m_fnBindAsRenderTarget");
_property = record_property<uint32_t(*)(Memory::ftable const*,Memory::free_list_allocator*,renderer_data*,char const*,char const*)>(_data, _type, offsetof(Duckvil::Graphics::Renderer::renderer_ftable, m_fnCreateShader), "m_fnCreateShader");
_property = record_property<uint32_t(*)(Memory::ftable const*,Memory::free_list_allocator*,renderer_data*,uint32_t,uint32_t,frame_buffer_reader::Format,frame_buffer_reader::Type)>(_data, _type, offsetof(Duckvil::Graphics::Renderer::renderer_ftable, m_fnCreateFrameBufferReader), "m_fnCreateFrameBufferReader");
_property = record_property<uint32_t(*)(Memory::ftable const*,Memory::free_list_allocator*,renderer_data*,texture_descriptor const&)>(_data, _type, offsetof(Duckvil::Graphics::Renderer::renderer_ftable, m_fnCreateTexture), "m_fnCreateTexture");
_property = record_property<uint32_t(*)(Memory::ftable const*,Memory::free_list_allocator*,renderer_data*,texture_object_descriptor const&)>(_data, _type, offsetof(Duckvil::Graphics::Renderer::renderer_ftable, m_fnCreateTextureObject), "m_fnCreateTextureObject");
_property = record_property<uint32_t(*)(Memory::ftable const*,Memory::free_list_allocator*,renderer_data*,framebuffer_descriptor const&)>(_data, _type, offsetof(Duckvil::Graphics::Renderer::renderer_ftable, m_fnCreateFramebuffer), "m_fnCreateFramebuffer");
_property = record_property<uint32_t(*)(Memory::ftable const*,Memory::free_list_allocator*,renderer_data*,vertex_array_object_descriptor const&)>(_data, _type, offsetof(Duckvil::Graphics::Renderer::renderer_ftable, m_fnCreateVAO), "m_fnCreateVAO");
_property = record_property<void(*)(Memory::ftable const*,Memory::free_list_allocator*,renderer_data*,uint32_t)>(_data, _type, offsetof(Duckvil::Graphics::Renderer::renderer_ftable, m_fnDestroyTexture), "m_fnDestroyTexture");
_property = record_property<void(*)(Memory::ftable const*,Memory::free_list_allocator*,renderer_data*,uint32_t)>(_data, _type, offsetof(Duckvil::Graphics::Renderer::renderer_ftable, m_fnDestroyFramebuffer), "m_fnDestroyFramebuffer");
_property = record_property<uint32_t(*)(Memory::ftable const*,Memory::free_list_allocator*,renderer_data*,uint32_t,char const*)>(_data, _type, offsetof(Duckvil::Graphics::Renderer::renderer_ftable, m_fnGetUniformLocation), "m_fnGetUniformLocation");
_property = record_property<void*(*)(renderer_data*,uint32_t)>(_data, _type, offsetof(Duckvil::Graphics::Renderer::renderer_ftable, m_fnGetTexture), "m_fnGetTexture");
_property = record_property<GLuint*(*)(renderer_data*,uint32_t)>(_data, _type, offsetof(Duckvil::Graphics::Renderer::renderer_ftable, m_fnGetTextures), "m_fnGetTextures");
_namespaces.pop_back();
_namespaces.pop_back();
}
_namespaces.pop_back();
}
_namespaces.pop_back();
}
return duckvil_recorded_types_create(_data._pMemoryInterface, _data._pAllocator, _recordedTypes, "Graphics/Renderer/Renderer.h", 3);
}
#ifdef DUCKVIL_RUNTIME_COMPILE
DUCKVIL_EXPORT uint32_t duckvil_get_recorder_index()
{
return 3;
}
#endif
