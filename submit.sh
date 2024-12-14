#!/usr/bin/sh

set -e

mkdir xstigl00

cd Debug
make clean
cd -

cp -r .settings Debug Includes Project_Settings Sources .cproject \
    .cwGeneratedFileSetLog .project xstigl00/

cd doc
make
cd -

cp doc/document.pdf xstigl00/documentation.pdf

rm xstigl00.zip
zip -r xstigl00.zip xstigl00

rm -r xstigl00
