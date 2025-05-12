# Tutorial Tasks
```
1. Writing a FlatBuffers schema file (.fbs).
2. Using the flatc compiler to transform the schema into language-specific code.
3. Importing the generated code and libraries into your application.
4. Serializing data into a flatbuffer.
5. Deserializing a flatbuffer.
```

```
schema -> flatc -> language specific code and libraries -> include in app ->serialize -> deserialize
```

```bash
# Code Generation

# c++
flatc -o generated  --gen-all -c model_dvm.fbs

g++ -g -O0 producer.cc -Igenerated -I/local/tools/installs/oss/flatbuffers/23.3.3/include/ -L/local/tools/installs/oss/flatbuffers/23.3.3/lib64/ -lflatbuffers -o producercc && ./producercc


g++ -g -O0 consumer.cc -Igenerated -I/local/tools/installs/oss/flatbuffers/23.3.3/include/ -L/local/tools/installs/oss/flatbuffers/23.3.3/lib64/ -lflatbuffers -o consumercc && ./consumercc


#c
flatcc -a model_dvm.fbs


gcc -g -O0 producer.c -I/local/tools/installs/oss/flatcc/0.6.0/include/ -L/local/tools/installs/oss/flatcc/0.6.0/lib/ -lflatccrt -o producerc && ./producerc

gcc -g -O0 consumer.c -I/local/tools/installs/oss/flatcc/0.6.0/include/ -L/local/tools/installs/oss/flatcc/0.6.0/lib/ -lflatccrt -o consumerc && ./consumerc



```