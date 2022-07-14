#pragma once

using IOCompletionCallback = std::function<void(DWORD transferredBytes, LPOVERLAPPED IOData)>;
