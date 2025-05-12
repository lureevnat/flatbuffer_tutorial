```bash
flatc -o generated  --gen-all -c model_dvm.fbs
g++ -g -O0 consumer.cc -Igenerated -I/local/tools/installs/oss/flatbuffers/23.3.3/include/ -L/local/tools/installs/oss/flatbuffers/23.3.3/lib64/ -lflatbuffers -o consumer && ./consumer
```
