#!/bin/sh
# indent.sh: A little something something to make everybody's life easier :-)
# Explanation of switches:
# -kr	K&R-style (inb4 Zed Shaw) with ONE EXCEPTION...
# -brf	Put braces on function definition line. In pre-ANSI C, type
# 	declarations of parameters came in between the right parentheses and
#	left curly brace. That was the era the first edition of K&R was 
#	written in and the new-line-brace was indoubtably habitual for them.
#	It's kinda retarded to do it now, so let's not.
# -ts4	Treat tabs like four spaces. This is just to determine which column is 
#	column 80.
indent -kr -brf -ts4 src/*.[ch]
