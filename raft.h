#ifndef RAFT_H
#define RAFT_H

// Brian Chrzanowski
//
// LICENSE information at the bottom of the file.
//
// USAGE
//
// In at least **one** C/C++ file, do the following:
//
//   #define RAFT_IMPLEMENTATION
//   #include "raft.h"
//
// This will include the RAFT implementation in that translation unit.
//
// As a user of this library, you are effectively responsible for a few things to get a small,
// distributed system up and running.
//
// 1. 4 callback functions (apply, commit, restore, snapshot).
// 1a. "apply" will receive a single
// 2a. A free and available port on the local machine, if bootstrapping a cluster.
//     OR
// 2b. Knowledge of at least **one** endpoint for a running RAFT server, if joining a new server.
//
// A demonstration program is included **after** the implementation (RAFT_IMPLEMENTATION), and
// before the LICENSE.
//
// GOALS
//
// The goals of this header file is to make it TRIVIAL for anyone to implement a distributed system,
// in any programming language using the RAFT protocol as implemented in this file.
//
// In addition to **just** implementing RAFT and exposing it for all manner of "simple" distributed
// systems, I'm making a few design decisions that are probably pretty silly for a real production
// use-case, but should be useful for experimentation.
//
// 1. When a new member joins a RAFT cluster, the cluster assumes it has never joined before.
//     - It will be sent the entire log from the leader.
// 2. When a member leaves the RAFT cluster, the cluster assumes it will NEVER rejoin again.
//     - It is safe for the cluster to assume all data on that machine is LOST for FOREVER.
//     - If a member rejoins, it is resent the latest snapshot in addition to all log records.
//
// TODO (Brian)
//
// 1. Network Layer 
// 1. Server + Client
// 1. Commands With Overloading
//
// CHANGELOG

// TODO REMOVE COMMON.H DEPENDENCY
// TODO REMOVE STB_DS.H DEPENDENCY

#include "common.h"

#include <stdint.h>

// CONFIGURATION DEFAULTS

#define RAFT_DEFAULTCONFIG_ELECTIONTIMEOUTMS     150
#define RAFT_DEFAULTCONFIG_HEARTBEATINTERVALMS    50
#define RAFT_DEFAULTCONFIG_SNAPSHOTTHRESHOLD      16

typedef enum {
    RAFTMESSAGE_NONE                     = 0,
    RAFTMESSAGE_REQ_VOTE                 = 1,
    RAFTMESSAGE_RESP_VOTE                = 2,
    RAFTMESSAGE_REQ_APPENDENTRY          = 3,
    RAFTMESSAGE_RESP_APPENDENTRY         = 4,
    RAFTMESSAGE_REQ_INSTALLSNAPSHOT      = 5,
    RAFTMESSAGE_RESP_INSTALLSNAPSHOT     = 6,
    RAFTMESSAGE_REQ_ADDSERVER            = 7,
    RAFTMESSAGE_RESP_ADDSERVER           = 8,
    RAFTMESSAGE_REQ_REMOVESERVER         = 9,
    RAFTMESSAGE_RESP_REMOVESERVER        = 10,
    RAFTMESSAGE_REQ_CLIENTREQUEST        = 11,
    RAFTMESSAGE_RESP_CLIENTREQUEST       = 12,
    RAFTMESSAGE_REQ_FORWARD              = 13,
    RAFTMESSAGE_RESP_FORWARD             = 14,
    RAFTMESSAGE_REQ_LEADERREDIRECT       = 15,
    RAFTMESSAGE_REQ_GETRAFTID            = 16,
    RAFTMESSAGE_RESP_GETRAFTID           = 17,
    RAFTMESSAGE_END                      = 100
} RaftMessageType;

enum {
    RAFT_RESULT_OK    = 0,
    RAFT_RESULT_ERROR = 1,
};

typedef i64 RaftId;
typedef i64 RaftResult;
typedef u64 RaftMessageMagic;

