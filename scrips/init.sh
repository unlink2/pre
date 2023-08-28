#!/bin/bash

# rename to project specific values
NAME="pre"
CONST="PRE"
STRUCT="Pre"
FN="pre"

# will be replaced
PRE_NAME="pre"
PRE_STRUCT="Pre"
PRE_CONST="PRE"
PRE_FN="pre"

function replace() {
	echo "Replacing $1 with $2"
	find ./ -type f -name '*.c' -exec sed -i "s/$1/$2/g" {} \;
	find ./ -type f -name '*.h' -exec sed -i "s/$1/$2/g" {} \;
	find ./ -type f -name '*.md' -exec sed -i "s/$1/$2/g" {} \;
	find ./ -type f -name '*.lua' -exec sed -i "s/$1/$2/g" {} \;
	find ./ -type f -name '*.sh' -exec sed -i "s/$1/$2/g" {} \;
	find ./doc/ -type f -name '*' -exec sed -i "s/$1/$2/g" {} \;
}

replace $PRE_NAME $NAME
replace $PRE_CONST $CONST
replace $PRE_STRUCT $STRUCT
replace $PRE_FN $FN

mv "include/$PRE_NAME.h" "include/$NAME.h"
mv "src/$PRE_NAME.c" "src/$NAME.c"
mv "doc/$PRE_NAME" "doc/$NAME" 
