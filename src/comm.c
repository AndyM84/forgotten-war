/***************************************************************************
 *  Original Diku Mud copyright (C) 1990, 1991 by Sebastian Hammer,        *
 *  Michael Seifert, Hans Henrik St{rfeldt, Tom Madsen, and Katja Nyboe.   *
 *                                                                         *
 *  Merc Diku Mud improvments copyright (C) 1992, 1993 by Michael          *
 *  Chastain, Michael Quan, and Mitchell Tse.                              *
 *                                                                         *
 *  In order to use any part of this Merc Diku Mud, you must comply with   *
 *  both the original Diku license in 'license.doc' as well the Merc       *
 *  license in 'license.txt'.  In particular, you may not remove either of *
 *  these copyright notices.                                               *
 *                                                                         *
 *  Much time and thought has gone into this software and you are          *
 *  benefitting.  We hope that you share your changes too.  What goes      *
 *  around, comes around.                                                  *
 ***************************************************************************/

/*
 * This file contains all of the OS-dependent stuff:
 *   startup, signals, BSD sockets for tcp/ip, i/o, timing.
 *
 * The data flow for input is:
 *    Game_loop ---> Read_from_descriptor ---> Read
 *    Game_loop ---> Read_from_buffer
 *
 * The data flow for output is:
 *    Game_loop ---> Process_Output ---> Write_to_descriptor -> Write
 *
 * The OS-dependent functions are Read_from_descriptor and Write_to_descriptor.
 * -- Furey  26 Jan 1993
 */

#if defined(WIN32)
#include <windows.h>
#if !defined(bzero)
#define bzero(b, len) memset((b), 0, (len))
#endif
#else
#include <sys/time.h>
#include <unistd.h>
#endif

#include <sys/types.h>
#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <stdarg.h>

#include "merc.h"

/* command procedures needed */
DECLARE_DO_FUN(do_help);
DECLARE_DO_FUN(do_look);
DECLARE_DO_FUN(do_save);

/* Local prototypes. */
char *doparseprompt args( ( CHAR_DATA *ch ) );

/*
 * crypt() stub for Windows (which lacks libcrypt).
 * This is a simple passthrough -- passwords are stored in plaintext.
 * For production use, replace with a proper hash (bcrypt, etc.).
 */
#if defined(WIN32)
static char *crypt( const char *key, const char *salt )
{
    return (char *)key;
}
#else
char *crypt args( ( const char *key, const char *salt ) );
#endif

/*
 * Signal handling.
 */
#if defined(unix)
#include <signal.h>
#endif

/*
 * Socket and TCP/IP stuff.
 */
#if defined(unix)
#include <fcntl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/telnet.h>
const char echo_off_str[] = { IAC, WILL, TELOPT_ECHO, '\0' };
const char echo_on_str[] = { IAC, WONT, TELOPT_ECHO, '\0' };
const char go_ahead_str[] = { IAC, GA, '\0' };
#endif

#if defined(WIN32)
#include "Win32Common\telnet.h"
const char echo_off_str[] = { (char)IAC, (char)WILL, TELOPT_ECHO, '\0' };
const char echo_on_str[] = { (char)IAC, (char)WONT, TELOPT_ECHO, '\0' };
const char go_ahead_str[] = { (char)IAC, (char)GA, '\0' };
#endif

/*
 * OS-dependent declarations.
 */
#if defined(_AIX)
#include <sys/select.h>
int accept args((int s, struct sockaddr *addr, int *addrlen));
int bind args((int s, struct sockaddr *name, int namelen));
void bzero args((char *b, int length));
int getpeername args((int s, struct sockaddr *name, int *namelen));
int getsockname args((int s, struct sockaddr *name, int *namelen));
int gettimeofday args((struct timeval *tp, struct timezone *tzp));
int listen args((int s, int backlog));
int setsockopt args((int s, int level, int optname, void *optval, int optlen));
int socket args((int domain, int type, int protocol));
#endif

#if defined(linux)
int close args((int fd));
int select args((int width, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, struct timeval *timeout));
int socket args((int domain, int type, int protocol));
#endif

/*
 * Global variables.
 */
DESCRIPTOR_DATA *descriptor_free = 0;	/* Free list for descriptors    */
DESCRIPTOR_DATA *descriptor_list = 0;	/* All open descriptors         */
DESCRIPTOR_DATA *d_next = 0;		/* Next descriptor in loop      */
FILE *fpReserve = 0;			/* Reserved file handle         */
bool merc_down;				/* Shutdown                     */
bool wizlock;				/* Game is wizlocked            */
bool newlock;				/* Game is newlocked            */
char str_boot_time[MAX_INPUT_LENGTH];
time_t current_time;			/* time of this pulse           */
int port;
int control;

extern char log_buf[2 * MAX_INPUT_LENGTH];

/*
 * OS-dependent local functions.
 */
int game_loop args((int control));
int init_socket args((int port));
void new_descriptor args((int control));
bool read_from_descriptor args((DESCRIPTOR_DATA *d, bool color));
bool write_to_descriptor args((int desc, char *txt, int length, bool color));

/*
 * Other local functions (OS-independent).
 */
bool check_parse_name args((char *name));
bool check_reconnect args((DESCRIPTOR_DATA *d, char *name, bool fConn));
bool check_playing args((DESCRIPTOR_DATA *d, char *name));
int main args((int argc, char **argv));
void nanny args((DESCRIPTOR_DATA *d, char *argument));
bool process_output args((DESCRIPTOR_DATA *d, bool fPrompt));
void read_from_buffer args((DESCRIPTOR_DATA *d, bool color));
void stop_idling args((CHAR_DATA *ch));

/* External functions */
extern char *get_curtime args((void));

/*
 * update_last - no-op stub, called by update.c
 */
void update_last(char *what, char *who, char *where)
{
	return;
}

