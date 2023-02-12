# Copy in Square.
## Or research on copy/move semantics in c++;

### Introduction.
###### No. It's just lirycs.
I'd been very glad when I knew about overloading. So I've decided to write long arithmetics in C++.
*Imagine that I wrote class Num performing long aritmetics.* 
As for test I decided to solve square equations. But programm worked too slow. I've been searching for ages to optimize, but that gave nothing.
One old man adviced me: "Too many copies". That was all he'd sad - he had to go rest. So I've decided to research this problem.

### See the beast. 
To examine about copies I wrote simple wrapper class. And logged 
Lets create a template wrapper watcher class to check how many copying was done and amount of tmp objects. So I've got next image:




<!-- idea ptr to rgb. -->
<!-- legened of graph -->