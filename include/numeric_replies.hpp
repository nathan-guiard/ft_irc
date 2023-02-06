/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   numeric_replies.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eleotard <eleotard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/27 18:48:00 by eleotard          #+#    #+#             */
/*   Updated: 2023/02/06 18:45:51 by eleotard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef NUMERIC_REPLIES_HPP
# define NUMERIC_REPLIES_HPP

#include "irc.hpp"

# define RPL_WELCOME(nick, user, host) ("001 " + nick + " :Welcome to the Internet Relay Network " + nick + "!" + user + "@" + host + "\r\n")

# define PING(localhost) ("PING " + localhost + "\r\n")

# define PONG(localhost) ("PONG " + localhost + "\r\n")

# define QUIT_REASON(nick, user, host, msg) (nick + "!" + user + "@" + host  + " QUIT :" + msg + "\r\n")
# define QUIT(nick, user, host) (nick + "!" + user + "@" + host  + " QUIT\r\n")

# define NICK(old_nick, nick) (":" + old_nick + " NICK " + nick + "\r\n")

# define JOIN(nick, user, host, chan) (":" + nick + "!" + user + "@" + host  + " JOIN " + chan + "\r\n")
//message pour tous les gens sur le channel ":<nick persone qui a join>!<user>@<host> JOIN <channel>"

# define PRIVMSG(nick, user, host, dest, str) (":" + nick + "!" + user + "@" + host + " PRIVMSG " + dest + " :" + str + "\r\n") //dest cest le nom du channel
//pour envoyer 

# define NOTICE(chan, str) ("NOTICE " + chan + " :" + str + "\r\n")

# define PART(nick, user, host, chan) (":" + nick + "!" + user + "@" + host + " PART " + chan + "\r\n")

# define PART_REASON(nick, user, host, chan, reason) (":" + nick + "!" + user + "@" + host + " PART " + chan + " :" + reason + "\r\n")
//message envoye a tous les gens dans le channel quand la personne nick quitte le channel

# define KILL(nick, target) (":" + nick + " KILL " + target + "\r\n")
# define KILL_REASON(nick, target, reason) (":" + nick + " KILL " + target + " :" + reason + "\r\n")


# define KICK(nick, user, host, chan, kicked, reason) (":" + nick + "!" + user + "@" + host + " KICK " + chan + " " + kicked + " :" + reason + "\r\n")

# define INVITE(nick, user, host, nickinvite)  (nick + "!" + user + "@" + host + " INIVITE " + nickivite + " " + channel + "\r\n")
//message que recoit la personne qui est invitee dans un channel
//dans la console de la personne qui est invitee et aussi dans log : <nick>!<user>@host INIVITE <nick> <channel>

# define RPL_TOPIC(chan, topic) ("332 " + chan + " :" + topic + "\r\n")
//ecrit le topic du channel si celui ci est set 
# define RPL_NOTOPIC(chan) ("331 " + chan + " :No topic is set\r\n")

//topic #chan
	//->renvoie 331 ou 332 return true
//topic #chan "nouveau ntopic"
	//->envoie a tout le monde
//topic #chan ""
	//->clear le topic du chan
	//tt les gens sur le chan meme moi recoit topic #chan "new topic"
//si le user est pas sur le chan
	//442 NOTONCHANNEL return false
//si RPL_TOPIC est envoye envoyer aussi  RPL_TOPICWHOTIME 333
//si le mode protected channel et pas les permissions
	//ERR_CHANOPRIVSNEEDED (482) return false


# define ERR_ALREADYREGISTERED "462 :Unauthorized command (already registered)\r\n"

# define ERR_NEEDMOREPARAMS(command) ("461 " + command + " :Not enough parameters\r\n")

# define ERR_PASSWDMISMATCH "464 :Password incorrect\r\n"

# define ERR_NONICKNAMEGIVEN "431 :No nickname given\r\n"

# define ERR_ERRONEUSNICKNAME(nick) ("432 " + nick + " :Erroneous nickname\r\n")
/*NICKNAME ERROR*/
# define ERR_NICKNAMEISUSE(nick) ("433 NICK :" + nick + "\r\n")

# define ERR_NOSUCHNICK(invitenick) ("401 " + invitenick + " :No such nick/channel\r\n")
# define ERR_NOSUCHCHANNEL(channel) ("403 " + channel + " :No such channel\r\n")

# define RPL_YOUREOPER(nick) ("381 " + nick + " :You are now an IRC operator\r\n")
/*CHANNELS*/
# define ERR_BADCHANMASK(channel) ("476 " + channel + " :Bad Channel Mask\r\n")

# define ERR_NOPRIVILEGES "481 :Permission Denied- You're not an IRC operator\r\n"

# define ERR_BANNEDFROMCHAN(nick, channel) ( "475 " + nick + " " + channel + " :Cannot join channel (+b)\r\n")

# define ERR_CHANNELISFULL(nick, channel) ("471 " + nick + " " + channel + " :Cannot join channel (+l)\r\n")

# define RPL_NAMREPLY(nick, user, host, channel) ("353 " + nick + "!" + user + "@" + host + " = " + channel + " :")
//2e message standard recu quand on rejoint un channel

# define RPL_ENDOFNAMES(nick, user, host, channel) ("366 " + nick + "!" + user + "@" + host + " " + channel + " :End of NAMES list\r\n")
//3e message standard qu on recoit quand on rejoint un channel

# define RPL_INVITING(nick, user, host, invitenick, channel) ("341 " + nick + "!" + user + "@" + host + " " + invitenick + " " + channel)
//afficher ce message dans la log console et dans la console de la personne qui invite
//pas dans la console de la personne qui est invitee. La personne invitee a le message du #define INVITE

# define ERR_USERNOTINCHANNEL(nick, chan) ("441 " + nick + " " + chan + " :They aren't on that channel\r\n")
# define ERR_NOTONCHANNEL(chan) ("442 " + chan + " :You're not on that channel\r\n")
# define ERR_CHANOPRIVSNEEDED(chan) ("482 " + chan + " :You're not channel operator\r\n")
# define ERR_INVITEONLYCHAN(nick, chan) ("473 " + nick + " " + chan + " :Cannot join channel (+i)")

#define ERR_USERSDONTMATCH(nick) ("502 " + nick + " :Cannot change change mode for other users\r\n")
// #define RPL_UMODEIS()

#endif

//quand qqun rentre dans un chan
//JOIN message pour tous les gens sur le channel ":<nick persone qui a join>!<user>@<host> JOIN <channel>"
//353 RPL_NAMREPLY message -> sur sa console (et la console de log du coup)
//366 RPL_ENDOFNAMES message -> sur sa console (et la console de log du coup)
