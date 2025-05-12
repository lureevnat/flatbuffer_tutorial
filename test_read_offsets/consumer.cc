#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <iostream>
#include <cassert>
#include <iomanip>
#include <cstdint>
#include <fstream>  // Required for std::ifstream

#include "model_dvm_generated.h"
void print_hex_bytes(const uint8_t* address, size_t count = 10) {
    for (size_t i = 0; i < count; ++i) {
        std::cout << "Byte " << i << ": 0x"
                  << std::hex << std::setw(2) << std::setfill('0')
                  << static_cast<int>(address[i]) << std::endl;
    }
}

int main() {
    {
            // Read the FlatBuffer binary file into a buffer
        std::ifstream infile("model.dvm", std::ios::binary | std::ios::ate);
        if (!infile) {
            std::cerr << "Failed to open model.dvm" << std::endl;
            return 1;
        }

        std::streamsize size = infile.tellg();
        infile.seekg(0, std::ios::beg);

        std::vector<char> buffer(size);
        if (!infile.read(buffer.data(), size)) {
            std::cerr << "Failed to read model.dvm" << std::endl;
            return 1;
        }

        // Create a verifier with the buffer
        flatbuffers::Verifier verifier(reinterpret_cast<const uint8_t*>(buffer.data()), size);

        // Verify the buffer
        if (ARA2_Model::VerifyModelBuffer(verifier)) {
            std::cout << "FlatBuffer verification succeeded." << std::endl;
        } else {
            std::cerr << "FlatBuffer verification failed." << std::endl;
            return 1;
        }   
    }

    const char* filename = "model.dvm";
    int fd = open(filename, O_RDONLY);
    assert(fd != -1);

    struct stat sb;
    fstat(fd, &sb);
    size_t size = sb.st_size;

    void* mmap_ptr = mmap(nullptr, size, PROT_READ, MAP_PRIVATE, fd, 0);
    assert(mmap_ptr != MAP_FAILED);
    close(fd);
    std::cout << "mmap start address = " << mmap_ptr << std::endl;

    const uint8_t* buffer = reinterpret_cast<const uint8_t*>(mmap_ptr);
    auto model = ARA2_Model::GetModel(buffer);
    // auto mkl_ptr = model->mcp_kernel_lib();  // returns pointer to union object
    // print_hex_bytes((uint8_t*)mkl_ptr);

   // Step 2: Get the type of the mkl union
   auto mkl_type = model->mcp_kernel_lib_type();

   if (mkl_type == ARA2_Model::File_Stream) {
       // Step 3: Access the union as a T2_Model::S
       const ARA2_Model::Stream* s_obj = model->mcp_kernel_lib_as_Stream();
       if (s_obj && s_obj->bitstream() && s_obj->bitstream()->size() > 0) {
        const uint8_t* bitstream_data = s_obj->bitstream()->data();  // Pointer to first element
        std::cout << "Address of first bs element: " << static_cast<const void*>(bitstream_data) << "\n";
        std::cout << "size of bitstream: " << s_obj->bitstream()->size() << "\n";

        // Optional: offset relative to start of mmap
        size_t offset = bitstream_data - buffer;
        std::cout << "Offset in file: " << offset << " bytes\n";
       }
   } else {
       std::cout << "mkl is not of type S.\n";
   }


}