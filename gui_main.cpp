#include <windows.h>
#include <dwmapi.h>
#include <commctrl.h>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#include <algorithm>

#include "Database.h"
#include "User.h"
#include "Workout.h"
#include "Excercise.h"
#include "Set.h"

// Common Controls v6 Visual Styles preprocessor link
#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

// Global Variables
HINSTANCE hInst;
HWND hWndMain;
HFONT hNormalFont;
HFONT hBoldFont;
HFONT hTitleFont;
HFONT hLargeTitleFont;
HFONT hSmallFont;
HFONT hIconFont;
HBRUSH hBgBrush;
HBRUSH hSidebarBrush;
HBRUSH hCardBrush;
HBRUSH hAccentBrush;
HBRUSH hWhiteBrush;
HBRUSH hInputBgBrush;
HBRUSH hSeparatorBrush;
int gClientW = 1280;
int gClientH = 720;

// Global State
Database db("Data.txt");
User* currentUser = nullptr;
Workout* activeWorkout = nullptr;
Excercise* activeExercise = nullptr;

// Panel Handles
HWND hwndLoginPanel = NULL;
HWND hwndDashboardPanel = NULL;
HWND hwndWorkoutPanel = NULL;
HWND hwndProgressPanel = NULL;

// Control Handles - Login
HWND hwndLogUser = NULL;
HWND hwndLogPass = NULL;
HWND hwndRegUser = NULL;
HWND hwndRegPass = NULL;
HWND hwndRegConfirm = NULL;
HWND hwndRegConfirmLabel = NULL;
HWND hwndLoginTitle = NULL;
HWND hwndLoginError = NULL;
bool isRegisterMode = false;

// Control Handles - Dashboard
HWND hwndDashWelcome = NULL;
HWND hwndWorkoutList = NULL;

// Control Handles - Workout Editor
HWND hwndWorkTitle = NULL;
HWND hwndExerciseList = NULL;
HWND hwndSetList = NULL;
HWND hwndAddSetBtn = NULL;
HWND hwndEditSetBtn = NULL;
HWND hwndRemoveSetBtn = NULL;
HWND hwndSetHeaderLabel = NULL;

// Control Handles - Progress
HWND hwndProgCombo = NULL;
HWND hwndProgResult = NULL;

// Control IDs
enum ControlID {
    // General
    ID_PANEL_LOGIN = 100,
    ID_PANEL_DASHBOARD,
    ID_PANEL_WORKOUT,
    ID_PANEL_PROGRESS,

    // Login Panel
    ID_LOGIN_SUBMIT = 1001,
    ID_REGISTER_TOGGLE,
    ID_REGISTER_SUBMIT,
    ID_BACK_TO_LOGIN,

    // Dashboard Panel
    ID_DASH_WORKOUT_LIST = 2001,
    ID_DASH_ADD_WORKOUT,
    ID_DASH_OPEN_WORKOUT,
    ID_DASH_REMOVE_WORKOUT,
    ID_DASH_PROGRESS_CHECK,
    ID_DASH_SIGNOUT,

    // Workout Panel
    ID_WORK_EXERCISE_LIST = 3001,
    ID_WORK_ADD_STRENGTH,
    ID_WORK_ADD_CARDIO,
    ID_WORK_REMOVE_EXERCISE,
    ID_WORK_SET_LIST,
    ID_WORK_ADD_SET,
    ID_WORK_EDIT_SET,
    ID_WORK_REMOVE_SET,
    ID_WORK_FINISH,
    ID_WORK_BACK_DASH,

    // Progress Panel
    ID_PROG_SEARCH_EDIT = 4001,
    ID_PROG_SEARCH_BTN,
    ID_PROG_BACK
};

// Dialog Structures
struct SetDialogData {
    int reps;
    float weight;
    int rpe;
    char comment[128];
    bool okClicked;
};

struct InputDialogData {
    std::string prompt;
    char buffer[128];
    bool okClicked;
};

struct ExerciseDialogData {
    std::string prompt;
    std::vector<std::string> suggestions;
    char buffer[128];
    bool okClicked;
};

// Function Declarations
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK PanelWndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK InputDlgProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK SetDlgProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK ExerciseDlgProc(HWND, UINT, WPARAM, LPARAM);
bool ShowExerciseInputDialog(HWND hwndParent, const char* title, const char* prompt, std::string& outValue, const std::vector<std::string>& suggestions);
std::vector<std::string> GetExerciseSuggestions(const std::string& type);
void PopulateProgressComboBox();
void DrawCustomButton(LPDRAWITEMSTRUCT pdis);
void DrawCustomListBoxItem(LPDRAWITEMSTRUCT pdis);
void DisableTheme(HWND hwnd);

void CreateLoginPanel(HWND parent);
void CreateDashboardPanel(HWND parent);
void CreateWorkoutPanel(HWND parent);
void CreateProgressPanel(HWND parent);

void SwitchToPanel(HWND targetPanel);
void ApplyFontToChildren(HWND hwndParent, HFONT hFont);
void RefreshWorkoutList();
void RefreshExerciseList();
void RefreshSetList();

// Helper to center windows
void CenterWindow(HWND hwnd) {
    RECT rect;
    GetWindowRect(hwnd, &rect);
    int width = rect.right - rect.left;
    int height = rect.bottom - rect.top;
    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);
    SetWindowPos(hwnd, NULL, (screenWidth - width) / 2, (screenHeight - height) / 2, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
}

