#include<iostream>
#include <fstream>  // Required for std::ifstream
#include <iomanip>
#include <cstdint>
using namespace std;

#include "model_dvm_generated.h"


void print_hex_bytes(const uint8_t* address, size_t count = 10) {
    for (size_t i = 0; i < count; ++i) {
        std::cout << "Byte " << i << ": 0x"
                  << std::hex << std::setw(2) << std::setfill('0')
                  << static_cast<int>(address[i]) << std::endl;
    }
}

void offset(const uint8_t* buffer){
        // Read the root offset
        uint32_t root_offset = *reinterpret_cast<const uint32_t*>(buffer);
        const uint8_t* table_ptr = buffer + root_offset;
    
        // Read the vtable offset
        int32_t vtable_offset = *reinterpret_cast<const int32_t*>(table_ptr);
        const uint8_t* vtable_ptr = table_ptr - vtable_offset;
    
        // Access the vtable entries
        const uint16_t* vtable = reinterpret_cast<const uint16_t*>(vtable_ptr);
        uint16_t field_index = 9;  // Replace with your actual field index
        uint16_t field_offset = vtable[2 + field_index];
    
        // Calculate the absolute offset
        if (field_offset != 0) {
            const uint8_t* field_ptr = table_ptr + field_offset;
            size_t absolute_offset = field_ptr - buffer;
            std::cout << "Absolute offset of field at index " << field_index << ": " << absolute_offset << " bytes" << std::endl;
        } else {
            std::cout << "Field at index " << field_index << " is not present in the buffer." << std::endl;
        }
}

int main(int argc, char **argv){
  // Open the binary file containing the FlatBuffer
  std::ifstream infile("model.dvm", std::ios::binary | std::ios::ate);
  if (!infile) {
    std::cerr << "Failed to open file." << std::endl;
    return 1;
  }

  // Determine the size of the file and read its contents into a buffer
  std::streamsize size = infile.tellg();
  infile.seekg(0, std::ios::beg);
  std::vector<char> buffer(size);
  if (!infile.read(buffer.data(), size)) {
    std::cerr << "Failed to read file." << std::endl;
    return 1;
  }


    offset(( uint8_t*)buffer.data());
    return 0;

    // Get an view to the root object inside the buffer.
    auto model = ARA2_Model::GetModel(buffer.data());

    // 1. Pointer to the start of the table (Model)
    const uint8_t* table_ptr = reinterpret_cast<const uint8_t*>(model);
    // 2. First 2 bytes of the table are the offset to the vtable
    int32_t vtable_offset = *reinterpret_cast<const int32_t*>(table_ptr);
    const uint8_t* vtable_ptr = table_ptr - vtable_offset;


    const uint16_t* vtable_entries = reinterpret_cast<const uint16_t*>(vtable_ptr);
    for(int ii =0;ii<40;ii++){
        int16_t mkl_offset_in_table = vtable_entries[ii];  // 2 bytes header + 2 bytes object size + 2 * field index

        if (mkl_offset_in_table == 0) {
            std::cout << "mkl not present in buffer.\n";
        } else {
            std::cout << "Offset of mkl field from table start: " << mkl_offset_in_table << " bytes\n";
            const uint8_t* mkl_data_ptr = table_ptr + mkl_offset_in_table;
            std::cout << "Address of mkl data: " << static_cast<const void*>(mkl_data_ptr) << "\n";
            print_hex_bytes(mkl_data_ptr);
        }
    
    }
    return 0;
}