#include "windows.h"
#include <string>
#include <iostream>
#include <fstream>
#include <cstdio>

#pragma warning (disable : 4996)


using namespace std;

static HINSTANCE hinstLib = 0;


void load()
{
    char sysdir [4096*8];

    GetSystemDirectory(sysdir, 4096);
    string opencl_dll = string(sysdir) + "\\OpenCL.dll";

    if (hinstLib == 0)
        hinstLib = LoadLibrary(opencl_dll.c_str());

    cout << (hinstLib == 0 ? " Falied to load: " : "Hooked: ") << opencl_dll << endl;
}


void dump(const char *source, size_t length)
{
    static int i = 0;
    char buf[4096];
    sprintf(buf, "opencl_source_%03d.txt", i++);

    cout << ">>> writing " << buf << endl;

    ofstream ofs(buf);
    ofs << string(source, source + length);
};

void dumpbin(const unsigned char* binary, size_t length)
{
    static int i = 0;
    char buf[4096];
    sprintf(buf, "opencl_binary_%03d.txt", i++);

    cout << ">>> writing " << buf << endl;

    FILE *f = fopen(buf, "w+b");
    fwrite(binary, 1, length, f);
    fclose(f);
}

BOOL APIENTRY DllMain( HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH: load(); break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

typedef unsigned __int32  cl_uint;
typedef signed __int8   cl_char;
typedef unsigned __int8   cl_uchar;
typedef signed __int16  cl_short;
typedef unsigned __int16  cl_ushort;
typedef signed __int32  cl_int;
typedef unsigned __int32  cl_uint;
typedef signed __int64  cl_long;
typedef unsigned __int64  cl_ulong;

typedef unsigned __int16  cl_half;
typedef float   cl_float;
typedef double  cl_double;


/******************************************************************************/

typedef struct _cl_platform_id* cl_platform_id;
typedef struct _cl_device_id* cl_device_id;
typedef struct _cl_context* cl_context;
typedef struct _cl_command_queue* cl_command_queue;
typedef struct _cl_mem* cl_mem;
typedef struct _cl_program* cl_program;
typedef struct _cl_kernel* cl_kernel;
typedef struct _cl_event* cl_event;
typedef struct _cl_sampler* cl_sampler;

typedef cl_uint cl_bool;
typedef cl_ulong cl_bitfield;
typedef cl_bitfield cl_device_type;
typedef cl_uint cl_platform_info;
typedef cl_uint cl_device_info;
typedef cl_bitfield cl_device_fp_config;
typedef cl_uint cl_device_mem_cache_type;
typedef cl_uint cl_device_local_mem_type;
typedef cl_bitfield cl_device_exec_capabilities;
typedef cl_bitfield cl_command_queue_properties;

typedef intptr_t cl_context_properties;
typedef cl_uint cl_context_info;
typedef cl_uint cl_command_queue_info;
typedef cl_uint cl_channel_order;
typedef cl_uint cl_channel_type;
typedef cl_bitfield cl_mem_flags;
typedef cl_uint cl_mem_object_type;
typedef cl_uint cl_mem_info;
typedef cl_uint cl_image_info;
typedef cl_uint cl_buffer_create_type;
typedef cl_uint cl_addressing_mode;
typedef cl_uint cl_filter_mode;
typedef cl_uint cl_sampler_info;
typedef cl_bitfield cl_map_flags;
typedef cl_uint cl_program_info;
typedef cl_uint cl_program_build_info;
typedef cl_int cl_build_status;
typedef cl_uint cl_kernel_info;
typedef cl_uint cl_kernel_work_group_info;
typedef cl_uint cl_event_info;
typedef cl_uint cl_command_type;
typedef cl_uint cl_profiling_info;

typedef cl_uint cl_gl_object_type;
typedef cl_uint cl_gl_texture_info;
typedef cl_uint cl_gl_platform_info;
typedef struct __GLsync *cl_GLsync;
typedef unsigned int cl_GLuint;
typedef int  cl_GLint;
typedef unsigned int cl_GLenum;
typedef cl_uint cl_gl_context_info;


typedef struct _cl_image_format {
    cl_channel_order image_channel_order;
    cl_channel_type image_channel_data_type;
} cl_image_format;


typedef struct _cl_buffer_region {
    size_t origin;
    size_t size;
} cl_buffer_region;
//
/********************************************************************************************************/

#define EXPT(ret) extern "C" ret __stdcall
//#define EXPT extern "C" __declspec(dllexport)

//Platform API
EXPT(cl_int) clGetPlatformIDs(cl_uint num_entries, cl_platform_id* platforms, cl_uint* num_platforms)
{
    typedef cl_int (__stdcall *Type)(cl_uint num_entries, cl_platform_id* platforms, cl_uint* num_platforms);
    Type proc = (Type)GetProcAddress(hinstLib, "clGetPlatformIDs");
    return proc(num_entries, platforms, num_platforms);
}

EXPT(cl_int) clGetPlatformInfo(cl_platform_id platform, cl_platform_info param_name, size_t param_value_size, void* param_value, size_t* param_value_size_ret)
{
    typedef cl_int (__stdcall *Type)(cl_platform_id platform, cl_platform_info param_name, size_t param_value_size, void* param_value, size_t* param_value_size_ret);

    Type proc = (Type)GetProcAddress(hinstLib, "clGetPlatformInfo");
    return proc(platform, param_name, param_value_size, param_value, param_value_size_ret);
}


//Device APIs
EXPT(cl_int) clGetDeviceIDs(cl_platform_id platform, cl_device_type device_type, cl_uint num_entries, cl_device_id* devices, cl_uint* num_devices)
{
    typedef cl_int (__stdcall *Type)(cl_platform_id platform, cl_device_type device_type, cl_uint num_entries, cl_device_id* devices, cl_uint* num_devices) ;

    Type proc = (Type)GetProcAddress(hinstLib, "clGetDeviceIDs");
    return proc(platform, device_type, num_entries, devices, num_devices);
}

EXPT(cl_int) clGetDeviceInfo(cl_device_id device, cl_device_info param_name, size_t param_value_size, void* param_value, size_t* param_value_size_ret)
{
    typedef cl_int (__stdcall *Type)(cl_device_id device, cl_device_info param_name, size_t param_value_size, void* param_value, size_t* param_value_size_ret);

    Type proc = (Type)GetProcAddress(hinstLib, "clGetDeviceInfo");
    return proc(device, param_name, param_value_size, param_value, param_value_size_ret);
}

//Context APIs 
EXPT(cl_context) clCreateContext(const cl_context_properties* properties, cl_uint num_devices, const cl_device_id* devices,
                void (__stdcall* pfn_notify)(const char *, const void *, size_t, void *), void* user_data, cl_int* errcode_ret)
{
    typedef cl_context (__stdcall *Type)(const cl_context_properties* properties, cl_uint num_devices, const cl_device_id* devices, 
        void (__stdcall* pfn_notify)(const char *, const void *, size_t, void *), void* user_data, cl_int* errcode_ret);

    Type proc = (Type)GetProcAddress(hinstLib, "clCreateContext");
    return proc(properties, num_devices, devices, pfn_notify, user_data, errcode_ret);
}

EXPT(cl_context) clCreateContextFromType(const cl_context_properties* properties, cl_device_type device_type, void (__stdcall *pfn_notify)(const char *, const void *, size_t, void *),void* user_data, cl_int* errcode_ret)
{
    typedef cl_context (__stdcall *Type)(const cl_context_properties* properties, cl_device_type device_type, void (__stdcall *pfn_notify)(const char *, const void *, size_t, void *), void* user_data, cl_int* errcode_ret);

    Type proc = (Type)GetProcAddress(hinstLib, "clCreateContextFromType");
    return proc(properties, device_type, pfn_notify, user_data, errcode_ret);
}

EXPT(cl_int) clRetainContext(cl_context context)
{
    typedef cl_int (__stdcall *Type)(cl_context context);
    Type proc = (Type)GetProcAddress(hinstLib, "clRetainContext");
    return proc(context);
}


EXPT(cl_int) clReleaseContext(cl_context context)
{
    typedef cl_int (__stdcall *Type)(cl_context context);
    Type proc = (Type)GetProcAddress(hinstLib, "clReleaseContext");
    return proc(context);
}

EXPT(cl_int) clGetContextInfo(cl_context context, cl_context_info param_name, size_t param_value_size, void* param_value, size_t* param_value_size_ret)
{
    typedef cl_int (__stdcall *Type)(cl_context context, cl_context_info param_name, size_t param_value_size, void* param_value, size_t* param_value_size_ret);
    Type proc = (Type)GetProcAddress(hinstLib, "clGetContextInfo");
    return proc(context, param_name, param_value_size, param_value, param_value_size_ret);
}

//Command Queue APIs
EXPT(cl_command_queue) clCreateCommandQueue(cl_context context, cl_device_id device, cl_command_queue_properties properties, cl_int* errcode_ret)
{
    typedef cl_command_queue (__stdcall *Type)(cl_context context, cl_device_id device, cl_command_queue_properties properties, cl_int* errcode_ret);
    Type proc = (Type)GetProcAddress(hinstLib, "clCreateCommandQueue");
    return proc(context, device, properties, errcode_ret);
}

EXPT(cl_int) clRetainCommandQueue(cl_command_queue command_queue)
{
    typedef cl_int (__stdcall *Type)(cl_command_queue command_queue);
    Type proc = (Type)GetProcAddress(hinstLib, "clRetainCommandQueue");
    return proc(command_queue);
}

EXPT(cl_int) clReleaseCommandQueue(cl_command_queue command_queue)
{
    typedef cl_int (__stdcall *Type)(cl_command_queue command_queue);
    Type proc = (Type)GetProcAddress(hinstLib, "clReleaseCommandQueue");
    return proc(command_queue);
}

EXPT(cl_int) clGetCommandQueueInfo(cl_command_queue command_queue, cl_command_queue_info param_name, size_t param_value_size, void* param_value, size_t* param_value_size_ret)
{
    typedef cl_int (__stdcall *Type)(cl_command_queue command_queue, cl_command_queue_info param_name, size_t param_value_size, void* param_value, size_t* param_value_size_ret);
    Type proc = (Type)GetProcAddress(hinstLib, "clGetCommandQueueInfo");
    return proc(command_queue, param_name,param_value_size,  param_value, param_value_size_ret);
}

//Memory Object APIs
EXPT(cl_mem) clCreateBuffer(cl_context context, cl_mem_flags flags, size_t size, void* host_ptr, cl_int* errcode_ret)
{
    typedef cl_mem (__stdcall *Type)(cl_context context, cl_mem_flags flags, size_t size, void* host_ptr, cl_int* errcode_ret);
    Type proc = (Type)GetProcAddress(hinstLib, "clCreateBuffer");
    return proc(context, flags, size, host_ptr, errcode_ret);
}

EXPT(cl_mem) clCreateSubBuffer(cl_mem buffer, cl_mem_flags flags, cl_buffer_create_type buffer_create_type, const void* buffer_create_info, cl_int* errcode_ret)
{
    typedef cl_mem (__stdcall *Type)(cl_mem buffer, cl_mem_flags flags, cl_buffer_create_type buffer_create_type, const void* buffer_create_info, cl_int* errcode_ret);
    Type proc = (Type)GetProcAddress(hinstLib, "clCreateSubBuffer");
    return proc(buffer, flags, buffer_create_type, buffer_create_info, errcode_ret);
}


EXPT(cl_mem) clCreateImage2D(cl_context context, cl_mem_flags flags, const cl_image_format* image_format, size_t image_width, size_t image_height,
                size_t image_row_pitch, void* host_ptr, cl_int* errcode_ret)
{
    typedef cl_mem (__stdcall *Type)(cl_context context, cl_mem_flags flags, const cl_image_format* image_format, size_t image_width, size_t image_height,size_t image_row_pitch, void* host_ptr, cl_int* errcode_ret);
    Type proc = (Type)GetProcAddress(hinstLib, "clCreateImage2D");
    return proc(context, flags, image_format, image_width, image_height, image_row_pitch, host_ptr, errcode_ret);
}


EXPT(cl_mem) clCreateImage3D(cl_context context, cl_mem_flags flags, const cl_image_format* image_format, size_t image_width, size_t image_height,
                size_t image_depth, size_t image_row_pitch, size_t image_slice_pitch, void* host_ptr, cl_int* errcode_ret)
{
    typedef cl_mem (__stdcall *Type)(cl_context context, cl_mem_flags flags, const cl_image_format* image_format, size_t image_width, size_t image_height, size_t image_depth, size_t image_row_pitch, size_t image_slice_pitch, void* host_ptr, cl_int* errcode_ret);
    Type proc = (Type)GetProcAddress(hinstLib, "clCreateImage3D");
    return proc(context, flags, image_format, image_width, image_height, image_depth, image_row_pitch, image_slice_pitch, host_ptr, errcode_ret);
}

EXPT(cl_int) clRetainMemObject(cl_mem memobj)
{
    typedef cl_int (__stdcall *Type)(cl_mem memobj);
    Type proc = (Type)GetProcAddress(hinstLib, "clRetainMemObject");
    return proc(memobj);
}

EXPT(cl_int) clReleaseMemObject(cl_mem memobj)
{
    typedef cl_int (__stdcall *Type)(cl_mem memobj);
    Type proc = (Type)GetProcAddress(hinstLib, "clReleaseMemObject");
    return proc(memobj);
}

EXPT(cl_int) clGetSupportedImageFormats(cl_context context, cl_mem_flags flags, cl_mem_object_type image_type, cl_uint num_entries, cl_image_format* image_formats, cl_uint* num_image_formats)
{
    typedef cl_int (__stdcall *Type)(cl_context context, cl_mem_flags flags, cl_mem_object_type image_type, cl_uint num_entries, cl_image_format* image_formats, cl_uint* num_image_formats) ;
    Type proc = (Type)GetProcAddress(hinstLib, "clGetSupportedImageFormats");
    return proc(context, flags, image_type, num_entries, image_formats, num_image_formats);
}

EXPT(cl_int) clGetMemObjectInfo(cl_mem memobj, cl_mem_info param_name, size_t param_value_size, void* param_value, size_t* param_value_size_ret)
{
    typedef cl_int (__stdcall *Type)(cl_mem memobj, cl_mem_info param_name, size_t param_value_size, void* param_value, size_t* param_value_size_ret) ;
    Type proc = (Type)GetProcAddress(hinstLib, "clGetMemObjectInfo");
    return proc(memobj, param_name, param_value_size, param_value, param_value_size_ret);
}


EXPT(cl_int) clGetImageInfo(cl_mem image, cl_image_info param_name, size_t param_value_size, void* param_value, size_t* param_value_size_ret)
{
    typedef cl_int (__stdcall *Type)(cl_mem image, cl_image_info param_name, size_t param_value_size, void* param_value, size_t* param_value_size_ret) ;
    Type proc = (Type)GetProcAddress(hinstLib, "clGetImageInfo");
    return proc(image, param_name, param_value_size, param_value, param_value_size_ret);
}


EXPT(cl_int) clSetMemObjectDestructorCallback( cl_mem memobj, void (__stdcall* pfn_notify)( cl_mem memobj, void* user_data), void* user_data)
{
    typedef cl_int (__stdcall *Type)( cl_mem memobj, void (__stdcall* pfn_notify)( cl_mem memobj, void* user_data), void* user_data);
    Type proc = (Type)GetProcAddress(hinstLib, "clSetMemObjectDestructorCallback");
    return proc(memobj, pfn_notify, user_data);
}

//Sampler APIs 
EXPT(cl_sampler) clCreateSampler(cl_context context, cl_bool normalized_coords, cl_addressing_mode addressing_mode, cl_filter_mode filter_mode, cl_int* errcode_ret)
{
    typedef cl_sampler (__stdcall *Type)(cl_context context, cl_bool normalized_coords, cl_addressing_mode addressing_mode, cl_filter_mode filter_mode, cl_int* errcode_ret);
    Type proc = (Type)GetProcAddress(hinstLib, "clCreateSampler");
    return proc(context, normalized_coords, addressing_mode, filter_mode, errcode_ret);
}

EXPT(cl_int) clRetainSampler(cl_sampler sampler)
{
    typedef cl_int (__stdcall *Type)(cl_sampler sampler);
    Type proc = (Type)GetProcAddress(hinstLib, "clRetainSampler");
    return proc(sampler);
}

EXPT(cl_int) clReleaseSampler(cl_sampler sampler)
{
    typedef cl_int (__stdcall *Type)(cl_sampler sampler);
    Type proc = (Type)GetProcAddress(hinstLib, "clReleaseSampler");
    return proc(sampler);
}


EXPT(cl_int) clGetSamplerInfo(cl_sampler sampler, cl_sampler_info param_name, size_t param_value_size, void* param_value, size_t* param_value_size_ret)
{
    typedef cl_int (__stdcall *Type)(cl_sampler sampler, cl_sampler_info param_name, size_t param_value_size, void* param_value, size_t* param_value_size_ret);
    Type proc = (Type)GetProcAddress(hinstLib, "clGetSamplerInfo");
    return proc(sampler, param_name, param_value_size, param_value, param_value_size_ret);
}

//Program Object APIs 
EXPT(cl_program) clCreateProgramWithSource(cl_context context, cl_uint count, const char ** strings, const size_t* lengths, cl_int* errcode_ret)
{
    cout << " Source == " << count << endl;
    for(cl_uint i = 0; i < count; ++i)
        dump(strings[i], lengths[i]);
    typedef cl_program (__stdcall *Type)(cl_context context, cl_uint count, const char ** strings, const size_t* lengths, cl_int* errcode_ret);
    Type proc = (Type)GetProcAddress(hinstLib, "clCreateProgramWithSource");
    return proc(context, count, strings, lengths, errcode_ret);
}

EXPT(cl_program) clCreateProgramWithBinary(cl_context context, cl_uint num_devices, const cl_device_id* device_list, const size_t* lengths, const unsigned char ** binaries, cl_int* binary_status, cl_int* errcode_ret)
{
    cout << " Bianry =  " << num_devices << endl;
    for(cl_uint i = 0; i < num_devices; ++i)
        dumpbin(binaries[i], lengths[i]);

    typedef cl_program (__stdcall *Type)(cl_context context, cl_uint num_devices, const cl_device_id* device_list, const size_t* lengths, const unsigned char ** binaries, cl_int* binary_status, cl_int* errcode_ret);
    Type proc = (Type)GetProcAddress(hinstLib, "clCreateProgramWithBinary");
    return proc(context, num_devices, device_list, lengths, binaries, binary_status, errcode_ret);
}

EXPT(cl_int) clRetainProgram(cl_program program)
{
    typedef cl_int (__stdcall *Type)(cl_program program);
    Type proc = (Type)GetProcAddress(hinstLib, "clRetainProgram");
    return proc(program);
}

EXPT(cl_int) clReleaseProgram(cl_program program)
{
    typedef cl_int (__stdcall *Type)(cl_program program);
    Type proc = (Type)GetProcAddress(hinstLib, "clReleaseProgram");
    return proc(program);
}

EXPT(cl_int) clBuildProgram(cl_program program, cl_uint num_devices, const cl_device_id* device_list, const char* options, void (__stdcall* pfn_notify)(cl_program program, void* user_data), void* user_data)
{
    typedef cl_int (__stdcall *Type)(cl_program program, cl_uint num_devices, const cl_device_id* device_list, const char* options, void (__stdcall* pfn_notify)(cl_program program, void* user_data), void* user_data) ;
    Type proc = (Type)GetProcAddress(hinstLib, "clBuildProgram");
    return proc(program, num_devices, device_list, options, pfn_notify, user_data);
}

EXPT(cl_int) clUnloadCompiler(void)
{
    typedef cl_int (__stdcall *Type)();
    Type proc = (Type)GetProcAddress(hinstLib, "clUnloadCompiler");
    return proc();
}

EXPT(cl_int) clGetProgramInfo(cl_program program, cl_program_info param_name, size_t param_value_size, void* param_value, size_t* param_value_size_ret)
{
    typedef cl_int (__stdcall *Type)(cl_program program, cl_program_info param_name, size_t param_value_size, void* param_value, size_t* param_value_size_ret);
    Type proc = (Type)GetProcAddress(hinstLib, "clGetProgramInfo");
    return proc(program,param_name,param_value_size, param_value, param_value_size_ret);
}

EXPT(cl_int) clGetProgramBuildInfo(cl_program program, cl_device_id device, cl_program_build_info param_name, size_t param_value_size, void* param_value, size_t* param_value_size_ret)
{
    typedef cl_int (__stdcall *Type)(cl_program program, cl_device_id device, cl_program_build_info param_name, size_t param_value_size, void* param_value, size_t* param_value_size_ret);
    Type proc = (Type)GetProcAddress(hinstLib, "clGetProgramBuildInfo");
    return proc(program,device,param_name, param_value_size, param_value, param_value_size_ret);
}

//Kernel Object APIs
EXPT(cl_kernel) clCreateKernel(cl_program program, const char* kernel_name, cl_int* errcode_ret)
{
    typedef cl_kernel (__stdcall *Type)(cl_program program, const char* kernel_name, cl_int* errcode_ret);
    Type proc = (Type)GetProcAddress(hinstLib, "clCreateKernel");
    return proc(program, kernel_name, errcode_ret);
}

EXPT(cl_int) clCreateKernelsInProgram(cl_program program, cl_uint num_kernels, cl_kernel* kernels, cl_uint* num_kernels_ret)
{
    typedef cl_int (__stdcall *Type)(cl_program program, cl_uint num_kernels, cl_kernel* kernels, cl_uint* num_kernels_ret);
    Type proc = (Type)GetProcAddress(hinstLib, "clCreateKernelsInProgram");
    return proc(program, num_kernels, kernels, num_kernels_ret);
}

EXPT(cl_int) clRetainKernel(cl_kernel kernel)
{
    typedef cl_int (__stdcall *Type)(cl_kernel kernel);
    Type proc = (Type)GetProcAddress(hinstLib, "clRetainKernel");
    return proc(kernel);
}

EXPT(cl_int) clReleaseKernel(cl_kernel kernel)
{
    typedef cl_int (__stdcall *Type)(cl_kernel kernel);
    Type proc = (Type)GetProcAddress(hinstLib, "clReleaseKernel");
    return proc(kernel);
}

EXPT(cl_int) clSetKernelArg(cl_kernel kernel,cl_uint arg_index,size_t arg_size,const void* arg_value)
{
    typedef cl_int (__stdcall *Type)(cl_kernel kernel,cl_uint arg_index,size_t arg_size,const void* arg_value);
    Type proc = (Type)GetProcAddress(hinstLib, "clSetKernelArg");
    return proc(kernel, arg_index, arg_size, arg_value);
}

EXPT(cl_int) clGetKernelInfo(cl_kernel kernel,cl_kernel_info param_name,size_t param_value_size,void* param_value,size_t* param_value_size_ret)
{
    typedef cl_int (__stdcall *Type)(cl_kernel kernel,cl_kernel_info param_name,size_t param_value_size,void* param_value,size_t* param_value_size_ret);
    Type proc = (Type)GetProcAddress(hinstLib, "clGetKernelInfo");
    return proc(kernel, param_name, param_value_size, param_value, param_value_size_ret);
}

EXPT(cl_int) clGetKernelWorkGroupInfo(cl_kernel kernel,cl_device_id device,cl_kernel_work_group_info param_name,size_t param_value_size,void* param_value,size_t* param_value_size_ret)
{
    typedef cl_int (__stdcall *Type)(cl_kernel kernel,cl_device_id device,cl_kernel_work_group_info param_name,size_t param_value_size,void* param_value,size_t* param_value_size_ret);
    Type proc = (Type)GetProcAddress(hinstLib, "clGetKernelWorkGroupInfo");
    return proc(kernel, device, param_name, param_value_size, param_value, param_value_size_ret);
}

//Event Object APIs 
EXPT(cl_int) clWaitForEvents(cl_uint num_events,const cl_event* event_list)
{
    typedef cl_int (__stdcall *Type)(cl_uint num_events,const cl_event* event_list);
    Type proc = (Type)GetProcAddress(hinstLib, "clWaitForEvents");
    return proc(num_events, event_list);
}

EXPT(cl_int) clGetEventInfo(cl_event event,cl_event_info param_name,size_t param_value_size,void* param_value,size_t* param_value_size_ret)
{
    typedef cl_int (__stdcall *Type)(cl_event event,cl_event_info param_name,size_t param_value_size,void* param_value,size_t* param_value_size_ret);
    Type proc = (Type)GetProcAddress(hinstLib, "clGetEventInfo");
    return proc(event, param_name, param_value_size, param_value, param_value_size_ret);
}

EXPT(cl_event) clCreateUserEvent(cl_context context,cl_int* errcode_ret)
{
    typedef cl_event (__stdcall *Type)(cl_context context,cl_int* errcode_ret);
    Type proc = (Type)GetProcAddress(hinstLib, "clCreateUserEvent");
    return proc(context, errcode_ret);
}

EXPT(cl_int) clRetainEvent(cl_event event)
{
    typedef cl_int (__stdcall *Type)(cl_event event);
    Type proc = (Type)GetProcAddress(hinstLib, "clRetainEvent");
    return proc(event);
}

EXPT(cl_int) clReleaseEvent(cl_event event)
{
    typedef cl_int (__stdcall *Type)(cl_event event);
    Type proc = (Type)GetProcAddress(hinstLib, "clReleaseEvent");
    return proc(event);
}

EXPT(cl_int) clSetUserEventStatus(cl_event event,cl_int execution_status)
{
    typedef cl_int (__stdcall *Type)(cl_event event,cl_int execution_status);
    Type proc = (Type)GetProcAddress(hinstLib, "clSetUserEventStatus");
    return proc(event, execution_status);
}

EXPT(cl_int) clSetEventCallback( cl_event event, cl_int command_exec_callback_type, void (__stdcall* pfn_notify)(cl_event, cl_int, void *),void* user_data)
{
    typedef cl_int (__stdcall *Type)( cl_event event, cl_int command_exec_callback_type, void (__stdcall* pfn_notify)(cl_event, cl_int, void *),void* user_data);
    Type proc = (Type)GetProcAddress(hinstLib, "clSetEventCallback");
    return proc(event, command_exec_callback_type, pfn_notify, user_data);
}

//Profiling APIs 
EXPT(cl_int) clGetEventProfilingInfo(cl_event event, cl_profiling_info param_name, size_t param_value_size, void* param_value,size_t* param_value_size_ret)
{
    typedef cl_int (__stdcall *Type)(cl_event event, cl_profiling_info param_name, size_t param_value_size, void* param_value,size_t* param_value_size_ret);
    Type proc = (Type)GetProcAddress(hinstLib, "clGetEventProfilingInfo");
    return proc(event, param_name, param_value_size, param_value, param_value_size_ret);
}

//Flush and Finish APIs
EXPT(cl_int) clFlush(cl_command_queue command_queue)
{
    typedef cl_int (__stdcall *Type)(cl_command_queue command_queue);
    Type proc = (Type)GetProcAddress(hinstLib, "clFlush");
    return proc(command_queue);
}

EXPT(cl_int) clFinish(cl_command_queue command_queue)
{
    typedef cl_int (__stdcall *Type)(cl_command_queue command_queue);
    Type proc = (Type)GetProcAddress(hinstLib, "clFinish");
    return proc(command_queue);
}

//Enqueued Commands APIs
EXPT(cl_int) clEnqueueReadBuffer(cl_command_queue command_queue, cl_mem buffer, cl_bool blocking_read, size_t offset, size_t cb, 
                    void* ptr, cl_uint num_events_in_wait_list, const cl_event* event_wait_list, cl_event* event)
{
    typedef cl_int (__stdcall *Type)(cl_command_queue command_queue, cl_mem buffer, cl_bool blocking_read, size_t offset, size_t cb, 
        void* ptr, cl_uint num_events_in_wait_list, const cl_event* event_wait_list, cl_event* event);
    Type proc = (Type)GetProcAddress(hinstLib, "clEnqueueReadBuffer");
    return proc(command_queue, buffer, blocking_read, offset, cb, ptr, num_events_in_wait_list, event_wait_list, event);
}

EXPT(cl_int)
clEnqueueReadBufferRect(cl_command_queue command_queue, cl_mem buffer, cl_bool blocking_read, const size_t* buffer_origin, const size_t* host_origin, const size_t* region, size_t buffer_row_pitch,
                        size_t buffer_slice_pitch, size_t host_row_pitch, size_t host_slice_pitch, void* ptr, cl_uint num_events_in_wait_list, const cl_event* event_wait_list, cl_event* event)
{
    typedef cl_int (__stdcall *Type)(cl_command_queue command_queue, cl_mem buffer, cl_bool blocking_read, const size_t* buffer_origin, const size_t* host_origin, const size_t* region, size_t buffer_row_pitch,
        size_t buffer_slice_pitch, size_t host_row_pitch, size_t host_slice_pitch, void* ptr, cl_uint num_events_in_wait_list, const cl_event* event_wait_list, cl_event* event);
    Type proc = (Type)GetProcAddress(hinstLib, "clEnqueueReadBufferRect");
    return proc(command_queue, buffer, blocking_read, buffer_origin, host_origin, region, buffer_row_pitch, buffer_slice_pitch, host_row_pitch, host_slice_pitch, ptr, num_events_in_wait_list, event_wait_list, event);
}


EXPT(cl_int) clEnqueueWriteBuffer(cl_command_queue command_queue, cl_mem buffer, cl_bool blocking_write, size_t offset, size_t cb, const void* ptr, cl_uint num_events_in_wait_list, const cl_event* event_wait_list, cl_event* event)
{
    typedef cl_int (__stdcall *Type)(cl_command_queue command_queue, cl_mem buffer, cl_bool blocking_write, size_t offset, size_t cb, const void* ptr, cl_uint num_events_in_wait_list, const cl_event* event_wait_list, cl_event* event);
    Type proc = (Type)GetProcAddress(hinstLib, "clEnqueueWriteBuffer");
    return proc(command_queue, buffer, blocking_write, offset, cb, ptr, num_events_in_wait_list, event_wait_list, event);
}

EXPT(cl_int) clEnqueueWriteBufferRect(cl_command_queue command_queue, cl_mem buffer, cl_bool blocking_write, const size_t* buffer_origin, const size_t* host_origin, const size_t* region, size_t buffer_row_pitch, size_t buffer_slice_pitch, size_t host_row_pitch, size_t host_slice_pitch, const void* ptr, cl_uint num_events_in_wait_list, const cl_event* event_wait_list, cl_event* event)
{
    typedef cl_int (__stdcall *Type)(cl_command_queue command_queue, cl_mem buffer, cl_bool blocking_write, const size_t* buffer_origin, const size_t* host_origin, const size_t* region, 
        size_t buffer_row_pitch, size_t buffer_slice_pitch, size_t host_row_pitch, size_t host_slice_pitch, const void* ptr, cl_uint num_events_in_wait_list, 
        const cl_event* event_wait_list, cl_event* event);
    Type proc = (Type)GetProcAddress(hinstLib, "clEnqueueWriteBufferRect");
    return proc(command_queue, buffer, blocking_write, buffer_origin, host_origin, region, buffer_row_pitch, buffer_slice_pitch, host_row_pitch, host_slice_pitch, ptr, num_events_in_wait_list, event_wait_list, event);
}

EXPT(cl_int) clEnqueueCopyBuffer(cl_command_queue command_queue, cl_mem src_buffer, cl_mem dst_buffer, size_t src_offset, size_t dst_offset, size_t cb, cl_uint num_events_in_wait_list, const cl_event* event_wait_list, cl_event* event)
{
    typedef cl_int (__stdcall *Type)(cl_command_queue command_queue, cl_mem src_buffer, cl_mem dst_buffer, size_t src_offset, size_t dst_offset, size_t cb, cl_uint num_events_in_wait_list, const cl_event* event_wait_list, cl_event* event);
    Type proc = (Type)GetProcAddress(hinstLib, "clEnqueueCopyBuffer");
    return proc(command_queue, src_buffer, dst_buffer, src_offset, dst_offset, cb, num_events_in_wait_list, event_wait_list, event);
}

EXPT(cl_int) clEnqueueCopyBufferRect(cl_command_queue command_queue, cl_mem src_buffer, cl_mem dst_buffer, const size_t* src_origin, const size_t* dst_origin, const size_t* region, size_t src_row_pitch, size_t src_slice_pitch, size_t dst_row_pitch, size_t dst_slice_pitch, cl_uint num_events_in_wait_list, const cl_event* event_wait_list, cl_event* event)
{
    typedef cl_int (__stdcall *Type)(cl_command_queue command_queue, cl_mem src_buffer, cl_mem dst_buffer, const size_t* src_origin, 
        const size_t* dst_origin, const size_t* region, size_t src_row_pitch, size_t src_slice_pitch, size_t dst_row_pitch, 
        size_t dst_slice_pitch, cl_uint num_events_in_wait_list, const cl_event* event_wait_list, cl_event* event);
    Type proc = (Type)GetProcAddress(hinstLib, "clEnqueueCopyBufferRect");
    return proc(command_queue, src_buffer, dst_buffer, src_origin, dst_origin, region, src_row_pitch, src_slice_pitch,
        dst_row_pitch, dst_slice_pitch, num_events_in_wait_list, event_wait_list, event);
}

EXPT(cl_int) clEnqueueReadImage(cl_command_queue command_queue, cl_mem image, cl_bool blocking_read, const size_t* origin[3], const size_t* region[3], size_t row_pitch, size_t slice_pitch, void* ptr, cl_uint num_events_in_wait_list, const cl_event* event_wait_list, cl_event* event)
{
    typedef cl_int (__stdcall *Type)(cl_command_queue command_queue, cl_mem image, cl_bool blocking_read, const size_t* origin[3], const size_t* region[3], size_t row_pitch, size_t slice_pitch,
        void* ptr, cl_uint num_events_in_wait_list, const cl_event* event_wait_list, cl_event* event);
    Type proc = (Type)GetProcAddress(hinstLib, "clEnqueueReadImage");
    return proc(command_queue, image, blocking_read, origin, region, row_pitch, slice_pitch, ptr, num_events_in_wait_list, event_wait_list, event);
}

EXPT(cl_int) clEnqueueWriteImage(cl_command_queue command_queue, cl_mem image, cl_bool blocking_write, const size_t* origin[3], const size_t* region[3], size_t input_row_pitch,
                    size_t input_slice_pitch, const void* ptr, cl_uint num_events_in_wait_list, const cl_event* event_wait_list, cl_event* event)
{
    typedef cl_int (__stdcall *Type)(cl_command_queue command_queue, cl_mem image, cl_bool blocking_write, const size_t* origin[3], const size_t* region[3], size_t input_row_pitch,
        size_t input_slice_pitch, const void* ptr, cl_uint num_events_in_wait_list, const cl_event* event_wait_list, cl_event* event);
    Type proc = (Type)GetProcAddress(hinstLib, "clEnqueueWriteImage");
    return proc(command_queue, image, blocking_write, origin, region, input_row_pitch, input_slice_pitch, ptr, num_events_in_wait_list, event_wait_list, event);
}

EXPT(cl_int) clEnqueueCopyImage(cl_command_queue command_queue, cl_mem src_image, cl_mem dst_image, const size_t* src_origin[3], const size_t* dst_origin[3],
                   const size_t* region[3], cl_uint num_events_in_wait_list, const cl_event* event_wait_list, cl_event* event)
{
    typedef cl_int (__stdcall *Type)(cl_command_queue command_queue, cl_mem src_image, cl_mem dst_image, const size_t* src_origin[3], const size_t* dst_origin[3],
        const size_t* region[3], cl_uint num_events_in_wait_list, const cl_event* event_wait_list, cl_event* event);
    Type proc = (Type)GetProcAddress(hinstLib, "clEnqueueCopyImage");
    return proc(command_queue, src_image, dst_image, src_origin, dst_origin, region, num_events_in_wait_list, event_wait_list, event);
}

EXPT(cl_int) clEnqueueCopyImageToBuffer(cl_command_queue command_queue, cl_mem src_image, cl_mem dst_buffer, const size_t* src_origin[3], const size_t* region[3], size_t dst_offset,
                           cl_uint num_events_in_wait_list, const cl_event* event_wait_list, cl_event* event)
{
    typedef cl_int (__stdcall *Type)(cl_command_queue command_queue, cl_mem src_image, cl_mem dst_buffer, const size_t* src_origin[3], const size_t* region[3], size_t dst_offset,
        cl_uint num_events_in_wait_list, const cl_event* event_wait_list, cl_event* event);
    Type proc = (Type)GetProcAddress(hinstLib, "clEnqueueCopyImageToBuffer");
    return proc(command_queue, src_image, dst_buffer, src_origin, region, dst_offset, num_events_in_wait_list, event_wait_list, event);
}

EXPT(cl_int) clEnqueueCopyBufferToImage(cl_command_queue command_queue, cl_mem src_buffer, cl_mem dst_image, size_t src_offset, const size_t* dst_origin[3], const size_t* region[3], cl_uint num_events_in_wait_list,
                           const cl_event* event_wait_list, cl_event* event)
{
    typedef cl_int (__stdcall *Type)(cl_command_queue command_queue, cl_mem src_buffer, cl_mem dst_image, size_t src_offset, const size_t* dst_origin[3], const size_t* region[3], cl_uint num_events_in_wait_list,
        const cl_event* event_wait_list, cl_event* event);
    Type proc = (Type)GetProcAddress(hinstLib, "clEnqueueCopyBufferToImage");
    return proc(command_queue, src_buffer, dst_image, src_offset, dst_origin, region, num_events_in_wait_list, event_wait_list, event);
}

EXPT(void*) clEnqueueMapBuffer(cl_command_queue command_queue, cl_mem buffer, cl_bool blocking_map, cl_map_flags map_flags, size_t offset, size_t cb, cl_uint num_events_in_wait_list, const cl_event* event_wait_list, cl_event* event, cl_int* errcode_ret) 
{
    typedef void* (__stdcall *Type)(cl_command_queue command_queue, cl_mem buffer, cl_bool blocking_map, cl_map_flags map_flags, size_t offset, size_t cb, cl_uint num_events_in_wait_list, const cl_event* event_wait_list, cl_event* event, cl_int* errcode_ret) ;
    Type proc = (Type)GetProcAddress(hinstLib, "clEnqueueMapBuffer");
    return proc(command_queue, buffer, blocking_map, map_flags, offset, cb, num_events_in_wait_list, event_wait_list, event, errcode_ret);
}

EXPT(void*) clEnqueueMapImage(cl_command_queue command_queue , cl_mem image , cl_bool blocking_map , cl_map_flags map_flags , const size_t* origin[3] , const size_t* region[3] ,
                  size_t* image_row_pitch, size_t* image_slice_pitch, cl_uint num_events_in_wait_list, const cl_event* event_wait_list, cl_event* event, cl_int* errcode_ret)
{
    typedef void* (__stdcall *Type)(cl_command_queue command_queue , cl_mem image , cl_bool blocking_map , cl_map_flags map_flags , const size_t* origin[3] , const size_t* region[3] ,
        size_t* image_row_pitch, size_t* image_slice_pitch, cl_uint num_events_in_wait_list, const cl_event* event_wait_list, cl_event* event, cl_int* errcode_ret);
    Type proc = (Type)GetProcAddress(hinstLib, "clEnqueueMapImage");
    return proc(command_queue, image, blocking_map, map_flags, origin, region, image_row_pitch, image_slice_pitch, num_events_in_wait_list, event_wait_list, event, errcode_ret);
}

EXPT(cl_int) clEnqueueUnmapMemObject(cl_command_queue command_queue, cl_mem memobj, void* mapped_ptr, cl_uint num_events_in_wait_list, const cl_event* event_wait_list, cl_event* event)
{
    typedef cl_int (__stdcall *Type)(cl_command_queue command_queue, cl_mem memobj, void* mapped_ptr, cl_uint num_events_in_wait_list, const cl_event* event_wait_list, cl_event* event);
    Type proc = (Type)GetProcAddress(hinstLib, "clEnqueueUnmapMemObject");
    return proc(command_queue, memobj, mapped_ptr, num_events_in_wait_list, event_wait_list, event);
}

EXPT(cl_int)
clEnqueueNDRangeKernel(cl_command_queue command_queue, cl_kernel kernel, cl_uint work_dim, const size_t* global_work_offset, const size_t* global_work_size, const size_t* local_work_size,
                       cl_uint num_events_in_wait_list, const cl_event* event_wait_list, cl_event* event)
{
    typedef cl_int (__stdcall *Type)(cl_command_queue command_queue, cl_kernel kernel, cl_uint work_dim, const size_t* global_work_offset, const size_t* global_work_size, const size_t* local_work_size,
        cl_uint num_events_in_wait_list, const cl_event* event_wait_list, cl_event* event);
    Type proc = (Type)GetProcAddress(hinstLib, "clEnqueueNDRangeKernel");
    return proc(command_queue, kernel, work_dim, global_work_offset, global_work_size, local_work_size, num_events_in_wait_list, event_wait_list, event);
}


EXPT(cl_int) clEnqueueTask(cl_command_queue command_queue, cl_kernel kernel, cl_uint num_events_in_wait_list, const cl_event* event_wait_list, cl_event* event)
{
    typedef cl_int (__stdcall *Type)(cl_command_queue command_queue, cl_kernel kernel, cl_uint num_events_in_wait_list, const cl_event* event_wait_list, cl_event* event);
    Type proc = (Type)GetProcAddress(hinstLib, "clEnqueueTask");
    return proc(command_queue, kernel, num_events_in_wait_list, event_wait_list, event);
}

EXPT(cl_int) clEnqueueNativeKernel(cl_command_queue command_queue, void (*user_func)(void *), void* args, size_t cb_args, 
                      cl_uint num_mem_objects, const cl_mem* mem_list, const void ** args_mem_loc, cl_uint num_events_in_wait_list, const cl_event* event_wait_list, cl_event* event)
{
    typedef cl_int (__stdcall *Type)(cl_command_queue command_queue, void (*user_func)(void *), void* args, size_t cb_args, 
        cl_uint num_mem_objects, const cl_mem* mem_list, const void ** args_mem_loc, cl_uint num_events_in_wait_list, const cl_event* event_wait_list, cl_event* event);
    Type proc = (Type)GetProcAddress(hinstLib, "clEnqueueNativeKernel");

    return proc(command_queue, user_func, args, cb_args, num_mem_objects, mem_list, args_mem_loc, num_events_in_wait_list, event_wait_list, event);
}

EXPT(cl_int) clEnqueueMarker(cl_command_queue command_queue, cl_event *event)
{
    typedef cl_int (__stdcall *Type)(cl_command_queue command_queue, cl_event *event);
    Type proc = (Type)GetProcAddress(hinstLib, "clEnqueueMarker");

    return proc(command_queue, event);
}


EXPT(cl_int) clEnqueueWaitForEvents(cl_command_queue command_queue, cl_uint num_events, const cl_event *event_list)
{
    typedef cl_int (__stdcall *Type)(cl_command_queue command_queue, cl_uint num_events, const cl_event *event_list);
    Type proc = (Type)GetProcAddress(hinstLib, "clEnqueueWaitForEvents");

    return proc(command_queue, num_events, event_list);
}


EXPT(cl_int) clEnqueueBarrier(cl_command_queue command_queue)
{
    typedef cl_int (__stdcall *Type)(cl_command_queue command_queue);
    Type proc = (Type)GetProcAddress(hinstLib, "clEnqueueBarrier");

    return proc(command_queue);
}

EXPT(void*) clGetExtensionFunctionAddress(const char *func_name)
{
    typedef void* (__stdcall *Type)(const char *func_name);
    Type proc = (Type)GetProcAddress(hinstLib, "clGetExtensionFunctionAddress");

    return proc(func_name);
}

EXPT(cl_mem) clCreateFromGLBuffer(cl_context context, cl_mem_flags flags , cl_GLuint bufobj, int* errcode_ret)
{
    typedef cl_mem (__stdcall *Type)(cl_context context, cl_mem_flags flags , cl_GLuint bufobj, int* errcode_ret);
    Type proc = (Type)GetProcAddress(hinstLib, "clCreateFromGLBuffer");

    return proc(context, flags, bufobj, errcode_ret);
}

EXPT(cl_mem) clCreateFromGLTexture2D(cl_context context, cl_mem_flags flags, cl_GLenum target, cl_GLint miplevel, cl_GLuint texture, cl_int *errcode_ret)
{
    typedef cl_mem (__stdcall *Type)(cl_context context, cl_mem_flags flags, cl_GLenum target, cl_GLint miplevel, cl_GLuint texture, cl_int *errcode_ret);
    Type proc = (Type)GetProcAddress(hinstLib, "clCreateFromGLTexture2D");
    return proc(context, flags, target, miplevel, texture, errcode_ret);
}

EXPT(cl_mem) clCreateFromGLTexture3D(cl_context context, cl_mem_flags flags, cl_GLenum target, cl_GLint miplevel, cl_GLuint texture, cl_int* errcode_ret)
{
    typedef cl_mem (__stdcall *Type)(cl_context context, cl_mem_flags flags, cl_GLenum target ,cl_GLint miplevel, cl_GLuint texture, cl_int* errcode_ret);
    Type proc = (Type)GetProcAddress(hinstLib, "clCreateFromGLTexture3D");
    return proc(context, flags, target, miplevel, texture, errcode_ret);
}

EXPT(cl_mem) clCreateFromGLRenderbuffer(cl_context context, cl_mem_flags flags, cl_GLuint  renderbuffer, cl_int* errcode_ret)
{
    typedef cl_mem (__stdcall *Type)(cl_context context, cl_mem_flags flags, cl_GLuint  renderbuffer, cl_int* errcode_ret);
    Type proc = (Type)GetProcAddress(hinstLib, "clCreateFromGLRenderbuffer");
    return proc(context, flags, renderbuffer, errcode_ret);
}

EXPT(cl_int) clGetGLObjectInfo(cl_mem memobj, cl_gl_object_type* gl_object_type , cl_GLuint* gl_object_name)
{
    typedef cl_int (__stdcall *Type)(cl_mem memobj, cl_gl_object_type* gl_object_type , cl_GLuint* gl_object_name);
    Type proc = (Type)GetProcAddress(hinstLib, "clGetGLObjectInfo");
    return proc(memobj, gl_object_type, gl_object_name);
}

EXPT(cl_int) clGetGLTextureInfo(cl_mem memobj, cl_gl_texture_info param_name, size_t param_value_size, void* param_value, size_t* param_value_size_ret)
{
    typedef cl_int (__stdcall *Type)(cl_mem memobj, cl_gl_texture_info param_name, size_t param_value_size, void* param_value, size_t* param_value_size_ret);
    Type proc = (Type)GetProcAddress(hinstLib, "clGetGLTextureInfo");
    return proc(memobj, param_name, param_value_size, param_value, param_value_size_ret);
}


EXPT(cl_int) clEnqueueAcquireGLObjects(cl_command_queue command_queue, cl_uint num_objects, const cl_mem* mem_objects, cl_uint num_events_in_wait_list, const cl_event* event_wait_list, cl_event* event)
{
    typedef cl_int (__stdcall *Type)(cl_command_queue command_queue, cl_uint num_objects, const cl_mem* mem_objects, cl_uint num_events_in_wait_list, const cl_event* event_wait_list, cl_event* event);
    Type proc = (Type)GetProcAddress(hinstLib, "clEnqueueAcquireGLObjects");
    return proc(command_queue, num_objects, mem_objects, num_events_in_wait_list, event_wait_list, event);
}

EXPT(cl_int) clEnqueueReleaseGLObjects(cl_command_queue command_queue, cl_uint num_objects, const cl_mem* mem_objects, cl_uint num_events_in_wait_list, const cl_event* event_wait_list, cl_event* event)
{
    typedef cl_int (__stdcall *Type)(cl_command_queue command_queue, cl_uint num_objects, const cl_mem* mem_objects, cl_uint num_events_in_wait_list, const cl_event* event_wait_list, cl_event* event);
    Type proc = (Type)GetProcAddress(hinstLib, "clEnqueueReleaseGLObjects");
    return proc(command_queue, num_objects, mem_objects, num_events_in_wait_list, event_wait_list, event);
}

EXPT(cl_int) clGetGLContextInfoKHR(const cl_context_properties* properties, cl_gl_context_info param_name, size_t param_value_size, void* param_value, size_t* param_value_size_ret) 
{
    typedef cl_int (__stdcall *Type)(const cl_context_properties* properties, cl_gl_context_info param_name, size_t param_value_size, void* param_value, size_t* param_value_size_ret) ;
    Type proc = (Type)GetProcAddress(hinstLib, "clGetGLContextInfoKHR");
    return proc(properties, param_name, param_value_size, param_value, param_value_size_ret);
}


EXPT(cl_int) clSetCommandQueueProperty(cl_command_queue command_queue, cl_command_queue_properties properties, cl_bool enable, cl_command_queue_properties *old_properties)
{
    typedef cl_int (__stdcall *Type)(cl_command_queue command_queue, cl_command_queue_properties properties, cl_bool enable, cl_command_queue_properties *old_properties);
    Type proc = (Type)GetProcAddress(hinstLib, "clSetCommandQueueProperty");
    return proc(command_queue, properties, enable, old_properties);
}
