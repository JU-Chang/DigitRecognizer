#!/usr/bin/env sh

./build/tools/caffe train --gpu all --solver=examples/digitRecog/lenet_solver.prototxt
