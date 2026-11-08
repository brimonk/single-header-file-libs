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
// TODO IRC
//
// - parse message
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

#define IRC_DEFAULTPORT_PLAINTEXT    6667
#define IRC_DEFAULTPORT_TLS          6697

#define IRC_READ_BUFFER_SIZE          512

// SOURCE: RFC-1459 Section 6.1
#define IRC_ERR_NOSUCHNICK        401
#define IRC_ERR_NOSUCHSERVER      402
#define IRC_ERR_NOSUCHCHANNEL     403
#define IRC_ERR_CANNOTSENDTOCHAN  404
#define IRC_ERR_TOOMANYCHANNELS   405
#define IRC_ERR_WASNOSUCHNICK     406
#define IRC_ERR_TOOMANYTARGETS    407
#define IRC_ERR_NOORIGIN          409
#define IRC_ERR_NORECIPIENT       411
#define IRC_ERR_NOTEXTTOSEND      412
#define IRC_ERR_NOTOPLEVEL        413
#define IRC_ERR_WILDTOPLEVEL      414
#define IRC_ERR_UNKNOWNCOMMAND    421
#define IRC_ERR_NOMOTD            422
#define IRC_ERR_NOADMININFO       423
#define IRC_ERR_FILEERROR         424
#define IRC_ERR_NONICKNAMEGIVEN   431
#define IRC_ERR_ERRONEUSNICKNAME  432
#define IRC_ERR_NICKNAMEINUSE     433
#define IRC_ERR_NICKCOLLISION     436
#define IRC_ERR_USERNOTINCHANNEL  441
#define IRC_ERR_NOTONCHANNEL      442
#define IRC_ERR_USERONCHANNEL     443
#define IRC_ERR_NOLOGIN           444
#define IRC_ERR_SUMMONDISABLED    445
#define IRC_ERR_USERSDISABLED     446
#define IRC_ERR_NOTREGISTERED     451
#define IRC_ERR_NEEDMOREPARAMS    461
#define IRC_ERR_ALREADYREGISTRED  462
#define IRC_ERR_NOPERMFOHOST      463
#define IRC_ERR_PASSWDMISMATCH    464
#define IRC_ERR_YOUREBANNEDCREEP  465
#define IRC_ERR_KEYSET            467
#define IRC_ERR_CHANNELISFULL     471
#define IRC_ERR_UNKNOWNMODE       472
#define IRC_ERR_INVITEONLYCHAN    473
#define IRC_ERR_BANNEDFROMCHAN    474
#define IRC_ERR_BADCHANNELKEY     475
#define IRC_ERR_NOPRIVILEGES      481
#define IRC_ERR_CHANOPRIVSNEEDED  482
#define IRC_ERR_CANTKILLSERVER    483
#define IRC_ERR_NOOPERHOST        491
#define IRC_ERR_UMODEUNKNOWNFLAG  501
#define IRC_ERR_USERSDONTMATCH    502

