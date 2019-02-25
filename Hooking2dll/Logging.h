// #include <string>
//
// class Logger
// {
//     static HWND textArea;
//     static std::wstring curLog;
//
//     static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
//     {
//         switch (msg)
//         {
//         case WM_CLOSE:
//             DestroyWindow(hwnd);
//             break;
//         case WM_DESTROY:
//             PostQuitMessage(0);
//             break;
//         default:
//             return DefWindowProc(hwnd, msg, wParam, lParam);
//         }
//         return 0;
//     }
//
//     void Init(HINSTANCE hInstance)
//     {
//         auto className = L"Hook Logger";
//
//         WNDCLASSEX wc;
//         wc.cbSize = sizeof(WNDCLASSEX);
//         wc.style = 0;
//         wc.lpfnWndProc = WndProc;
//         wc.cbClsExtra = 0;
//         wc.cbWndExtra = 0;
//         wc.hInstance = hInstance;
//         wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
//         wc.hCursor = LoadCursor(NULL, IDC_ARROW);
//         wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
//         wc.lpszMenuName = NULL;
//         wc.lpszClassName = className;
//         wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
//
//         if (!RegisterClassEx(&wc))
//         {
//             MessageBox(NULL, L"Window Registration Failed!", L"Error!",
//                 MB_ICONEXCLAMATION | MB_OK);
//             return;
//         }
//
//         HWND hwnd = CreateWindowEx(
//             WS_EX_CLIENTEDGE,
//             className,
//             className,
//             WS_OVERLAPPEDWINDOW,
//             CW_USEDEFAULT, CW_USEDEFAULT, 400, 400,
//             NULL, NULL, hInstance, NULL);
//
//         if (hwnd == NULL)
//         {
//             MessageBox(NULL, L"Window Creation Failed!", L"Error!",
//                 MB_ICONEXCLAMATION | MB_OK);
//             return;
//         }
//
//         // textArea = CreateWindowEx(WS_EX_CLIENTEDGE, L"edit", L"LOG START\n",
//         //     WS_CHILD | WS_VISIBLE | ES_MULTILINE | ES_AUTOHSCROLL, 0, 0, 400, 380, hwnd, NULL, NULL, NULL);
//
//         ShowWindow(hwnd, SW_SHOW);
//         UpdateWindow(hwnd);
//     }
//
// public:
//
//     Logger(HINSTANCE hInstance)
//     {
//         Init(hInstance);
//     }
//
//     void Log(std::string msg, bool pause = false)
//     {
//         printf(msg.c_str());
//         printf("\n");
//         return;
//         //msg = std::to_wstring(((uint64_t)
//         //    std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count())
//         //    %10000000000ul)
//         //    + L": " + msg + L"\r\n";
//         //curLog = msg + curLog;
//
//         //if (curLog.size() > 1000)
//         //    curLog.resize(1000);
//
//         //TCHAR *pbuf = (TCHAR*)&(curLog.data()[0]);
//         //SetWindowText(textArea, pbuf);
//         ////todo pause (for dbg)
//     }
// };
