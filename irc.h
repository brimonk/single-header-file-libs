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
// - send PING/PONG messages automatically (we probably don't care about doing that manually)
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

#define IRC_DEFAULTPORT_PLAINTEXT 6667
#define IRC_DEFAULTPORT_TLS       6697

#define IRC_DEFAULT_BUFFER_SIZE       512
#define IRC_USERNAME_MAXCHARS       10

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

typedef enum {
    IRC_CHANNELMODE_CREATOR,
    IRC_CHANNELMODE_CHANOPPRIV,
    IRC_CHANNELMODE_VOICEPRIV,
    IRC_CHANNELMODE_TOGGLEANON,
    IRC_CHANNELMODE_TOGGLEINVITE,
    IRC_CHANNELMODE_TOGGLEMOD,
    IRC_CHANNELMODE_TOGGLEOUTSIDE,
    IRC_CHANNELMODE_TOGGLEQUIET,
    IRC_CHANNELMODE_TOGGLEPRIVATE,
    IRC_CHANNELMODE_TOGGLESECRET,
    IRC_CHANNELMODE_TOGGLESERVREOP,
    IRC_CHANNELMODE_TOGGLETOPIC,
    IRC_CHANNELMODE_CHANNELKEY,
    IRC_CHANNELMODE_USERLIMIT,
    IRC_CHANNELMODE_BANMASK,
    IRC_CHANNELMODE_EXCEPTIONMASK,
    IRC_CHANNELMODE_INVITATIONMASK,
    IRC_CHANNELMODE__TOTAL,
} IRC_ChannelMode;

typedef struct IRC_String8 {
    char *ptr;
    u32 len;
} IRC_String8;

typedef enum {
    IRC_RESULT_OK             = 0,
    IRC_RESULT_SOCKETFAIL     = 1,
    IRC_RESULT_BINDFAIL       = 2,
    IRC_RESULT_CONNECTFAIL    = 3,
    IRC_RESULT_PARAMERR       = 4,
    IRC_RESULT_PARSEERR       = 5,
    IRC_RESULT_SENDERR        = 6,
    IRC_RESULT_RECVERR        = 7,
    IRC_RESULT_INVALIDCOMMAND = 8,
    IRC_RESULT__TOTAL
} IRC_Result;

struct IRC_Message;
typedef struct IRC_Message IRC_Message;

#define IRC_MODEFLAGS_OPER       0x00000001     // 'o'
#define IRC_MODEFLAGS_PRIVATE    0x00000002     // 'p'
#define IRC_MODEFLAGS_SECRET     0x00000004     // 's'
#define IRC_MODEFLAGS_INVITE     0x00000008     // 'i'
#define IRC_MODEFLAGS_NOMSGS     0x00000010     // 'n'
#define IRC_MODEFLAGS_MODERATED  0x00000020     // 'm'
#define IRC_MODEFLAGS_USERLIMIT  0x00000040     // 'l'
#define IRC_MODEFLAGS_BANMASK    0x00000080     // 'b'
#define IRC_MODEFLAGS_SPEAK      0x00000100     // 'v'
#define IRC_MODEFLAGS_KEY        0x00000200     // 'k'
#define IRC_MODEFLAGS_SERVNOTICE 0x00000400     // 's'
#define IRC_MODEFLAGS_WALLOPS    0x00000800     // 'w'

#define IRC_MODEFLAGS_INVIS      0x00000008     // 'i'

#define IRC_MODEFLAGS_CHANNEL    0x10000000     // for changing 'channel' modes
#define IRC_MODEFLAGS_USER       0x20000000     // for changing 'user' modes
#define IRC_MODEFLAGS_NEGATE     0x80000000     // '-'

