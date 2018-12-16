#!/bin/bash

make
cat example.8b | ./compile | ./run
