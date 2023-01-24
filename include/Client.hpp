/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nguiard <nguiard@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/21 18:42:50 by nguiard           #+#    #+#             */
/*   Updated: 2023/01/24 14:19:54 by nguiard          ###   ########.fr       */
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
	Client();
	Client(int id, int fd);
	Client(const Client &copy);
	// Client(string Nick, string User): _nick(Nick), _user(User) {}
	~Client() {}

	Client &operator = (const Client &copy);

	/*	Commandes relatives exclusivements aux clients	*/
	bool	NICK(const string &new_nick, const client_map &c_map);
	bool	USER(const string &new_user, const string &new_realname);

	string	get_user()	const;
	string	get_nick()	const;
	string	get_real()	const;
	int		get_id()	const;
	int		get_fd()	const;

private:
	int		_id, _fd;
	string	_nick, _user, _realname;
};

#endif