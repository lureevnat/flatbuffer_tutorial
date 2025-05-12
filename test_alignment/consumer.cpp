#include <iostream>
#include <fstream>
#include "model_dvm_generated.h"

int main() {
    // Load binary file
    std::ifstream infile("model.bin", std::ios::binary | std::ios::ate);
    if (!infile) {
        std::cerr << "Failed to open model.bin\n";
        return 1;
    }

    std::streamsize size = infile.tellg();
    infile.seekg(0, std::ios::beg);

    std::vector<uint8_t> buffer(size);
    if (!infile.read(reinterpret_cast<char*>(buffer.data()), size)) {
        std::cerr << "Failed to read file\n";
        return 1;
    }

    // Validate buffer
    if (!flatbuffers::Verifier(buffer.data(), buffer.size()).VerifyBuffer<ARA2_Model::Model>()) {
        std::cerr << "Invalid FlatBuffer\n";
        return 1;
    }

    // Deserialize root
    const ARA2_Model::Model *model = flatbuffers::GetRoot<ARA2_Model::Model>(buffer.data());

    // Access struct fields
    const ARA2_Model::align8 *a = model->a();
    const ARA2_Model::align16 *b = model->b();
    const ARA2_Model::align32 *c = model->c();

    std::cout << "a.x = " << static_cast<int>(static_cast<int8_t>(a->x())) << std::endl;
    std::cout << "b.x = " << static_cast<int>(static_cast<int8_t>(b->x())) << std::endl;
    std::cout << "c.x = " << static_cast<int>(static_cast<int8_t>(c->x())) << std::endl;
    


    return 0;
}
