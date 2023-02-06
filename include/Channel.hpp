/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nguiard <nguiard@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/24 17:16:05 by nguiard           #+#    #+#             */
/*   Updated: 2023/02/06 18:30:19 by nguiard          ###   ########.fr       */
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

	bool	is_op(User *user)	const;

	//	Modes
	void	set_invite_only(bool status);
	void	set_moderated(bool status);
	void	set_limit(size_t limit);

	bool	invite(User *nick);
	bool	ban(User *nick);
	bool	unban(User *nick);

private:
	string		_name;
	user_set	_users;
	set<User *>	_banned;
	set<User *>	_invited;
	bool		_invite_only, _moderated;
	size_t		_limit;

	bool	_add_user_verifications(User *new_user)	const;
};

#endif