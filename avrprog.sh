#!/bin/sh

avrdude -p $1 -c stk500 -U flash:w:$2:i
