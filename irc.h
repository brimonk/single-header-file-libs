#ifndef IRC_H
#define IRC_H

// Brian Chrzanowski
//
// LICENSE information at the bottom of the file.
//
// USAGE
//
// In **one** C/C++ file, do the following:
//
//   #define IRC_IMPLEMENTATION
//   #include "irc.h"
//
// This will include the IRC implementation in that translation unit.
//
// TODO / BRAINSTORM
//
// - platform socketing
//   - TCP connect/disconnect non-blocking
//   - send/receive
//   - error handling + timeouts
//
// - server bind
// - client connect
// - client
//
// 1. write client usage code
// 2. write server usage code
// 3. write socketing code
// 4. write TLS socketing code
//
// - expose APIs to manually send type-safe-ish messages to servers/clients
//
// RFCs Implemented:
//
// - RFC 1459: Internet Relay Chat Protocol
//   - https://datatracker.ietf.org/doc/html/rfc1459
// - RFC 2810: Internet Relay Chat: Architecture
//   - https://datatracker.ietf.org/doc/html/rfc2810
// - RFC 2811: Internet Relay Chat: Channel Management
//   - https://datatracker.ietf.org/doc/html/rfc2811
// - RFC 2812: Internet Relay Chat: Client Protocol
//   - https://datatracker.ietf.org/doc/html/rfc2812
// - RFC 2813: Internet Relay Chat: Server Protocol
//   - https://datatracker.ietf.org/doc/html/rfc2813
// - RFC 7194: Default Port for IRC via TLS/SSL
//   - https://datatracker.ietf.org/doc/html/rfc7194

// TODO remove common.h
#define COMMON_IMPLEMENTATION
#include "common.h"

// TODO abstract the most minimal portion of this library and use it with the irc library.
// TODO only include the implementation in the implementation section
#define TINYCSOCKET_IMPLEMENTATION
#include "tinycsocket.h"

#include <stdint.h>

typedef struct IRC_Socket {
    int fd;
} IRC_Socket;

typedef enum {
    IRC_RESULT_OK           = 0,
    IRC_RESULT_SOCKETFAIL   = 1,
    IRC_RESULT_BINDFAIL     = 2,
    IRC_RESULT_CONNECTFAIL  = 3,
    IRC_RESULT__TOTAL
} IRC_Result;

typedef struct IRC_ServerConfig {
    u16 port;
} IRC_ServerConfig;

typedef struct IRC_ClientConfig {
    char *address;
    u16 port;
} IRC_ClientConfig;

typedef struct IRC_Server {
    IRC_ServerConfig config;
    TcsSocket socket;
} IRC_Server;

typedef struct IRC_Client {
    IRC_ClientConfig config;
    TcsSocket socket;
} IRC_Client;

// IRC_ServerStart: starts a server according to the configuration
IRC_Result IRC_ServerStart(IRC_ServerConfig config);

#ifdef IRC_IMPLEMENTATION

static int sockets_init_count = 0;

static void IRC__InitSockets(void)
{
    if (sockets_init_count >= 0) {
        tcs_lib_init();
        sockets_init_count++;
    }
}

static void IRC__ShutdownSockets(void)
{
    if (sockets_init_count > 0) {
        tcs_lib_cleanup();
        sockets_init_count--;
    }
}

IRC_Result IRC_StartServer(IRC_Server *server, IRC_ServerConfig config)
{
    TcsResult tresult;

    IRC__InitSockets();

    struct TcsAddress taddress = TCS_ADDRESS_NONE;
    taddress.family = TCS_FAMILY_IPV4;
    taddress.data.ipv4.address = TCS_ADDRESS_IPV4_ANY;
    taddress.data.ipv4.port = config.port;

    TcsSocket tsocket = TCS_SOCKET_INVALID;
    tresult = tcs_socket(&tsocket, TCS_FAMILY_IPV4, TCS_SOCKET_STREAM, TCS_PROTOCOL_IP_TCP);
    if (tresult != TCS_SUCCESS) {
        IRC__ShutdownSockets();
        return IRC_RESULT_SOCKETFAIL;
    }

    tresult = tcs_bind(tsocket, &taddress);
    if (tresult != TCS_SUCCESS) {
        tcs_close(&tsocket);
        IRC__ShutdownSockets();
        return IRC_RESULT_BINDFAIL;
    }

    server->config = config;
    server->socket = tsocket;

    return IRC_RESULT_OK;
}

void IRC_ShutdownServer(IRC_Server *server)
{
    if (server == NULL) return;
    tcs_shutdown(server->socket, TCS_SHUTDOWN_BOTH);
    tcs_close(&server->socket);
    IRC__ShutdownSockets();
}

IRC_Result IRC_ConnectClient(IRC_Client *client, IRC_ClientConfig config)
{
    TcsResult tresult;

    IRC__InitSockets();

    TcsSocket tsocket = TCS_SOCKET_INVALID;
    tresult = tcs_socket(&tsocket, TCS_FAMILY_IPV4, TCS_SOCKET_STREAM, TCS_PROTOCOL_IP_TCP);
    if (tresult != TCS_SUCCESS) {
        IRC__ShutdownSockets();
        return IRC_RESULT_SOCKETFAIL;
    }

    tresult = tcs_connect_str(tsocket, config.address, config.port);
    if (tresult != TCS_SUCCESS) {
        tcs_close(&tsocket);
        IRC__ShutdownSockets();
        return IRC_RESULT_CONNECTFAIL;
    }

    client->config = config;
    client->socket = tsocket;

    return IRC_RESULT_OK;
}

void IRC_ShutdownClient(IRC_Client *client)
{
    tcs_shutdown(client->socket, TCS_SHUTDOWN_BOTH);
    tcs_close(&client->socket);
    IRC__ShutdownSockets();
}

#endif // IRC_IMPLEMENTATION

#ifdef IRC_IMPLEMENTATION_EXAMPLE
int main(int argc, char **argv)
{
}
#endif // IRC_IMPLEMENTATION_EXAMPLE

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

#endif // IRC_H