typedef u32 IRC_ModeFlags;

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
    IRC_CALLBACK_PART       =  8,
    IRC_CALLBACK_MODE       =  9,
    IRC_CALLBACK_TOPIC      = 10,
    IRC_CALLBACK_NAMES      = 11,
    IRC_CALLBACK_LIST       = 12,
    IRC_CALLBACK_INVITE     = 13,
    IRC_CALLBACK_KICK       = 14,
    IRC_CALLBACK_VERSION    = 15,
    IRC_CALLBACK_STATS      = 16,
    IRC_CALLBACK_LINKS      = 17,
    IRC_CALLBACK_TIME       = 18,
    IRC_CALLBACK_CONNECT    = 19,
    IRC_CALLBACK_TRACE      = 20,
    IRC_CALLBACK_ADMIN      = 21,
    IRC_CALLBACK_INFO       = 22,
    IRC_CALLBACK_PRIVMSG    = 23,
    IRC_CALLBACK_NOTICE     = 24,
    IRC_CALLBACK_WHO        = 25,
    IRC_CALLBACK_WHOIS      = 26,
    IRC_CALLBACK_WHOWAS     = 27,
    IRC_CALLBACK_KILL       = 28,
    IRC_CALLBACK_PING       = 29,
    IRC_CALLBACK_PONG       = 30,
    IRC_CALLBACK_ERROR      = 31,
    IRC_CALLBACK_AWAY       = 32,
    IRC_CALLBACK_REHASH     = 33,
    IRC_CALLBACK_RESTART    = 34,
    IRC_CALLBACK_SUMMON     = 35,
    IRC_CALLBACK_USERS      = 36,
    IRC_CALLBACK_WALLOPS    = 37,
    IRC_CALLBACK_USERHOST   = 38,
    IRC_CALLBACK_ISON       = 39,
    IRC_CALLBACK__TOTAL
} IRC_CallbackKind;

typedef struct IRC_ServerConfig {
    u16 port;
    IRC_Result (*default_callback_fns[IRC_CALLBACK__TOTAL])(IRC_Message *);
    IRC_Result (*user_callback_fns[IRC_CALLBACK__TOTAL])(IRC_Message *);
} IRC_ServerConfig;

typedef struct IRC_Member {
    char username[IRC_USERNAME_MAXCHARS]; // MAX LENGTH OF A USERNAME IS 9 CHARACTERS
    TcsSocket socket;
} IRC_Member;

typedef struct IRC_Channel {
    u32 *members;
} IRC_Channel;

typedef struct IRC_Server {
    IRC_ServerConfig config;
    TcsSocket socket; // the bound socket

    IRC_Member  *members;
    IRC_Channel *channels;
} IRC_Server;

typedef struct IRC_ClientConfig {
    char *address;
    u16 port;
} IRC_ClientConfig;

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
static char IRC__CHANNELMODE_CHARACTER[IRC_CHANNELMODE__TOTAL] = {
    'O', 'o', 'v', 'a', 'i', 'm', 'n', 'q', 'p', 's', 'r', 't', 'k', 'l', 'b', 'e', 'I'
};

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

    memset(message, 0, sizeof(*message));

    return IRC_RESULT_OK;
}

IRC_Result IRC_SetServerCallback(IRC_ServerConfig *config, IRC_CallbackKind kind, IRC_Result (*fn)(IRC_Message *))
{
    if (config == NULL || fn == NULL || !(IRC_CALLBACK__START <= kind && kind <= IRC_CALLBACK__TOTAL)) {
        return IRC_RESULT_PARAMERR;
    }

    return IRC_RESULT_OK;
}

IRC_Result IRC_HandleServer_NICK(IRC_Message *message)
{
    return IRC_RESULT_OK;
}

IRC_Result IRC_HandleServer_PASS(IRC_Message *message)
{
    return IRC_RESULT_OK;
}

IRC_Result IRC_HandleServer_USER(IRC_Message *message)
{
    return IRC_RESULT_OK;
}

IRC_Result IRC_HandleServer_SERVER(IRC_Message *message)
{
    return IRC_RESULT_OK;
}

IRC_Result IRC_HandleServer_OPER(IRC_Message *message)
{
    return IRC_RESULT_OK;
}

IRC_Result IRC_HandleServer_QUIT(IRC_Message *message)
{
    return IRC_RESULT_OK;
}

IRC_Result IRC_HandleServer_SQUIT(IRC_Message *message)
{
    return IRC_RESULT_OK;
}

IRC_Result IRC_HandleServer_JOIN(IRC_Message *message)
{
    return IRC_RESULT_OK;
}

IRC_Result IRC_HandleServer_MODE(IRC_Message *message)
{
    return IRC_RESULT_OK;
}

IRC_Result IRC_HandleServer_TOPIC(IRC_Message *message)
{
    return IRC_RESULT_OK;
}

IRC_Result IRC_HandleServer_NAMES(IRC_Message *message)
{
    return IRC_RESULT_OK;
}

IRC_Result IRC_HandleServer_LIST(IRC_Message *message)
{
    return IRC_RESULT_OK;
}

IRC_Result IRC_HandleServer_INVITE(IRC_Message *message)
{
    return IRC_RESULT_OK;
}

