�ĵ��ṹ��
caffe/data/digitRecog:ѵ���Ͳ�������
caffe/tools/:��ȡ�����Ĵ����ļ�
caffe/build/example/digitRecog/:�������ɵ�bin�ļ�
caffe/example/digitRecog/:������������ļ�

=================

1������,����
./build/examples/digitRecog/convert_digitRecog_train_lmdb.bin��./build/examples/digitRecog/convert_digitRecog_lmdb.bin 

make all �Cj8


2����csv��ʽ������ת����lmdb��ʽ


./examples/digitRecog/create_digitRecog.sh

3��ѵ��



./examples/digitRecog/digitRecog_train.sh



4����ȡpro���������õ����ս���ļ�./build/examples/digitRecog/feature0.csv

./examples/digitRecog/digitRecog_extract_features.sh 