// SOURCE: RFC-1459 Section 6.2
#define IRC_RPL_NONE              300
#define IRC_RPL_USERHOST          302
#define IRC_RPL_ISON              303
#define IRC_RPL_AWAY              301
#define IRC_RPL_UNAWAY            305
#define IRC_RPL_NOWAWAY           306
#define IRC_RPL_WHOISUSER         311
#define IRC_RPL_WHOISSERVER       312
#define IRC_RPL_WHOISOPERATOR     313
#define IRC_RPL_WHOISIDLE         317
#define IRC_RPL_ENDOFWHOIS        318
#define IRC_RPL_WHOISCHANNELS     319
#define IRC_RPL_WHOWASUSER        314
#define IRC_RPL_ENDOFWHOWAS       369
#define IRC_RPL_LISTSTART         321
#define IRC_RPL_LIST              322
#define IRC_RPL_LISTEND           323
#define IRC_RPL_CHANNELMODEIS     324
#define IRC_RPL_NOTOPIC           331
#define IRC_RPL_TOPIC             332
#define IRC_RPL_INVITING          341
#define IRC_RPL_SUMMONING         342
#define IRC_RPL_VERSION           351
#define IRC_RPL_WHOREPLY          352
#define IRC_RPL_ENDOFWHO          315
#define IRC_RPL_NAMEREPLY         353
#define IRC_RPL_ENDOFNAMES        366
#define IRC_RPL_LINKS             364
#define IRC_RPL_ENDOFLINKS        365
#define IRC_RPL_BANLIST           367
#define IRC_RPL_ENDOFBANLIST      368
#define IRC_RPL_INFO              371
#define IRC_RPL_ENDOFINFO         374
#define IRC_RPL_MOTDSTART         375
#define IRC_RPL_MOTD              372
#define IRC_RPL_ENDOFMOTD         376
#define IRC_RPL_YOUREOPER         381
#define IRC_RPL_REHASHING         382
#define IRC_RPL_TIME              391
#define IRC_RPL_USERSSTART        392
#define IRC_RPL_USERS             393
#define IRC_RPL_ENDOFUSERS        394
#define IRC_RPL_NOUSERS           395
#define IRC_RPL_TRACELINK         200
#define IRC_RPL_TRACECONNECTING   201
#define IRC_RPL_TRACEHANDSHAKE    202
#define IRC_RPL_TRACEUNKNOWN      203
#define IRC_RPL_TRACEOPERATOR     204
#define IRC_RPL_TRACEUSER         205
#define IRC_RPL_TRACESERVER       206
#define IRC_RPL_TRACENEWTYPE      208
#define IRC_RPL_TRACELOG          261
#define IRC_RPL_STATSLINKINFO     211
#define IRC_RPL_STATSCOMMANDS     212
#define IRC_RPL_STATSCLINE        213
#define IRC_RPL_STATSNLINE        214
#define IRC_RPL_STATSKLINE        215
#define IRC_RPL_STATSYLINE        218
#define IRC_RPL_ENDOFSTATS        219
#define IRC_RPL_STATSLLINE        241
#define IRC_RPL_STATSUPTIME       242
#define IRC_RPL_STATSOLINE        243
#define IRC_RPL_STATSHLINE        244
#define IRC_RPL_UMODEIS           221
#define IRC_RPL_LUSERCLIENT       251
#define IRC_RPL_LUSEROP           252
#define IRC_RPL_LUSERUNKNOWN      253
#define IRC_RPL_LUSERCHANNELS     254
#define IRC_RPL_LUSERME           255
#define IRC_RPL_ADMINME           256
#define IRC_RPL_ADMINLOC1         257
#define IRC_RPL_ADMINLOC2         258
#define IRC_RPL_ADMINEMAIL        259

// TODO Should we include the reserved numerics somewhere in here?

typedef struct IRC_String8 {
    char *ptr;
    u32 len;
} IRC_String8;

typedef enum {
    IRC_RESULT_OK           = 0,
    IRC_RESULT_SOCKETFAIL   = 1,
    IRC_RESULT_BINDFAIL     = 2,
    IRC_RESULT_CONNECTFAIL  = 3,
    IRC_RESULT_PARAMERR     = 4,
    IRC_RESULT_PARSEERR     = 5,
    IRC_RESULT_RECVERR      = 6,
    IRC_RESULT__TOTAL
} IRC_Result;

struct IRC_Message;
typedef struct IRC_Message IRC_Message;