typedef struct RaftMessage {
    RaftMessageMagic magic;
    RaftMessageType type;
    RaftId from;
    RaftId term;

    union {
        struct {
        } VoteRequest;

        struct {
        } VoteResponse;

        struct {
        } AppendEntryRequest;

        struct {
        } AppendEntryResponse;

        struct {
        } InstallSnapshotRequest;

        struct {
        } InstallSnapshotResponse;

        struct {
        } AddServerRequest;

        struct {
        } AddServerResponse;

        struct {
        } RemoveServerRequest;

        struct {
        } RemoveServerResponse;

        struct {
        } ClientRequestRequest;

        struct {
        } ClientRequestResponse;

        struct {
        } ForwardRequest;

        struct {
        } ForwardResponse;

        struct {
        } LeaderRedirect;
    };
} RaftMessage;

// RaftConfiguration: 
typedef struct RaftConfiguration {
    i32 election_timeout_ms;
    i32 heartbeat_interval_ms;
    i32 snapshot_threshold;
} RaftConfiguration;

// RaftBuffer: just an allocated pointer with a length. Users SHOULD NOT store a pointer to this or
// any internal data structures. It is assumed that this *may* be free'd immediately upon return
// from a callback function.
typedef struct RaftBuffer {
    void *data;
    size_t size;
} RaftBuffer;

// RaftStateMachine: callbacks user must implement to implement RAFT.
typedef struct RaftStateMachine {
    RaftResult (*apply)(void *, RaftBuffer);
    RaftResult (*snapshot)(void *, RaftBuffer **);
    RaftResult (*restore)(void *, RaftBuffer);
    void (*commit)(void *, RaftResult);
    void *context;
} RaftStateMachine;

typedef struct RaftSocket {
    RaftId   server_id;
    int      socket;
} RaftSocket;

// RaftServer: contains all of the state for a given server, or a member of the RAFT cluster.
typedef struct RaftServer {
    RaftId   term;
    RaftId   candidtate_id;
    RaftId   voted_for;      // < 0 if none
    RaftId   last_log_index;
    RaftId   last_log_term;

    RaftConfiguration config;
    RaftStateMachine state_machine;

    RaftSocket sock; // the socket this instance is bound to
    RaftSocket *sockets; // our peer sockets
} RaftServer;

// RAFT_MembersForConsensus: returns the number of machines for this cluster that need to vote for
// a majority consensus.
u32 RAFT_MembersForConsensus(u32 count);

// RAFT_GetDefaultConfig: returns a default configuration object
RaftConfiguration RAFT_GetDefaultConfig();

// RAFT_CreateBuffer: create a buffer object
RaftBuffer *RAFT_CreateBuffer(void *ptr, size_t size);

// RAFT_FreeBuffer: release a buffer object
void RAFT_FreeBuffer(RaftBuffer *buffer);

// RAFT_Create: creates a server object given a config and a state machine
RaftServer *RAFT_Create(RaftConfiguration config, RaftStateMachine state_machine);

// RAFT_BindServer: binds and creates a RaftServer object.
RaftResult RAFT_BindServer(RaftServer *server, u16 port);

// RAFT_Join: attempts to join the RAFT cluster with one existing member at 'address'.
RaftResult RAFT_Join(RaftServer *server, const char *address);

// RAFT_Propose: attempts to propose the message to the cluster
RaftResult RAFT_Propose(RaftServer *server, void *message, size_t size);

// RAFT_Tick: services all callbacks and handles network I/O.
void RAFT_Tick(RaftServer *server);

// RAFT_Leave: designates this RAFT server to leave the current cluster configuration.
void RAFT_Leave(RaftServer *server);

// RAFT_Free: this will deallocate and release all of the resources for the RaftServer.
void RAFT_Free(RaftServer *server);

// RAFT_LeaveAndFree: combo API that calls RAFT_Leave and immediately calls RAFT_Free.
void RAFT_LeaveAndFree(RaftServer *server);

#ifdef RAFT_IMPLEMENTATION

#define COMMON_IMPLEMENTATION
#include "common.h"

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <sys/un.h>
#include <netdb.h>

u32 RAFT_MembersForConsensus(u32 count)
{
    // = 2 * n + 1
    // ...
    // = (count - 1) / 2

    return (count - 1) / 2;
}

RaftConfiguration RAFT_GetDefaultConfig()
{
    RaftConfiguration config = {
        .election_timeout_ms   = RAFT_DEFAULTCONFIG_ELECTIONTIMEOUTMS,
        .heartbeat_interval_ms = RAFT_DEFAULTCONFIG_HEARTBEATINTERVALMS,
        .snapshot_threshold    = RAFT_DEFAULTCONFIG_SNAPSHOTTHRESHOLD
    };
    return config;
}

