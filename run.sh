name=$(basename $(ls *.cpp) .cpp)

g++ $name.cpp -o $name.bin
./$name.bin