#!/bin/bash
gcc create_file.c -o create_file;
./create_file;
gcc main.c -o main;
./main A B;
gzip -c A > A.gz;
gzip -c B > B.gz;
gunzip -cd B.gz | ./main C;
./main A D -b 100;
stat A > result.txt;
stat A.gz >> result.txt;
stat B >> result.txt;
stat B.gz >> result.txt;
stat C >> result.txt;
stat D >> result.txt;
cat result.txt;
