#!/bin/bash

make
cat test.asm | ./compile | ./run