// Custom Modal Input Dialog
bool ShowInputDialog(HWND hwndParent, const char* title, const char* prompt, std::string& outValue) {
    InputDialogData data;
    data.prompt = prompt;
    data.buffer[0] = '\0';
    data.okClicked = false;

    // Register dialog class
    WNDCLASSA wc = {0};
    wc.lpfnWndProc = InputDlgProc;
    wc.hInstance = hInst;
    wc.hbrBackground = hBgBrush;
    wc.lpszClassName = "GGInputDialogClass";
    RegisterClassA(&wc);

    HWND hwndDlg = CreateWindowExA(
        WS_EX_DLGMODALFRAME,
        "GGInputDialogClass",
        title,
        WS_POPUPWINDOW | WS_CAPTION,
        0, 0, 380, 200,
        hwndParent,
        NULL,
        hInst,
        &data
    );

    CenterWindow(hwndDlg);
    BOOL useDark = TRUE;
    DwmSetWindowAttribute(hwndDlg, 20, &useDark, sizeof(useDark));
    EnableWindow(hwndParent, FALSE);
    ShowWindow(hwndDlg, SW_SHOW);
    UpdateWindow(hwndDlg);

    // Modal message loop
    MSG msg;
    while (IsWindow(hwndDlg) && GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    EnableWindow(hwndParent, TRUE);
    SetFocus(hwndParent);
    UnregisterClassA("GGInputDialogClass", hInst);

    if (data.okClicked) {
        outValue = data.buffer;
        return true;
    }
    return false;
}

LRESULT CALLBACK InputDlgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    static InputDialogData* pData = nullptr;
    switch (msg) {
        case WM_CREATE: {
            CREATESTRUCTA* pcs = (CREATESTRUCTA*)lParam;
            pData = (InputDialogData*)pcs->lpCreateParams;
            SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)pData);

            // Create controls
            HWND lbl = CreateWindowA("STATIC", pData->prompt.c_str(), WS_CHILD | WS_VISIBLE, 20, 20, 320, 40, hwnd, NULL, hInst, NULL);
            HWND edt = CreateWindowA("EDIT", "", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL, 20, 70, 320, 26, hwnd, (HMENU)102, hInst, NULL);
            HWND btnOk = CreateWindowA("BUTTON", "OK", WS_CHILD | WS_VISIBLE | BS_OWNERDRAW, 150, 115, 90, 30, hwnd, (HMENU)IDOK, hInst, NULL);
            HWND btnCancel = CreateWindowA("BUTTON", "Cancel", WS_CHILD | WS_VISIBLE | BS_OWNERDRAW, 250, 115, 90, 30, hwnd, (HMENU)IDCANCEL, hInst, NULL);

            ApplyFontToChildren(hwnd, hNormalFont);
            SendMessage(lbl, WM_SETFONT, (WPARAM)hBoldFont, TRUE);
            SetFocus(edt);
            break;
        }
        case WM_COMMAND: {
            pData = (InputDialogData*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
            if (LOWORD(wParam) == IDOK) {
                GetDlgItemTextA(hwnd, 102, pData->buffer, 128);
                pData->okClicked = true;
                DestroyWindow(hwnd);
            } else if (LOWORD(wParam) == IDCANCEL) {
                pData->okClicked = false;
                DestroyWindow(hwnd);
            }
            break;
        }
        case WM_DRAWITEM:
            DrawCustomButton((LPDRAWITEMSTRUCT)lParam);
            return TRUE;

        case WM_CTLCOLORSTATIC: {
            HDC hdcStatic = (HDC)wParam;
            SetTextColor(hdcStatic, RGB(243, 244, 246));
            SetBkMode(hdcStatic, TRANSPARENT);
            return (INT_PTR)hBgBrush;
        }

        case WM_CTLCOLOREDIT: {
            HDC hdcEdit = (HDC)wParam;
            SetTextColor(hdcEdit, RGB(243, 244, 246));
            SetBkColor(hdcEdit, RGB(15, 20, 32));
            return (INT_PTR)hInputBgBrush;
        }

        case WM_CLOSE:
            DestroyWindow(hwnd);
            break;
        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

// Custom Modal Set Dialog (Strength Exercise)
bool ShowSetDialog(HWND hwndParent, int& reps, float& weight, int& rpe, std::string& comment, bool isEdit) {
    SetDialogData data;
    data.reps = reps;
    data.weight = weight;
    data.rpe = rpe;
    strncpy(data.comment, comment.c_str(), 127);
    data.comment[127] = '\0';
    data.okClicked = false;

    // Register dialog class
    WNDCLASSA wc = {0};
    wc.lpfnWndProc = SetDlgProc;
    wc.hInstance = hInst;
    wc.hbrBackground = hBgBrush;
    wc.lpszClassName = "GGSetDialogClass";
    RegisterClassA(&wc);

    HWND hwndDlg = CreateWindowExA(
        WS_EX_DLGMODALFRAME,
        "GGSetDialogClass",
        isEdit ? "Edit Strength Set" : "Add Strength Set",
        WS_POPUPWINDOW | WS_CAPTION,
        0, 0, 380, 290,
        hwndParent,
        NULL,
        hInst,
        &data
    );

    CenterWindow(hwndDlg);
    BOOL useDark = TRUE;
    DwmSetWindowAttribute(hwndDlg, 20, &useDark, sizeof(useDark));
    EnableWindow(hwndParent, FALSE);
    ShowWindow(hwndDlg, SW_SHOW);
    UpdateWindow(hwndDlg);

    // Modal message loop
    MSG msg;
    while (IsWindow(hwndDlg) && GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    EnableWindow(hwndParent, TRUE);
    SetFocus(hwndParent);
    UnregisterClassA("GGSetDialogClass", hInst);

    if (data.okClicked) {
        reps = data.reps;
        weight = data.weight;
        rpe = data.rpe;
        comment = data.comment;
        return true;
    }
    return false;
}

LRESULT CALLBACK SetDlgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    static SetDialogData* pData = nullptr;
    switch (msg) {
        case WM_CREATE: {
            CREATESTRUCTA* pcs = (CREATESTRUCTA*)lParam;
            pData = (SetDialogData*)pcs->lpCreateParams;
            SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)pData);

            // Labels
            CreateWindowA("STATIC", "Reps:", WS_CHILD | WS_VISIBLE, 20, 20, 100, 20, hwnd, NULL, hInst, NULL);
            CreateWindowA("STATIC", "Weight (kg):", WS_CHILD | WS_VISIBLE, 20, 60, 100, 20, hwnd, NULL, hInst, NULL);
            CreateWindowA("STATIC", "RPE (1-10):", WS_CHILD | WS_VISIBLE, 20, 100, 100, 20, hwnd, NULL, hInst, NULL);
            CreateWindowA("STATIC", "Comment:", WS_CHILD | WS_VISIBLE, 20, 140, 100, 20, hwnd, NULL, hInst, NULL);

            // Edits
            HWND edtReps = CreateWindowA("EDIT", "", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_NUMBER, 140, 18, 200, 24, hwnd, (HMENU)101, hInst, NULL);
            HWND edtWeight = CreateWindowA("EDIT", "", WS_CHILD | WS_VISIBLE | WS_BORDER, 140, 58, 200, 24, hwnd, (HMENU)102, hInst, NULL);
            HWND edtRpe = CreateWindowA("EDIT", "", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_NUMBER, 140, 98, 200, 24, hwnd, (HMENU)103, hInst, NULL);
            HWND edtComm = CreateWindowA("EDIT", "", WS_CHILD | WS_VISIBLE | WS_BORDER, 140, 138, 200, 24, hwnd, (HMENU)104, hInst, NULL);

            // Set initial values if editing
            if (pData->reps > 0) {
                SetDlgItemInt(hwnd, 101, pData->reps, FALSE);
                std::stringstream ss;
                ss << pData->weight;
                SetDlgItemTextA(hwnd, 102, ss.str().c_str());
                SetDlgItemInt(hwnd, 103, pData->rpe, FALSE);
                SetDlgItemTextA(hwnd, 104, pData->comment);
            }

            CreateWindowA("BUTTON", "OK", WS_CHILD | WS_VISIBLE | BS_OWNERDRAW, 150, 195, 90, 32, hwnd, (HMENU)IDOK, hInst, NULL);
            CreateWindowA("BUTTON", "Cancel", WS_CHILD | WS_VISIBLE | BS_OWNERDRAW, 250, 195, 90, 32, hwnd, (HMENU)IDCANCEL, hInst, NULL);

            ApplyFontToChildren(hwnd, hNormalFont);
            SetFocus(edtReps);
            break;
        }
        case WM_COMMAND: {
            pData = (SetDialogData*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
            if (LOWORD(wParam) == IDOK) {
                char wBuf[32];
                pData->reps = GetDlgItemInt(hwnd, 101, NULL, FALSE);
                GetDlgItemTextA(hwnd, 102, wBuf, 32);
                pData->weight = (float)atof(wBuf);
                pData->rpe = GetDlgItemInt(hwnd, 103, NULL, FALSE);
                GetDlgItemTextA(hwnd, 104, pData->comment, 128);

                pData->okClicked = true;
                DestroyWindow(hwnd);
            } else if (LOWORD(wParam) == IDCANCEL) {
                pData->okClicked = false;
                DestroyWindow(hwnd);
            }
            break;
        }
        case WM_DRAWITEM:
            DrawCustomButton((LPDRAWITEMSTRUCT)lParam);
            return TRUE;

        case WM_CTLCOLORSTATIC: {
            HDC hdcStatic = (HDC)wParam;
            SetTextColor(hdcStatic, RGB(243, 244, 246));
            SetBkMode(hdcStatic, TRANSPARENT);
            return (INT_PTR)hBgBrush;
        }

        case WM_CTLCOLOREDIT: {
            HDC hdcEdit = (HDC)wParam;
            SetTextColor(hdcEdit, RGB(243, 244, 246));
            SetBkColor(hdcEdit, RGB(15, 20, 32));
            return (INT_PTR)hInputBgBrush;
        }

        case WM_CLOSE:
            DestroyWindow(hwnd);
            break;
        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

LRESULT CALLBACK PanelWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
        case WM_COMMAND:
        case WM_NOTIFY:
        case WM_CTLCOLORSTATIC:
        case WM_CTLCOLOREDIT:
        case WM_CTLCOLORLISTBOX:
        case WM_DRAWITEM:
        case WM_MEASUREITEM:
            return SendMessage(GetParent(hwnd), msg, wParam, lParam);
        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
    }
}

