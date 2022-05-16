/*
 * Routines for making a HTTP connection to a server on the Internet,
 * sending a simple HTTP GET request, and interpreting the response headers
 * that come back.
 *
 * E. Stark, 11/18/97 for CSE 230
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <netdb.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "http.h"

typedef struct HDRNODE *HEADERS;
HEADERS http_parse_headers(HTTP *http);
void http_free_headers(HEADERS env);

/*
 * Routines to manage HTTP connections
 */

typedef enum { ST_REQ, ST_HDRS, ST_BODY, ST_DONE } HTTP_STATE;

struct http {
  FILE *file;			/* Stream to remote server */
  HTTP_STATE state;		/* State of the connection */
  int code;			/* Response code */
  char version[4];		/* HTTP version from the response */
  char *response;		/* Response string with message */
  HEADERS headers;		/* Reply headers */
};

/*
 * Open an HTTP connection for a specified IP address and port number
 */

HTTP *
http_open(IPADDR *addr, int port)
{
  HTTP *http;
  struct sockaddr_in sa;
  int sock;

  if(addr == NULL)
    return(NULL);
  if((http = malloc(sizeof(*http))) == NULL)
    return(NULL);
  bzero(http, sizeof(*http));
  if((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    free(http);
    return(NULL);
  }
  bzero(&sa, sizeof(sa));
  sa.sin_len = sizeof(sa);
  sa.sin_family = AF_INET;
  sa.sin_port = htons(port);
  bcopy(addr, &sa.sin_addr.s_addr, sizeof(struct in_addr));
  if(connect(sock, (struct sockaddr *)(&sa), sizeof(sa)) < 0
     || (http->file = fdopen(sock, "w+")) == NULL) {
    free(http);
    close(sock);
    return(NULL);
  }
  http->state = ST_REQ;
  return(http);
}

/*
 * Close an HTTP connection that was previously opened.
 */

int
http_close(HTTP *http)
{
  int err;

  err = fclose(http->file);
  free(http->response);
  http_free_headers(http->headers);
  free(http);
  return(err);
}

/*
 * Obtain the underlying FILE in an HTTP connection.
 * This can be used to issue additional headers after the request.
 */

FILE *
http_file(HTTP *http)
{
  return(http->file);
}

/*
 * Issue an HTTP GET request for a URL on a connection.
 * After calling this function, the caller may send additional
 * headers, if desired, then must call http_response() before
 * attempting to read any document returned on the connection.
 */

int http_request(HTTP *http, URL *up)
{
  void *prev;

  if(http->state != ST_REQ)
    return(1);
  /* Ignore SIGPIPE so we don't die while doing this */
  prev = signal(SIGPIPE, SIG_IGN);
  if(fprintf(http->file, "GET %s://%s:%d%s HTTP/1.0\r\nHost: %s\r\n",
	     url_method(up), url_hostname(up), url_port(up),
	     url_path(up), url_hostname(up)) == -1) {
    signal(SIGPIPE, prev);
    return(1);
  }
  http->state = ST_HDRS;
  signal(SIGPIPE, prev);
  return(0);
}

/*
 * Finish outputting an HTTP request and read the reply
 * headers from the response.  After calling this, http_getc()
 * may be used to collect any document returned as part of the
 * response.
 */

int
http_response(HTTP *http)
{
  void *prev;
  char *response;
  int len;

  if(http->state != ST_HDRS)
    return(1);
  /* Ignore SIGPIPE so we don't die while doing this */
  prev = signal(SIGPIPE, SIG_IGN);
  if(fprintf(http->file, "\r\n") == -1
     || fflush(http->file) == EOF) {
    signal(SIGPIPE, prev);
    return(1);
  }
  rewind(http->file);
  signal(SIGPIPE, prev);
  response = fgetln(http->file, &len);
  if(response == NULL
     || (http->response = malloc(len+1)) == NULL)
    return(1);
  strncpy(http->response, response, len);
  do
    http->response[len--] = '\0';
  while(len >= 0 &&
	(http->response[len] == '\r'
	 || http->response[len] == '\n'));
  if(sscanf(http->response, "HTTP/%3s %d ", http->version, &http->code) != 2)
    return(1);
  http->headers = http_parse_headers(http);
  http->state = ST_BODY;
  return(0);
}

/*
 * Retrieve the HTTP status line and code returned as the
 * first line of the response from the server
 */

char *
http_status(HTTP *http, int *code)
{
  if(http->state != ST_BODY)
    return(NULL);
  if(code != NULL)
    *code = http->code;
  return(http->response);
}

/*
 * Read the next character of a document from an HTTP connection
 */

int
http_getc(HTTP *http)
{
  if(http->state != ST_BODY)
    return(EOF);
  return(fgetc(http->file));
}

/*
 * Routines to interpret and manage URL's
 */

struct url {
  char *stuff;			/* Working storage containing all parts */
  char *method;			/* The access method (http, ftp, etc.) */
  char *hostname;		/* The server host name */
  int port;			/* The TCP port to contact */
  char *path;			/* The path of the document on the server */
  int dnsdone;			/* Have we done DNS lookup yet? */
  struct in_addr addr;		/* IP address of the server */
};

/*
 * Parse a URL given as a string into its constituent parts,
 * and return it as a URL object.
 */

URL *
url_parse(char *url)
{
  URL *up;
  char *cp, c;
  char *slash, *colon;

  if((up = malloc(sizeof(*up))) == NULL)
    return(NULL);
  /*
   * Make a copy of the argument that we can fiddle with
   */
  if((up->stuff = strdup(url)) == NULL) {
    free(up);
    return(NULL);
  }
  up->dnsdone = 0;
  bzero(&up->addr, sizeof(struct in_addr));
  /*
   * Now ready to parse the URL
   */
  cp = up->stuff;
  slash = strchr(cp, '/');
  colon = strchr(cp, ':');
  if(colon != NULL) {
    /*
     * If a colon occurs before any slashes, then we assume the portion
     * of the URL before the colon is the access method.
     */
    if(slash == NULL || colon < slash) {
      *colon = '\0';
      free(up->method);
      up->method = strdup(cp);
      cp = colon+1;
      if(!strcasecmp(up->method, "http"))
	up->port = 80;
    }
    if(slash != NULL && *(slash+1) == '/') {
      /*
       * If there are two slashes, then we have a full, absolute URL,
       * and the following string, up to the next slash, colon or the end
       * of the URL, is the host name.
       */
      for(cp = slash+2; *cp != '\0' && *cp != ':' && *cp != '/'; cp++)
	;
      c = *cp;
      *cp = '\0';
      free(up->hostname);
      up->hostname = strdup(slash+2);
      *cp = c;
      /*
       * If we found a ':', then we have to collect the port number
       */
      if(*cp == ':') {
	char *cp1;
	cp1 = ++cp;
	while(isdigit(*cp))
	  cp++;
	c = *cp;
	*cp = '\0';
	up->port = atoi(cp1);
	*cp = c;
      }
    }
    if(*cp == '\0')
      up->path = "/";
    else
      up->path = cp;
  } else {
    /*
     * No colon: a relative URL with no method or hostname
     */
    up->path = cp;
  }
  return(up);
}

/*
 * Free a URL object that was previously created by url_parse()
 */

void
url_free(URL *up)
{
  free(up->stuff);
  if(up->method != NULL) free(up->method);
  if(up->hostname != NULL) free(up->hostname);
  free(up);
}

/*
 * Extract the "access method" portion of a URL
 */

char *
url_method(URL *up)
{
  return(up->method);
}

/*
 * Extract the hostname portion of a URL
 */

char *
url_hostname(URL *up)
{
  return(up->hostname);
}

/*
 * Obtain the TCP port number portion of a URL
 */

int
url_port(URL *up)
{
  return(up->port);
}

/*
 * Obtain the path portion of a URL
 */

char *
url_path(URL *up)
{
  return(up->path);
}

/*
 * Obtain the network (IP) address of the host specified in a URL.
 * This will cause a DNS lookup if the address is not already cached
 * in the URL object.
 */

IPADDR *
url_address(URL *up)
{
  struct hostent *he;

  if(!up->dnsdone) {
    if(up->hostname != NULL && *up->hostname != '\0') {
      if((he = gethostbyname(up->hostname)) == NULL)
	return(NULL);
      bcopy(he->h_addr, &up->addr, sizeof(struct in_addr));
    }
    up->dnsdone = 1;
  }
  return(&up->addr);
}

/*
 * Print out the components of a parsed URL, for debugging purposes
 */

void
url_print(FILE *f, URL *up)
{
  fprintf(f, "URL: method: '%s', host: '%s(%s)', "
	  "port: %d, path: '%s'\n",
	  up->method != NULL ? up->method : "?",
	  up->hostname != NULL ? up->hostname : "?",
	  up->dnsdone ? inet_ntoa(up->addr) : "?.?.?.?",
	  up->port, up->path);
}

/*
 * Routines for parsing the RFC822-style headers that come back
 * as part of the response to an HTTP request.
 */

typedef struct HDRNODE {
    char *key;
    char *value;
    struct HDRNODE *next;
} HDRNODE;

/*
 * Function for parsing RFC 822 header lines directly from input stream.
 */

HEADERS
http_parse_headers(HTTP *http)
{
    FILE *f = http->file;
    HEADERS env = NULL, last = NULL;
    HDRNODE *node;
    int len;
    char *line, *l, *ll, *cp;

    while((ll = fgetln(f, &len)) != NULL) {
	line = l = malloc(len+1);
	l[len] = '\0';
	strncpy(l, ll, len);
	while(len > 0 && (l[len-1] == '\n' || l[len-1] == '\r'))
	      l[--len] = '\0';
	if(len == 0) {
	    free(line);
	    break;
	}
	node = malloc(sizeof(HDRNODE));
	node->next = NULL;
	for(cp = l; *cp == ' '; cp++) ;
	l = cp;
	for( ; *cp != ':' && *cp != '\0'; cp++) ;
	if(*cp == '\0' || *(cp+1) != ' ') {
	    free(line);
	    free(node);
	    continue;
	}
	*cp++ = '\0';
	node->key = strdup(l);
	while(*cp == ' ')
	    cp++;
	node->value = strdup(cp);
	for(cp = node->key; *cp != NULL; cp++)
	    if(isupper(*cp))
		*cp = tolower(*cp);
	if(last == NULL)
	    env = last = node;
	else {
	    last->next = node;
	    last = node;
	}
	free(line);
    }
    return(env);
}

/*
 * Free headers previously created by http_parse_headers()
 */

void
http_free_headers(HEADERS env)
{
    HEADERS next;

    while(env != NULL) {
	free(env->key);
	free(env->value);
	next = env->next;
	free(env);
	env = next;
    }
}

/*
 * Find the value corresponding to a given key in the headers
 */

char *
http_headers_lookup(HTTP *http, char *key)
{
    HEADERS env = http->headers;
    while(env != NULL) {
	if(!strcmp(env->key, key))
	    return(env->value);
	env = env->next;
    }
    return(NULL);
}

/*
 * Print out headers for debugging purposes
 */

void http_print_headers(FILE *f, HTTP *http)
{
    HEADERS env = http->headers;
    while(env != NULL) {
	fprintf(f, "%s: %s\n", env->key, env->value);
	env = env->next;
    }
}
