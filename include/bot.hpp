/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bot.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nguiard <nguiard@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/07 14:05:52 by nguiard           #+#    #+#             */
/*   Updated: 2023/02/07 17:22:34 by nguiard          ###   ########.fr       */
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
	Client(): a(false), b(a), c(a), d(a), e(a), has_failed(a) {}
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
		switch (level) {
			case 0:
				a = true;
			case 1:
				b = true;
			case 2:
				c = true;
			case 3:
				d = true;
			case 4:
				e = true;
		}
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