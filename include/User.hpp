/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nguiard <nguiard@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/21 18:42:50 by nguiard           #+#    #+#             */
/*   Updated: 2023/01/24 14:19:54 by nguiard          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef User_IRC
#define User_IRC

#include "irc.hpp"

/**
 * @brief Classe User
 * 
 * @param Nick nickname NICK -> Unique parmis les users
 * @param User username USER
**/

class User {
public:
	User();
	User(int id, int fd);
	User(const User &copy);
	~User() {}

	User &operator = (const User &copy);

	/*	Commandes relatives exclusivements aux users	*/
	bool	command_NICK(const string &new_nick, const user_map &c_map);
	bool	command_USER(const string &new_user, const string &new_realname);

	bool	send_to(int code, string text);

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