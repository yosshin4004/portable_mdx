#!/bin/sh

find ./ -name '*.MDX' -print0 | xargs -I{} --null ./simple_player -maxLoops 1 \"{}\"
