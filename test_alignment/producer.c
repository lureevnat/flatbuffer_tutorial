#include "model_dvm_builder.h"
#undef ns
#define ns(x) FLATBUFFERS_WRAP_NAMESPACE(ARA2_Model,x)
#define c_vec_len(V) (sizeof(V)/sizeof((V)[0]))
#include <stdio.h>
int main(){
    flatcc_builder_t builder, *B;
    B = &builder;
    flatcc_builder_init(B);
    // Create aligned struct values with constants
    // Aligned struct values
    ns(align8_t)  a_val = { .x = 0xab };
    ns(align16_t) b_val = { .x = 0xcd };
    ns(align32_t) c_val = { .x = 0xef };

    ns(Model_ref_t) model = ns(Model_create)(B, &a_val, &b_val, &c_val);

    size_t size;
    const void *buffer = flatcc_builder_get_direct_buffer(B, &size);

    // Dump to file
    FILE *fp = fopen("model.bin", "wb");
    if (!fp) {
        perror("Failed to open output file");
        return 1;
    }

    fwrite(buffer, 1, size, fp);
    fclose(fp);

    printf("FlatBuffer written to model.bin (%zu bytes)\n", size);

    flatcc_builder_clear(B);
    
} 