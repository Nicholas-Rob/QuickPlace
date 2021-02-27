#include <iostream>
#include <Windows.h>
#include <tchar.h>
#include <Psapi.h>


#define NOCORNER -1
#define TOPLEFT 0
#define TOPRIGHT 1
#define BOTTOMLEFT 2
#define BOTTOMRIGHT 3


typedef struct monitor {

    RECT area;
    RECT work;

} Monitor;


typedef struct monitors_cont {

    int amount = 0;
    Monitor* monitors;

} MonitorContainer;



void GetConsoleInput(HWND*);
void PlaceWindow(HWND handle, int corner, Monitor* monitor);
static BOOL CALLBACK MonitorEnum(HMONITOR hMon, HDC hdc, LPRECT lprcMonitor, LPARAM lParam);



int main()
{

    HWND handle = NULL;

    GetConsoleInput(&handle);

    


    return 0;
}


void GetConsoleInput(HWND* handle) {

    MonitorContainer monCon = {0, new Monitor[3]};

    POINT pos;

    
    if (EnumDisplayMonitors(NULL, NULL, MonitorEnum, (LPARAM)&monCon) == 0) {
        printf("failed");
    }
    
    printf("%d Monitors Detected", monCon.amount);

    while(1) {



        if (GetKeyState(VK_CONTROL) < 0) {

            GetCursorPos(&pos);

            //printf("X: %d, Y: %d", pos.x, pos.y);

            int monitor;
            int corner = NOCORNER;

            for (monitor = 0; monitor < monCon.amount; monitor++) {

                RECT rect = monCon.monitors[monitor].area;

                if ( ( pos.x < (rect.left + 10) && pos.x >= rect.left ) && 
                     ( pos.y < (rect.top + 10) && pos.y >= rect.top ) ) {

                    corner = TOPLEFT;
                    break;

                }
                else if ( ( pos.x > (rect.right - 10) && pos.x <= (rect.right - 1) ) &&
                          ( pos.y < (rect.top + 10) && pos.y >= rect.top ) ) {

                    corner = TOPRIGHT;
                    break;

                }
                else if ( ( pos.x < (rect.left + 10) && pos.x >= rect.left ) &&
                          ( pos.y > (rect.bottom - 10) && pos.y <= (rect.bottom - 1) ) ) {

                    corner = BOTTOMLEFT;
                    break;

                }
                else if ( ( pos.x > (rect.right - 10) && pos.x <= (rect.right - 1) ) &&
                          ( pos.y > (rect.bottom - 10) && pos.y <= (rect.bottom - 1) ) ) {

                    corner = BOTTOMRIGHT;
                    break;

                }

            }


            HWND handle = GetForegroundWindow();

            PlaceWindow(handle, corner, &(monCon.monitors[monitor]));

            

        }

        Sleep(50);

    }

}


void PlaceWindow(HWND handle, int corner, Monitor* monitor) {

    int width = (monitor->work.right - monitor->work.left) / 2;
    int height = (monitor->work.bottom - monitor->work.top) / 2;

    
    switch (corner) {

    case NOCORNER:
        return;
        break;

    case TOPLEFT:
        ShowWindow(handle, SW_SHOWNORMAL);
        SetWindowPos(handle, HWND_TOP, monitor->work.left, monitor->work.top, width, height, 0);

        break;

    case TOPRIGHT:
        ShowWindow(handle, SW_SHOWNORMAL);
        SetWindowPos(handle, HWND_TOP, monitor->work.left + width, monitor->work.top, width, height, 0);
        break;

    case BOTTOMLEFT:
        ShowWindow(handle, SW_SHOWNORMAL);
        SetWindowPos(handle, HWND_TOP, monitor->work.left, monitor->work.top + height, width, height, 0);
        break;

    case BOTTOMRIGHT:
        ShowWindow(handle, SW_SHOWNORMAL);
        SetWindowPos(handle, HWND_TOP, monitor->work.left + width, monitor->work.top + height, width, height, 0);
        break;

    default:
        break;

    }

    
}



static BOOL CALLBACK MonitorEnum(HMONITOR hMon, HDC hdc, LPRECT lprcMonitor, LPARAM lParam) {
    MonitorContainer* monCon = (MonitorContainer*)lParam;

    MONITORINFO info = {};

    ZeroMemory(&info, sizeof(MONITORINFO));

    info.cbSize = sizeof(MONITORINFO);


    if (GetMonitorInfo(hMon, &info) == 0) {
        printf("\nfailed\n");
    }

    // Saves total screen area values of each monitor
    RECT* screen = &(monCon->monitors[monCon->amount].area);

    screen->left = (*lprcMonitor).left;
    screen->right = (*lprcMonitor).right;
    screen->top = (*lprcMonitor).top;
    screen->bottom = (*lprcMonitor).bottom;

    
    // Saves work area values of each monitor
    RECT* work = &(monCon->monitors[monCon->amount].work);

    work->left = info.rcWork.left;
    work->right = info.rcWork.right;
    work->top = info.rcWork.top;
    work->bottom = info.rcWork.bottom;

    
    monCon->amount++;


    return TRUE;

}