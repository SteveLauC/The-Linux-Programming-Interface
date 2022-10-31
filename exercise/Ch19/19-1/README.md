#### Question

Write a program that logs all file creations, deletions, and renames under the 
directory named in its command-line argument. The program should monitor events
in all of the subdirectories under the specified directory. To obtain a list of
all of the these subdirectories, you will need to make use of `nftw()`. When a
new directory is added under the tree or a direcotry is deleted, the set of 
monitored subdirectories should be updated accrodingly.

#### Answer
