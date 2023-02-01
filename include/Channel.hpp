/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nguiard <nguiard@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/24 17:16:05 by nguiard           #+#    #+#             */
/*   Updated: 2023/02/01 17:02:06 by nguiard          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_IRC
#define CHANNEL_IRC

#include "irc.hpp"

class Channel {
public:
	typedef	set<pair<User *, bool> >	user_set;

	Channel();
	Channel(string name);
	Channel(const Channel &copy);
	~Channel();

	Channel &operator = (const Channel &copy);

	string	get_name()	const;

	bool	add_user(User *new_user, bool is_op);
	bool	rm_user(User *user);
	bool	has_user(User *user);
	bool	is_empty() const;
	bool	broadcast(string const& str, User *excluded);

private:
	string		_name;
	user_set	_users;
};

#endif