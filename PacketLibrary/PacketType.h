#pragma once

enum class PACKET_TYPE
{
	LOGIN,
	LOGIN_RESULT,
	JOIN,
	JOIN_RESULT,
	SEND_CHATTING,
	RECV_CHATTING,
	IDCHECK,
	IDCHECK_RESULT,
	ADD_FRIEND,
	ADD_FRIEND_RESULT,
	LOGOUT
};
