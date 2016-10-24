文档结构：
caffe/data/digitRecog:训练和测试数据
caffe/tools/:提取特征的代码文件
caffe/build/example/digitRecog/:编译生成的bin文件
caffe/example/digitRecog/:其他所有相关文件

=================

1、编译,生成
./build/examples/digitRecog/convert_digitRecog_train_lmdb.bin和./build/examples/digitRecog/convert_digitRecog_lmdb.bin 

make all –j8

2、把csv格式的数据转换成lmdb格式
./examples/digitRecog/create_digitRecog.sh


3、训练
./examples/digitRecog/digitRecog_train.sh


4、提取pro层特征，得到最终结果文件./build/examples/digitRecog/feature0.csv
./examples/digitRecog/digitRecog_extract_features.sh 


5.保存、解析日志
./examples/digitRecog/digitRecog_train.sh 2>&1 |tee digitRecog_train.log

./tools/extra/parse_log.sh digitRecog_train.log



