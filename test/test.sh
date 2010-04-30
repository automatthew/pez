#!/bin/bash

gcc -Wall -I. -o test/test_all test/test_all.c .libs/libpez.a /opt/local/lib/libgc.a  && 
  test/test_all