int main(int argc, char **argv)
{
	struct timeval now_time = { 0, 0 };

	gettimeofday(&now_time, NULL);
	current_time = (time_t)now_time.tv_sec;
	strcpy(str_boot_time, get_curtime());

	if ((fpReserve = fopen(NULL_FILE, "r")) == NULL) {
		perror(NULL_FILE);
		exit(1);
	}

	port = 9000;
	if (argc > 1) {
		if (!is_number(argv[1])) {
			fprintf(stderr, "Usage: %s [port #]\n", argv[0]);
			exit(1);
		} else if ((port = atoi(argv[1])) <= 1024) {
			fprintf(stderr, "Port number must be above 1024.\n");
			exit(1);
		}
	}

	control = init_socket(port);
	if (control == -1) {
		log_string("Error: init_socket(), shutting down.");
		return -1;
	}

	boot_db();

	sprintf(log_buf, "EmberMUD is ready to rock on port %d.", port);
	log_string(log_buf);

	if (game_loop(control)) {
		log_string("Error: game_loop(), shutting down.");
		#if defined(WIN32)
		closesocket(control);
		#else
		close(control);
		#endif
		return -1;
	}

	#if defined(WIN32)
	closesocket(control);
	#else
	close(control);
	#endif

	log_string("Normal termination of game.");
	exit(0);
	return 0;
}

int init_socket(int port)
{
	static struct sockaddr_in sa_zero;
	struct sockaddr_in sa;
	int x = 1;
	int fd;

	#if defined(WIN32)
	WSADATA wsaData;
	wsaData.wVersion = 2;
	WSAStartup(MAKEWORD(2, 2), &wsaData);
	#endif

	if ((fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("Init_socket: socket");
		exit(1);
	}

	if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (char *)&x, sizeof(x)) < 0) {
		perror("Init_socket: SO_REUSEADDR");
		#if defined(WIN32)
		closesocket(fd);
		#else
		close(fd);
		#endif
		exit(1);
	}

	#if defined(SO_DONTLINGER) && !defined(SYSV)
	{
		struct linger ld;
		ld.l_onoff = 1;
		ld.l_linger = 1000;
		if (setsockopt(fd, SOL_SOCKET, SO_DONTLINGER, (char *)&ld, sizeof(ld)) < 0) {
			perror("Init_socket: SO_DONTLINGER");
			#if defined(WIN32)
			closesocket(fd);
			#else
			close(fd);
			#endif
			exit(1);
		}
	}
	#endif

	sa = sa_zero;
	sa.sin_family = AF_INET;
	sa.sin_port = htons((u_short)port);

	if (bind(fd, (struct sockaddr *)&sa, sizeof(sa)) < 0) {
		perror("Init socket: bind");
		#if defined(WIN32)
		closesocket(fd);
		#else
		close(fd);
		#endif
		exit(1);
	}

	if (listen(fd, 3) < 0) {
		perror("Init socket: listen");
		#if defined(WIN32)
		closesocket(fd);
		#else
		close(fd);
		#endif
		exit(1);
	}

	return fd;
}

#if defined(unix)
void sigchld_handler(int sig)
{
	return;
}
#endif

int game_loop(int control)
{
	static struct timeval null_time;
	struct timeval last_time;
	bool color;

	#if defined(unix)
	static struct sigaction sa;
	sigemptyset(&sa.sa_mask);
	#ifndef SA_RESTART
	#define SA_RESTART 0
	#endif
	sa.sa_flags = SA_RESTART;
	sa.sa_handler = sigchld_handler;
	if (sigaction(SIGCHLD, &sa, 0) < 0)
		return -1;
	signal(SIGPIPE, SIG_IGN);
	#endif

	gettimeofday(&last_time, NULL);
	current_time = (time_t)last_time.tv_sec;

	/* Main loop */
	while (!merc_down) {
		fd_set in_set;
		fd_set out_set;
		fd_set exc_set;
		DESCRIPTOR_DATA *d;
		int maxdesc;

		FD_ZERO(&in_set);
		FD_ZERO(&out_set);
		FD_ZERO(&exc_set);
		FD_SET(control, &in_set);
		maxdesc = control;

		for (d = descriptor_list; d; d = d->next) {
			maxdesc = UMAX(maxdesc, d->descriptor);
			FD_SET(d->descriptor, &in_set);
			FD_SET(d->descriptor, &out_set);
			FD_SET(d->descriptor, &exc_set);
		}

		if (select(maxdesc + 1, &in_set, &out_set, &exc_set, &null_time) < 0) {
			perror("Game_loop: select: poll");
			exit(1);
		}

		/* New connection? */
		if (FD_ISSET(control, &in_set))
			new_descriptor(control);

		/* Kick out the freaky folks. */
		for (d = descriptor_list; d != NULL; d = d_next) {
			d_next = d->next;
			if (FD_ISSET(d->descriptor, &exc_set)) {
				FD_CLR((unsigned int)d->descriptor, &in_set);
				FD_CLR((unsigned int)d->descriptor, &out_set);
				if (d->character && d->character->level > 1)
					save_char_obj(d->character);
				d->outtop = 0;
				close_socket(d);
			}
		}

		/* Process input. */
		for (d = descriptor_list; d != NULL; d = d_next) {
			d_next = d->next;
			d->fcommand = FALSE;

			if (FD_ISSET(d->descriptor, &in_set)) {
				if (d->character != NULL) {
					d->character->timer = 0;
					color = IS_SET(d->character->act, PLR_COLOUR) ? TRUE : FALSE;
				} else {
					color = FALSE;
				}

				if (!read_from_descriptor(d, color)) {
					FD_CLR((unsigned int)d->descriptor, &out_set);
					if (d->character != NULL && d->character->level > 1)
						save_char_obj(d->character);
					d->outtop = 0;
					close_socket(d);
					continue;
				}
			}

			if (d->character != NULL && d->character->wait > 0) {
				--d->character->wait;
				continue;
			}

			read_from_buffer(d, FALSE);

			if (d->incomm[0] != '\0') {
				d->fcommand = TRUE;
				stop_idling(d->character);

				if (d->showstr_point)
					show_string(d, d->incomm);
				else if (d->connected == CON_PLAYING)
					substitute_alias(d, d->incomm);
				else
					nanny(d, d->incomm);

				d->incomm[0] = '\0';
			}
		}

		/* Autonomous game motion. */
		update_handler();

		/* Output. */
		for (d = descriptor_list; d != NULL; d = d_next) {
			d_next = d->next;
			if ((d->fcommand || d->outtop > 0) && FD_ISSET(d->descriptor, &out_set)) {
				if (!process_output(d, TRUE)) {
					if (d->character != NULL && d->character->level > 1)
						save_char_obj(d->character);
					d->outtop = 0;
					close_socket(d);
				}
			}
		}

		/*
		 * Synchronize to a clock.
		 * Sleep( last_time + 1/PULSE_PER_SECOND - now ).
		 */
		{
			struct timeval now_time;
			long secDelta;
			long usecDelta;

			gettimeofday(&now_time, NULL);
			usecDelta = ((int)last_time.tv_usec) - ((int)now_time.tv_usec) + 1000000 / PULSE_PER_SECOND;
			secDelta = ((int)last_time.tv_sec) - ((int)now_time.tv_sec);

			#if defined(WIN32)
			if (usecDelta > 0)
				Sleep(UMIN(usecDelta, 250000) / 1000);
			#else
			while (usecDelta < 0) {
				usecDelta += 1000000;
				secDelta -= 1;
			}
			while (usecDelta >= 1000000) {
				usecDelta -= 1000000;
				secDelta += 1;
			}
			if (secDelta > 0 || (secDelta == 0 && usecDelta > 0)) {
				struct timeval stall_time;
				stall_time.tv_usec = usecDelta;
				stall_time.tv_sec = secDelta;
				if (select(0, NULL, NULL, NULL, &stall_time) < 0) {
					if (errno != EINTR) {
						perror("Game_loop: select: stall");
						exit(1);
					}
				}
			}
			#endif
		}

		gettimeofday(&last_time, NULL);
		current_time = (time_t)last_time.tv_sec;
	}

	return 0;
}

