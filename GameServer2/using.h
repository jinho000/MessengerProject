#pragma once

using IOCompletionCallback = std::function<void(DWORD _transferredBytes, IOCompletionData* _IOData)>;

