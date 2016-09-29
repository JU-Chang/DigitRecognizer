#!/usr/bin/env sh
# This script converts the digitRecog data into lmdb/leveldb format,
# depending on the value assigned to $BACKEND.

EXAMPLE=examples/digitRecog
DATA=data/digitRecog
BUILD=build/examples/digitRecog

BACKEND="lmdb"

echo "Creating ${BACKEND}..."

rm -rf $EXAMPLE/digitRecog_train_${BACKEND}
rm -rf $EXAMPLE/digitRecog_test_${BACKEND}
rm -rf $EXAMPLE/digitRecog_val_${BACKEND}

$BUILD/convert_digitRecog_train_val_lmdb.bin \
data/digitRecog/train_all.csv \
examples/digitRecog/digitRecog_val_lmdb \
examples/digitRecog/digitRecog_train_lmdb 1000

$BUILD/convert_digitRecog_lmdb.bin $DATA/test.csv \
$EXAMPLE/digitRecog_test_${BACKEND} --backend=${BACKEND}

echo "Done."
