#!/bin/sh

find . -type f -regextype posix-egrep -regex ".*\.(MDX|mdx)" -print0 | xargs -I{} --null ./simple_player -maxLoops 1 \"{}\"
