#include <iostream>
#include <GameServer2/pch.h>
#include <GameServer2/ClientSocket.h>

ClientSocket* clientSocket = nullptr;

void WorkThread()
{

    while (true)
    {
        char buffer[255] = {};
        int len = 255;
        
        int result = recv(clientSocket->GetSocket(),  buffer, len, 0);
        if (result == 0)
        {
            break;
        }
        
        std::string recvStr(buffer);
        std::cout << recvStr << std::endl;
        
        if (recvStr == "quit")
        {
            break;
        }
    }
}

#include <vector>
#include <string>
#include <stack>
using namespace std;
struct numInfo
{
    int number;
    int index;
};

int main()
{
    int count = 0; cin >> count;
    vector<int> resultArry;
    stack<numInfo> st;
    resultArry.resize(count, -1);

    for (int i = 0; i < count; ++i)
    {
        int num = 0; cin >> num;

        while (st.empty() == false && st.top().number < num)
        {
            resultArry[st.top().index] = num;
            st.pop();
        }

        st.push({ num, i });
    }

    string result;
    for (int i = 0; i < count; ++i)
    {
        result += to_string(resultArry[i]);
        result += " ";
    }

    cout << result << endl;
   
    return 0;

    ServerHelper::WSAStart();
    
    clientSocket = new ClientSocket(9900, "127.0.0.1", IPPROTO::IPPROTO_TCP);
    while (clientSocket->ConnectServer() == false)
    {
        std::cout << "서버 연결에 실패했습니다 서버에 다시 연결하려면 아무키나 입력" << std::endl;
        std::string wait;
        std::cin >> wait;

    }

    std::thread workerThread(WorkThread);

    while (true)
    {
        std::string input;
        std::cin >> input;

        send(clientSocket->GetSocket(), input.c_str(), input.length(), 0);

        if (input == "quit")
        {
            break;
        }
    }

    workerThread.join();

    return 0;
}
