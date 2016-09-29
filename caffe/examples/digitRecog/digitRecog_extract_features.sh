#!/usr/bin/env sh

./build/tools/digit_extract_features.bin \
examples/digitRecog/lenet_iter_10000.caffemodel \
examples/digitRecog/deploy.prototxt prob \
examples/digitRecog/features 280 lmdb examples/digitRecog/feature 
