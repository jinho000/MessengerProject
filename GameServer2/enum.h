#pragma once

enum class IOTYPE
{
	SEND,
	RECV,
	ACCEPT,
	DISCONNECT
};

enum 
{
	IOBUFFER_SIZE = 255
};