void new_descriptor(int control)
{
	char buf[MAX_STRING_LENGTH];
	static DESCRIPTOR_DATA d_zero;
	DESCRIPTOR_DATA *dnew;
	struct sockaddr_in sock;
	#ifndef NO_RDNS
	struct hostent *from;
	#endif
	int desc;
	int size;
	#if defined(WIN32)
	int OptVal;
	#endif

	size = sizeof(sock);
	getsockname(control, (struct sockaddr *)&sock, (unsigned int *)&size);

	if ((desc = accept(control, (struct sockaddr *)&sock, (unsigned int *)&size)) < 0) {
		perror("New_descriptor: accept");
		return;
	}

	#if !defined(FNDELAY)
	#define FNDELAY O_NDELAY
	#endif

	#if defined(WIN32)
	if (setsockopt(desc, IPPROTO_TCP, TCP_NODELAY, (char *)&OptVal, sizeof(int))) {
		perror("New_descriptor: setsockopt: TCP_NODELAY");
		return;
	}
	#else
	if (fcntl(desc, F_SETFL, FNDELAY) == -1) {
		perror("New_descriptor: fcntl: FNDELAY");
		return;
	}
	#endif

	/*
	 * Cons a new descriptor.
	 */
	if (descriptor_free == NULL) {
		dnew = alloc_perm(sizeof(*dnew));
	} else {
		dnew = descriptor_free;
		descriptor_free = descriptor_free->next;
	}

	*dnew = d_zero;
	dnew->descriptor = desc;
	dnew->connected = CON_GET_ANSI;
	dnew->showstr_head = NULL;
	dnew->showstr_point = NULL;
	dnew->outsize = 2000;
	dnew->outbuf = alloc_mem(dnew->outsize);

	size = sizeof(sock);

	if (getpeername(desc, (struct sockaddr *)&sock, (unsigned int *)&size) < 0) {
		perror("New_descriptor: getpeername");
		dnew->host = str_dup("(unknown)");
	} else {
		int addr;
		addr = ntohl(sock.sin_addr.s_addr);
		sprintf(buf, "%d.%d.%d.%d",
			(addr >> 24) & 0xFF, (addr >> 16) & 0xFF,
			(addr >> 8) & 0xFF, (addr) & 0xFF);
		sprintf(log_buf, "Sock.sinaddr:  %s", buf);
		log_string(log_buf);

		#ifndef NO_RDNS
		from = gethostbyaddr((char *)&sock.sin_addr, sizeof(sock.sin_addr), AF_INET);
		dnew->host = str_dup(from ? from->h_name : buf);
		#else
		dnew->host = str_dup(buf);
		#endif
	}

	dnew->next = descriptor_list;
	descriptor_list = dnew;

	write_to_buffer(dnew, CFG_CONNECT_MSG, 0);
	write_to_buffer(dnew, CFG_ASK_ANSI, 0);
}

void close_socket(DESCRIPTOR_DATA *dclose)
{
	CHAR_DATA *ch;

	if (dclose->outtop > 0)
		process_output(dclose, FALSE);

	if ((ch = dclose->character) != NULL) {
		sprintf(log_buf, "Closing link to %s.", ch->name);
		log_string(log_buf);

		if (dclose->connected == CON_PLAYING) {
			act("$n has lost $s link.", ch, NULL, NULL, TO_ROOM);
			ch->desc = NULL;
		} else {
			free_char(dclose->character);
		}
	}

	if (d_next == dclose)
		d_next = d_next->next;

	if (dclose == descriptor_list) {
		descriptor_list = descriptor_list->next;
	} else {
		DESCRIPTOR_DATA *d;
		for (d = descriptor_list; d && d->next != dclose; d = d->next) { }
		if (d != NULL)
			d->next = dclose->next;
		else
			bug("Close_socket: dclose not found.", 0);
	}

	#if defined(WIN32)
	closesocket(dclose->descriptor);
	#else
	close(dclose->descriptor);
	#endif

	free_string(&dclose->host);
	free_mem(&dclose->outbuf);
	dclose->next = descriptor_free;
	descriptor_free = dclose;
}