IRC_Result IRC_HandleServer_KICK(IRC_Message *message)
{
    return IRC_RESULT_OK;
}

IRC_Result IRC_HandleServer_VERSION(IRC_Message *message)
{
    return IRC_RESULT_OK;
}

IRC_Result IRC_HandleServer_STATS(IRC_Message *message)
{
    return IRC_RESULT_OK;
}

IRC_Result IRC_HandleServer_LINKS(IRC_Message *message)
{
    return IRC_RESULT_OK;
}

IRC_Result IRC_HandleServer_TIME(IRC_Message *message)
{
    return IRC_RESULT_OK;
}

IRC_Result IRC_HandleServer_CONNECT(IRC_Message *message)
{
    return IRC_RESULT_OK;
}

IRC_Result IRC_HandleServer_TRACE(IRC_Message *message)
{
    return IRC_RESULT_OK;
}

IRC_Result IRC_HandleServer_ADMIN(IRC_Message *message)
{
    return IRC_RESULT_OK;
}

IRC_Result IRC_HandleServer_INFO(IRC_Message *message)
{
    return IRC_RESULT_OK;
}

IRC_Result IRC_HandleServer_PRIVMSG(IRC_Message *message)
{
    return IRC_RESULT_OK;
}

IRC_Result IRC_HandleServer_NOTICE(IRC_Message *message)
{
    return IRC_RESULT_OK;
}

IRC_Result IRC_HandleServer_WHO(IRC_Message *message)
{
    return IRC_RESULT_OK;
}

IRC_Result IRC_HandleServer_WHOIS(IRC_Message *message)
{
    return IRC_RESULT_OK;
}

IRC_Result IRC_HandleServer_WHOWAS(IRC_Message *message)
{
    return IRC_RESULT_OK;
}

IRC_Result IRC_HandleServer_KILL(IRC_Message *message)
{
    return IRC_RESULT_OK;
}

IRC_Result IRC_HandleServer_PING(IRC_Message *message)
{
    return IRC_RESULT_OK;
}

IRC_Result IRC_HandleServer_PONG(IRC_Message *message)
{
    return IRC_RESULT_OK;
}

IRC_Result IRC_HandleServer_ERROR(IRC_Message *message)
{
    return IRC_RESULT_OK;
}

IRC_Result IRC_HandleServer_AWAY(IRC_Message *message)
{
    return IRC_RESULT_OK;
}

IRC_Result IRC_HandleServer_REHASH(IRC_Message *message)
{
    return IRC_RESULT_OK;
}

IRC_Result IRC_HandleServer_RESTART(IRC_Message *message)
{
    return IRC_RESULT_OK;
}

IRC_Result IRC_HandleServer_SUMMON(IRC_Message *message)
{
    return IRC_RESULT_OK;
}

IRC_Result IRC_HandleServer_USERS(IRC_Message *message)
{
    return IRC_RESULT_OK;
}

IRC_Result IRC_HandleServer_WALLOPS(IRC_Message *message)
{
    return IRC_RESULT_OK;
}

IRC_Result IRC_HandleServer_USERHOST(IRC_Message *message)
{
    return IRC_RESULT_OK;
}

IRC_Result IRC_HandleServer_ISON(IRC_Message *message)
{
    return IRC_RESULT_OK;
}

