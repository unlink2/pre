#!/bin/bash

if [ "${1}" = "mem" ]; then
  premake5 gmake && make &&
    valgrind --leak-check=full --track-origins=yes ./bin/Debug/testpre
else 
  premake5 gmake && make && ./bin/Debug/testpre
fi

