#!/bin/sh
set -e
if test "$CONFIGURATION" = "Debug"; then :
  cd "/Users/shreyashonnalli/Desktop/Group Project/GroupRepo/cpp codebase/buildXcode/_deps/googletest-subbuild"
  /opt/homebrew/Cellar/cmake/3.26.0/bin/cmake -Dcfgdir=/$CONFIGURATION$EFFECTIVE_PLATFORM_NAME -P "/Users/shreyashonnalli/Desktop/Group Project/GroupRepo/cpp codebase/buildXcode/_deps/googletest-subbuild/googletest-populate-prefix/tmp/googletest-populate-mkdirs.cmake"
  /opt/homebrew/Cellar/cmake/3.26.0/bin/cmake -E touch "/Users/shreyashonnalli/Desktop/Group Project/GroupRepo/cpp codebase/buildXcode/_deps/googletest-subbuild/googletest-populate-prefix/src/googletest-populate-stamp/$CONFIGURATION$EFFECTIVE_PLATFORM_NAME/googletest-populate-mkdir"
fi