IRC_ServerConfig IRC_GetDefaultServerConfig(void)
{
    IRC_ServerConfig config = { 0 };

    // TODO Do we default to

    config.port = IRC_DEFAULTPORT_PLAINTEXT;

    config.default_callback_fns[IRC_CALLBACK_NICK]     = IRC_HandleServer_NICK;
    config.default_callback_fns[IRC_CALLBACK_PASS]     = IRC_HandleServer_PASS;
    config.default_callback_fns[IRC_CALLBACK_USER]     = IRC_HandleServer_USER;
    config.default_callback_fns[IRC_CALLBACK_SERVER]   = IRC_HandleServer_SERVER;
    config.default_callback_fns[IRC_CALLBACK_OPER]     = IRC_HandleServer_OPER;
    config.default_callback_fns[IRC_CALLBACK_QUIT]     = IRC_HandleServer_QUIT;
    config.default_callback_fns[IRC_CALLBACK_SQUIT]    = IRC_HandleServer_SQUIT;
    config.default_callback_fns[IRC_CALLBACK_JOIN]     = IRC_HandleServer_JOIN;
    config.default_callback_fns[IRC_CALLBACK_MODE]     = IRC_HandleServer_MODE;
    config.default_callback_fns[IRC_CALLBACK_TOPIC]    = IRC_HandleServer_TOPIC;
    config.default_callback_fns[IRC_CALLBACK_NAMES]    = IRC_HandleServer_NAMES;
    config.default_callback_fns[IRC_CALLBACK_LIST]     = IRC_HandleServer_LIST;
    config.default_callback_fns[IRC_CALLBACK_INVITE]   = IRC_HandleServer_INVITE;
    config.default_callback_fns[IRC_CALLBACK_KICK]     = IRC_HandleServer_KICK;
    config.default_callback_fns[IRC_CALLBACK_VERSION]  = IRC_HandleServer_VERSION;
    config.default_callback_fns[IRC_CALLBACK_STATS]    = IRC_HandleServer_STATS;
    config.default_callback_fns[IRC_CALLBACK_LINKS]    = IRC_HandleServer_LINKS;
    config.default_callback_fns[IRC_CALLBACK_TIME]     = IRC_HandleServer_TIME;
    config.default_callback_fns[IRC_CALLBACK_CONNECT]  = IRC_HandleServer_CONNECT;
    config.default_callback_fns[IRC_CALLBACK_TRACE]    = IRC_HandleServer_TRACE;
    config.default_callback_fns[IRC_CALLBACK_ADMIN]    = IRC_HandleServer_ADMIN;
    config.default_callback_fns[IRC_CALLBACK_INFO]     = IRC_HandleServer_INFO;
    config.default_callback_fns[IRC_CALLBACK_PRIVMSG]  = IRC_HandleServer_PRIVMSG;
    config.default_callback_fns[IRC_CALLBACK_NOTICE]   = IRC_HandleServer_NOTICE;
    config.default_callback_fns[IRC_CALLBACK_WHO]      = IRC_HandleServer_WHO;
    config.default_callback_fns[IRC_CALLBACK_WHOIS]    = IRC_HandleServer_WHOIS;
    config.default_callback_fns[IRC_CALLBACK_WHOWAS]   = IRC_HandleServer_WHOWAS;
    config.default_callback_fns[IRC_CALLBACK_KILL]     = IRC_HandleServer_KILL;
    config.default_callback_fns[IRC_CALLBACK_PING]     = IRC_HandleServer_PING;
    config.default_callback_fns[IRC_CALLBACK_PONG]     = IRC_HandleServer_PONG;
    config.default_callback_fns[IRC_CALLBACK_ERROR]    = IRC_HandleServer_ERROR;
    config.default_callback_fns[IRC_CALLBACK_AWAY]     = IRC_HandleServer_AWAY;
    config.default_callback_fns[IRC_CALLBACK_REHASH]   = IRC_HandleServer_REHASH;
    config.default_callback_fns[IRC_CALLBACK_RESTART]  = IRC_HandleServer_RESTART;
    config.default_callback_fns[IRC_CALLBACK_SUMMON]   = IRC_HandleServer_SUMMON;
    config.default_callback_fns[IRC_CALLBACK_USERS]    = IRC_HandleServer_USERS;
    config.default_callback_fns[IRC_CALLBACK_WALLOPS]  = IRC_HandleServer_WALLOPS;
    config.default_callback_fns[IRC_CALLBACK_USERHOST] = IRC_HandleServer_USERHOST;
    config.default_callback_fns[IRC_CALLBACK_ISON]     = IRC_HandleServer_ISON;

    return config;
}