RaftServer *RAFT_Create(RaftConfiguration config, RaftStateMachine state_machine)
{
    RaftServer *server = calloc(1, sizeof(*server));
    if (server) {
        server->config = config;
        server->state_machine = state_machine;
    }
    return server;
}

RaftResult RAFT_OpenSocket(RaftSocket *sock, const char *node, const char *service)
{
    int rc;

    assert(sock != NULL);

    struct addrinfo hints = { 0 }, *res = NULL;

    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM; 
    hints.ai_flags = AI_PASSIVE;

    if ((rc = getaddrinfo(node, service, &hints, &res)) < 0) {
        return RAFT_RESULT_ERROR;
    }

    memset(sock, 0, sizeof(*sock));

    sock->socket = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (sock->socket < 0) {
        return RAFT_RESULT_ERROR;
    }

    return RAFT_RESULT_OK;
}

RaftResult RAFT_BindSocket(RaftSocket *sock)
{
    struct sockaddr_un addr = { 0 };
    int rc;

    rc = bind(sock->socket, (struct sockaddr *)&addr, sizeof(addr));

    return rc == -1 ? RAFT_RESULT_ERROR : RAFT_RESULT_OK;
}

RaftResult RAFT_CloseSocket(RaftSocket *sock)
{
    close(sock->socket);
    return RAFT_RESULT_OK;
}

RaftResult RAFT_WriteSocket(RaftSocket *socket, void *data, size_t size)
{
    return RAFT_RESULT_OK;
}

RaftResult RAFT_ReadSocket(RaftSocket *socket, void *data, size_t size)
{
    return RAFT_RESULT_OK;
}

RaftResult RAFT_BindServer(RaftServer *server, u16 port)
{
    RaftResult result;
    char service[32] = { 0 };

    snprintf(service, sizeof service, "%hu", port); // WARN

    RaftSocket sock = { 0 };

    result = RAFT_OpenSocket(&sock, NULL, service);
    if (result != RAFT_RESULT_OK) {
        return result;
    }

    result = RAFT_BindSocket(&sock);
    if (result != RAFT_RESULT_OK) {
        RAFT_CloseSocket(&sock);
        return result;
    }

    server->sock = sock;

    return result;
}

RaftResult RAFT_Join(RaftServer *server, const char *address)
{
    // TODO join an existing cluster - probably blocking on joining

    char buf[64] = { 0 };
    strncpy(buf, address, sizeof buf); // WARN

    char *t = strchr(buf, ':');
    assert(t);

    *t = 0;
    t++;

    RaftSocket sock = { 0 };
    RaftResult result = RAFT_OpenSocket(&sock, buf, t);
    if (result != RAFT_RESULT_OK) {
        return RAFT_RESULT_ERROR;
    }

    arrput(server->sockets, sock);

    return RAFT_RESULT_OK;
}

RaftResult RAFT_Propose(RaftServer *server, void *message, size_t size)
{
    return RAFT_RESULT_OK;
}

void RAFT_Tick(RaftServer *server)
{
    // TODO
}

void RAFT_Leave(RaftServer *server)
{
    // TODO Send the leave message to the cluster.
    // TODO Probably block here until we get an ACK from our peers?? Hmm...

    for (int i = 0; i < arrlen(server->sockets); i++)
        RAFT_CloseSocket(&server->sockets[i]);
    RAFT_CloseSocket(&server->sock);
}

void RAFT_Free(RaftServer *server)
{
    free(server);
}

void RAFT_LeaveAndFree(RaftServer *server)
{
    RAFT_Leave(server);
    RAFT_Free(server);
}

RaftBuffer *RAFT_CreateBuffer(void *ptr, size_t size)
{
    RaftBuffer *buffer = calloc(1, sizeof(*buffer));
    if (buffer) {
        buffer->data = malloc(size);
        if (buffer->data == NULL) {
            free(buffer);
            return NULL;
        }
        memcpy(buffer->data, ptr, size);
        buffer->size = size;
    }
    return buffer;
}

#endif // RAFT_IMPLEMENTATION

#ifdef RAFT_IMPLEMENTATION_EXAMPLE

