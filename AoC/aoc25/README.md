For 2025, I decided to use C++ with standard libraries allowed. I used generic
release compiler flags (shown below) for personal speed benchmarking. I have
only provided the test inputs to respect the AoC guidelines.

```
g++ -o main day*.cc -O3
```
Day 10 was unique in that I used an external library (`libglpk`) for its ILP/MIP
capabilities. I also proposed three solutions with significant speed increases
for the first part.

Day 12 does not work for the test input, but it does for the real input. If you
want to test these solutions out for yourself, you should login and get a real
puzzle input.