LRESULT CALLBACK ExerciseDlgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    static ExerciseDialogData* pData = nullptr;
    switch (msg) {
        case WM_CREATE: {
            CREATESTRUCTA* pcs = (CREATESTRUCTA*)lParam;
            pData = (ExerciseDialogData*)pcs->lpCreateParams;
            SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)pData);

            // Create prompt
            HWND lbl = CreateWindowA("STATIC", pData->prompt.c_str(), WS_CHILD | WS_VISIBLE, 20, 20, 320, 40, hwnd, NULL, hInst, NULL);
            
            // Create combobox (CBS_DROPDOWN style so user can select or type!)
            HWND cb = CreateWindowA("COMBOBOX", "", WS_CHILD | WS_VISIBLE | CBS_DROPDOWN | WS_VSCROLL | WS_TABSTOP, 20, 70, 320, 200, hwnd, (HMENU)102, hInst, NULL);
            DisableTheme(cb);
            
            HWND btnOk = CreateWindowA("BUTTON", "OK", WS_CHILD | WS_VISIBLE | BS_OWNERDRAW, 150, 115, 90, 30, hwnd, (HMENU)IDOK, hInst, NULL);
            HWND btnCancel = CreateWindowA("BUTTON", "Cancel", WS_CHILD | WS_VISIBLE | BS_OWNERDRAW, 250, 115, 90, 30, hwnd, (HMENU)IDCANCEL, hInst, NULL);

            ApplyFontToChildren(hwnd, hNormalFont);
            SendMessage(lbl, WM_SETFONT, (WPARAM)hBoldFont, TRUE);

            // Populate combobox
            for (const auto& suggestion : pData->suggestions) {
                SendMessageA(cb, CB_ADDSTRING, 0, (LPARAM)suggestion.c_str());
            }

            // Set focus to combobox
            SetFocus(cb);
            break;
        }
        case WM_COMMAND: {
            pData = (ExerciseDialogData*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
            if (LOWORD(wParam) == IDOK) {
                GetDlgItemTextA(hwnd, 102, pData->buffer, 128);
                pData->okClicked = true;
                DestroyWindow(hwnd);
            } else if (LOWORD(wParam) == IDCANCEL) {
                pData->okClicked = false;
                DestroyWindow(hwnd);
            }
            break;
        }
        case WM_DRAWITEM:
            DrawCustomButton((LPDRAWITEMSTRUCT)lParam);
            return TRUE;

        case WM_CTLCOLORSTATIC: {
            HDC hdcStatic = (HDC)wParam;
            SetTextColor(hdcStatic, RGB(243, 244, 246));
            SetBkMode(hdcStatic, TRANSPARENT);
            return (INT_PTR)hBgBrush;
        }

        case WM_CTLCOLOREDIT:
        case WM_CTLCOLORLISTBOX: {
            HDC hdcEdit = (HDC)wParam;
            SetTextColor(hdcEdit, RGB(243, 244, 246));
            SetBkColor(hdcEdit, RGB(15, 20, 32));
            return (INT_PTR)hInputBgBrush;
        }

        case WM_CLOSE:
            DestroyWindow(hwnd);
            break;
        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

bool ShowExerciseInputDialog(HWND hwndParent, const char* title, const char* prompt, std::string& outValue, const std::vector<std::string>& suggestions) {
    ExerciseDialogData data;
    data.prompt = prompt;
    data.suggestions = suggestions;
    data.buffer[0] = '\0';
    data.okClicked = false;

    WNDCLASSA wc = {0};
    wc.lpfnWndProc = ExerciseDlgProc;
    wc.hInstance = hInst;
    wc.hbrBackground = hBgBrush;
    wc.lpszClassName = "GGExerciseInputDialogClass";
    RegisterClassA(&wc);

    HWND hwndDlg = CreateWindowExA(
        WS_EX_DLGMODALFRAME,
        "GGExerciseInputDialogClass",
        title,
        WS_POPUPWINDOW | WS_CAPTION,
        0, 0, 380, 200,
        hwndParent,
        NULL,
        hInst,
        &data
    );

    CenterWindow(hwndDlg);
    BOOL useDark = TRUE;
    DwmSetWindowAttribute(hwndDlg, 20, &useDark, sizeof(useDark));
    EnableWindow(hwndParent, FALSE);
    ShowWindow(hwndDlg, SW_SHOW);
    UpdateWindow(hwndDlg);

    MSG msg;
    while (IsWindow(hwndDlg) && GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    EnableWindow(hwndParent, TRUE);
    SetFocus(hwndParent);
    UnregisterClassA("GGExerciseInputDialogClass", hInst);

    if (data.okClicked) {
        outValue = data.buffer;
        return true;
    }
    return false;
}

std::vector<std::string> GetExerciseSuggestions(const std::string& type) {
    std::vector<std::string> suggestions;
    if (type == "Strength") {
        suggestions = { "Bench Press", "Squat", "Deadlift", "Overhead Press", "Bicep Curl", "Tricep Pushdown", "Lat Pulldown", "Leg Press", "Dumbbell Fly", "Lateral Raise" };
    } else {
        suggestions = { "Running", "Cycling", "Rowing", "Swimming", "Treadmill", "Elliptical", "Jump Rope" };
    }

    if (currentUser != nullptr) {
        for (Workout* w : currentUser->getWorkouts()) {
            if (w == nullptr) continue;
            for (const auto& exPair : w->excercises) {
                if (exPair.second != nullptr && exPair.second->getType() == type) {
                    std::string name = exPair.second->name;
                    auto it = std::find_if(suggestions.begin(), suggestions.end(),
                        [&name](const std::string& s) {
                            return _stricmp(s.c_str(), name.c_str()) == 0;
                        });
                    if (it == suggestions.end()) {
                        suggestions.push_back(name);
                    }
                }
            }
        }
    }
    return suggestions;
}

void PopulateProgressComboBox() {
    SendMessage(hwndProgCombo, CB_RESETCONTENT, 0, 0);
    if (currentUser == nullptr) return;

    std::vector<std::string> uniqueExercises;
    for (Workout* w : currentUser->getWorkouts()) {
        if (w == nullptr) continue;
        for (const auto& exPair : w->excercises) {
            if (exPair.second != nullptr && exPair.second->getType() == "Strength") {
                std::string name = exPair.second->name;
                auto it = std::find_if(uniqueExercises.begin(), uniqueExercises.end(),
                    [&name](const std::string& s) {
                        return _stricmp(s.c_str(), name.c_str()) == 0;
                    });
                if (it == uniqueExercises.end()) {
                    uniqueExercises.push_back(name);
                }
            }
        }
    }

    std::sort(uniqueExercises.begin(), uniqueExercises.end());
    for (const auto& name : uniqueExercises) {
        SendMessageA(hwndProgCombo, CB_ADDSTRING, 0, (LPARAM)name.c_str());
    }

    if (!uniqueExercises.empty()) {
        SendMessage(hwndProgCombo, CB_SETCURSEL, 0, 0);
    }
}

void DrawCustomButton(LPDRAWITEMSTRUCT pdis) {
    HDC hdc = pdis->hDC;
    RECT rect = pdis->rcItem;
    
    char text[128];
    GetWindowTextA(pdis->hwndItem, text, 128);

    COLORREF bgColor;
    COLORREF textColor = RGB(243, 244, 246);
    
    bool isPressed = (pdis->itemState & ODS_SELECTED);
    
    bool isPrimary = (pdis->CtlID == ID_LOGIN_SUBMIT || pdis->CtlID == ID_REGISTER_SUBMIT || 
                      pdis->CtlID == ID_DASH_ADD_WORKOUT || pdis->CtlID == ID_WORK_FINISH ||
                      pdis->CtlID == ID_PROG_SEARCH_BTN || pdis->CtlID == IDOK);

    if (isPrimary) {
        bgColor = isPressed ? RGB(29, 78, 216) : RGB(37, 99, 235); // Blue Accent
    } else {
        bgColor = isPressed ? RGB(31, 41, 55) : RGB(55, 65, 81);  // Gray Accent
    }

    HBRUSH hBtnBrush = CreateSolidBrush(bgColor);
    HPEN hBtnPen = CreatePen(PS_SOLID, 1, bgColor);
    HGDIOBJ oldBrush = SelectObject(hdc, hBtnBrush);
    HGDIOBJ oldPen = SelectObject(hdc, hBtnPen);
    
    RoundRect(hdc, rect.left, rect.top, rect.right, rect.bottom, 8, 8);
    
    SelectObject(hdc, oldBrush);
    SelectObject(hdc, oldPen);
    DeleteObject(hBtnBrush);
    DeleteObject(hBtnPen);

    SetTextColor(hdc, textColor);
    SetBkMode(hdc, TRANSPARENT);
    
    SelectObject(hdc, isPrimary ? hBoldFont : hNormalFont);
    DrawTextA(hdc, text, -1, &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
}

void DrawCustomListBoxItem(LPDRAWITEMSTRUCT pdis) {
    if (pdis->itemID == -1) return;

    HDC hdc = pdis->hDC;
    RECT rect = pdis->rcItem;

    // Get item text
    char text[256] = {0};
    SendMessageA(pdis->hwndItem, LB_GETTEXT, pdis->itemID, (LPARAM)text);

    // States
    bool isSelected = (pdis->itemState & ODS_SELECTED);

    // Colors
    COLORREF bgColor, textColor, subTextColor;
    if (isSelected) {
        bgColor    = RGB(29, 60, 140);
        textColor  = RGB(255, 255, 255);
        subTextColor = RGB(191, 219, 254);
    } else {
        bgColor    = RGB(18, 24, 40);
        textColor  = RGB(229, 232, 240);
        subTextColor = RGB(148, 158, 180);
    }

    // Paint item background
    HBRUSH hBg = CreateSolidBrush(bgColor);
    FillRect(hdc, &rect, hBg);
    DeleteObject(hBg);

    // Draw subtle bottom divider
    HPEN hPen = CreatePen(PS_SOLID, 1, isSelected ? RGB(37, 80, 180) : RGB(28, 36, 58));
    HGDIOBJ oldPen = SelectObject(hdc, hPen);
    MoveToEx(hdc, rect.left, rect.bottom - 1, NULL);
    LineTo(hdc, rect.right, rect.bottom - 1);
    SelectObject(hdc, oldPen);
    DeleteObject(hPen);

    SetBkMode(hdc, TRANSPARENT);

    RECT textRect = rect;
    textRect.left  += 16;
    textRect.right -= 12;

    std::string sText(text);

    // ---- Workout list: 2-line layout ----
    if (pdis->CtlID == ID_DASH_WORKOUT_LIST) {
        size_t paren = sText.find("   (");
        if (paren != std::string::npos) {
            std::string name = sText.substr(0, paren);
            std::string date = sText.substr(paren + 4);
            if (!date.empty() && date.back() == ')') date.pop_back();

            // Workout name - bold, larger
            SelectObject(hdc, hBoldFont);
            SetTextColor(hdc, textColor);
            RECT nameR = textRect;
            nameR.top    = rect.top + 7;
            nameR.bottom = rect.top + 26;
            DrawTextA(hdc, name.c_str(), -1, &nameR, DT_LEFT | DT_SINGLELINE);

            // Date - muted small
            SelectObject(hdc, hSmallFont);
            SetTextColor(hdc, subTextColor);
            RECT dateR = textRect;
            dateR.top    = rect.top + 27;
            dateR.bottom = rect.bottom - 5;
            DrawTextA(hdc, date.c_str(), -1, &dateR, DT_LEFT | DT_SINGLELINE);
            return;
        }
    }

    // ---- Exercise list: draw colored badge for [STR] / [CAR] ----
    if (pdis->CtlID == ID_WORK_EXERCISE_LIST) {
        COLORREF badgeColor;
        std::string badge, name;
        if (sText.size() >= 7 && sText.substr(0, 7) == "[STR]  ") {
            badge = "STR";
            name  = sText.substr(7);
            badgeColor = isSelected ? RGB(100, 200, 130) : RGB(52, 168, 83);  // Green
        } else if (sText.size() >= 7 && sText.substr(0, 7) == "[CAR]  ") {
            badge = "CAR";
            name  = sText.substr(7);
            badgeColor = isSelected ? RGB(255, 180, 80) : RGB(234, 133, 17);  // Orange
        } else {
            badge = "";
            name  = sText;
            badgeColor = 0;
        }

        int badgeW = 0;
        if (!badge.empty()) {
            // Draw rounded badge rectangle
            RECT badgeRect;
            badgeRect.left   = textRect.left;
            badgeRect.right  = textRect.left + 42;
            badgeRect.top    = rect.top + (rect.bottom - rect.top - 20) / 2;
            badgeRect.bottom = badgeRect.top + 20;

            HBRUSH hBadge = CreateSolidBrush(badgeColor);
            HPEN hBadgePen = CreatePen(PS_SOLID, 0, badgeColor);
            HGDIOBJ ob = SelectObject(hdc, hBadge);
            HGDIOBJ op = SelectObject(hdc, hBadgePen);
            RoundRect(hdc, badgeRect.left, badgeRect.top, badgeRect.right, badgeRect.bottom, 6, 6);
            SelectObject(hdc, ob);
            SelectObject(hdc, op);
            DeleteObject(hBadge);
            DeleteObject(hBadgePen);

            // Badge text
            SelectObject(hdc, hSmallFont);
            SetTextColor(hdc, RGB(255, 255, 255));
            DrawTextA(hdc, badge.c_str(), -1, &badgeRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

            badgeW = 50;  // space after badge
        }

        // Exercise name
        RECT nameRect = textRect;
        nameRect.left += badgeW;
        SelectObject(hdc, isSelected ? hBoldFont : hNormalFont);
        SetTextColor(hdc, textColor);
        DrawTextA(hdc, name.c_str(), -1, &nameRect, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
        return;
    }

    // ---- Default: sets and others ----
    SelectObject(hdc, isSelected ? hBoldFont : hNormalFont);
    SetTextColor(hdc, textColor);
    DrawTextA(hdc, text, -1, &textRect, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
}

void DisableTheme(HWND hwnd) {
    HMODULE hUxTheme = LoadLibraryA("uxtheme.dll");
    if (hUxTheme) {
        typedef HRESULT(WINAPI* PFNSETWINDOWTHEME)(HWND, LPCWSTR, LPCWSTR);
        PFNSETWINDOWTHEME pSetWindowTheme = (PFNSETWINDOWTHEME)GetProcAddress(hUxTheme, "SetWindowTheme");
        if (pSetWindowTheme) {
            pSetWindowTheme(hwnd, L"", L"");
        }
        FreeLibrary(hUxTheme);
    }
}

// Main Window Entry Point
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    hInst = hInstance;

    // Load Database
    db.load();

    // Fonts initialization
    hNormalFont     = CreateFontA(15, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_SWISS, "Segoe UI");
    hBoldFont       = CreateFontA(15, 0, 0, 0, FW_BOLD,   FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_SWISS, "Segoe UI");
    hTitleFont      = CreateFontA(26, 0, 0, 0, FW_BOLD,   FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_SWISS, "Segoe UI");
    hLargeTitleFont = CreateFontA(42, 0, 0, 0, FW_BOLD,   FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_SWISS, "Segoe UI");
    hSmallFont      = CreateFontA(12, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_SWISS, "Segoe UI");
    hIconFont       = CreateFontA(18, 0, 0, 0, FW_BOLD,   FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_SWISS, "Segoe UI");

    // Colors/Brushes
    hBgBrush        = CreateSolidBrush(RGB(13, 17, 28));   // Deep midnight
    hSidebarBrush   = CreateSolidBrush(RGB(10, 13, 22));   // Even darker sidebar
    hCardBrush      = CreateSolidBrush(RGB(22, 30, 48));   // Card surface
    hAccentBrush    = CreateSolidBrush(RGB(37, 99, 235));  // Primary blue
    hWhiteBrush     = CreateSolidBrush(RGB(15, 20, 32));
    hInputBgBrush   = CreateSolidBrush(RGB(18, 24, 40));
    hSeparatorBrush = CreateSolidBrush(RGB(30, 40, 62));

    // Initialize Common Controls
    INITCOMMONCONTROLSEX icex;
    icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
    icex.dwICC = ICC_STANDARD_CLASSES | ICC_LISTVIEW_CLASSES;
    InitCommonControlsEx(&icex);

    // Register Main Window Class
    WNDCLASSA wc = {0};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = hBgBrush;
    wc.lpszClassName = "GymGainsMainWindow";
    RegisterClassA(&wc);

    // Register Panel Class
    WNDCLASSA pwc = {0};
    pwc.lpfnWndProc = PanelWndProc;
    pwc.hInstance = hInstance;
    pwc.hbrBackground = hBgBrush;
    pwc.lpszClassName = "GGPanelClass";
    RegisterClassA(&pwc);

    // Create Main Window (fixed comfortable size, centered)
    hWndMain = CreateWindowA(
        "GymGainsMainWindow",
        "GYM GAINS - Premium Fitness Tracking",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 1120, 700,
        NULL,
        NULL,
        hInstance,
        NULL
    );

    if (!hWndMain) return 0;

    CenterWindow(hWndMain);

    // Enable Immersive Dark Mode for the Title Bar (Windows 10/11)
    BOOL useDark = TRUE;
    DwmSetWindowAttribute(hWndMain, 20, &useDark, sizeof(useDark));

    ShowWindow(hWndMain, nCmdShow);
    UpdateWindow(hWndMain);

    // Message Loop
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    // Save Database on clean exit
    db.save();

    // Cleanup resources
    DeleteObject(hNormalFont);
    DeleteObject(hBoldFont);
    DeleteObject(hTitleFont);
    DeleteObject(hLargeTitleFont);
    DeleteObject(hSmallFont);
    DeleteObject(hIconFont);
    DeleteObject(hBgBrush);
    DeleteObject(hSidebarBrush);
    DeleteObject(hCardBrush);
    DeleteObject(hAccentBrush);
    DeleteObject(hSeparatorBrush);

    return (int)msg.wParam;
}

// Recursively apply fonts to Win32 controls
void ApplyFontToChildren(HWND hwndParent, HFONT hFont) {
    HWND hwndChild = GetWindow(hwndParent, GW_CHILD);
    while (hwndChild != NULL) {
        SendMessage(hwndChild, WM_SETFONT, (WPARAM)hFont, TRUE);
        ApplyFontToChildren(hwndChild, hFont);
        hwndChild = GetWindow(hwndChild, GW_HWNDNEXT);
    }
}

// Window Procedure
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
        case WM_CREATE: {
            // Instantiates Panels
            CreateLoginPanel(hwnd);
            CreateDashboardPanel(hwnd);
            CreateWorkoutPanel(hwnd);
            CreateProgressPanel(hwnd);

            // Apply global styling initially
            ApplyFontToChildren(hwndLoginPanel, hNormalFont);
            ApplyFontToChildren(hwndDashboardPanel, hNormalFont);
            ApplyFontToChildren(hwndWorkoutPanel, hNormalFont);
            ApplyFontToChildren(hwndProgressPanel, hNormalFont);

            // Default to Login view
            SwitchToPanel(hwndLoginPanel);
            break;
        }
        case WM_COMMAND: {
            int wmId = LOWORD(wParam);
            int wmEvent = HIWORD(wParam);

            // Handle Login Sign-In button click
            if (wmId == ID_LOGIN_SUBMIT) {
                char uBuf[64], pBuf[64];
                GetWindowTextA(hwndLogUser, uBuf, 64);
                GetWindowTextA(hwndLogPass, pBuf, 64);

                std::string username(uBuf);
                std::string password(pBuf);

                User* authenticatedUser = nullptr;
                for (User* u : db.getUsers()) {
                    if (u != nullptr && u->username == username && u->checkPassword(password)) {
                        authenticatedUser = u;
                        break;
                    }
                }

                if (authenticatedUser != nullptr) {
                    currentUser = authenticatedUser;
                    SetWindowTextA(hwndLogUser, "");
                    SetWindowTextA(hwndLogPass, "");
                    SetWindowTextA(hwndLoginError, "");

                    // Set Dashboard Welcome text
                    std::string welcome = "Welcome back, ";
                    if (currentUser->surname.empty()) {
                        welcome += currentUser->name + "!";
                    } else {
                        welcome += currentUser->name + " " + currentUser->surname + "!";
                    }
                    SetWindowTextA(hwndDashWelcome, welcome.c_str());

                    RefreshWorkoutList();
                    SwitchToPanel(hwndDashboardPanel);
                } else {
                    SetWindowTextA(hwndLoginError, "Invalid username or password!");
                }
            }

            // Register Toggle Button
            else if (wmId == ID_REGISTER_TOGGLE) {
                isRegisterMode = !isRegisterMode;
                if (isRegisterMode) {
                    SetWindowTextA(hwndLoginTitle, "Create Account");
                    ShowWindow(hwndRegConfirmLabel, SW_SHOW);
                    ShowWindow(hwndRegConfirm, SW_SHOW);
                    // Hide primary Sign In panel items and show register buttons
                    ShowWindow(GetDlgItem(hwndLoginPanel, ID_LOGIN_SUBMIT), SW_HIDE);
                    ShowWindow(GetDlgItem(hwndLoginPanel, ID_REGISTER_TOGGLE), SW_HIDE);
                    ShowWindow(GetDlgItem(hwndLoginPanel, ID_REGISTER_SUBMIT), SW_SHOW);
                    ShowWindow(GetDlgItem(hwndLoginPanel, ID_BACK_TO_LOGIN), SW_SHOW);
                } else {
                    SetWindowTextA(hwndLoginTitle, "GYM GAINS");
                    ShowWindow(hwndRegConfirmLabel, SW_HIDE);
                    ShowWindow(hwndRegConfirm, SW_HIDE);
                    ShowWindow(GetDlgItem(hwndLoginPanel, ID_LOGIN_SUBMIT), SW_SHOW);
                    ShowWindow(GetDlgItem(hwndLoginPanel, ID_REGISTER_TOGGLE), SW_SHOW);
                    ShowWindow(GetDlgItem(hwndLoginPanel, ID_REGISTER_SUBMIT), SW_HIDE);
                    ShowWindow(GetDlgItem(hwndLoginPanel, ID_BACK_TO_LOGIN), SW_HIDE);
                }
                SetWindowTextA(hwndLoginError, "");
            }

            // Back to Login Button
            else if (wmId == ID_BACK_TO_LOGIN) {
                isRegisterMode = false;
                SetWindowTextA(hwndLoginTitle, "GYM GAINS");
                ShowWindow(hwndRegConfirmLabel, SW_HIDE);
                ShowWindow(hwndRegConfirm, SW_HIDE);
                ShowWindow(GetDlgItem(hwndLoginPanel, ID_LOGIN_SUBMIT), SW_SHOW);
                ShowWindow(GetDlgItem(hwndLoginPanel, ID_REGISTER_TOGGLE), SW_SHOW);
                ShowWindow(GetDlgItem(hwndLoginPanel, ID_REGISTER_SUBMIT), SW_HIDE);
                ShowWindow(GetDlgItem(hwndLoginPanel, ID_BACK_TO_LOGIN), SW_HIDE);
                SetWindowTextA(hwndLoginError, "");
            }

            // Register Submit Button
            else if (wmId == ID_REGISTER_SUBMIT) {
                char uBuf[64], pBuf[64], cpBuf[64];
                GetWindowTextA(hwndLogUser, uBuf, 64);
                GetWindowTextA(hwndRegPass, pBuf, 64);
                GetWindowTextA(hwndRegConfirm, cpBuf, 64);

                std::string username(uBuf);
                std::string password(pBuf);
                std::string confirm(cpBuf);

                if (username.empty() || password.empty()) {
                    SetWindowTextA(hwndLoginError, "All fields are required!");
                } else if (password != confirm) {
                    SetWindowTextA(hwndLoginError, "Passwords do not match!");
                } else {
                    // Check duplicate username
                    bool exists = false;
                    for (User* u : db.getUsers()) {
                        if (u != nullptr && u->username == username) {
                            exists = true;
                            break;
                        }
                    }

                    if (exists) {
                        SetWindowTextA(hwndLoginError, "Username already exists!");
                    } else {
                        // User constructor: name = username, surname = ""
                        User* newUser = new User(username, "", password, username);
                        db.addUser(newUser);
                        db.save();
                        currentUser = newUser;

                        // Clear inputs
                        SetWindowTextA(hwndLogUser, "");
                        SetWindowTextA(hwndRegPass, "");
                        SetWindowTextA(hwndRegConfirm, "");
                        SetWindowTextA(hwndLoginError, "");

                        // Reset screen to Login style for next time
                        isRegisterMode = false;
                        SetWindowTextA(hwndLoginTitle, "GYM GAINS");
                        ShowWindow(hwndRegConfirmLabel, SW_HIDE);
                        ShowWindow(hwndRegConfirm, SW_HIDE);
                        ShowWindow(GetDlgItem(hwndLoginPanel, ID_LOGIN_SUBMIT), SW_SHOW);
                        ShowWindow(GetDlgItem(hwndLoginPanel, ID_REGISTER_TOGGLE), SW_SHOW);
                        ShowWindow(GetDlgItem(hwndLoginPanel, ID_REGISTER_SUBMIT), SW_HIDE);
                        ShowWindow(GetDlgItem(hwndLoginPanel, ID_BACK_TO_LOGIN), SW_HIDE);

                        // Set Dashboard Welcome text
                        std::string welcome = "Welcome back, " + currentUser->name + "!";
                        SetWindowTextA(hwndDashWelcome, welcome.c_str());

                        RefreshWorkoutList();
                        SwitchToPanel(hwndDashboardPanel);
                    }
                }
            }

            // Dashboard - Sign Out
            else if (wmId == ID_DASH_SIGNOUT) {
                currentUser = nullptr;
                db.save();
                SwitchToPanel(hwndLoginPanel);
            }

            // Dashboard - Start New Workout
            else if (wmId == ID_DASH_ADD_WORKOUT) {
                std::string workoutName;
                if (ShowInputDialog(hwnd, "Start Workout", "Enter a name for the new workout:", workoutName)) {
                    if (workoutName.empty()) workoutName = "Unnamed Workout";
                    currentUser->addWorkoutGUI(workoutName);
                    db.save();
                    activeWorkout = currentUser->getWorkouts().back();
                    activeExercise = nullptr;

                    // Update workout screen details
                    std::string header = activeWorkout->name + " (" + activeWorkout->date + ")";
                    SetWindowTextA(hwndWorkTitle, header.c_str());

                    RefreshExerciseList();
                    RefreshSetList();
                    SwitchToPanel(hwndWorkoutPanel);
                }
            }

            // Dashboard - Open Selected Workout
            else if (wmId == ID_DASH_OPEN_WORKOUT) {
                int selection = (int)SendMessage(hwndWorkoutList, LB_GETCURSEL, 0, 0);
                if (selection != LB_ERR) {
                    activeWorkout = currentUser->getWorkouts()[selection];
                    activeExercise = nullptr;

                    // Update workout screen details
                    std::string header = activeWorkout->name + " (" + activeWorkout->date + ")";
                    SetWindowTextA(hwndWorkTitle, header.c_str());

                    RefreshExerciseList();
                    RefreshSetList();
                    SwitchToPanel(hwndWorkoutPanel);
                } else {
                    MessageBoxA(hwnd, "Please select a workout from the list first.", "Information", MB_OK | MB_ICONINFORMATION);
                }
            }

            // Dashboard - Delete Selected Workout
            else if (wmId == ID_DASH_REMOVE_WORKOUT) {
                int selection = (int)SendMessage(hwndWorkoutList, LB_GETCURSEL, 0, 0);
                if (selection != LB_ERR) {
                    if (MessageBoxA(hwnd, "Are you sure you want to delete this workout?", "Confirm Delete", MB_YESNO | MB_ICONWARNING) == IDYES) {
                        delete currentUser->getWorkouts()[selection];
                        currentUser->getWorkouts().erase(currentUser->getWorkouts().begin() + selection);
                        RefreshWorkoutList();
                        db.save();
                    }
                } else {
                    MessageBoxA(hwnd, "Please select a workout from the list first.", "Information", MB_OK | MB_ICONINFORMATION);
                }
            }

            // Dashboard - Switch to Progress Screen
            else if (wmId == ID_DASH_PROGRESS_CHECK) {
                PopulateProgressComboBox();
                SetWindowTextA(hwndProgResult, "Select a Strength exercise from the dropdown list and click Search Progress to view statistics.");
                SwitchToPanel(hwndProgressPanel);
            }

            // Progress Screen - Back
            else if (wmId == ID_PROG_BACK) {
                SwitchToPanel(hwndDashboardPanel);
            }

            // Progress Screen - Search Progress
            else if (wmId == ID_PROG_SEARCH_BTN) {
                int sel = (int)SendMessage(hwndProgCombo, CB_GETCURSEL, 0, 0);
                std::string targetExercise = "";
                if (sel != CB_ERR) {
                    char qBuf[128];
                    SendMessageA(hwndProgCombo, CB_GETLBTEXT, sel, (LPARAM)qBuf);
                    targetExercise = qBuf;
                }

                if (targetExercise.empty()) {
                    SetWindowTextA(hwndProgResult, "Please select an exercise from the list first. If the list is empty, you need to log some Strength workouts first!");
                    break;
                }

                float personalRecord = 0.0f;
                int totalSetsDone = 0;
                std::stringstream historyStream;

                // Simple case-insensitive match helper
                auto toLower = [](std::string s) {
                    std::transform(s.begin(), s.end(), s.begin(), ::tolower);
                    return s;
                };
                std::string targetLower = toLower(targetExercise);

                for (Workout* w : currentUser->getWorkouts()) {
                    if (w == nullptr) continue;
                    for (const auto& exPair : w->excercises) {
                        if (exPair.second != nullptr && toLower(exPair.second->name) == targetLower) {
                            StrengthExcercise* se = dynamic_cast<StrengthExcercise*>(exPair.second);
                            if (se != nullptr) {
                                for (const auto& setPair : se->sets) {
                                    if (setPair.second != nullptr && setPair.second->weight >= 0.0f) {
                                        totalSetsDone++;
                                        if (setPair.second->weight > personalRecord) {
                                            personalRecord = setPair.second->weight;
                                        }
                                        historyStream << " - " << w->date << ": Set " << setPair.first 
                                                      << " -> " << setPair.second->reps << " reps x " 
                                                      << setPair.second->weight << " kg (RPE " << setPair.second->RPE << ")\r\n";
                                    }
                                }
                            }
                        }
                    }
                }

                if (totalSetsDone > 0) {
                    std::stringstream out;
                    out << "=== Stats for [" << targetExercise << "] ===\r\n\r\n"
                        << "Personal Record (PR): " << personalRecord << " kg\r\n"
                        << "Total Sets Logged: " << totalSetsDone << "\r\n\r\n"
                        << "=== History of Logged Sets ===\r\n"
                        << historyStream.str();
                    SetWindowTextA(hwndProgResult, out.str().c_str());
                } else {
                    std::string noData = "No strength data found for exercise: " + targetExercise;
                    SetWindowTextA(hwndProgResult, noData.c_str());
                }
            }

            // Workout Panel - Add Strength Exercise
            else if (wmId == ID_WORK_ADD_STRENGTH) {
                std::string exName;
                auto suggestions = GetExerciseSuggestions("Strength");
                if (ShowExerciseInputDialog(hwnd, "Add Strength Exercise", "Select or type Strength Exercise Name:", exName, suggestions)) {
                    if (!exName.empty()) {
                        activeWorkout->addExcerciseGUI(exName, 0);
                        db.save();
                        RefreshExerciseList();
                    }
                }
            }

            // Workout Panel - Add Cardio Exercise
            else if (wmId == ID_WORK_ADD_CARDIO) {
                std::string exName;
                auto suggestions = GetExerciseSuggestions("Cardio");
                if (ShowExerciseInputDialog(hwnd, "Add Cardio Exercise", "Select or type Cardio Exercise Name:", exName, suggestions)) {
                    if (!exName.empty()) {
                        activeWorkout->addExcerciseGUI(exName, 1);
                        db.save();
                        RefreshExerciseList();
                    }
                }
            }

            // Workout Panel - Remove Exercise
            else if (wmId == ID_WORK_REMOVE_EXERCISE) {
                int selection = (int)SendMessage(hwndExerciseList, LB_GETCURSEL, 0, 0);
                if (selection != LB_ERR) {
                    if (MessageBoxA(hwnd, "Are you sure you want to remove this exercise?", "Confirm Remove", MB_YESNO | MB_ICONWARNING) == IDYES) {
                        // Find map key by list index
                        int listIndex = 0;
                        for (auto it = activeWorkout->excercises.begin(); it != activeWorkout->excercises.end(); ++it) {
                            if (listIndex == selection) {
                                delete it->second;
                                activeWorkout->excercises.erase(it);
                                break;
                            }
                            listIndex++;
                        }
                        activeExercise = nullptr;
                        db.save();
                        RefreshExerciseList();
                        RefreshSetList();
                    }
                } else {
                    MessageBoxA(hwnd, "Select an exercise first.", "Information", MB_OK | MB_ICONINFORMATION);
                }
            }

            // Workout Panel - Selection changed in Exercise List
            else if (wmId == ID_WORK_EXERCISE_LIST && wmEvent == LBN_SELCHANGE) {
                int selection = (int)SendMessage(hwndExerciseList, LB_GETCURSEL, 0, 0);
                if (selection != LB_ERR) {
                    int listIndex = 0;
                    for (auto& exPair : activeWorkout->excercises) {
                        if (listIndex == selection) {
                            activeExercise = exPair.second;
                            break;
                        }
                        listIndex++;
                    }
                } else {
                    activeExercise = nullptr;
                }
                RefreshSetList();
            }

            // Workout Panel - Add Set
            else if (wmId == ID_WORK_ADD_SET) {
                if (activeExercise == nullptr) {
                    MessageBoxA(hwnd, "Please select an exercise from the list first.", "Information", MB_OK | MB_ICONINFORMATION);
                    break;
                }

                if (activeExercise->getType() == "Strength") {
                    int reps = 0, rpe = 0;
                    float weight = 0.0f;
                    std::string comment = "";
                    if (ShowSetDialog(hwnd, reps, weight, rpe, comment, false)) {
                        activeExercise->addSetGUI(reps, weight, rpe, comment);
                        db.save();
                        RefreshSetList();
                    }
                } else {
                    // Cardio input parameters directly
                    std::string distStr, durStr;
                    if (ShowInputDialog(hwnd, "Cardio Distance", "Enter distance (km):", distStr)) {
                        if (ShowInputDialog(hwnd, "Cardio Duration", "Enter duration (minutes):", durStr)) {
                            Cardio* ce = dynamic_cast<Cardio*>(activeExercise);
                            if (ce != nullptr) {
                                ce->distance = (float)atof(distStr.c_str());
                                ce->duration = atoi(durStr.c_str());
                                db.save();
                                RefreshSetList();
                            }
                        }
                    }
                }
            }

            // Workout Panel - Edit Set
            else if (wmId == ID_WORK_EDIT_SET) {
                if (activeExercise == nullptr) break;

                if (activeExercise->getType() == "Strength") {
                    int selection = (int)SendMessage(hwndSetList, LB_GETCURSEL, 0, 0);
                    if (selection != LB_ERR) {
                        StrengthExcercise* se = dynamic_cast<StrengthExcercise*>(activeExercise);
                        if (se != nullptr) {
                            int listIndex = 0;
                            for (auto& sPair : se->sets) {
                                if (listIndex == selection) {
                                    int reps = sPair.second->reps;
                                    float weight = sPair.second->weight;
                                    int rpe = sPair.second->RPE;
                                    std::string comment = sPair.second->comment;

                                    if (ShowSetDialog(hwnd, reps, weight, rpe, comment, true)) {
                                        sPair.second->reps = reps;
                                        sPair.second->weight = weight;
                                        sPair.second->RPE = rpe;
                                        sPair.second->comment = comment;
                                        db.save();
                                        RefreshSetList();
                                    }
                                    break;
                                }
                                listIndex++;
                            }
                        }
                    } else {
                        MessageBoxA(hwnd, "Please select a set to edit.", "Information", MB_OK | MB_ICONINFORMATION);
                    }
                } else {
                    // For cardio, edit is just resetting/adding again
                    SendMessage(hwnd, WM_COMMAND, MAKEWPARAM(ID_WORK_ADD_SET, 0), 0);
                }
            }

            // Workout Panel - Remove Set
            else if (wmId == ID_WORK_REMOVE_SET) {
                if (activeExercise == nullptr) break;

                if (activeExercise->getType() == "Strength") {
                    int selection = (int)SendMessage(hwndSetList, LB_GETCURSEL, 0, 0);
                    if (selection != LB_ERR) {
                        if (MessageBoxA(hwnd, "Are you sure you want to remove this set?", "Confirm Remove", MB_YESNO | MB_ICONWARNING) == IDYES) {
                            StrengthExcercise* se = dynamic_cast<StrengthExcercise*>(activeExercise);
                            if (se != nullptr) {
                                int listIndex = 0;
                                for (auto it = se->sets.begin(); it != se->sets.end(); ++it) {
                                    if (listIndex == selection) {
                                        delete it->second;
                                        se->sets.erase(it);
                                        break;
                                    }
                                    listIndex++;
                                }
                                db.save();
                                RefreshSetList();
                            }
                        }
                    } else {
                        MessageBoxA(hwnd, "Please select a set to remove.", "Information", MB_OK | MB_ICONINFORMATION);
                    }
                } else {
                    // Reset Cardio Stats
                    if (MessageBoxA(hwnd, "Reset cardio stats to 0?", "Confirm Reset", MB_YESNO | MB_ICONWARNING) == IDYES) {
                        activeExercise->removeSet();
                        db.save();
                        RefreshSetList();
                    }
                }
            }

            // Workout Panel - Back to Dashboard (without finishing)
            else if (wmId == ID_WORK_BACK_DASH) {
                int ret = MessageBoxA(hwnd,
                    "Return to Dashboard without finishing?\n\nYour current progress is already saved.",
                    "Back to Dashboard",
                    MB_YESNO | MB_ICONQUESTION);
                if (ret == IDYES) {
                    activeWorkout = nullptr;
                    activeExercise = nullptr;
                    RefreshWorkoutList();
                    SwitchToPanel(hwndDashboardPanel);
                }
            }

            // Workout Panel - Finish Workout
            else if (wmId == ID_WORK_FINISH) {
                activeWorkout = nullptr;
                activeExercise = nullptr;
                db.save();
                RefreshWorkoutList();
                SwitchToPanel(hwndDashboardPanel);
            }
            break;
        }

        // Custom Static & Edit controls coloring for premium visual look
        case WM_CTLCOLORSTATIC: {
            HDC hdcStatic = (HDC)wParam;
            HWND hwndCtrl = (HWND)lParam;

            char className[64] = {0};
            GetClassNameA(hwndCtrl, className, 64);
            if (_stricmp(className, "combobox") == 0) {
                SetTextColor(hdcStatic, RGB(243, 244, 246));
                SetBkColor(hdcStatic, RGB(15, 20, 32));
                return (INT_PTR)hInputBgBrush;
            }

            // Make error text stand out as pastel red, others off-white
            if (hwndCtrl == hwndLoginError) {
                SetTextColor(hdcStatic, RGB(248, 113, 113)); // Soft Red
            } else {
                SetTextColor(hdcStatic, RGB(210, 220, 240)); // Sleek Off-white
            }
            SetBkMode(hdcStatic, TRANSPARENT);
            return (INT_PTR)hBgBrush;
        }

        case WM_CTLCOLOREDIT:
        case WM_CTLCOLORLISTBOX: {
            HDC hdcEdit = (HDC)wParam;
            SetTextColor(hdcEdit, RGB(220, 228, 245));
            SetBkColor(hdcEdit, RGB(18, 24, 40));
            return (INT_PTR)hInputBgBrush;
        }

        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);
            RECT cr;
            GetClientRect(hwnd, &cr);

            // Top accent bar
            RECT topBar = { cr.left, cr.top, cr.right, cr.top + 4 };
            HBRUSH hAccent = CreateSolidBrush(RGB(37, 99, 235));
            FillRect(hdc, &topBar, hAccent);
            DeleteObject(hAccent);

            EndPaint(hwnd, &ps);
            return 0;
        }

        case WM_MEASUREITEM: {
            LPMEASUREITEMSTRUCT pmis = (LPMEASUREITEMSTRUCT)lParam;
            if (pmis->CtlID == ID_DASH_WORKOUT_LIST) {
                pmis->itemHeight = 50;
                return TRUE;
            } else if (pmis->CtlID == ID_WORK_EXERCISE_LIST || pmis->CtlID == ID_WORK_SET_LIST) {
                pmis->itemHeight = 36;
                return TRUE;
            }
            break;
        }

        case WM_DRAWITEM: {
            LPDRAWITEMSTRUCT pdis = (LPDRAWITEMSTRUCT)lParam;
            if (pdis->CtlType == ODT_BUTTON) {
                DrawCustomButton(pdis);
            } else if (pdis->CtlType == ODT_LISTBOX) {
                DrawCustomListBoxItem(pdis);
            }
            return TRUE;
        }

        case WM_SIZE: {
            gClientW = LOWORD(lParam);
            gClientH = HIWORD(lParam);
            if (gClientW < 1) gClientW = 1;
            if (gClientH < 1) gClientH = 1;
            // Resize all panels to fill the window
            if (hwndLoginPanel)    MoveWindow(hwndLoginPanel,    0, 0, gClientW, gClientH, TRUE);
            if (hwndDashboardPanel) MoveWindow(hwndDashboardPanel, 0, 0, gClientW, gClientH, TRUE);
            if (hwndWorkoutPanel)  MoveWindow(hwndWorkoutPanel,  0, 0, gClientW, gClientH, TRUE);
            if (hwndProgressPanel) MoveWindow(hwndProgressPanel,  0, 0, gClientW, gClientH, TRUE);
            break;
        }

        case WM_DESTROY:
            PostQuitMessage(0);
            break;

        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

// GUI View Switcher
void SwitchToPanel(HWND targetPanel) {
    ShowWindow(hwndLoginPanel, SW_HIDE);
    ShowWindow(hwndDashboardPanel, SW_HIDE);
    ShowWindow(hwndWorkoutPanel, SW_HIDE);
    ShowWindow(hwndProgressPanel, SW_HIDE);

    ShowWindow(targetPanel, SW_SHOW);
    UpdateWindow(targetPanel);
}

// Create Sub-Panels (Children of Main Window)

void CreateLoginPanel(HWND parent) {
    hwndLoginPanel = CreateWindowA("GGPanelClass", "", WS_CHILD, 0, 0, 1104, 662, parent, (HMENU)ID_PANEL_LOGIN, hInst, NULL);

    // Title  (centered around x=552)
    hwndLoginTitle = CreateWindowA("STATIC", "GYM GAINS", WS_CHILD | WS_VISIBLE | SS_CENTER, 302, 80, 500, 50, hwndLoginPanel, NULL, hInst, NULL);
    SendMessage(hwndLoginTitle, WM_SETFONT, (WPARAM)hLargeTitleFont, TRUE);

    HWND hwndSub = CreateWindowA("STATIC", "Your personal fitness companion. Track lifts, beat PRs.", WS_CHILD | WS_VISIBLE | SS_CENTER, 252, 137, 600, 20, hwndLoginPanel, NULL, hInst, NULL);
    SendMessage(hwndSub, WM_SETFONT, (WPARAM)hNormalFont, TRUE);

    // Form labels & inputs  (form left edge at 352, width 400)
    CreateWindowA("STATIC", "Username", WS_CHILD | WS_VISIBLE, 352, 185, 260, 18, hwndLoginPanel, NULL, hInst, NULL);
    hwndLogUser = CreateWindowA("EDIT", "", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL, 352, 208, 400, 30, hwndLoginPanel, NULL, hInst, NULL);

    CreateWindowA("STATIC", "Password", WS_CHILD | WS_VISIBLE, 352, 252, 260, 18, hwndLoginPanel, NULL, hInst, NULL);
    hwndLogPass = CreateWindowA("EDIT", "", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_PASSWORD | ES_AUTOHSCROLL, 352, 275, 400, 30, hwndLoginPanel, NULL, hInst, NULL);

    // Registration extra field (hidden)
    hwndRegConfirmLabel = CreateWindowA("STATIC", "Confirm Password", WS_CHILD, 352, 320, 260, 18, hwndLoginPanel, NULL, hInst, NULL);
    hwndRegConfirm = CreateWindowA("EDIT", "", WS_CHILD | WS_BORDER | ES_PASSWORD | ES_AUTOHSCROLL, 352, 343, 400, 30, hwndLoginPanel, NULL, hInst, NULL);

    hwndRegUser = hwndLogUser;
    hwndRegPass = hwndLogPass;

    // Error label
    hwndLoginError = CreateWindowA("STATIC", "", WS_CHILD | WS_VISIBLE | SS_CENTER, 252, 385, 600, 20, hwndLoginPanel, NULL, hInst, NULL);

    // Buttons
    CreateWindowA("BUTTON", "Sign In",       WS_CHILD | WS_VISIBLE | BS_OWNERDRAW, 352, 415, 190, 38, hwndLoginPanel, (HMENU)ID_LOGIN_SUBMIT,    hInst, NULL);
    CreateWindowA("BUTTON", "Sign Up",        WS_CHILD | WS_VISIBLE | BS_OWNERDRAW, 562, 415, 190, 38, hwndLoginPanel, (HMENU)ID_REGISTER_TOGGLE, hInst, NULL);
    CreateWindowA("BUTTON", "Create Profile", WS_CHILD | BS_OWNERDRAW,              352, 415, 190, 38, hwndLoginPanel, (HMENU)ID_REGISTER_SUBMIT,  hInst, NULL);
    CreateWindowA("BUTTON", "Back to Login",  WS_CHILD | BS_OWNERDRAW,              562, 415, 190, 38, hwndLoginPanel, (HMENU)ID_BACK_TO_LOGIN,    hInst, NULL);
}

void CreateDashboardPanel(HWND parent) {
    hwndDashboardPanel = CreateWindowA("GGPanelClass", "", WS_CHILD, 0, 0, 1104, 662, parent, (HMENU)ID_PANEL_DASHBOARD, hInst, NULL);

    // Welcome title
    hwndDashWelcome = CreateWindowA("STATIC", "Welcome, Athlete!", WS_CHILD | WS_VISIBLE, 30, 20, 680, 36, hwndDashboardPanel, NULL, hInst, NULL);
    SendMessage(hwndDashWelcome, WM_SETFONT, (WPARAM)hTitleFont, TRUE);

    HWND hSub = CreateWindowA("STATIC", "Select a past session to review or start a new one.", WS_CHILD | WS_VISIBLE, 30, 60, 600, 16, hwndDashboardPanel, NULL, hInst, NULL);
    SendMessage(hSub, WM_SETFONT, (WPARAM)hSmallFont, TRUE);

    HWND hHist = CreateWindowA("STATIC", "WORKOUT HISTORY", WS_CHILD | WS_VISIBLE, 30, 92, 300, 16, hwndDashboardPanel, NULL, hInst, NULL);
    SendMessage(hHist, WM_SETFONT, (WPARAM)hBoldFont, TRUE);

    // Workouts Listbox
    hwndWorkoutList = CreateWindowA("LISTBOX", "", WS_CHILD | WS_VISIBLE | WS_BORDER | WS_VSCROLL | LBS_NOTIFY | LBS_OWNERDRAWFIXED | LBS_HASSTRINGS,
        30, 115, 710, 510, hwndDashboardPanel, (HMENU)ID_DASH_WORKOUT_LIST, hInst, NULL);

    // Sidebar actions (starting at x=770)
    HWND hActLabel = CreateWindowA("STATIC", "ACTIONS", WS_CHILD | WS_VISIBLE, 770, 92, 300, 16, hwndDashboardPanel, NULL, hInst, NULL);
    SendMessage(hActLabel, WM_SETFONT, (WPARAM)hBoldFont, TRUE);

    CreateWindowA("BUTTON", "[+]  Start New Workout",       WS_CHILD | WS_VISIBLE | BS_OWNERDRAW, 760, 115, 310, 46, hwndDashboardPanel, (HMENU)ID_DASH_ADD_WORKOUT,    hInst, NULL);
    CreateWindowA("BUTTON", "[>]  Open Selected Session",   WS_CHILD | WS_VISIBLE | BS_OWNERDRAW, 760, 171, 310, 46, hwndDashboardPanel, (HMENU)ID_DASH_OPEN_WORKOUT,   hInst, NULL);
    CreateWindowA("BUTTON", "[x]  Delete Selected Workout", WS_CHILD | WS_VISIBLE | BS_OWNERDRAW, 760, 227, 310, 46, hwndDashboardPanel, (HMENU)ID_DASH_REMOVE_WORKOUT, hInst, NULL);
    CreateWindowA("BUTTON", "[~]  Check Progress & PRs",    WS_CHILD | WS_VISIBLE | BS_OWNERDRAW, 760, 283, 310, 46, hwndDashboardPanel, (HMENU)ID_DASH_PROGRESS_CHECK, hInst, NULL);

    CreateWindowA("BUTTON", "Sign Out",                     WS_CHILD | WS_VISIBLE | BS_OWNERDRAW, 760, 579, 310, 46, hwndDashboardPanel, (HMENU)ID_DASH_SIGNOUT,        hInst, NULL);
}

void CreateWorkoutPanel(HWND parent) {
    hwndWorkoutPanel = CreateWindowA("GGPanelClass", "", WS_CHILD, 0, 0, 1104, 662, parent, (HMENU)ID_PANEL_WORKOUT, hInst, NULL);

    // --- Title bar ---
    hwndWorkTitle = CreateWindowA("STATIC", "Active Workout Session", WS_CHILD | WS_VISIBLE, 30, 14, 800, 32, hwndWorkoutPanel, NULL, hInst, NULL);
    SendMessage(hwndWorkTitle, WM_SETFONT, (WPARAM)hTitleFont, TRUE);

    // ---- Left column: Exercises (x=30, w=370) ----
    HWND hExLabel = CreateWindowA("STATIC", "EXERCISES", WS_CHILD | WS_VISIBLE, 30, 56, 200, 16, hwndWorkoutPanel, NULL, hInst, NULL);
    SendMessage(hExLabel, WM_SETFONT, (WPARAM)hBoldFont, TRUE);

    // List: y=78, height=490  →  bottom = 568
    hwndExerciseList = CreateWindowA("LISTBOX", "", WS_CHILD | WS_VISIBLE | WS_BORDER | WS_VSCROLL | LBS_NOTIFY | LBS_OWNERDRAWFIXED | LBS_HASSTRINGS,
        30, 78, 370, 490, hwndWorkoutPanel, (HMENU)ID_WORK_EXERCISE_LIST, hInst, NULL);

    // Row 1 exercise action buttons: y=575, h=30
    CreateWindowA("BUTTON", "[S+] Strength", WS_CHILD | WS_VISIBLE | BS_OWNERDRAW,  30, 575, 118, 30, hwndWorkoutPanel, (HMENU)ID_WORK_ADD_STRENGTH,   hInst, NULL);
    CreateWindowA("BUTTON", "[C+] Cardio",   WS_CHILD | WS_VISIBLE | BS_OWNERDRAW, 154, 575, 118, 30, hwndWorkoutPanel, (HMENU)ID_WORK_ADD_CARDIO,      hInst, NULL);
    CreateWindowA("BUTTON", "[x] Remove",    WS_CHILD | WS_VISIBLE | BS_OWNERDRAW, 278, 575, 122, 30, hwndWorkoutPanel, (HMENU)ID_WORK_REMOVE_EXERCISE, hInst, NULL);

    // ---- Right column: Sets (x=416, w=658) ----
    hwndSetHeaderLabel = CreateWindowA("STATIC", "PERFORMANCE DETAILS (SETS)", WS_CHILD | WS_VISIBLE, 416, 56, 660, 16, hwndWorkoutPanel, NULL, hInst, NULL);
    SendMessage(hwndSetHeaderLabel, WM_SETFONT, (WPARAM)hBoldFont, TRUE);

    // List: y=78, height=490  →  bottom = 568
    hwndSetList = CreateWindowA("LISTBOX", "", WS_CHILD | WS_VISIBLE | WS_BORDER | WS_VSCROLL | LBS_NOTIFY | LBS_OWNERDRAWFIXED | LBS_HASSTRINGS,
        416, 78, 658, 490, hwndWorkoutPanel, (HMENU)ID_WORK_SET_LIST, hInst, NULL);

    // Row 1 set action buttons: y=575, h=30
    hwndAddSetBtn    = CreateWindowA("BUTTON", "Add Set",    WS_CHILD | WS_VISIBLE | BS_OWNERDRAW, 416, 575, 215, 30, hwndWorkoutPanel, (HMENU)ID_WORK_ADD_SET,    hInst, NULL);
    hwndEditSetBtn   = CreateWindowA("BUTTON", "Edit Set",   WS_CHILD | WS_VISIBLE | BS_OWNERDRAW, 637, 575, 215, 30, hwndWorkoutPanel, (HMENU)ID_WORK_EDIT_SET,   hInst, NULL);
    hwndRemoveSetBtn = CreateWindowA("BUTTON", "Remove Set", WS_CHILD | WS_VISIBLE | BS_OWNERDRAW, 858, 575, 216, 30, hwndWorkoutPanel, (HMENU)ID_WORK_REMOVE_SET, hInst, NULL);

    // Row 2 navigation: y=613, h=34  — Back (left) + Finish (rest of width)
    CreateWindowA("BUTTON", "[<]  Back to Dashboard",        WS_CHILD | WS_VISIBLE | BS_OWNERDRAW,  30, 613, 200, 34, hwndWorkoutPanel, (HMENU)ID_WORK_BACK_DASH, hInst, NULL);
    CreateWindowA("BUTTON", "[OK]  Finish & Save Workout",   WS_CHILD | WS_VISIBLE | BS_OWNERDRAW, 240, 613, 834, 34, hwndWorkoutPanel, (HMENU)ID_WORK_FINISH,    hInst, NULL);
}

void CreateProgressPanel(HWND parent) {
    hwndProgressPanel = CreateWindowA("GGPanelClass", "", WS_CHILD, 0, 0, 1104, 662, parent, (HMENU)ID_PANEL_PROGRESS, hInst, NULL);

    HWND hTitle = CreateWindowA("STATIC", "Exercise Progress & Personal Records", WS_CHILD | WS_VISIBLE, 30, 20, 800, 34, hwndProgressPanel, NULL, hInst, NULL);
    SendMessage(hTitle, WM_SETFONT, (WPARAM)hTitleFont, TRUE);

    HWND hSub = CreateWindowA("STATIC", "Select a Strength exercise to view statistics, PRs and full history.", WS_CHILD | WS_VISIBLE, 30, 60, 900, 18, hwndProgressPanel, NULL, hInst, NULL);
    SendMessage(hSub, WM_SETFONT, (WPARAM)hSmallFont, TRUE);

    CreateWindowA("STATIC", "SELECT EXERCISE", WS_CHILD | WS_VISIBLE, 30, 94, 300, 16, hwndProgressPanel, NULL, hInst, NULL);
    hwndProgCombo = CreateWindowA("COMBOBOX", "", WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | WS_VSCROLL | WS_TABSTOP,
        30, 114, 650, 200, hwndProgressPanel, (HMENU)ID_PROG_SEARCH_EDIT, hInst, NULL);
    DisableTheme(hwndProgCombo);

    CreateWindowA("BUTTON", "Search Progress", WS_CHILD | WS_VISIBLE | BS_OWNERDRAW, 698, 111, 376, 40, hwndProgressPanel, (HMENU)ID_PROG_SEARCH_BTN, hInst, NULL);

    // Stats result
    CreateWindowA("STATIC", "STATISTICS", WS_CHILD | WS_VISIBLE, 30, 168, 300, 16, hwndProgressPanel, NULL, hInst, NULL);
    hwndProgResult = CreateWindowA("EDIT",
        "Select an exercise and click Search Progress to see your stats here.",
        WS_CHILD | WS_VISIBLE | WS_BORDER | ES_MULTILINE | ES_READONLY | WS_VSCROLL | ES_AUTOVSCROLL,
        30, 190, 1044, 420, hwndProgressPanel, NULL, hInst, NULL);

    CreateWindowA("BUTTON", "[<]  Back to Dashboard", WS_CHILD | WS_VISIBLE | BS_OWNERDRAW, 30, 624, 280, 36, hwndProgressPanel, (HMENU)ID_PROG_BACK, hInst, NULL);
}

// Data Load Refresh Operations

void RefreshWorkoutList() {
    SendMessage(hwndWorkoutList, LB_RESETCONTENT, 0, 0);
    if (currentUser == nullptr) return;

    for (Workout* w : currentUser->getWorkouts()) {
        std::string label = w->name + "   (" + w->date + ")";
        SendMessageA(hwndWorkoutList, LB_ADDSTRING, 0, (LPARAM)label.c_str());
    }
}

void RefreshExerciseList() {
    SendMessage(hwndExerciseList, LB_RESETCONTENT, 0, 0);
    if (activeWorkout == nullptr) return;

    for (const auto& exPair : activeWorkout->excercises) {
        // Prepend a text icon based on type for visual clarity
        std::string typeTag;
        if (exPair.second->getType() == "Strength") {
            typeTag = "[STR]  ";
        } else {
            typeTag = "[CAR]  ";
        }
        std::string label = typeTag + exPair.second->name;
        SendMessageA(hwndExerciseList, LB_ADDSTRING, 0, (LPARAM)label.c_str());
    }
}

void RefreshSetList() {
    SendMessage(hwndSetList, LB_RESETCONTENT, 0, 0);

    if (activeExercise == nullptr) {
        SetWindowTextA(hwndSetHeaderLabel, "Performance Details (No Exercise Selected)");
        EnableWindow(hwndAddSetBtn, FALSE);
        EnableWindow(hwndEditSetBtn, FALSE);
        EnableWindow(hwndRemoveSetBtn, FALSE);
        return;
    }

    std::string header = "Performance Details - " + activeExercise->name + " (" + activeExercise->getType() + ")";
    SetWindowTextA(hwndSetHeaderLabel, header.c_str());
    EnableWindow(hwndAddSetBtn, TRUE);

    if (activeExercise->getType() == "Strength") {
        EnableWindow(hwndEditSetBtn, TRUE);
        EnableWindow(hwndRemoveSetBtn, TRUE);
        SetWindowTextA(hwndAddSetBtn, "Add Set");

        StrengthExcercise* se = dynamic_cast<StrengthExcercise*>(activeExercise);
        if (se != nullptr) {
            for (const auto& sPair : se->sets) {
                std::stringstream ss;
                ss << "Set " << sPair.first << ":   " << sPair.second->reps << " reps   x   "
                   << sPair.second->weight << " kg   (RPE: " << sPair.second->RPE << ")";
                if (!sPair.second->comment.empty()) {
                    ss << "  -  \"" << sPair.second->comment << "\"";
                }
                SendMessageA(hwndSetList, LB_ADDSTRING, 0, (LPARAM)ss.str().c_str());
            }
        }
    } else {
        // Cardio Exercise doesn't use multiple sets list. Shows distance/duration.
        EnableWindow(hwndEditSetBtn, FALSE);
        EnableWindow(hwndRemoveSetBtn, TRUE); // Will act as "Reset"
        SetWindowTextA(hwndAddSetBtn, "Edit Stats");
        SetWindowTextA(hwndRemoveSetBtn, "Reset Stats");

        Cardio* ce = dynamic_cast<Cardio*>(activeExercise);
        if (ce != nullptr) {
            std::stringstream ss1, ss2, ss3;
            ss1 << "Cardio Performance Info:";
            ss2 << " - Distance:  " << ce->distance << " km";
            ss3 << " - Duration:  " << ce->duration << " minutes";
            SendMessageA(hwndSetList, LB_ADDSTRING, 0, (LPARAM)ss1.str().c_str());
            SendMessageA(hwndSetList, LB_ADDSTRING, 0, (LPARAM)ss2.str().c_str());
            SendMessageA(hwndSetList, LB_ADDSTRING, 0, (LPARAM)ss3.str().c_str());
        }
    }
}