// NOTE This example program implements a very simple integer counting service. The keys are
// integers, and the values can be incremented to INT_MAX, and even to an overflow. The keys can be
// removed from the service via sending an OPERATION_DELETE message to the cluster.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include <unistd.h>
#include <errno.h>

#define USAGE "USAGE:  %s <endpoint>\n" \
              "USAGE:  %s --bootstrap <port>\n"

#define MAX_KEYS 256

static bool run_program = true;

typedef struct KVEntry {
    int value;
    int count;
    int in_use;
} KVEntry;

typedef struct KVState {
    KVEntry entries[MAX_KEYS];
} KVState;

KVEntry *KV_Find(KVState *state, int value)
{
    for (int i = 0; i < MAX_KEYS; i++) {
        if (state->entries[i].in_use && state->entries[i].value == value) {
            return &state->entries[i];
        }
    }
    return NULL;
}

KVEntry *KV_FindFree(KVState *state)
{
    for (int i = 0; i < MAX_KEYS; i++) {
        if (!state->entries[i].in_use) {
            state->entries[i].in_use = true;
            return &state->entries[i];
        }
    }
    return NULL;
}

enum {
    OPERATION_DELETE = 0,
    OPERATION_INC    = 1
};

typedef struct Message {
    int operation;
    int value;
} Message;

RaftResult apply(void *context, RaftBuffer buffer)
{
    // NOTE "apply" in RAFT is simply an operation we're applying to the state machine.

    assert(context);

    KVState *state = (KVState *)context;
    Message *message = (Message *)buffer.data;

    if (message->operation == OPERATION_INC) {
        KVEntry *entry = KV_FindFree(state);

        if (entry == NULL) {
            return RAFT_RESULT_OK;
        } else {
            return RAFT_RESULT_ERROR;
        }

    } else if (message->operation == OPERATION_DELETE) {
        KVEntry *entry = KV_Find(state, message->value);

        if (entry == NULL) {
            return RAFT_RESULT_ERROR;
        } else {
            memset(entry, 0, sizeof(*entry));
            return RAFT_RESULT_OK;
        }
    } else {
        return RAFT_RESULT_ERROR;
    }
}

RaftResult snapshot(void *context, RaftBuffer **output)
{
    KVState *state = (KVState *)context;

    RaftBuffer *buf = RAFT_CreateBuffer(state, sizeof(*state));

    if (buf == NULL) {
        return RAFT_RESULT_ERROR;
    } else {
        *output = buf;
        return RAFT_RESULT_OK;
    }
}

RaftResult restore(void *context, RaftBuffer buffer)
{
    KVState *state = (KVState *)context;

    if (buffer.size != sizeof(*state)) {
        return RAFT_RESULT_ERROR;
    } else {
        memcpy(state, buffer.data, buffer.size);
        return RAFT_RESULT_OK;
    }
}

void commit(void *context, RaftResult result)
{
}

int main(int argc, char **argv)
{
    if (argc < 2) {
        fprintf(stderr, USAGE, argv[0], argv[0]);
        exit(1);
    }

    KVState state = { 0 };

    RaftStateMachine rsm = {
        .apply = apply,
        .snapshot = snapshot,
        .restore = restore,
        .commit = commit,
        .context = &state
    };

    RaftConfiguration config = RAFT_GetDefaultConfig();

    RaftServer *server = RAFT_Create(config, rsm);
    assert(server != NULL);

    if (strcmp(argv[1], "--bootstrap") == 0) {
        // NOTE handle issues better than this
        errno = 0;
        long result = (u16)strtoul(argv[2], NULL, 10);
        assert(errno != ERANGE);
        assert(errno != EINVAL);
        assert(1 <= result && result <= ((1 << 16) - 1));
        RAFT_BindServer(server, (u16)result);
    } else {
        RAFT_Join(server, argv[1]);
    }

    while (run_program) {
        int elapsed_ms = 10;
        RAFT_Tick(server);
        usleep(elapsed_ms * 1000);
    }

    RAFT_LeaveAndFree(server);
}
#endif // RAFT_IMPLEMENTATION_EXAMPLE

// LICENSE
//
// Copyright 2026 Brian Chrzanowski
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and
// associated documentation files (the “Software”), to deal in the Software without restriction,
// including without limitation the rights to use, copy, modify, merge, publish, distribute,
// sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all copies or
// substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT
// NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
// NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
// DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH

#endif // RAFT_H
