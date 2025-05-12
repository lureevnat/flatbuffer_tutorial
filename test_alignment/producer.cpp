#include <fstream>
#include "flatbuffers/flatbuffers.h"
#include "model_dvm_generated.h"
#include<iostream>

int main() {
    flatbuffers::FlatBufferBuilder builder;

    // Step 1: Create struct values
    ARA2_Model::align8 a_struct{static_cast<int8_t>(0xAB)};
    ARA2_Model::align16 b_struct{static_cast<int8_t>(0xCD)};
    ARA2_Model::align32 c_struct{static_cast<int8_t>(0xEF)};
    

    // Step 2: Create table using builder.CreateStruct()
    auto model = ARA2_Model::CreateModel(builder, &a_struct, &b_struct, &c_struct);

    // Step 3: Finish buffer
    builder.Finish(model);

    // Step 4: Write to file
    std::ofstream out("model.bin", std::ios::binary);
    out.write(reinterpret_cast<const char *>(builder.GetBufferPointer()), builder.GetSize());
    out.close();

    std::cout << "✅ model.bin written with a=0xAB, b=0xCD, c=0xEF\n";
    return 0;
}