typedef enum {
    IRC_CALLBACK__START     =  0,
    IRC_CALLBACK_NICK       =  0,
    IRC_CALLBACK_PASS       =  1,
    IRC_CALLBACK_USER       =  2,
    IRC_CALLBACK_SERVER     =  3,
    IRC_CALLBACK_OPER       =  4,
    IRC_CALLBACK_QUIT       =  5,
    IRC_CALLBACK_SQUIT      =  6,
    IRC_CALLBACK_JOIN       =  7,
    IRC_CALLBACK_MODE       =  8,
    IRC_CALLBACK_TOPIC      =  9,
    IRC_CALLBACK_NAMES      = 10,
    IRC_CALLBACK_LIST       = 11,
    IRC_CALLBACK_INVITE     = 12,
    IRC_CALLBACK_KICK       = 13,
    IRC_CALLBACK_VERSION    = 14,
    IRC_CALLBACK_STATS      = 15,
    IRC_CALLBACK_LINKS      = 16,
    IRC_CALLBACK_TIME       = 17,
    IRC_CALLBACK_CONNECT    = 18,
    IRC_CALLBACK_TRACE      = 19,
    IRC_CALLBACK_ADMIN      = 20,
    IRC_CALLBACK_INFO       = 21,
    IRC_CALLBACK_PRIVMSG    = 22,
    IRC_CALLBACK_NOTICE     = 23,
    IRC_CALLBACK_WHO        = 24,
    IRC_CALLBACK_WHOIS      = 25,
    IRC_CALLBACK_WHOWAS     = 26,
    IRC_CALLBACK_KILL       = 27,
    IRC_CALLBACK_PING       = 28,
    IRC_CALLBACK_PONG       = 29,
    IRC_CALLBACK_ERROR      = 30,
    IRC_CALLBACK_AWAY       = 31,
    IRC_CALLBACK_REHASH     = 32,
    IRC_CALLBACK_RESTART    = 33,
    IRC_CALLBACK_SUMMON     = 34,
    IRC_CALLBACK_USERS      = 35,
    IRC_CALLBACK_WALLOPS    = 36,
    IRC_CALLBACK_USERHOST   = 37,
    IRC_CALLBACK_ISON       = 38,
    IRC_CALLBACK__TOTAL
} IRC_CallbackKind;

