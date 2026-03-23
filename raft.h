#ifndef RAFT_H
#define RAFT_H

#ifdef RAFT_IMPLEMENTATION
#endif // RAFT_IMPLEMENTATION

#ifdef RAFT_IMPLEMENTATION_EXAMPLE
#include <stdio.h>
int main(int argc, char **argv)
{
    __int128 some_value = 5;
    printf("%d\n", (int)some_value);
}
#endif // RAFT_IMPLEMENTATION_EXAMPLE

#endif // RAFT_H
