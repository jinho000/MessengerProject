#pragma once


class TCPSession;
class PacketBase;

void DispatchLoginPacket(TCPSession* _TCPSession, std::unique_ptr<PacketBase> _loginPacket);
void DispatchJoinPacket(TCPSession* _TCPSession, std::unique_ptr<PacketBase> _joinPacket);
void DispatchIDCheckPacket(TCPSession* _TCPSession, std::unique_ptr<PacketBase> _IDCheckPacket);
void DispatchAddFriendPacket(TCPSession* _TCPSession, std::unique_ptr<PacketBase> _AddFriendPacket);
void DispatchSendChattingPacket(TCPSession* _TCPSession, std::unique_ptr<PacketBase> _sendChattingPacket);
void DispatchLogoutPacket(TCPSession* _TCPSession, std::unique_ptr<PacketBase> _sendChattingPacket);
void DispatchReadChattingPacket(TCPSession* _TCPSession, std::unique_ptr<PacketBase> _sendChattingPacket);
void DispatchClientExitPacket(TCPSession* _TCPSession, std::unique_ptr<PacketBase> _clientExitPacket);