#!/bin/bash
grep -o  "\b[Aa][a-zA-Z0-9]*" $1 | grep -v  "[a-zA-Z0-9]*[tT][xX][tT]"
