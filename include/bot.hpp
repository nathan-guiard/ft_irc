/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bot.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nguiard <nguiard@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/07 14:05:52 by nguiard           #+#    #+#             */
/*   Updated: 2023/02/08 10:02:17 by nguiard          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BOT_HPP
#define BOT_HPP

#include <iostream>
#include <string>
#include <iosfwd>
#include <sstream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <signal.h>
#include <sys/epoll.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <map>

using namespace std;

class Client {
public:
	Client(): a(false), b(false), c(false), d(false), e(false), has_failed(a) {}
	~Client() {}
	
	short	level()	const {
		if (a) {
			if (b) {
				if (c) {
					if (d) {
						if (e) {
							return 5;
						} else
							return 4;
					} else
						return 3;
				} else
					return 2;
			} else
				return 1;
		} else if (has_failed) {
			return -1;
		}
		return 0;
	}

	void	passed(short level) {
		if (level == 0)
			a = true;
		if (level == 1)
			b = true;
		if (level == 2)
			c = true;
		if (level == 3)
			d = true;
		if (level == 4)
			e = true;
	}

	void	failed() {
		a = false;
		b = false;
		c = false;
		d = false;
		e = false;
		has_failed = true;
	}

private:
	bool a, b, c, d, e, has_failed;
};

#endif