IRC_ServerConfig IRC__CopyServerConfigFromUser(IRC_ServerConfig config)
{
    IRC_ServerConfig useme = IRC_GetDefaultServerConfig();

    useme.port = config.port;
    memcpy(useme.user_callback_fns, config.user_callback_fns, sizeof(useme.user_callback_fns));

    return useme;
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

    server->config = IRC__CopyServerConfigFromUser(config);
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
    uint8_t buffer[IRC_DEFAULT_BUFFER_SIZE] = { 0 };

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
        { IRC_CALLBACK_NICK,     "NICK" },
        { IRC_CALLBACK_PASS,     "PASS" },
        { IRC_CALLBACK_USER,     "USER" },
        { IRC_CALLBACK_SERVER,   "SERVER" },
        { IRC_CALLBACK_OPER,     "OPER" },
        { IRC_CALLBACK_QUIT,     "QUIT" },
        { IRC_CALLBACK_SQUIT,    "SQUIT" },
        { IRC_CALLBACK_JOIN,     "JOIN" },
        { IRC_CALLBACK_MODE,     "MODE" },
        { IRC_CALLBACK_TOPIC,    "TOPIC" },
        { IRC_CALLBACK_NAMES,    "NAMES" },
        { IRC_CALLBACK_LIST,     "LIST" },
        { IRC_CALLBACK_INVITE,   "INVITE" },
        { IRC_CALLBACK_KICK,     "KICK" },
        { IRC_CALLBACK_VERSION,  "VERSION" },
        { IRC_CALLBACK_STATS,    "STATS" },
        { IRC_CALLBACK_LINKS,    "LINKS" },
        { IRC_CALLBACK_TIME,     "TIME" },
        { IRC_CALLBACK_CONNECT,  "CONNECT" },
        { IRC_CALLBACK_TRACE,    "TRACE" },
        { IRC_CALLBACK_ADMIN,    "ADMIN" },
        { IRC_CALLBACK_INFO,     "INFO" },
        { IRC_CALLBACK_PRIVMSG,  "PRIVMSG" },
        { IRC_CALLBACK_NOTICE,   "NOTICE" },
        { IRC_CALLBACK_WHO,      "WHO" },
        { IRC_CALLBACK_WHOIS,    "WHOIS" },
        { IRC_CALLBACK_WHOWAS,   "WHOWAS" },
        { IRC_CALLBACK_KILL,     "KILL" },
    };

    bool command_found = false;

    for (i32 i = 0; i < ARRSIZE(function_map); i++) {
        if (streq(message.strings[IRC_MESSAGEPART_COMMAND], function_map[i].string)) {
            iresult = server->config.default_callback_fns[function_map[i].kind](&message);
            command_found = true;

            if (server->config.user_callback_fns[function_map[i].kind]) {
                iresult = server->config.user_callback_fns[function_map[i].kind](&message);
            }

            break;
        }
    }

    if (!command_found) {
        iresult = IRC_RESULT_INVALIDCOMMAND;
    }

    return iresult;
}

// IRC_SendSocketF: sends the data on the socket. Caller doesn't need to append \r\n, this function
// does that.
__attribute__ ((format (printf, 2, 3)))
IRC_Result IRC_SendSocketF(TcsSocket *socket, const char *fmt, ...)
{
    int rc;
    char buffer[IRC_DEFAULT_BUFFER_SIZE];
    va_list ap;

    va_start(ap, fmt);
    rc = vsnprintf(buffer, sizeof(buffer) - 3, fmt, ap);
    va_end(ap);

    (void)rc; // ??

    // put our \r\n at the end of our message

    buffer[strlen(buffer)] = '\r';
    buffer[strlen(buffer)] = '\n';
    buffer[strlen(buffer)] = '\0';

    size_t to_send_length = MIN(sizeof(buffer), strlen(buffer));
    size_t sent_length = 0;

    TcsResult sock_result = tcs_send(*socket, (const u8 *)buffer, to_send_length, TCS_FLAG_NONE, &sent_length);

    return sock_result == TCS_SUCCESS ? IRC_RESULT_OK : IRC_RESULT_SENDERR;
}

// IRC__SeparateList: concatenates a 'list' into 'dst' using 'sep', not exceeding 'len'.
// Returns 'true' if the whole list fit, 'false' if not. Will not modify 'dst' if there is
// insufficient space.
static bool IRC__SeparateList(char *dst, size_t len, char **list, char *sep)
{
    size_t required_size = 0;

    for (i32 i = 0; i < arrlen(list); i++) {
        required_size += strlen(list[i]);
        if (i < arrlen(list) - 1) {
            required_size++;
        }
    }

    // terminate with NUL
    required_size++;

    if (required_size > len) {
        return false;
    }

    size_t curr_size = 0;

    for (i32 i = 0; i < arrlen(list); i++) {
        curr_size += snprintf(
            dst + curr_size,
            len - curr_size,
            "%s%s",
            list[i],
            i < arrlen(list) - 1 ? sep : ""
        );
    }

    return true;
}

static bool IRC__CommaSeparateList(char *dst, size_t len, char **list)
{
    return IRC__SeparateList(dst, len, list, ",");
}

IRC_Result IRC_SendClient_NICK(IRC_Client *client, char *nickname)
{
    IRC_Result result = IRC_SendSocketF(&client->socket, "NICK %s", nickname);
    return result;
}

IRC_Result IRC_SendClient_PASS(IRC_Client *client, char *password)
{
    IRC_Result result = IRC_SendSocketF(&client->socket, "PASS %s", password);
    return result;
}