typedef struct IRC_ServerConfig {
    u16 port;
    i32 use_callbacks;
    IRC_Result (*fns[IRC_CALLBACK__TOTAL])(IRC_Message *);
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

enum {
    IRC_MESSAGEPART_PREFIX,
    IRC_MESSAGEPART_COMMAND,
    IRC_MESSAGEPART_SERVERNAME,
    IRC_MESSAGEPART_NICKNAME,
    IRC_MESSAGEPART_USERNAME,
    IRC_MESSAGEPART_HOSTNAME,
    IRC_MESSAGEPART__TOTAL
};

typedef struct IRC_Message {
    char *strings[IRC_MESSAGEPART__TOTAL];
} IRC_Message;

// IRC_ServerStart: starts a server according to the configuration
IRC_Result IRC_ServerStart(IRC_ServerConfig config);

#ifdef IRC_IMPLEMENTATION

static int IRC__SOCKETS_INIT_COUNT = 0;

static void IRC__InitSockets(void)
{
    if (IRC__SOCKETS_INIT_COUNT >= 0) {
        tcs_lib_init();
        IRC__SOCKETS_INIT_COUNT++;
    }
}

static void IRC__ShutdownSockets(void)
{
    if (IRC__SOCKETS_INIT_COUNT > 0) {
        tcs_lib_cleanup();
        IRC__SOCKETS_INIT_COUNT--;
    }
}

IRC_Result IRC_ParseMessage(IRC_Message *message, char *buffer, size_t buffer_size)
{
    assert(message != NULL);
    assert(buffer != NULL);

    return IRC_RESULT_OK;
}

IRC_Result IRC_SetServerCallback(IRC_ServerConfig *config, IRC_CallbackKind kind, IRC_Result (*fn)(IRC_Message *))
{
    if (config == NULL || fn == NULL || !(IRC_CALLBACK__START <= kind && kind <= IRC_CALLBACK__TOTAL)) {
        return IRC_RESULT_PARAMERR;
    }

    return IRC_RESULT_OK;
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

IRC_Result IRC_ListenServerFetchMessage(IRC_Server *server, IRC_Message *message)
{
    // NOTE RFC-1459 suggests that a buffer of 512 bytes is sufficient for one or more IRC messages.
    uint8_t buffer[IRC_READ_BUFFER_SIZE] = { 0 };

    size_t received_size = 0;
    TcsResult tresult;
    IRC_Result iresult;

    if (server == NULL || message == NULL) {
        return IRC_RESULT_PARAMERR;
    }

    tresult = tcs_receive(server->socket, buffer, sizeof buffer, 0, &received_size);
    if (tresult != TCS_SUCCESS) {
        return IRC_RESULT_RECVERR;
    }

    IRC_Message internal_message = { 0 };

    iresult = IRC_ParseMessage(&internal_message, (char *)buffer, received_size);
    if (iresult != IRC_RESULT_OK) {
        return IRC_RESULT_PARSEERR;
    }

    *message = internal_message;

    return IRC_RESULT_OK;
}

IRC_Result IRC_ListenServerWithCallbacks(IRC_Server *server)
{
    IRC_Message message = { 0 };

    IRC_Result iresult = IRC_ListenServerFetchMessage(server, &message);
    if (iresult != IRC_RESULT_OK) {
        return iresult;
    }

    struct {
        IRC_CallbackKind kind;
        char *string;
    } function_map[] = {
        { IRC_CALLBACK_NICK,     "nick" },
        { IRC_CALLBACK_PASS,     "pass" },
        { IRC_CALLBACK_USER,     "user" },
        { IRC_CALLBACK_SERVER,   "server" },
        { IRC_CALLBACK_OPER,     "oper" },
        { IRC_CALLBACK_QUIT,     "quit" },
        { IRC_CALLBACK_SQUIT,    "squit" },
        { IRC_CALLBACK_JOIN,     "join" },
        { IRC_CALLBACK_MODE,     "mode" },
        { IRC_CALLBACK_TOPIC,    "topic" },
        { IRC_CALLBACK_NAMES,    "names" },
        { IRC_CALLBACK_LIST,     "list" },
        { IRC_CALLBACK_INVITE,   "invite" },
        { IRC_CALLBACK_KICK,     "kick" },
        { IRC_CALLBACK_VERSION,  "version" },
        { IRC_CALLBACK_STATS,    "stats" },
        { IRC_CALLBACK_LINKS,    "links" },
        { IRC_CALLBACK_TIME,     "time" },
        { IRC_CALLBACK_CONNECT,  "connect" },
        { IRC_CALLBACK_TRACE,    "trace" },
        { IRC_CALLBACK_ADMIN,    "admin" },
        { IRC_CALLBACK_INFO,     "info" },
        { IRC_CALLBACK_PRIVMSG,  "privmsg" },
        { IRC_CALLBACK_NOTICE,   "notice" },
        { IRC_CALLBACK_WHO,      "who" },
        { IRC_CALLBACK_WHOIS,    "whois" },
        { IRC_CALLBACK_WHOWAS,   "whowas" },
        { IRC_CALLBACK_KILL,     "kill" },
    };

    for (i32 i = 0; i < ARRSIZE(function_map); i++) {
        if (strieq(message.strings[IRC_MESSAGEPART_COMMAND], function_map[i].string)) {
            iresult = server->config.fns[function_map[i].kind](&message);
            break;
        }
    }

    return iresult;
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
    if (client == NULL) return;
    tcs_shutdown(client->socket, TCS_SHUTDOWN_BOTH);
    tcs_close(&client->socket);
    IRC__ShutdownSockets();
}

#endif // IRC_IMPLEMENTATION

#ifdef IRC_IMPLEMENTATION_EXAMPLE

#define PORT     7000

void DoServer(void)
{
}

void DoClient(void)
{
}

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
