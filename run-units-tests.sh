#!/bin/bash

TARGET=$(basename "$(pwd)")
make remove -j4 && make tests -j4
echo "--Files that will be runned--"
find bin/ -type f ! -name "${TARGET}_server" ! -name "${TARGET}_client"
echo "--End list--"
find bin/ -type f ! -name "${TARGET}_server" ! -name "${TARGET}_client" -exec {} \;
