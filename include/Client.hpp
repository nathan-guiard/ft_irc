/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nguiard <nguiard@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/21 18:42:50 by nguiard           #+#    #+#             */
/*   Updated: 2023/01/24 10:18:46 by nguiard          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_IRC
#define CLIENT_IRC

#include "irc.hpp"

/**
 * @brief Classe Client
 * 
 * @param Nick nickname NICK -> Unique parmis les clients
 * @param User username USER
**/

class Client {
public:
	Client(): _nick(), _user() {}
	// Client(string Nick, string User): _nick(Nick), _user(User) {}
	~Client() {}

	/*	Commandes relatives exclusivements aux clients	*/
	bool	NICK(const string &new_nick, const client_map &c_set);
	bool	USER(const string &new_user, const string &new_realname);

	string	get_user()	const {
		return _user;
	}

	string	get_nick()	const {
		return _nick;
	}

private:
	string	_nick, _user, _realname;
};

#endif