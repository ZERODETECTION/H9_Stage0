#include <windows.h>
#include <winhttp.h>
#include <stdio.h>

#pragma comment(lib, "winhttp.lib")

#define SERVER_HOST L"wslab.de"
#define SERVER_PATH L"/commands"
#define SERVER_PORT 4443

void error(const char *message) {
    fprintf(stderr, "Error: %s\n", message);
    exit(EXIT_FAILURE);
}

int main() {
    HINTERNET hSession = NULL, hConnect = NULL, hRequest = NULL;
    DWORD dwSize = 0;
    BOOL  bResults = FALSE;
    DWORD dwDownloaded = 0;
    LPSTR pszOutBuffer;
    DWORD dwOutBufferSize;
    WCHAR szRequest[1024];

    // Initialize WinHTTP session
    hSession = WinHttpOpen(L"A WinHTTP Example Program/1.0",  
                           WINHTTP_ACCESS_TYPE_DEFAULT_PROXY,
                           WINHTTP_NO_PROXY_NAME, 
                           WINHTTP_NO_PROXY_BYPASS, 0);
    if (!hSession)
        error("Failed to initialize WinHTTP session");

    // Connect to server
    hConnect = WinHttpConnect(hSession, SERVER_HOST, SERVER_PORT, 0);
    if (!hConnect)
        error("Failed to connect to server");

    // Build request
    swprintf_s(szRequest, L"GET %s HTTP/1.1\r\nHost: %s\r\nConnection: close\r\n\r\n", SERVER_PATH, SERVER_HOST);

    // Open request handle
    hRequest = WinHttpOpenRequest(hConnect, L"GET", SERVER_PATH,
                                  NULL, WINHTTP_NO_REFERER,
                                  WINHTTP_DEFAULT_ACCEPT_TYPES,
                                  WINHTTP_FLAG_SECURE);
    if (!hRequest)
        error("Failed to open request");
	
    // Set security flags to ignore SSL certificate errors
    DWORD dwFlags = SECURITY_FLAG_IGNORE_UNKNOWN_CA | SECURITY_FLAG_IGNORE_CERT_DATE_INVALID | SECURITY_FLAG_IGNORE_CERT_CN_INVALID | SECURITY_FLAG_IGNORE_CERT_WRONG_USAGE;
    WinHttpSetOption(hRequest, WINHTTP_OPTION_SECURITY_FLAGS, &dwFlags, sizeof(dwFlags));


    // Send request
    bResults = WinHttpSendRequest(hRequest,
                                  WINHTTP_NO_ADDITIONAL_HEADERS, 0,
                                  WINHTTP_NO_REQUEST_DATA, 0,
                                  0, 0);
    if (!bResults)
        error("Failed to send request");

    // Receive response
    bResults = WinHttpReceiveResponse(hRequest, NULL);
    if (!bResults)
        error("Failed to receive response");

    // Read the response
    do {
        dwSize = 0;
        if (!WinHttpQueryDataAvailable(hRequest, &dwSize))
            error("Failed to query data availability");

        // Allocate space for the buffer
        pszOutBuffer = (LPSTR)malloc(dwSize + 1);
        if (!pszOutBuffer)
            error("Failed to allocate memory for response buffer");

        // Read the response into the buffer
        ZeroMemory(pszOutBuffer, dwSize + 1);

        if (!WinHttpReadData(hRequest, (LPVOID)pszOutBuffer,
                             dwSize, &dwDownloaded))
            error("Failed to read data");

        // Print the response
        printf("%s", pszOutBuffer);

        // Free the memory allocated to the buffer
        free(pszOutBuffer);

    } while (dwSize > 0);

    // Clean up
    WinHttpCloseHandle(hRequest);
    WinHttpCloseHandle(hConnect);
    WinHttpCloseHandle(hSession);

    return 0;
}