bool read_from_descriptor(DESCRIPTOR_DATA *d, bool color)
{
	int iStart;
	bool bOverflow = FALSE;

	if (d->incomm[0] != '\0')
		return TRUE;

	iStart = strlen(d->inbuf);
	if (iStart >= sizeof(d->inbuf) - 10) {
		sprintf(log_buf, "%s input overflow!", d->host);
		log_string(log_buf);
		write_to_descriptor(d->descriptor, "\n\r*** PUT A LID ON IT!!! ***\n\r", 0, color);
		return FALSE;
	}

	for (;;) {
		int nRead;
		int nBufSize;

		nBufSize = sizeof(d->inbuf) - 10 - iStart;

		#if defined(WIN32)
		nRead = recv(d->descriptor, d->inbuf + iStart, nBufSize, 0);
		#else
		nRead = read(d->descriptor, d->inbuf + iStart, nBufSize);
		#endif

		if (nRead > 0) {
			iStart += nRead;
			if (bOverflow) {
				iStart = 0;
				if (nRead < sizeof(d->inbuf) - 10)
					break;
			} else if (d->inbuf[iStart - 1] == '\n' || d->inbuf[iStart - 1] == '\r' || nRead < nBufSize) {
				break;
			} else if (iStart >= sizeof(d->inbuf) - 10) {
				if (iStart - nRead > 0)
					iStart -= nRead;
				else
					iStart = 0;
				bOverflow = TRUE;
			}
		} else if (nRead == 0) {
			log_string("EOF encountered on read.");
			return FALSE;
		}
		#if defined(unix)
		else if (errno == EWOULDBLOCK)
			break;
		#endif
		else {
			perror("Read_from_descriptor");
			return FALSE;
		}
	}

	d->inbuf[iStart] = '\0';

	if (bOverflow) {
		sprintf(log_buf, "%s input overflow!", d->host);
		log_string(log_buf);
		write_to_descriptor(d->descriptor, "\n\rLine too long, ignored.\n\r", 0, color);
	}

	return TRUE;
}

/*
 * Transfer one line from input buffer to input line.
 */
void read_from_buffer(DESCRIPTOR_DATA *d, bool color)
{
	int i, j, k;

	if (d->incomm[0] != '\0')
		return;

	for (i = 0; d->inbuf[i] != '\n' && d->inbuf[i] != '\r'; i++) {
		if (d->inbuf[i] == '\0')
			return;
	}

	for (i = 0, k = 0; d->inbuf[i] != '\n' && d->inbuf[i] != '\r'; i++) {
		if (k >= MAX_INPUT_LENGTH - 2) {
			write_to_descriptor(d->descriptor, "Line too long.\n\r", 0, color);
			for (; d->inbuf[i] != '\0'; i++) {
				if (d->inbuf[i] == '\n' || d->inbuf[i] == '\r')
					break;
			}
			d->inbuf[i] = '\n';
			d->inbuf[i + 1] = '\0';
			break;
		}

		if (d->inbuf[i] == '\b' && k > 0)
			--k;
		else if (isascii(d->inbuf[i]) && isprint(d->inbuf[i]))
			d->incomm[k++] = d->inbuf[i];
	}

	if (k == 0)
		d->incomm[k++] = ' ';
	d->incomm[k] = '\0';

	if (d->incomm[0] == '!')
		strcpy(d->incomm, d->inlast);
	else
		strcpy(d->inlast, d->incomm);

	while (d->inbuf[i] == '\n' || d->inbuf[i] == '\r')
		i++;

	for (j = 0; (d->inbuf[j] = d->inbuf[i + j]) != '\0'; j++) { }
}

/*
 * Low level output function.
 */
bool process_output(DESCRIPTOR_DATA *d, bool fPrompt)
{
	char buf[MAX_STRING_LENGTH];
	extern bool merc_down;
	bool color;

	color = (d->character != NULL && IS_SET(d->character->act, PLR_COLOUR)) ? TRUE : FALSE;

	if (!merc_down) {
		if (d->showstr_point) {
			write_to_buffer(d, "`W\n\r[Hit Return to continue]\n\r`0", 0);
		} else if (fPrompt && d->connected == CON_PLAYING) {
			CHAR_DATA *ch = d->character;

			if (ch != NULL) {
				if (!IS_SET(ch->comm, COMM_COMPACT))
					write_to_buffer(d, "\n\r", 2);

				if (IS_SET(ch->comm, COMM_PROMPT)) {
					if (!IS_NPC(ch) && ch->pcdata && ch->pcdata->prompt && ch->pcdata->prompt[0] != '\0')
						sprintf(buf, "%s", doparseprompt(ch));
					else
						sprintf(buf, "> ");
					write_to_buffer(d, buf, 0);
				}
			}
		}
	}

	if (d->outtop == 0)
		return TRUE;

	if (!write_to_descriptor(d->descriptor, d->outbuf, d->outtop, color)) {
		d->outtop = 0;
		return FALSE;
	} else {
		d->outtop = 0;
		return TRUE;
	}
}

/*
 * Append onto an output buffer.
 */
void write_to_buffer(DESCRIPTOR_DATA *d, const char *txt, int length)
{
	if (length <= 0)
		length = strlen(txt);

	if (d->outtop == 0 && !d->fcommand) {
		d->outbuf[0] = '\n';
		d->outbuf[1] = '\r';
		d->outtop = 2;
	}

	while (d->outtop + length >= d->outsize) {
		char *outbuf;
		if (d->outsize + 2048 > MAX_OUTPUT_BUFFER)
			return;
		outbuf = alloc_mem(d->outsize + 2048);
		strncpy(outbuf, d->outbuf, d->outtop);
		free_mem(&d->outbuf);
		d->outbuf = outbuf;
		d->outsize += 2048;
	}

	strcpy(d->outbuf + d->outtop, txt);
	d->outtop += length;
}

/*
 * Lowest level output function.
 * Write a block of text to the file descriptor.
 */
