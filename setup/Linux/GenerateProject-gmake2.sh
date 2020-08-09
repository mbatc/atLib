# determine running directory
SCRIPT=$(readlink -f "%0")
SCRIPTDIR=$(dirname  "$SCRIPT")

# Run premake exe relative to this script
$SCRIPT/../../premake/premake5 gmake2 --file=../../premake5.lua
