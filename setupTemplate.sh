#!/usr/bin/bash


cd inputs && cp template.txt "input$1.txt" && cd ../solutions && cp template.cpp "aoc$1.cpp" && cd .. && lvim
