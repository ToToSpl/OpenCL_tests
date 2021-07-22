#include <iostream>
#include <vector>

#ifdef __APPLE__
   #include <OpenCL/cl.hpp> 
#endif
#ifdef __linux__ 
    #include <CL/cl.hpp>
#endif

int main()
{
    std::vector<cl::Platform> platforms;
    cl::Platform::get(&platforms);
    if(platforms.size() == 0)
    {
        std::cout << "No platforms found! Exiting..." << std::endl;
        return 0;
    }

    std::cout << "Found platforms. Listing...\n";
    for (auto platform : platforms)
    {
        std::cout << "platform:\t" << platform.getInfo<CL_PLATFORM_NAME>() << std::endl;
        std::vector<cl::Device> devices;
        platform.getDevices(CL_DEVICE_TYPE_ALL, &devices);
        if(devices.size() == 0)
        {
            std::cout << "\tNo devices for this platform." << std::endl;
        }
        else
        {
            std::cout << "\tDevices:" << std::endl;
            for (auto device : devices)
            {
                std::cout << "\t" << device.getInfo<CL_DEVICE_NAME>() << std::endl;
            }
        }
    }
}