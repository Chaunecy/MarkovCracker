#!/bin/bash
# $1 file to be processed
# $2 file to put processed contents
python3 -m venv .venv
source .venv/bin/activate
pip install matplotlib numpy
#grep -P "^[\x21-\x7e]{1,255}$" <(cat "$1") >"$2"