IRC_Result IRC_SendClient_USER(IRC_Client *client, char *username, char *hostname, char *realname)
{
    IRC_Result result = IRC_SendSocketF(&client->socket, "USER %s %s :%s", username, hostname, realname);
    return result;
}

IRC_Result IRC_SendClient_OPER(IRC_Client *client, char *username, char *password)
{
    IRC_Result result = IRC_SendSocketF(&client->socket, "OPER %s %s", username, password);
    return result;
}

IRC_Result IRC_SendClient_QUIT(IRC_Client *client, char *message)
{
    IRC_Result result = IRC_SendSocketF(&client->socket, "QUIT %s", message);
    return result;
}

IRC_Result IRC_SendClient_JOIN(IRC_Client *client, char *channels, char *keys)
{
    IRC_Result result = IRC_SendSocketF(&client->socket, "JOIN %s %s", channels, keys);
    return result;
}

IRC_Result IRC_SendClient_JOINList(IRC_Client *client, char **channels, char **keys)
{
    char chan_buffer[IRC_DEFAULT_BUFFER_SIZE] = { 0 };
    char keys_buffer[IRC_DEFAULT_BUFFER_SIZE] = { 0 };

    bool cconcat = IRC__CommaSeparateList(chan_buffer, sizeof chan_buffer, channels);
    bool kconcat = IRC__CommaSeparateList(keys_buffer, sizeof keys_buffer, keys);

    // WARN
    //
    // I guess since this is going to be a user string, we maybe don't want to assert here, but
    // instead we want to assert like, the channels and keys or something.
    //
    // Maybe it's the case that actually, multiple JOIN messages for the various channels, given the
    // keys array is something that works...
    //
    // Whatever the case might be, we'll probably have a situation here at some point. This should
    // be tested.

    assert(cconcat);
    assert(kconcat);

    IRC_Result result = IRC_SendClient_JOIN(client, chan_buffer, keys_buffer);
    return result;
}

IRC_Result IRC_SendClient_PART(IRC_Client *client, char *channels)
{
    IRC_Result result = IRC_SendSocketF(&client->socket, "PART %s", channels);
    return result;
}

IRC_Result IRC_SendClient_PARTList(IRC_Client *client, char **channels)
{
    char chan_buffer[IRC_DEFAULT_BUFFER_SIZE] = { 0 };

    bool concat = IRC__CommaSeparateList(chan_buffer, sizeof chan_buffer, channels);

    assert(concat);

    IRC_Result result = IRC_SendClient_PART(client, chan_buffer);
    return result;
}

IRC_Result IRC_SendClient_MODE(IRC_Client *client, char *thing, IRC_ModeFlags flags, char *arg)
{
    // TODO
    return IRC_RESULT_OK;
}

// Changes the topic to 'topic' for the channel 'channel'.
IRC_Result IRC_SendClient_TOPIC(IRC_Client *client, char *channel, char *topic)
{
    IRC_Result result = IRC_SendSocketF(&client->socket, "TOPIC %s :%s", channel, topic);
    return result;
}

// List all nicknames visible to them on any channel the user can see.
IRC_Result IRC_SendClient_NAMES(IRC_Client *client, char *channels)
{
    IRC_Result result = IRC_SendSocketF(&client->socket, "NAMES %s", channels);
    return result;
}

// List channels and their topics.
IRC_Result IRC_SendClient_LIST(IRC_Client *client, char *channels, char *server)
{
    IRC_Result result;

    if (server == NULL) {
        result = IRC_SendSocketF(&client->socket, "LIST %s", channels);
    } else {
        result = IRC_SendSocketF(&client->socket, "LIST %s %s", channels, server);
    }

    return result;
}

// Invites a user with 'nickname' to the 'channel'.
IRC_Result IRC_SendClient_INVITE(IRC_Client *client, char *nickname, char *channel)
{
    IRC_Result result = IRC_SendSocketF(&client->socket, "INVITE %s %s", nickname, channel);
    return result;
}

// Forcibly remove the 'user' from the 'channel', optionally sending the 'comment'.
IRC_Result IRC_SendClient_KICK(IRC_Client *client, char *channel, char *user, char *comment)
{
    IRC_Result result;

    if (comment == NULL) {
        result = IRC_SendSocketF(&client->socket, "KICK %s %s", channel, user);
    } else {
        result = IRC_SendSocketF(&client->socket, "KICK %s %s :%s", channel, user, comment);
    }

    return result;
}

