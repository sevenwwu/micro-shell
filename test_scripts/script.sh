echo showshift is named $0
echo Number of arguments is $#.
echo Argument 1 is $1.
echo Argument 2 is $2.
echo Argument 3 is $3.
echo Argument 4 is $4.
shift 3
echo Number of arguments is $#.
echo Argument 1 is $1.
echo Argument 2 is $2.
echo Argument 3 is $3.
echo Argument 4 is $4.
unshift 1
echo Number of arguments is $#.
echo Argument 1 is $1.
unshift
echo Number of arguments is $#.
echo Now $1 is Argument 1.