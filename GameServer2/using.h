#pragma once

using IOCompletionCallback = std::function<void(DWORD _transferredBytes, LPOVERLAPPED _IOData)>;