// Return the version of the connected server, optionally 'server' is used to query a specific
// server in the network.
IRC_Result IRC_SendClient_VERSION(IRC_Client *client, char *server)
{
    IRC_Result result = IRC_SendSocketF(&client->socket, "VERSION");
    return result;
}

// Query statistics of a certain server.
IRC_Result IRC_SendClient_STATS(IRC_Client *client)
{
    // TODO
    return IRC_RESULT_OK;
}

// List all of the servers known by the server answering the query.
IRC_Result IRC_SendClient_LINKS(IRC_Client *client, char *server_mask, char *remote_server)
{
    IRC_Result result;

    if (remote_server == NULL) {
        result = IRC_SendSocketF(&client->socket, "LINKS %s", server_mask);
    } else {
        result = IRC_SendSocketF(&client->socket, "LINKS %s %s", server_mask, remote_server);
    }

    return result;
}

// Query the local time from the (optionally) specified server.
IRC_Result IRC_SendClient_TIME(IRC_Client *client, char *server)
{
    IRC_Result result;

    if (server == NULL) {
        result = IRC_SendSocketF(&client->socket, "TIME");
    } else {
        result = IRC_SendSocketF(&client->socket, "TIME %s", server);
    }

    return result;
}

// Used to force a server to try to establish a new connection to another server immediately. This
// is a privileged command and is (probably?) only available to IRC operators.
//
// Pass 'port' != 0 to add the port, and 'remote_server' != NULL for remote_server.
IRC_Result IRC_SendClient_CONNECT(IRC_Client *client, char *target_server, u16 port, char *remote_server)
{
    IRC_Result result;

    // NOTE This is not exhaustive for the permutation of inputs.

    if (port == 0 || remote_server == NULL) {
        result = IRC_SendSocketF(&client->socket, "CONNECT %s", target_server);
    } else {
        result = IRC_SendSocketF(&client->socket, "CONNECT %s %hu %s", target_server, port, remote_server);
    }

    return result;
}

// Used to find the route to the specified server.
IRC_Result IRC_SendClient_TRACE(IRC_Client *client, char *server)
{
    IRC_Result result;

    if (server == NULL) {
        result = IRC_SendSocketF(&client->socket, "TRACE");
    } else {
        result = IRC_SendSocketF(&client->socket, "TRACE %s", server);
    }

    return result;
}

// Used to find the name of the administrator of the server.
IRC_Result IRC_SendClient_ADMIN(IRC_Client *client, char *server)
{
    IRC_Result result;

    if (server == NULL) {
        result = IRC_SendSocketF(&client->socket, "ADMIN");
    } else {
        result = IRC_SendSocketF(&client->socket, "ADMIN %s", server);
    }

    return result;
}

IRC_Result IRC_SendClient_INFO(IRC_Client *client, char *server)
{
    IRC_Result result;

    if (server == NULL) {
        result = IRC_SendSocketF(&client->socket, "INFO");
    } else {
        result = IRC_SendSocketF(&client->socket, "INFO %s", server);
    }

    return result;
}

// Sends a message to 'receiver' - sends all of the text to be sent. Sends nothing on NULL.
IRC_Result IRC_SendClient_PRIVMSG(IRC_Client *client, char *recipients, char *message)
{
    IRC_Result result = IRC_SendSocketF(&client->socket, "PRIVMSG %s :%s", recipients, message);
    return result;
}

IRC_Result IRC_SendClient_PRIVMSGList(IRC_Client *client, char **recipients, char *message)
{
    char csv[IRC_DEFAULT_BUFFER_SIZE] = { 0 };

    bool concat = IRC__CommaSeparateList(csv, sizeof csv, recipients);

    assert(concat);

    IRC_Result result = IRC_SendClient_PRIVMSG(client, csv, message);
    return result;
}

// Sends a message to 'nickname', with the 'message'. Almost the same as 'PRIVMSG', but doesn't
// allow automated replies of any kind.
IRC_Result IRC_SendClient_NOTICE(IRC_Client *client, char *nickname, char *message)
{
    IRC_Result result = IRC_SendSocketF(&client->socket, "NOTICE %s :%s", nickname, message);
    return result;
}

// Generates a query which returns a list of information that "matches" the 'name' parameter. If
// 'operators_only' is true, will only return operators that match.
IRC_Result IRC_SendClient_WHO(IRC_Client *client, char *name, bool operators_only)
{
    IRC_Result result = IRC_SendSocketF(&client->socket, "WHO %s%s", name, operators_only ? " o" : "");
    return result;
}

