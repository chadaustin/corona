#!/bin/sh

VERSION=1.0.2
BASE=corona-$VERSION-doxygen

doxygen corona.doxy || exit 1
cp -r html $BASE
zip -rq $BASE.zip $BASE
tar cfz $BASE.tgz $BASE
tar cfj $BASE.tbz $BASE
