#!/bin/bash

cd ./works/makeproject
make
cd ../../
./dist_check_and_run.125.sh
#cp ../run_area/server/replay.txt ../ViewClientForReplay/replay.txt
