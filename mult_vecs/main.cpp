#include <iostream>
#include <vector>
#include <string>

#ifdef __APPLE__
   #include <OpenCL/cl.hpp> 
   #define PLATFORM_INDEX 0
   #define DEVICE_INDEX 1
#endif
#ifdef __linux__ 
    #include <CL/cl.hpp>
    #define PLATFORM_INDEX 0
    #define DEVICE_INDEX 0
#endif



int main()
{
    cl::Platform platform;
    {
        std::vector<cl::Platform> platforms;
        cl::Platform::get(&platforms);
        if (platforms.size() < PLATFORM_INDEX + 1)
        {
            std::cout << "No desired platform found (index: " << PLATFORM_INDEX << ")! Exiting..." << std::endl;
            return 0;
        }
        platform = platforms[PLATFORM_INDEX];
    }

    cl::Device device;
    {
        std::vector<cl::Device> devices;
        platform.getDevices(CL_DEVICE_TYPE_ALL, &devices);
        if (devices.size() < DEVICE_INDEX + 1)
        {
            std::cout << "No desired device found (index: " << DEVICE_INDEX << ")! Exiting..." << std::endl;
            return 0;
        }
        device = devices[DEVICE_INDEX];
    }

    std::cout << "Device: " << device.getInfo<CL_DEVICE_NAME>() << " will be used." << std::endl;

    cl::Context context({device});
    cl::Program::Sources sources;

    std::string kernel_code = 
        "void kernel simple_mult(global const int* A, global const int* B, global int* C)"
        "{"
        "   C[get_global_id(0)] = A[get_global_id(0)] * B[get_global_id(0)];"
        "}";
    
    sources.push_back({kernel_code.c_str(), kernel_code.length()});

    cl::Program program(context, sources);
    if (program.build({device}) != CL_SUCCESS)
    {
        std::cout << "Error building kernel! " << program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(device) << std::endl;
        return 0;
    }

    cl::Buffer buffer_A(context, CL_MEM_READ_WRITE, sizeof(int) * 10);
    cl::Buffer buffer_B(context, CL_MEM_READ_WRITE, sizeof(int) * 10);
    cl::Buffer buffer_C(context, CL_MEM_READ_WRITE, sizeof(int) * 10);

    int A[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 0};
    int B[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 0};

    cl::CommandQueue queue(context, device);
    queue.enqueueWriteBuffer(buffer_A, CL_TRUE, 0, sizeof(int) * 10, A);
    queue.enqueueWriteBuffer(buffer_B, CL_TRUE, 0, sizeof(int) * 10, B);

    cl::Kernel simple_mult(program, "simple_mult");
    simple_mult.setArg(0, buffer_A);
    simple_mult.setArg(1, buffer_B);
    simple_mult.setArg(2, buffer_C);
    queue.enqueueNDRangeKernel(simple_mult, cl::NullRange, cl::NDRange(10), cl::NullRange);
    queue.finish();

    int C[10];
    queue.enqueueReadBuffer(buffer_C, CL_TRUE, 0, sizeof(int) * 10, C);
    
    for(int i = 0; i < 10; i++)
        std::cout << A[i] << " * " << B[i] << " = " << C[i] << std::endl;

    return 0;
}