// Query information about a particular user. If 'server' is provided, will send the query to this
// particular server. 'nickmasks' is a comma delimited set of nickname masks.
IRC_Result IRC_SendClient_WHOIS(IRC_Client *client, char *server, char *nickmasks)
{
    IRC_Result result;
   
    result = IRC_SendSocketF(&client->socket, "WHOIS %s%s%s",
        server == NULL ? "" : server,
        server == NULL ? "" : " ",
        nickmasks == NULL ? "" : nickmasks
    );

    return result;
}

// Asks for information about a nickname that no longer exists. If 'server' is provided, queries
// only that server.
//
// For sending no count, set 'count' <= 0, no server, set 'server' to NULL
IRC_Result IRC_SendClient_WHOWAS(IRC_Client *client, char *nickname, int count, char *server)
{
    IRC_Result result;

    // this is silly, and I shouldn't do it like this

    if (count <= 0) {
        result = IRC_SendSocketF(&client->socket, "WHOWAS %s%s%s",
            nickname,
            server == NULL ? "" : " ",
            server == NULL ? "" : server
        );
    } else {
        result = IRC_SendSocketF(&client->socket, "WHOWAS %s %d%s%s",
            nickname,
            count,
            server == NULL ? "" : " ",
            server == NULL ? "" : server
        );
    }

    return result;
}

// Causes a client-server communication to be closed by the server. Operator only command.
IRC_Result IRC_SendClient_KILL(IRC_Client *client, char *nickname, char *comment)
{
    IRC_Result result = IRC_SendSocketF(&client->socket, "KILL %s%s%s",
        nickname,
        comment == NULL ? "" : " ",
        comment == NULL ? "" : comment
    );
    return result;
}

// Set an automatic reply string for PRIVMSG commands being directed to the client.
IRC_Result IRC_SendClient_AWAY(IRC_Client *client, char *message)
{
    IRC_Result result = IRC_SendSocketF(&client->socket, "AWAY%s%s",
        message == NULL ? "" : " ",
        message == NULL ? "" : message
    );
    return result;
}

// Causes the server to reread and reprocess their configuration file.
IRC_Result IRC_SendClient_REHASH(IRC_Client *client)
{
    IRC_Result result = IRC_SendSocketF(&client->socket, "REHASH");
    return result;
}

// Used by an operator to issue a restart command to the server.
IRC_Result IRC_SendClient_RESTART(IRC_Client *client)
{
    IRC_Result result = IRC_SendSocketF(&client->socket, "RESTART");
    return result;
}

// Sends a message to the 'user' asking them to join the 'server'.
IRC_Result IRC_SendClient_SUMMON(IRC_Client *client, char *user, char *server)
{
    IRC_Result result = IRC_SendSocketF(&client->socket, "SUMMON %s%s%s",
        user,
        server == NULL ? "" : " ",
        server == NULL ? "" : server
    );
    return result;
}

// Returns a list of users connected to the 'server'.
IRC_Result IRC_SendClient_USERS(IRC_Client *client, char *server)
{
    IRC_Result result = IRC_SendSocketF(&client->socket, "USERS%s%s",
        server == NULL ? "" : " ",
        server == NULL ? "" : server
    );
    return result;
}

IRC_Result IRC_SendClient_USERHOST(IRC_Client *client, char *nicknames)
{
    IRC_Result result = IRC_SendSocketF(&client->socket, "USERHOST %s", nicknames);
    return result;
}

IRC_Result IRC_SendClient_USERHOSTList(IRC_Client *client, char **nicknames)
{
    char csv[IRC_DEFAULT_BUFFER_SIZE] = { 0 };
    bool concat = IRC__CommaSeparateList(csv, sizeof csv, nicknames);
    assert(concat);
    IRC_Result result = IRC_SendClient_USERHOST(client, csv);
    return result;
}

IRC_Result IRC_SendClient_ISON(IRC_Client *client, char *nicknames)
{
    IRC_Result result = IRC_SendSocketF(&client->socket, "ISON %s", nicknames);
    return result;
}

IRC_Result IRC_SendClient_ISONList(IRC_Client *client, char **nicknames)
{
    char csv[IRC_DEFAULT_BUFFER_SIZE] = { 0 };
    bool concat = IRC__SeparateList(csv, sizeof csv, nicknames, " ");
    assert(concat);
    IRC_Result result = IRC_SendClient_ISON(client, csv);
    return result;
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
