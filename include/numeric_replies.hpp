/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   numeric_replies.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nguiard <nguiard@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/27 18:48:00 by eleotard          #+#    #+#             */
/*   Updated: 2023/01/31 16:22:05 by nguiard          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef NUMERIC_REPLIES_HPP
# define NUMERIC_REPLIES_HPP

#include "irc.hpp"

# define RPL_WELCOME(nick, user, host) ("001 " + nick + " :Welcome to the Internet Relay Network " + nick + "!" + user + "@" + host + "\r\n")


# define PING(localhost) ("PING " + localhost + "\r\n")

# define PONG(localhost) ("PONG " + localhost + "\r\n")

# define QUIT(nick, user, host, msg) (nick + "!" + user + "@" + host  + " QUIT :" + msg + "\r\n")

# define NICK(nick) ("NICK " + nick + "\r\n")

# define JOIN(nick, user, host, chan) (":" + nick + "!" + user + "@" + host  + " JOIN " + chan + "\r\n")
//message pour tous les gens sur le channel ":<nick persone qui a join>!<user>@<host> JOIN <channel>"

# define PRIVMSG(dest, str) ("PRIVMSG " + dest + " :" + str + "\r\n") //dest cest le nom du channel
//pour envoyer 

# define NOTICE(chan, str) ("NOTICE " + chan + " :" + str + "\r\n")

# define PART(nick, user, host, chan) (":" + nick + "!" + user + "@" + host + " PART " + chan + "\r\n")
//message envoye a tous les gens dans le channel quand la personne nick quitte le channel

# define INVITE(nick, user, host, nickinvite)  (nick + "!" + user + "@" + host + " INIVITE " + nickivite + " " + channel + "\r\n")
//message que recoit la personne qui est invitee dans un channel
//dans la console de la personne qui est invitee et aussi dans log : <nick>!<user>@host INIVITE <nick> <channel>

# define ERR_ALREADYREGISTERED "462 :Unauthorized command (already registered)\r\n"

# define ERR_NEEDMOREPARAMS(command) ("461 " + command + " :Not enough parameters\r\n")

# define ERR_PASSWDMISMATCH "464 :Password incorrect\r\n"

# define ERR_NONICKNAMEGIVEN "431 :No nickname given\r\n"


/*NICKNAME ERROR*/
# define ERR_NICKNAMEISUSE(nick) ("433 NICK :" + nick + "\r\n")

# define ERR_NOSUCHNICK(nick, user, host, invitenick) ("401 " + invitenick + " :No such nick/channel\r\n")

/*CHANNELS*/
# define ERR_BADCHANMASK(channel) ("476" + channel = ":Bad Channel Mask\r\n")

# define ERR_NOPRIVILEGES() ("481 :Permission Denied- You're not an IRC operator\r\n")

# define ERR_BANNEDFROMCHAN(channel) ( "475 " + channel + " :Cannot join channel (+k)\r\n")

# define ERR_CHANNELISFULL(channel) ("471 " + channel + " :Cannot join channel (+l)\r\n")

# define RPL_NAMREPLY(nick, user, host, channel) ("353 " + nick + "!" + user + "@" + host + " = " + channel + " :")
//2e message standard recu quand on rejoint un channel

# define RPL_ENDOFNAMES(nick, user, host, channel) ("366 " + nick + "!" + user + "@" + host + " " + channel + " :End of NAMES list\r\n")
//3e message standard qu on recoit quand on rejoint un channel

# define RPL_INVITING(nick, user, host, invitenick, channel) ("341 " + nick + "!" + user + "@" + host + " " + invitenick + " " + channel)
//afficher ce message dans la log console et dans la console de la personne qui invite
//pas dans la console de la personne qui est invitee. La personne invitee a le message du #define INVITE

#endif

//quand qqun rentre dans un chan
//JOIN message pour tous les gens sur le channel ":<nick persone qui a join>!<user>@<host> JOIN <channel>"
//353 RPL_NAMREPLY message -> sur sa console (et la console de log du coup)
//366 RPL_ENDOFNAMES message -> sur sa console (et la console de log du coup)
