#!/bin/bash
ls -l $1 | grep "^...x-..-.." | awk '{print $9}'
