/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nguiard <nguiard@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/21 19:05:37 by nguiard           #+#    #+#             */
/*   Updated: 2023/01/24 14:21:07 by nguiard          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client(): _id(-1), _fd(-1), _nick(), _user(), _realname() {}

Client::Client(int id, int fd): _id(id), _fd(fd), _nick(), _user(), _realname() {}

Client::Client(const Client &copy) {
	*this = copy;
}

string	Client::get_user()	const {
	return _user;
}

string	Client::get_nick()	const {
	return _nick;
}

string	Client::get_real()	const {
	return _realname;
}

int	Client::get_id()	const {
	return _id;
}

int	Client::get_fd()	const {
	return _fd;
}

Client &Client::operator = (const Client &copy) {
	this->_fd = copy._fd;
	this->_id = copy._id;
	this->_nick = copy._nick;
	this->_realname = copy._realname;
	this->_user = copy._user;
	return *this;
}

/**
 * @brief Execute la commande NICK
 * 
 * @param new_nick	nouveau nickname
 * @param c_map 	set de tous les clients deja existants
 * @return true 	le nickname a ete change
 * @return false	le nickname n'a pas ete change (erreur)
 */
bool	Client::NICK(const string &new_nick, const client_map &c_map) {
	client_map::const_iterator	it = c_map.begin();
	client_map::const_iterator	ite = c_map.end();

	if (new_nick.empty()) {
		// Execute l'erreur ERR_NONICKNAMEGIVEN
		return false;
	}

	// Check si la string new_nick est bien formattee
	// sinon ERR_ERRONEUSNICKNAME et return false

	for (; it != ite; it++) {
		bool	is_the_same = (*it).second.get_nick() == new_nick;
		bool	is_not_me	= (*it).second.get_nick() != _nick;

		if (is_the_same && is_not_me) {
			// Execute l'erreur ERR_NICKNAMEINUSE
			return false;
		}
	}
	_nick = new_nick;
	return true;
}

/**
 * @brief Execute la commande USER
 * 
 * @param new_user		nouveau username
 * @param new_realname 	nouveau realname
 * @return true 		le username et le realname on ete enregistres
 * @return false		le username et le realname n'ont pas ete change (erreur)
 */
bool Client::USER(const string &new_user, const string &new_realname) {
	bool	not_enough_params = new_user.empty() || new_realname.empty();
	bool	already_registered = !_user.empty() || !_realname.empty();

	if (not_enough_params) {
		// Execute l'erreur ERR_NEEDMOREPARAMS
		return false;
	}

	// il faut check si on peut avoir des noms vides
	if (already_registered) {
		// Execute ERR_ALREADYREGISTERED
		return false;
	}

	_user = new_user;
	_realname = new_realname;
	return true;
}