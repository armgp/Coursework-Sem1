#!/bin/bash

cat /etc/shells | awk '/^\/usr\//' | awk -F "/" '{print $NF}'
