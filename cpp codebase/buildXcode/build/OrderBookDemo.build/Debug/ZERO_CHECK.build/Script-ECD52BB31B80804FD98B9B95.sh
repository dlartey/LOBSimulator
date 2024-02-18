#!/bin/sh
set -e
if test "$CONFIGURATION" = "Debug"; then :
  cd "/Users/shreyashonnalli/Desktop/Group Project/GroupRepo/cpp codebase/buildXcode"
  make -f /Users/shreyashonnalli/Desktop/Group\ Project/GroupRepo/cpp\ codebase/buildXcode/CMakeScripts/ReRunCMake.make
fi
if test "$CONFIGURATION" = "Release"; then :
  cd "/Users/shreyashonnalli/Desktop/Group Project/GroupRepo/cpp codebase/buildXcode"
  make -f /Users/shreyashonnalli/Desktop/Group\ Project/GroupRepo/cpp\ codebase/buildXcode/CMakeScripts/ReRunCMake.make
fi
if test "$CONFIGURATION" = "MinSizeRel"; then :
  cd "/Users/shreyashonnalli/Desktop/Group Project/GroupRepo/cpp codebase/buildXcode"
  make -f /Users/shreyashonnalli/Desktop/Group\ Project/GroupRepo/cpp\ codebase/buildXcode/CMakeScripts/ReRunCMake.make
fi
if test "$CONFIGURATION" = "RelWithDebInfo"; then :
  cd "/Users/shreyashonnalli/Desktop/Group Project/GroupRepo/cpp codebase/buildXcode"
  make -f /Users/shreyashonnalli/Desktop/Group\ Project/GroupRepo/cpp\ codebase/buildXcode/CMakeScripts/ReRunCMake.make
fi