bool write_to_descriptor(int desc, char *txt, int length, bool color)
{
	int iStart;
	int nWrite;
	int nBlock;
	char *colored;

	if (length <= 0)
		length = strlen(txt);

	colored = do_color(txt, color);
	length = strlen(colored);

	for (iStart = 0; iStart < length; iStart += nWrite) {
		nBlock = UMIN(length - iStart, 4096);
		#if defined(WIN32)
		if ((nWrite = send(desc, colored + iStart, nBlock, 0)) < 0) {
		#else
		if ((nWrite = write(desc, colored + iStart, nBlock)) < 0) {
		#endif
			perror("Write_to_descriptor");
			return FALSE;
		}
	}

	return TRUE;
}

/*
 * Deal with sockets that haven't logged in yet.
 */
void nanny(DESCRIPTOR_DATA *d, char *argument)
{
	char buf[MAX_STRING_LENGTH];
	CHAR_DATA *ch;
	char *pwdnew;
	char *p;
	bool fOld;
	DESCRIPTOR_DATA *d_old;

	while (isspace(*argument))
		argument++;

	ch = d->character;

	switch (d->connected) {
	default:
		bug("Nanny: bad d->connected %d.", d->connected);
		close_socket(d);
		return;

	case CON_GET_ANSI:
		if (argument[0] == 'n' || argument[0] == 'N')
			d->ansi = FALSE;
		else
			d->ansi = TRUE;

		{
			extern char *help_greeting;
			if (help_greeting[0] == '.')
				write_to_buffer(d, help_greeting + 1, 0);
			else
				write_to_buffer(d, help_greeting, 0);
		}

		d->connected = CON_GET_NAME;
		return;

	case CON_GET_NAME:
		if (argument[0] == '\0') {
			close_socket(d);
			return;
		}

		argument[0] = UPPER(argument[0]);
		if (!check_parse_name(argument)) {
			write_to_buffer(d, "Illegal name, try another.\n\rName: ", 0);
			return;
		}

		fOld = load_char_obj(d, argument);
		ch = d->character;

		if (IS_SET(ch->act, PLR_DENY)) {
			sprintf(log_buf, "Denying access to %s@%s.", argument, d->host);
			log_string(log_buf);
			write_to_buffer(d, "You are denied access.\n\r", 0);
			close_socket(d);
			return;
		}

		if (check_reconnect(d, argument, FALSE))
			fOld = TRUE;
		else if (wizlock && !IS_HERO(ch)) {
			write_to_buffer(d, "The game is wizlocked.\n\r", 0);
			close_socket(d);
			return;
		}

		if (fOld) {
			write_to_buffer(d, "Password: ", 0);
			write_to_buffer(d, echo_off_str, 0);
			d->connected = CON_GET_OLD_PASSWORD;
			return;
		} else {
			if (newlock) {
				write_to_buffer(d, "The game is newlocked.\n\r", 0);
				close_socket(d);
				return;
			}
			sprintf(buf, "Did I get that right, %s (Y/N)? ", argument);
			write_to_buffer(d, buf, 0);
			d->connected = CON_CONFIRM_NEW_NAME;
			return;
		}
		break;

	case CON_GET_OLD_PASSWORD:
		write_to_buffer(d, "\n\r", 2);
		if (strcmp(crypt(argument, ch->pcdata->pwd), ch->pcdata->pwd)) {
			write_to_buffer(d, "Wrong password.\n\r", 0);
			close_socket(d);
			return;
		}

		if (ch->pcdata->pwd[0] == 0) {
			write_to_buffer(d, "Warning! Null password!\n\r", 0);
			write_to_buffer(d, "Type 'password null <new password>' to fix.\n\r", 0);
		}

		write_to_buffer(d, echo_on_str, 0);

		if (check_reconnect(d, ch->name, TRUE))
			return;
		if (check_playing(d, ch->name))
			return;

		sprintf(log_buf, "%s@%s has connected.", ch->name, d->host);
		log_string(log_buf);
		do_help(ch, "motd");
		d->connected = CON_READ_MOTD;
		break;

	case CON_BREAK_CONNECT:
		switch (*argument) {
		case 'y': case 'Y':
			for (d_old = descriptor_list; d_old != NULL; d_old = d_next) {
				d_next = d_old->next;
				if (d_old == d || d_old->character == NULL)
					continue;
				if (str_cmp(ch->name, d_old->character->name))
					continue;
				close_socket(d_old);
				if (d->character != NULL) {
					free_char(d->character);
					d->character = NULL;
				}
				d->connected = CON_GET_NAME;
				break;
			}
			write_to_buffer(d, "Disconnected.   Re-enter name: ", 0);
			if (d->character != NULL) {
				free_char(d->character);
				d->character = NULL;
			}
			d->connected = CON_GET_NAME;
			break;

		case 'n': case 'N':
			write_to_buffer(d, "Name: ", 0);
			if (d->character != NULL) {
				free_char(d->character);
				d->character = NULL;
			}
			d->connected = CON_GET_NAME;
			break;

		default:
			write_to_buffer(d, "Please type Y or N? ", 0);
			break;
		}
		break;

	case CON_CONFIRM_NEW_NAME:
		switch (*argument) {
		case 'y': case 'Y':
			sprintf(buf, "New character.\n\rGive me a password for %s: %s", ch->name, echo_off_str);
			write_to_buffer(d, buf, 0);
			d->connected = CON_GET_NEW_PASSWORD;
			if (d->ansi)
				SET_BIT(ch->act, PLR_COLOUR);
			break;

		case 'n': case 'N':
			write_to_buffer(d, "Ok, what IS it, then? ", 0);
			free_char(d->character);
			d->character = NULL;
			d->connected = CON_GET_NAME;
			break;

		default:
			write_to_buffer(d, "Please type Yes or No? ", 0);
			break;
		}
		break;

	case CON_GET_NEW_PASSWORD:
		write_to_buffer(d, "\n\r", 2);
		if (strlen(argument) < 5) {
			write_to_buffer(d, "Password must be at least five characters long.\n\rPassword: ", 0);
			return;
		}

		pwdnew = crypt(argument, ch->name);
		for (p = pwdnew; *p != '\0'; p++) {
			if (*p == '~') {
				write_to_buffer(d, "New password not acceptable, try again.\n\rPassword: ", 0);
				return;
			}
		}

		free_string(&ch->pcdata->pwd);
		ch->pcdata->pwd = str_dup(pwdnew);
		write_to_buffer(d, "Please retype password: ", 0);
		d->connected = CON_CONFIRM_NEW_PASSWORD;
		break;

	case CON_CONFIRM_NEW_PASSWORD:
		write_to_buffer(d, "\n\r", 2);
		if (strcmp(crypt(argument, ch->pcdata->pwd), ch->pcdata->pwd)) {
			write_to_buffer(d, "Passwords don't match.\n\rRetype password: ", 0);
			d->connected = CON_GET_NEW_PASSWORD;
			return;
		}

		write_to_buffer(d, echo_on_str, 0);
		write_to_buffer(d, "\n\rThe following roles are available:\n\r", 0);
		write_to_buffer(d, "  [1] Wanderer  - A traveler and explorer\n\r", 0);
		write_to_buffer(d, "  [2] Merchant  - A trader and craftsperson\n\r", 0);
		write_to_buffer(d, "  [3] Scholar   - A seeker of knowledge\n\r", 0);
		write_to_buffer(d, "  [4] Guardian  - A protector of the realm\n\r", 0);
		write_to_buffer(d, "\n\rSelect a role: ", 0);
		d->connected = CON_GET_NEW_ROLE;
		break;

	case CON_GET_NEW_ROLE:
		switch (argument[0]) {
		case '1': set_title(ch, " the Wanderer"); break;
		case '2': set_title(ch, " the Merchant"); break;
		case '3': set_title(ch, " the Scholar");  break;
		case '4': set_title(ch, " the Guardian"); break;
		default:
			write_to_buffer(d, "That's not a valid role.\n\rSelect a role [1-4]: ", 0);
			return;
		}

		sprintf(log_buf, "%s@%s new player.", ch->name, d->host);
		log_string(log_buf);
		write_to_buffer(d, "\n\r", 2);
		do_help(ch, "motd");
		d->connected = CON_READ_MOTD;
		break;

	case CON_READ_MOTD:
		write_to_buffer(d, "\n\rWelcome to EmberMUD.\n\r", 0);

		ch->next = char_list;
		char_list = ch;
		ch->next_player = player_list;
		player_list = ch;

		d->connected = CON_PLAYING;

		/* Normalize title (ensure leading space). */
		if (ch->pcdata && ch->pcdata->title && ch->pcdata->title[0] != '\0')
			set_title(ch, ch->pcdata->title);

		if (ch->level == 0) {
			ch->level = 1;
			char_to_room(ch, get_room_index(ROOM_VNUM_TEMPLE));
			send_to_char("\n\r", ch);
			save_char_obj(ch);
		} else if (ch->in_room != NULL) {
			char_to_room(ch, ch->in_room);
		} else {
			char_to_room(ch, get_room_index(ROOM_VNUM_TEMPLE));
		}

		act("$n has entered the game.", ch, NULL, NULL, TO_ROOM);
		do_look(ch, "auto");
		save_char_obj(ch);
		break;
	}
}

/*
 * Parse a name for acceptability.
 */
bool check_parse_name(char *name)
{
	if (is_exact_name(name, "all auto immortal self someone something the you"))
		return FALSE;

	if (strlen(name) < 2)
		return FALSE;

	if (strlen(name) > 12)
		return FALSE;

	{
		char *pc;
		bool fIll;

		fIll = TRUE;
		for (pc = name; *pc != '\0'; pc++) {
			if (!isalpha(*pc))
				return FALSE;
			if (LOWER(*pc) != 'i' && LOWER(*pc) != 'l')
				fIll = FALSE;
		}

		if (fIll)
			return FALSE;
	}

	return TRUE;
}

/*
 * Look for link-dead player to reconnect.
 */
bool check_reconnect(DESCRIPTOR_DATA *d, char *name, bool fConn)
{
	CHAR_DATA *ch;

	for (ch = player_list; ch != NULL; ch = ch->next_player) {
		if ((!fConn || ch->desc == NULL) && !str_cmp(d->character->name, ch->name)) {
			if (fConn == FALSE) {
				free_string(&d->character->pcdata->pwd);
				d->character->pcdata->pwd = str_dup(ch->pcdata->pwd);
			} else {
				free_char(d->character);
				d->character = ch;
				ch->desc = d;
				ch->timer = 0;
				send_to_char("Reconnecting.\n\r", ch);
				act("$n has reconnected.", ch, NULL, NULL, TO_ROOM);
				sprintf(log_buf, "%s@%s reconnected.", ch->name, d->host);
				log_string(log_buf);
				d->connected = CON_PLAYING;
			}
			return TRUE;
		}
	}

	return FALSE;
}

/*
 * Check if already playing.
 */
bool check_playing(DESCRIPTOR_DATA *d, char *name)
{
	DESCRIPTOR_DATA *dold;

	for (dold = descriptor_list; dold; dold = dold->next) {
		if (dold != d
				&& dold->character != NULL
				&& dold->connected != CON_GET_NAME
				&& dold->connected != CON_GET_OLD_PASSWORD
				&& !str_cmp(name, dold->character->name)) {
			write_to_buffer(d,
				"That character is already playing.\n\rDisconnect that player? ", 0);
			d->connected = CON_BREAK_CONNECT;
			return TRUE;
		}
	}

	return FALSE;
}

void stop_idling(CHAR_DATA *ch)
{
	if (ch == NULL || ch->desc == NULL
			|| ch->desc->connected != CON_PLAYING
			|| ch->was_in_room == NULL
			|| ch->in_room != get_room_index(ROOM_VNUM_LIMBO))
		return;

	ch->timer = 0;
	char_from_room(ch);
	char_to_room(ch, ch->was_in_room);
	ch->was_in_room = NULL;
	act("$n has returned from the void.", ch, NULL, NULL, TO_ROOM);
}

/*
 * Write to one char.
 */
void send_to_char(const char *txt, CHAR_DATA *ch)
{
	if (txt != NULL && ch->desc != NULL)
		write_to_buffer(ch->desc, txt, strlen(txt));
}

void printf_to_char(CHAR_DATA *ch, char *fmt, ...)
{
	char param[MAX_STRING_LENGTH * 4];
	va_list args;
	va_start(args, fmt);
	vsprintf(param, fmt, args);
	va_end(args);
	if (param[0] && ch->desc)
		write_to_buffer(ch->desc, param, strlen(param));
}

/*
 * Send to all characters in a room.
 */
void send_to_room(const char *txt, ROOM_INDEX_DATA *room)
{
	CHAR_DATA *ch;

	if (txt == NULL || room == NULL)
		return;
	for (ch = room->people; ch != NULL; ch = ch->next_in_room) {
		if (ch->desc != NULL)
			send_to_char(txt, ch);
	}
}

/*
 * Send a page to one char.
 */
void page_to_char(const char *txt, CHAR_DATA *ch)
{
	if (txt == NULL || ch->desc == NULL)
		return;

	ch->desc->showstr_head = alloc_mem(strlen(txt) + 1);
	strcpy(ch->desc->showstr_head, txt);
	ch->desc->showstr_point = ch->desc->showstr_head;
	show_string(ch->desc, "");
}

/* string pager */
void show_string(struct descriptor_data *d, char *input)
{
	char buf[MAX_STRING_LENGTH];
	char buffer[4 * MAX_STRING_LENGTH];
	register char *scan, *chk;
	int lines = 0, toggle = 1;
	int show_lines;

	one_argument(input, buf);

	if (buf[0] != '\0') {
		if (d->showstr_head)
			free_mem(&d->showstr_head);
		d->showstr_point = 0;
		return;
	}

	if (d->character)
		show_lines = d->character->lines;
	else
		show_lines = 0;

	for (scan = buffer;; scan++, d->showstr_point++) {
		if (((*scan = *d->showstr_point) == '\n' || *scan == '\r') && (toggle = -toggle) < 0) {
			lines++;
		} else if (!*scan || (show_lines > 0 && lines >= show_lines)) {
			*scan = '\0';
			write_to_buffer(d, buffer, strlen(buffer));
			for (chk = d->showstr_point; isspace(*chk); chk++)
				;
			if (!*chk) {
				if (d->showstr_head)
					free_mem(&d->showstr_head);
				d->showstr_point = 0;

				d->showstr_point = 0;
			}
			return;
		}
	}
}

void act(const char *format, CHAR_DATA *ch, const void *arg1, const void *arg2, int type)
{
	act_new(format, ch, arg1, arg2, type, POS_RESTING);
}

#define NAME(ch)	(IS_NPC(ch) ? ch->short_descr : ch->name)

char *act_string(const char *format, CHAR_DATA *to, CHAR_DATA *ch, const void *arg1, const void *arg2)
{
	static char *const he_she[] = { "it", "he", "she" };
	static char *const him_her[] = { "it", "him", "her" };
	static char *const his_her[] = { "its", "his", "her" };
	static char buf[MAX_STRING_LENGTH];
	char fname[MAX_INPUT_LENGTH];
	char *point;
	const char *str = format;
	const char *i;
	CHAR_DATA *vch = (CHAR_DATA *)arg2;

	bzero(buf, sizeof(buf));
	point = buf;

	while (*str) {
		if (*str != '$') {
			*point++ = *str++;
			continue;
		}
		++str;

		if (!arg2 && *str >= 'A' && *str <= 'Z') {
			bug("Act: missing arg2 for code %d.", *str);
			i = " <@@@> ";
		} else {
			switch (*str) {
			default:  bug("Act: bad code %d.", *str); i = " <@@@> "; break;
			case 't': i = (char *)arg1;                              break;
			case 'T': i = (char *)arg2;                              break;
			case 'n': i = (to ? PERS(ch, to) : NAME(ch));           break;
			case 'N': i = (to ? PERS(vch, to) : NAME(vch));         break;
			case 'e': i = he_she[URANGE(0, ch->sex, 2)];            break;
			case 'E': i = he_she[URANGE(0, vch->sex, 2)];           break;
			case 'm': i = him_her[URANGE(0, ch->sex, 2)];           break;
			case 'M': i = him_her[URANGE(0, vch->sex, 2)];          break;
			case 's': i = his_her[URANGE(0, ch->sex, 2)];           break;
			case 'S': i = his_her[URANGE(0, vch->sex, 2)];          break;
			case 'd':
				if (!arg2 || ((char *)arg2)[0] == '\0')
					i = "door";
				else {
					one_argument((char *)arg2, fname);
					i = fname;
				}
				break;
			}
		}

		++str;
		while ((*point = *i))
			++point, ++i;
	}

	*point++ = '`';
	*point++ = 'w';
	*point++ = '\n';
	*point++ = '\r';
	*point = '\0';
	buf[0] = UPPER(buf[0]);

	return buf;
}

void act_new(const char *format, CHAR_DATA *ch, const void *arg1, const void *arg2, int type, int min_pos)
{
	char *txt = NULL;
	CHAR_DATA *to;
	CHAR_DATA *vch = (CHAR_DATA *)arg2;

	if (!format || !format[0])
		return;
	if (!ch) {
		bug("Act: null ch.", 0);
		return;
	}

	if (!ch->in_room)
		to = NULL;
	else if (type == TO_CHAR)
		to = ch;
	else
		to = ch->in_room->people;

	if (type == TO_VICT) {
		if (!vch) {
			bug("Act: null vch with TO_VICT.", 0);
			return;
		}
		if (!vch->in_room) {
			bug("Act: vch in NULL room!", 0);
			return;
		}
		to = vch;
	}

	for (; to; to = (type == TO_CHAR || type == TO_VICT) ? NULL : to->next_in_room) {
		if (!to->desc || to->position < min_pos)
			continue;
		if (type == TO_CHAR && to != ch)
			continue;
		if (type == TO_VICT && (to != vch || to == ch))
			continue;
		if (type == TO_ROOM && to == ch)
			continue;
		if (type == TO_NOTVICT && (to == ch || to == vch))
			continue;

		txt = act_string(format, to, ch, arg1, arg2);
		if (to->desc)
			write_to_buffer(to->desc, txt, strlen(txt));
	}
}

/*
 * Color code processing function.
 * Takes a plaintext string with `x color codes and returns a string
 * with ANSI escape sequences (if color is TRUE) or with codes stripped.
 */
char *do_color(char *plaintext, bool color)
{
	static char outbuf[MAX_OUTPUT_BUFFER];
	register char *inbuf = plaintext;
	register char *out = outbuf;
	char *origout = outbuf;
	int outlen = sizeof(outbuf);
	int inlen;

	if (plaintext == NULL) {
		outbuf[0] = '\0';
		return outbuf;
	}

	inlen = strlen(plaintext);

	while (*inbuf != '\0' && (inbuf - plaintext) < inlen && (out - origout) < outlen - 10) {
		if (*inbuf != '`') {
			*out++ = *inbuf++;
			continue;
		}

		inbuf++;
		if (*inbuf == '\0')
			break;

		if (!color) {
			if (*inbuf == '`')
				*out++ = '`';
			inbuf++;
		} else {
			switch (*inbuf++) {
			case '0':
				if ((out - origout) + 7 > outlen) { *out = '\0'; return outbuf; }
				strcpy(out, "\033[0m"); out += 4; break;
			case 'k':
				if ((out - origout) + 7 > outlen) { *out = '\0'; return outbuf; }
				strcpy(out, "\033[0;30m"); out += 7; break;
			case 'K':
				if ((out - origout) + 7 > outlen) { *out = '\0'; return outbuf; }
				strcpy(out, "\033[1;30m"); out += 7; break;
			case 'r':
				if ((out - origout) + 7 > outlen) { *out = '\0'; return outbuf; }
				strcpy(out, "\033[0;31m"); out += 7; break;
			case 'R':
				if ((out - origout) + 7 > outlen) { *out = '\0'; return outbuf; }
				strcpy(out, "\033[1;31m"); out += 7; break;
			case 'g':
				if ((out - origout) + 7 > outlen) { *out = '\0'; return outbuf; }
				strcpy(out, "\033[0;32m"); out += 7; break;
			case 'G':
				if ((out - origout) + 7 > outlen) { *out = '\0'; return outbuf; }
				strcpy(out, "\033[1;32m"); out += 7; break;
			case 'y':
				if ((out - origout) + 7 > outlen) { *out = '\0'; return outbuf; }
				strcpy(out, "\033[0;33m"); out += 7; break;
			case 'Y':
				if ((out - origout) + 7 > outlen) { *out = '\0'; return outbuf; }
				strcpy(out, "\033[1;33m"); out += 7; break;
			case 'b':
				if ((out - origout) + 7 > outlen) { *out = '\0'; return outbuf; }
				strcpy(out, "\033[0;34m"); out += 7; break;
			case 'B':
				if ((out - origout) + 7 > outlen) { *out = '\0'; return outbuf; }
				strcpy(out, "\033[1;34m"); out += 7; break;
			case 'm':
				if ((out - origout) + 7 > outlen) { *out = '\0'; return outbuf; }
				strcpy(out, "\033[0;35m"); out += 7; break;
			case 'M':
				if ((out - origout) + 7 > outlen) { *out = '\0'; return outbuf; }
				strcpy(out, "\033[1;35m"); out += 7; break;
			case 'c':
				if ((out - origout) + 7 > outlen) { *out = '\0'; return outbuf; }
				strcpy(out, "\033[0;36m"); out += 7; break;
			case 'C':
				if ((out - origout) + 7 > outlen) { *out = '\0'; return outbuf; }
				strcpy(out, "\033[1;36m"); out += 7; break;
			case 'w':
				if ((out - origout) + 7 > outlen) { *out = '\0'; return outbuf; }
				strcpy(out, "\033[0;37m"); out += 7; break;
			case 'W':
				if ((out - origout) + 7 > outlen) { *out = '\0'; return outbuf; }
				strcpy(out, "\033[1;37m"); out += 7; break;
			case '`':
				if ((out - origout) + 2 > outlen) { *out = '\0'; return outbuf; }
				*out++ = '`'; break;
			default:
				if ((out - origout) + 2 > outlen) { *out = '\0'; return outbuf; }
				*out++ = '`'; *out++ = *(inbuf - 1); break;
			}
		}
	}

	if (color) {
		if ((out - origout) + 7 <= outlen) {
			strcpy(out, "\033[0m");
			out += 4;
		}
	}

	*out = '\0';
	return outbuf;
}

/*
 * Simplified prompt parser.
 * Supports:
 *   %r  - newline
 *   %T  - current time
 *   %#  - room vnum (immortals only)
 *   %%  - literal %
 */
char *doparseprompt(CHAR_DATA *ch)
{
	static char finished_prompt[240];
	char workstr[100];
	char *fp_point;
	char *orig_prompt;

	bzero(finished_prompt, sizeof(finished_prompt));
	orig_prompt = ch->pcdata->prompt;
	fp_point = finished_prompt;

	while (*orig_prompt != '\0') {
		if (*orig_prompt != '%') {
			*fp_point++ = *orig_prompt++;
			continue;
		}

		orig_prompt++;
		switch (*orig_prompt) {
		case 'r':
			strcat(finished_prompt, "\n\r");
			fp_point += 2;
			orig_prompt++;
			break;
		case 'T':
			sprintf(workstr, "%s", get_curtime());
			strcat(finished_prompt, workstr);
			fp_point += strlen(workstr);
			orig_prompt++;
			break;
		case '#':
			if (IS_IMMORTAL(ch) && ch->in_room != NULL) {
				sprintf(workstr, "%d", ch->in_room->vnum);
				strcat(finished_prompt, workstr);
				fp_point += strlen(workstr);
			}
			orig_prompt++;
			break;
		default:
			strcat(finished_prompt, "%");
			fp_point++;
			break;
		}
	}

	return (finished_prompt);
}
