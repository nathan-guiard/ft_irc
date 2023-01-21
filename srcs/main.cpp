/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nguiard <nguiard@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/21 16:24:38 by nguiard           #+#    #+#             */
/*   Updated: 2023/01/21 17:36:17 by nguiard          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"

bool args_parsing(int argc, char **argv, int *port, string *password) {
	if (argc != 3) {
		cerr << "ircserv needs 2 arguments." << endl;
		cerr << "Usage: irceserv <PORT> <PASSWORD>" << endl;
		return true;
	}
	for (int i = 0; argv[1][i]; i++) {
		if (!isdigit(argv[1][i])) {
			cerr << "Port needs to be an integer." << endl;
			return true;
		}
	}
	*port = atoi(argv[1]);
	*password = argv[2];
	return false;
}

int main(int argc, char **argv) {
	int	port;
	string password;

	if (args_parsing(argc, argv, &port, &password)) {
		return 2;
	}
	//test nathan
	struct sockaddr_in	s;
	struct sockaddr_in cs;
	int	csock;

	socklen_t sinsize = sizeof cs;

	
	int	sock = socket(AF_INET, SOCK_STREAM, 0);
	s.sin_addr.s_addr = htonl(INADDR_ANY);
	s.sin_family = AF_INET;
	s.sin_port = htons(6667);

	bind(sock, (sockaddr *) &s, sizeof s);

	listen(sock, 1);


	csock = accept(sock, (sockaddr *) &cs, &sinsize);
	
	char buff[16];
	while (read(csock, buff, 16) > 0) {
		write(1, buff, 16);
	}
}