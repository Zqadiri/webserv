/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parseRequest.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zqadiri <zqadiri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/18 17:22:52 by zqadiri           #+#    #+#             */
/*   Updated: 2022/04/18 22:26:45 by zqadiri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// ! simple program to parse an http request 

#include <stdio.h>


typedef struct Request {
    // enum Method method;  //? possible methods are GET POST PUT
    char *version;
    char *url;
    struct Header *headers;
    char *body;
} Request;


int main(void) {
    //! http header
    char *raw_request = "GET /home.htmlf HTTP/1.1\r\n"
            "Host: localhost:8080\r\n"
            "Connection: keep-alive\r\n"
            "Upgrade-Insecure-Requests: 1\r\n"
            "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\n"
            "User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10_13_3) AppleWebKit/604.5.6 (KHTML, like Gecko) Version/11.0.3 Safari/604.5.6\r\n"
            "Accept-Language: en-us\r\n"
            "DNT: 1\r\n"
            "Accept-Encoding: gzip, deflate\r\n"
            "\r\n"
            "Usually GET requests don\'t have a body\r\n"
            "But I don\'t care in this case :)";
            
}