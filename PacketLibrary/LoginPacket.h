#pragma once

// 용도 :
// 분류 :
// 첨언 :
class LoginPacket
{
private: // member var
	

public: // default
	LoginPacket();
	~LoginPacket();

	LoginPacket(const LoginPacket& _other) = delete;
	LoginPacket(LoginPacket&& _other) = delete;

protected:
	LoginPacket& operator=(const LoginPacket& _other) = delete;
	LoginPacket& operator=(const LoginPacket&& _other) = delete;

private:

public: // member Func
};

