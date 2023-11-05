if [ -z "$1" ]
then
    echo "Enter the day as the argument number"
else
    cd solutions && clang++-13 -g -o exec "aoc$1.cpp" && ./exec
fi
