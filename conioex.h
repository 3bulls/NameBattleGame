/**
 * @file    conioex.h (macOS Port)
 * @brief   macOS/Linux Terminal I/O 拡張ヘッダ (Compatible with HAL Win32 version)
 *
 * @author  Ported for macOS/Linux
 * @date    2025/03/13
 * @version 1.03 (Mac Port)
 * * 制限事項:
 * 1. ジョイスティック入力はサポートしていません (常に0を返します)。
 * 2. ターミナル仕様のため、キーの「離した瞬間」の検出は不正確な場合があります。
 * 3. MIDI再生はサポートしていません (WAV/MP3のみ afplay で再生)。
 */
#ifndef __CONIOEX_H
#define __CONIOEX_H

#ifdef _WIN32

/**
 * @file    conioex.h
 * @brief   Win32 コンソール I/O 拡張ヘッダ for Visual C++/Borland C++
 *
 * @author  HAL名古屋 教務部　浜谷浩彦
 * @date    2009/10/08 (木)
 * @date    2009/10/15 (木) msleep 関数を追加。
 * @date    2010/01/13 (水) playsound 関数で、常に先頭へシーク。
 * @version 1.02
 *
 * @author  HAL東京 講師　荻野博文
 * @date    2025/03/13 (木) inport 関数で、ゲームパッドを3軸から6軸へ変更。視点コントロールを追加。
 * @date    2025/03/13 (木) opensound 関数にUNICODE対応を追加。
 * @date    2025/03/13 (木) 音関係の関数を32bitから64bit版での処理に変更。(opensound、closesound、playsound、stopsound、checksound、updatesound、setvolume)
 * @version 1.03
 */

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#include <process.h>
#include <windows.h>
#include <tchar.h>
#include <mmsystem.h>
#include <digitalv.h>
#pragma comment(lib, "winmm.lib")

#ifdef CONIOEX
#define CONIOEX_INST extern 
#else /* !CONIOEX */
#define CONIOEX_INST 
#endif /* CONIOEX */

#ifdef __BORLANDC__
#define NOCURSOR        _NOCURSOR
#define SOLIDCURSOR     _SOLIDCURSOR
#define NORMALCURSOR    _NORMALCURSOR
__inline void setcursortype(int cur_t) {_setcursortype(cur_t);}
__inline int _kbhit(void) {return kbhit();}
#else /* !__BORLANDC__ */

#pragma warning(disable:4996)

#define NOCURSOR        0
#define SOLIDCURSOR     1
#define NORMALCURSOR    2

#define _NOCURSOR       NOCURSOR
#define _SOLIDCURSOR    SOLIDCURSOR
#define _NORMALCURSOR   NORMALCURSOR

#ifndef __COLORS
#define __COLORS
enum COLORS {
    BLACK,          /* 黒       */
    BLUE,           /* 濃い青   */
    GREEN,          /* 緑       */
    CYAN,           /* 青緑     */
    RED,            /* 濃い赤   */
    MAGENTA,        /* 紫       */
    BROWN,          /* 濃い黄   */
    LIGHTGRAY,      /* 25%灰色  */
    DARKGRAY,       /* 50%灰色  */
    LIGHTBLUE,      /* 青       */
    LIGHTGREEN,     /* 明るい緑 */
    LIGHTCYAN,      /* 水色     */
    LIGHTRED,       /* 赤       */
    LIGHTMAGENTA,   /* ピンク   */
    YELLOW,         /* 黄       */
    WHITE           /* 白       */
};
#endif /* _COLORS */

CONIOEX_INST WORD   __conioex_h_wAttribute      /* 境界色 */
#ifndef CONIOEX
= LIGHTGRAY
#endif /* CONIOEX */
;



/**
 * @brief   水平方向のカーソル位置を取得
 *
 * @return  現在のカーソル位置のX座標(1～)
 */
int wherex(void)
#ifdef CONIOEX
;
#else /* !CONIOEX */
{
    CONSOLE_SCREEN_BUFFER_INFO  csbi;

    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    return csbi.dwCursorPosition.X - csbi.srWindow.Left + 1;
}
#endif /* CONIOEX */

/**
 * @brief   垂直方向のカーソル位置を取得
 *
 * @return  現在のカーソル位置のY座標(1～)
 */
int wherey(void)
#ifdef CONIOEX
;
#else /* !CONIOEX */
{
    CONSOLE_SCREEN_BUFFER_INFO  csbi;

    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    return csbi.dwCursorPosition.Y - csbi.srWindow.Top + 1;
}
#endif /* CONIOEX */

/**
 * @brief   カーソル位置の移動
 *
 * @param   x [入力] X座標(1～)
 * @param   y [入力] Y座標(1～)
 */
void gotoxy(int x, int y)
#ifdef CONIOEX
;
#else /* !CONIOEX */
{
    COORD c;
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);

    c.X = x - 1;
    c.Y = y - 1;
    if (GetConsoleScreenBufferInfo(h, &csbi))
        c.Y += csbi.srWindow.Top;
    SetConsoleCursorPosition(h, c);
}
#endif /* CONIOEX */

/**
 * @brief   行末まで消去
 */
void clreol(void)
#ifdef CONIOEX
;
#else /* !CONIOEX */
{
    CONSOLE_SCREEN_BUFFER_INFO  csbi;
    COORD   c;
    HANDLE  h = GetStdHandle(STD_OUTPUT_HANDLE);

    if (GetConsoleScreenBufferInfo(h, &csbi)) {
        DWORD num;

        c = csbi.dwCursorPosition;
        FillConsoleOutputAttribute(h, csbi.wAttributes,
            csbi.srWindow.Right - csbi.dwCursorPosition.X + 1, c, &num);
        FillConsoleOutputCharacter(h, ' ',
            csbi.srWindow.Right - csbi.dwCursorPosition.X + 1, c, &num);
    }
}
#endif /* CONIOEX */

/**
 * @brief   画面消去
 */
void clrscr(void)
#ifdef CONIOEX
;
#else /* !CONIOEX */
{
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    DWORD num;
    COORD c = {0, 0};
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);

    if (GetConsoleScreenBufferInfo(h, &csbi)) {
        __conioex_h_wAttribute = csbi.wAttributes;
        FillConsoleOutputAttribute(h, csbi.wAttributes, csbi.dwSize.X * csbi.dwSize.Y, c, &num);
        FillConsoleOutputCharacter(h, TEXT(' '), csbi.dwSize.X * csbi.dwSize.Y, c, &num);
        gotoxy(csbi.srWindow.Left + 1, csbi.srWindow.Top + 1);
    }
}
#endif /* CONIOEX */

/**
 * @brief   文字色高輝度化
 */
void highvideo(void)
#ifdef CONIOEX
;
#else /* !CONIOEX */
{
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);

    if (GetConsoleScreenBufferInfo(h, &csbi))
        SetConsoleTextAttribute(h, csbi.wAttributes | FOREGROUND_INTENSITY);
}
#endif /* CONIOEX */

/**
 * @brief   文字色低輝度化
 */
void lowvideo(void)
#ifdef CONIOEX
;
#else /* !CONIOEX */
{
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);

    if (GetConsoleScreenBufferInfo(h, &csbi))
        SetConsoleTextAttribute(h, csbi.wAttributes & ~FOREGROUND_INTENSITY);
}
#endif /* CONIOEX */

/**
 * @brief   既定文字色設定
 */
void normvideo(void)
#ifdef CONIOEX
;
#else /* !CONIOEX */
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), LIGHTGRAY);
}
#endif /* CONIOEX */

void textbackground(int newcolor)
#ifdef CONIOEX
;
#else /* !CONIOEX */
{
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    if (GetConsoleScreenBufferInfo(h, &csbi))
        SetConsoleTextAttribute(h, (csbi.wAttributes & ~0x00f0) | ((newcolor & 0x07) << 4));
}
#endif /* CONIOEX */

void textcolor(int newcolor)
#ifdef CONIOEX
;
#else /* !CONIOEX */
{
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    if (GetConsoleScreenBufferInfo(h, &csbi))
        SetConsoleTextAttribute(h, (csbi.wAttributes & ~0x0f) | (newcolor & 0x0f));
}
#endif /* CONIOEX */

void textattr(int newattr)
#ifdef CONIOEX
;
#else /* !CONIOEX */
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), newattr);
}
#endif /* CONIOEX */

void setcursortype(int cur_t)
#ifdef CONIOEX
;
#else /* !CONIOEX */
{
    CONSOLE_CURSOR_INFO cci;
    static  int nSize = -1;

    if (nSize < 0) {
        if (GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cci))
            nSize = (int)cci.dwSize;
        else
            nSize = 25;
    }
    cci.dwSize      = (cur_t < NORMALCURSOR) ? 100 : nSize;
    cci.bVisible    = (cur_t != NOCURSOR);
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cci);
}
#endif /* CONIOEX */
__inline void _setcursortype(int cur_t) {setcursortype(cur_t);}

/**
 * @brief   現在行に挿入
 */
void insline(void)
#ifdef CONIOEX
;
#else /* !CONIOEX */
{
    CONSOLE_SCREEN_BUFFER_INFO  csbi;
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);

    if (GetConsoleScreenBufferInfo(h, &csbi)) {
        COORD   c;
        DWORD   len;
        DWORD   num;
        LPTSTR  psz;
        LPWORD  pw;

        c.X = csbi.srWindow.Left;
        len = csbi.srWindow.Right - csbi.srWindow.Left + 1;
        psz = (LPTSTR)_alloca(len * sizeof(TCHAR));
        pw = (LPWORD)_alloca(len * sizeof(WORD));
        for (c.Y = csbi.srWindow.Bottom; c.Y > csbi.dwCursorPosition.Y; c.Y--) {
            c.Y--;
            ReadConsoleOutputAttribute(h, pw, len, c, &num);
            ReadConsoleOutputCharacter(h, psz, len, c, &num);
            c.Y++;
            WriteConsoleOutputAttribute(h, pw, len, c, &num);
            WriteConsoleOutputCharacter(h, psz, len, c, &num);
        }
        FillConsoleOutputAttribute(h, csbi.wAttributes, len, c, &num);
        FillConsoleOutputCharacter(h, TEXT(' '), len, c, &num);
    }
}
#endif /* CONIOEX */

/**
 * @brief   現在行の削除
 */
void delline(void)
#ifdef CONIOEX
;
#else /* !CONIOEX */
{
    CONSOLE_SCREEN_BUFFER_INFO  csbi;
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);

    if (GetConsoleScreenBufferInfo(h, &csbi)) {
        DWORD   num;
        COORD   c;
        DWORD   len;
        LPTSTR  psz;
        LPWORD  pw;

        c.X = csbi.srWindow.Left;
        len = csbi.srWindow.Right - csbi.srWindow.Left + 1;
        psz = (LPTSTR)_alloca(len * sizeof(TCHAR));
        pw = (LPWORD)_alloca(len * sizeof(WORD));
        for (c.Y = csbi.dwCursorPosition.Y; c.Y < csbi.srWindow.Bottom; c.Y++) {
            c.Y++;
            ReadConsoleOutputAttribute(h, pw, len, c, &num);
            ReadConsoleOutputCharacter(h, psz, len, c, &num);
            c.Y--;
            WriteConsoleOutputAttribute(h, pw, len, c, &num);
            WriteConsoleOutputCharacter(h, psz, len, c, &num);
        }
        FillConsoleOutputAttribute(h, __conioex_h_wAttribute, len, c, &num);
        FillConsoleOutputCharacter(h, TEXT(' '), len, c, &num);
    }
}
#endif /* CONIOEX */

#endif /* __BORLANDC__ */

__inline void setcaption(char *title) {SetConsoleTitleA(title);}
__inline int getcaption(char *title, int len) {return GetConsoleTitleA(title, len);}
__inline void msleep(unsigned long msecs) {Sleep(msecs);}

#define PK_ENTER                VK_RETURN
#define PK_ESC                  VK_ESCAPE
#define PK_F1                   VK_F1
#define PK_F2                   VK_F2
#define PK_F3                   VK_F3
#define PK_F4                   VK_F4
#define PK_F5                   VK_F5
#define PK_F6                   VK_F6
#define PK_F7                   VK_F7
#define PK_F8                   VK_F8
#define PK_F9                   VK_F9
#define PK_F10                  VK_F10
#define PK_F11                  VK_F11
#define PK_F12                  VK_F12
#define PK_SP                   VK_SPACE
#define PK_UP                   VK_UP
#define PK_DOWN                 VK_DOWN
#define PK_RIGHT                VK_RIGHT
#define PK_LEFT                 VK_LEFT
#define PK_SHIFT                VK_SHIFT
#define PK_CTRL                 VK_CONTROL
#define PK_ALT                  VK_MENU
#define PK_BS                   VK_BACK
#define PK_PAUSE                VK_PAUSE
#define PK_INS                  VK_INSERT
#define PK_DEL                  VK_DELETE
#define PK_TAB                  VK_TAB
#define PK_NFER                 VK_KANA     /* [無変換] */
#define PK_XFER                 VK_CONVERT  /* [変換]   */
#define PK_0                    0x30
#define PK_1                    0x31
#define PK_2                    0x32
#define PK_3                    0x33
#define PK_4                    0x34
#define PK_5                    0x35
#define PK_6                    0x36
#define PK_7                    0x37
#define PK_8                    0x38
#define PK_9                    0x39
#define PK_NUMPAD0              VK_NUMPAD0  /* テンキーの[0]    */
#define PK_NUMPAD1              VK_NUMPAD1  /* テンキーの[1]    */
#define PK_NUMPAD2              VK_NUMPAD2  /* テンキーの[2]    */
#define PK_NUMPAD3              VK_NUMPAD3  /* テンキーの[3]    */
#define PK_NUMPAD4              VK_NUMPAD4  /* テンキーの[4]    */
#define PK_NUMPAD5              VK_NUMPAD5  /* テンキーの[5]    */
#define PK_NUMPAD6              VK_NUMPAD6  /* テンキーの[6]    */
#define PK_NUMPAD7              VK_NUMPAD7  /* テンキーの[7]    */
#define PK_NUMPAD8              VK_NUMPAD8  /* テンキーの[8]    */
#define PK_NUMPAD9              VK_NUMPAD9  /* テンキーの[9]    */
#define PK_A                    0x41
#define PK_B                    0x42
#define PK_C                    0x43
#define PK_D                    0x44
#define PK_E                    0x45
#define PK_F                    0x46
#define PK_G                    0x47
#define PK_H                    0x48
#define PK_I                    0x49
#define PK_J                    0x4A
#define PK_K                    0x4B
#define PK_L                    0x4C
#define PK_M                    0x4D
#define PK_N                    0x4E
#define PK_O                    0x4F
#define PK_P                    0x50
#define PK_Q                    0x51
#define PK_R                    0x52
#define PK_S                    0x53
#define PK_T                    0x54
#define PK_U                    0x55
#define PK_V                    0x56
#define PK_W                    0x57
#define PK_X                    0x58
#define PK_Y                    0x59
#define PK_Z                    0x5A

#define PK_LT                   0x0BC       /* [,]  */
#define PK_GT                   0x0BE       /* [.]  */
#define PK_SLUSH                0x0BF       /* [?]  */
#define PK_DOT                  VK_DECIMAL  /* テンキーの[.]    */
#define PK_DIV                  VK_DIVIDE   /* テンキーの[/]    */
#define PK_BSLUSH               0x0E2       /* [_]  */

#define PK_SEMICOLON            0x0BB       /* [;]  */
#define PK_ADD                  VK_ADD      /* テンキーの[+]    */
#define PK_COLON                0x0BA       /* [:]  */
#define PK_MUL                  VK_MULTIPLY /* テンキーの[*]    */
#define PK_RBRACE               0x0DD       /* []]  */

#define PK_ATMARK               0x0C0       /* [@]  */
#define PK_LBRACE               0x0DB       /* [[]  */

#define PK_MINUS                0x0BD       /* [-]  */
#define PK_SUB                  VK_SUBTRACT /* テンキーの[-]    */
#define PK_XOR                  0x0DE       /* [^]  */
#define PK_YEN                  0x0DC       /* [\]  */

#define PK_KANJI                0x0F3       /* [半角/全角]  */
#define PK_CAPS                 0x0F0       /* [英数][ひらがな] */

#define PM_LEFT                 VK_LBUTTON
#define PM_MID                  VK_MBUTTON
#define PM_RIGHT                VK_RBUTTON
#define PM_CURX                 0x0101
#define PM_CURY                 0x0102

#define PJ1_XPOS                0x0200
#define PJ1_YPOS                0x0201
#define PJ1_ZPOS                0x0202
#define PJ1_BTNS                0x0203
#define PJ1_RPOS                0x0204
#define PJ1_UPOS                0x0205
#define PJ1_VPOS                0x0206
#define PJ1_POV                 0x0207
#define PJ2_XPOS                0x0210
#define PJ2_YPOS                0x0211
#define PJ2_ZPOS                0x0212
#define PJ2_BTNS                0x0213
#define PJ2_RPOS                0x0214
#define PJ2_UPOS                0x0215
#define PJ2_VPOS                0x0216
#define PJ2_POV                 0x0217
#define PJ3_XPOS                0x0220
#define PJ3_YPOS                0x0221
#define PJ3_ZPOS                0x0222
#define PJ3_BTNS                0x0223
#define PJ3_RPOS                0x0224
#define PJ3_UPOS                0x0225
#define PJ3_VPOS                0x0226
#define PJ3_POV                 0x0227
#define PJ4_XPOS                0x0230
#define PJ4_YPOS                0x0231
#define PJ4_ZPOS                0x0232
#define PJ4_BTNS                0x0233
#define PJ4_RPOS                0x0234
#define PJ4_UPOS                0x0235
#define PJ4_VPOS                0x0236
#define PJ4_POV                 0x0237
#define PJ_XPOS                 PJ1_XPOS
#define PJ_YPOS                 PJ1_YPOS
#define PJ_ZPOS                 PJ1_ZPOS
#define PJ_BTNS                 PJ1_BTNS
#define PJ_RPOS                 PJ1_RPOS
#define PJ_UPOS                 PJ1_UPOS
#define PJ_VPOS                 PJ1_VPOS
#define PJ_POV                  PJ1_POV

CONIOEX_INST DWORD  __conioex_h_dwKeyMap[8]
#ifndef CONIOEX
= {0, 0, 0, 0, 0, 0, 0, 0}
#endif /* CONIOEX */
;
CONIOEX_INST COORD  __conioex_h_crdMouse
#ifndef CONIOEX
= {0, 0}
#endif /* CONIOEX */
;

/**
 * @brief   キー情報リセット
 *
 * @return  なし
 */
void reinport(void)
#ifdef CONIOEX
;
#else /* !CONIOEX */
{
    int i;
    for(i = 0; i < 8; i++)
    {
        __conioex_h_dwKeyMap[i] = 0;
    }
}
#endif /* CONIOEX */


/**
 * @brief   各種リアルタイム入力
 *
 * @param   port [入力] ポート番号(P*_*)
 * @return  入力値
 */
int inport(int port)
#ifdef CONIOEX
;
#else /* !CONIOEX */
{
    DWORD   dwEvent;
    HANDLE  h;

    /* ゲームパッド入力 */
    if ((port & 0xfe00) == 0x0200) {
        int     id = (port & 0x01f0) >> 4;
        int     func = port & 0x0f;
        JOYINFOEX   ji;
        ji.dwSize = sizeof(JOYINFOEX);
        ji.dwFlags = JOY_RETURNALL;

        switch (func) {
        case 0: case 1: case 2: case 3: case 4: case 5: case 6: case 7:
            if (joyGetPosEx(id, &ji) != JOYERR_NOERROR)
                return -1;
            switch (func) {
            case 0:
                return ji.dwXpos;
            case 1:
                return ji.dwYpos;
            case 2:
                return ji.dwZpos;
            case 3:
                return ji.dwButtons;
            case 4:
                return ji.dwRpos;
            case 5:
                return ji.dwUpos;
            case 6:
                return ji.dwVpos;
            case 7:
                return ji.dwPOV;
            }
            break;
        default:
            break;
        }
        return 0;
    }
    // キー/マウス イベント チェック
    dwEvent = 0;
    h = GetStdHandle(STD_INPUT_HANDLE);
    if (GetNumberOfConsoleInputEvents(h, &dwEvent) && dwEvent) {
        DWORD           dwRead = 0;
        PINPUT_RECORD   pInpRec = (PINPUT_RECORD)_alloca(dwEvent * sizeof(INPUT_RECORD));

        if (ReadConsoleInput(h, pInpRec, dwEvent, &dwRead) && dwRead) {
            PINPUT_RECORD   pir = pInpRec;
            DWORD           dw;
            for (dw = 0; dw < dwRead; dw++, pir++) {
                switch (pir->EventType) {
                case KEY_EVENT: {
                    KEY_EVENT_RECORD*   pker = &pir->Event.KeyEvent;
                    if (pker->wVirtualKeyCode > 0x0FF)
                        break;
                    if (pker->bKeyDown)
                        __conioex_h_dwKeyMap[pker->wVirtualKeyCode >> 5] |= (0x01 << (pker->wVirtualKeyCode & 31));
                    else
                        __conioex_h_dwKeyMap[pker->wVirtualKeyCode >> 5] &= ~(0x01 << (pker->wVirtualKeyCode & 31));
                    if (pker->dwControlKeyState & (LEFT_ALT_PRESSED | RIGHT_ALT_PRESSED))
                        __conioex_h_dwKeyMap[VK_MENU >> 5] |= (0x01 << (VK_MENU & 31));
                    else
                        __conioex_h_dwKeyMap[VK_MENU >> 5] &= ~(0x01 << (VK_MENU & 31));
                    if (pker->dwControlKeyState & (LEFT_CTRL_PRESSED | RIGHT_CTRL_PRESSED))
                        __conioex_h_dwKeyMap[VK_CONTROL >> 5] |= (0x01 << (VK_CONTROL & 31));
                    else
                        __conioex_h_dwKeyMap[VK_CONTROL >> 5] &= ~(0x01 << (VK_CONTROL & 31));
                    if (pker->dwControlKeyState & SHIFT_PRESSED)
                        __conioex_h_dwKeyMap[VK_SHIFT >> 5] |= (0x01 << (VK_SHIFT & 31));
                    else
                        __conioex_h_dwKeyMap[VK_SHIFT >> 5] &= ~(0x01 << (VK_SHIFT & 31));
                    break;
                }
                case MOUSE_EVENT: {
                    MOUSE_EVENT_RECORD  *pmer = &pir->Event.MouseEvent;
                    __conioex_h_crdMouse = pmer->dwMousePosition;
                    if (pmer->dwButtonState & FROM_LEFT_1ST_BUTTON_PRESSED)
                        __conioex_h_dwKeyMap[VK_LBUTTON >> 5] |= (0x01 << (VK_LBUTTON & 31));
                    else
                        __conioex_h_dwKeyMap[VK_LBUTTON >> 5] &= ~(0x01 << (VK_LBUTTON & 31));
                    if (pmer->dwButtonState & FROM_LEFT_2ND_BUTTON_PRESSED)
                        __conioex_h_dwKeyMap[VK_MBUTTON >> 5] |= (0x01 << (VK_MBUTTON & 31));
                    else
                        __conioex_h_dwKeyMap[VK_MBUTTON >> 5] &= ~(0x01 << (VK_MBUTTON & 31));
                    if (pmer->dwButtonState & RIGHTMOST_BUTTON_PRESSED)
                        __conioex_h_dwKeyMap[VK_RBUTTON >> 5] |= (0x01 << (VK_RBUTTON & 31));
                    else
                        __conioex_h_dwKeyMap[VK_RBUTTON >> 5] &= ~(0x01 << (VK_RBUTTON & 31));
                    break;
                }
                default:
                    break;
                }
            }
        }
    }

    // マウス座標を返す
    switch (port) {
    case PM_CURX:
        return __conioex_h_crdMouse.X + 1;
    case PM_CURY:
        return __conioex_h_crdMouse.Y + 1;
    default:
        break;
    }
    // キー状態を返す
    return (__conioex_h_dwKeyMap[(port & 0x0FF) >> 5] & (0x01 << (port & 31))) != 0;
}
#endif /* CONIOEX */

typedef struct {
    int             nDevType;
    MCIDEVICEID     wDeviceID;
#ifdef UNICODE
    wchar_t         szPath[MAX_PATH];
#else // !UNICODE
    char            szPath[MAX_PATH];
#endif // !UNICODE
    int             nRepeat;
} __conioex_h_SoundInfo;

/**
 * @brief   サウンド ファイルを開く
 *
 * @param   path [入力] ファイル名
 * @retval  非0 サウンド ハンドル
 * @retval  0   エラー
 */
unsigned long long opensound(char *path)
#ifdef CONIOEX
;
#else /* !CONIOEX */
{
#ifdef UNICODE
    const wchar_t   szMidiExt[] = L".mid|.midi|.rmi";
    const wchar_t   szWaveExt[] = L".wav|.wave";
    const wchar_t   szMP3Ext[] = L".mp3";
    wchar_t     szExt[_MAX_EXT];
    wchar_t     wPath[MAX_PATH];
    union {
        MCI_WAVE_OPEN_PARMSW    wop;
        MCI_OPEN_PARMSW         op;
    } mw;
    DWORD       dwCmd;
    __conioex_h_SoundInfo   *psi;

    psi = (__conioex_h_SoundInfo *)malloc(sizeof(__conioex_h_SoundInfo));
    if (psi == NULL)
        return 0;
    ZeroMemory(psi, sizeof(*psi));
    ZeroMemory(&mw, sizeof(mw));
    ZeroMemory(&wPath, sizeof(wPath));
    MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, path, -1, wPath, MAX_PATH);
    _wsplitpath(wPath, NULL, NULL, NULL, szExt);
    wcslwr(szExt);
    dwCmd = MCI_OPEN_TYPE | MCI_OPEN_ELEMENT;
    if (wcsstr(szMidiExt, szExt)) {
        psi->nDevType = MCI_DEVTYPE_SEQUENCER;
        lstrcpynW(psi->szPath, wPath, MAX_PATH);
        mw.op.lpstrDeviceType = (LPCWSTR)MCI_DEVTYPE_SEQUENCER;
        mw.op.lpstrElementName = psi->szPath;
        dwCmd |= MCI_OPEN_TYPE_ID;
    } else if (wcsstr(szWaveExt, szExt)) {
        psi->nDevType = MCI_DEVTYPE_WAVEFORM_AUDIO;
        lstrcpynW(psi->szPath, wPath, MAX_PATH);
        mw.wop.lpstrDeviceType = (LPCWSTR)MCI_DEVTYPE_WAVEFORM_AUDIO;
        mw.wop.lpstrElementName = psi->szPath;
        dwCmd |= MCI_OPEN_TYPE_ID;
    } else if (wcsstr(szMP3Ext, szExt)) {
        psi->nDevType = MCI_DEVTYPE_DIGITAL_VIDEO;
        lstrcpynW(psi->szPath, wPath, MAX_PATH);
        mw.op.lpstrDeviceType = L"MPEGVideo";
        mw.op.lpstrElementName = psi->szPath;
    } else {
        free(psi);
        return 0;
    }
    if (mciSendCommandW(0, MCI_OPEN, dwCmd, (DWORD_PTR)&mw)) {
        free(psi);
        return 0;
    }
    psi->wDeviceID = mw.op.wDeviceID;
    return (unsigned long long)psi;
#else // !UNICODE
    const char  szMidiExt[] = ".mid|.midi|.rmi";
    const char  szWaveExt[] = ".wav|.wave";
    const char  szMP3Ext[] = ".mp3";
    char        szExt[_MAX_EXT];
    union {
        MCI_WAVE_OPEN_PARMSA    wop;
        MCI_OPEN_PARMSA         op;
    } m;
    DWORD       dwCmd;
    __conioex_h_SoundInfo* psi;

    psi = (__conioex_h_SoundInfo*)malloc(sizeof(__conioex_h_SoundInfo));
    if (psi == NULL)
        return 0;
    ZeroMemory(psi, sizeof(*psi));
    ZeroMemory(&m, sizeof(m));
    _splitpath(path, NULL, NULL, NULL, szExt);
    strlwr(szExt);
    dwCmd = MCI_OPEN_TYPE | MCI_OPEN_ELEMENT;
    if (strstr(szMidiExt, szExt)) {
        psi->nDevType = MCI_DEVTYPE_SEQUENCER;
        lstrcpynA(psi->szPath, path, MAX_PATH);
        m.op.lpstrDeviceType = (LPCSTR)MCI_DEVTYPE_SEQUENCER;
        m.op.lpstrElementName = psi->szPath;
        dwCmd |= MCI_OPEN_TYPE_ID;
    } else if (strstr(szWaveExt, szExt)) {
        psi->nDevType = MCI_DEVTYPE_WAVEFORM_AUDIO;
        lstrcpynA(psi->szPath, path, MAX_PATH);
        m.wop.lpstrDeviceType = (LPCSTR)MCI_DEVTYPE_WAVEFORM_AUDIO;
        m.wop.lpstrElementName = psi->szPath;
        dwCmd |= MCI_OPEN_TYPE_ID;
        //      m.wop.dwBufferSeconds  = 60;
        //      dwCmd |= MCI_WAVE_OPEN_BUFFER;
    }   else if (strstr(szMP3Ext, szExt)) {
        psi->nDevType = MCI_DEVTYPE_DIGITAL_VIDEO;
        lstrcpynA(psi->szPath, path, MAX_PATH);
        m.op.lpstrDeviceType = "MPEGVideo";
        m.op.lpstrElementName = psi->szPath;
    }
    else {
        free(psi);
        return 0;
    }
    if (mciSendCommandA(0, MCI_OPEN, dwCmd, (DWORD_PTR)&m)) {
        free(psi);
        return 0;
    }
    psi->wDeviceID = m.op.wDeviceID;
    return (unsigned long long)psi;
#endif // !UNICODE
}
#endif /* CONIOEX */

/**
 * @brief   サウンド ファイルを閉じる
 *
 * @param   hsound [入力] サウンド ハンドル
 */
void closesound(unsigned long long hsound)
#ifdef CONIOEX
;
#else /* !CONIOEX */
{
    __conioex_h_SoundInfo   *psi;

    if (!hsound)
        return;
    psi = (__conioex_h_SoundInfo *)hsound;
    if (psi->wDeviceID) {
        mciSendCommand(psi->wDeviceID, MCI_CLOSE, 0, 0);
        psi->wDeviceID = 0;
    }
    free(psi);
}
#endif /* CONIOEX */

/**
 * @brief   サウンドを再生する
 *
 * @param   hsound [入力] サウンド ハンドル
 * @param   repeat [入力] ループ有無
 */
void playsound(unsigned long long hsound, int repeat)
#ifdef CONIOEX
;
#else /* !CONIOEX */
{
    __conioex_h_SoundInfo   *psi;
    DWORD                   dwCmd;
    MCI_PLAY_PARMS          mpp;

    if (!hsound)
        return;
    psi = (__conioex_h_SoundInfo *)hsound;
    if (!psi->wDeviceID)
        return;
    psi->nRepeat = repeat;
    ZeroMemory(&mpp, sizeof(mpp));
    dwCmd = 0;
    if (repeat) {
        switch (psi->nDevType) {
        case MCI_DEVTYPE_DIGITAL_VIDEO:
            dwCmd |= (MCI_FROM | MCI_DGV_PLAY_REPEAT);
            mpp.dwFrom = 0;
            break;
        case MCI_DEVTYPE_SEQUENCER:
        case MCI_DEVTYPE_WAVEFORM_AUDIO:
            break;
        default:
            break;
        }
    }
    mciSendCommand(psi->wDeviceID, MCI_SEEK, MCI_SEEK_TO_START, 0);
    mciSendCommand(psi->wDeviceID, MCI_PLAY, dwCmd, (DWORD_PTR)&mpp);
}
#endif /* CONIOEX */

/**
 * @brief   サウンド再生を停止する
 *
 * @param   hsound [入力] サウンド ハンドル
 */
void stopsound(unsigned long long hsound)
#ifdef CONIOEX
;
#else /* !CONIOEX */
{
    __conioex_h_SoundInfo   *psi;

    if (!hsound)
        return;
    psi = (__conioex_h_SoundInfo *)hsound;
    if (!psi->wDeviceID)
        return;
    psi->nRepeat = 0;
    mciSendCommand(psi->wDeviceID, MCI_STOP, MCI_WAIT, 0);
    mciSendCommand(psi->wDeviceID, MCI_SEEK, MCI_SEEK_TO_START, 0);
}
#endif /* CONIOEX */

/**
 * @brief   サウンド再生状態の取得
 *
 * @param   hsound [入力] サウンド ハンドル
 * @return  再生中ならば 0 以外を返す。
 */
int checksound(unsigned long long hsound)
#ifdef CONIOEX
;
#else /* !CONIOEX */
{
    __conioex_h_SoundInfo   *psi;
    MCI_STATUS_PARMS        msp;

    if (!hsound)
        return 0;
    psi = (__conioex_h_SoundInfo *)hsound;
    if (!psi->wDeviceID)
        return 0;
    ZeroMemory(&msp, sizeof(msp));
    msp.dwItem = MCI_STATUS_MODE;
    if (mciSendCommand(psi->wDeviceID, MCI_STATUS, MCI_STATUS_ITEM, (DWORD_PTR)&msp))
        return 0;
    return msp.dwReturn == MCI_MODE_PLAY;
}
#endif /* CONIOEX */

/**
 * @brief   ループ再生の強制更新
 *
 * @param   hsound [入力] サウンド ハンドル
 */
void updatesound(unsigned long long hsound)
#ifdef CONIOEX
;
#else /* !CONIOEX */
{
    __conioex_h_SoundInfo   *psi;
    MCI_STATUS_PARMS        msp;

    if (!hsound)
        return;
    psi = (__conioex_h_SoundInfo *)hsound;
    if (!psi->wDeviceID || !psi->nRepeat)
        return;
    switch (psi->nDevType) {
    case MCI_DEVTYPE_DIGITAL_VIDEO:
        break;
    case MCI_DEVTYPE_SEQUENCER:
    case MCI_DEVTYPE_WAVEFORM_AUDIO:
        ZeroMemory(&msp, sizeof(msp));
        msp.dwItem = MCI_STATUS_MODE;
        if (!mciSendCommand(psi->wDeviceID, MCI_STATUS, MCI_STATUS_ITEM, (DWORD_PTR)&msp)) {
            if (msp.dwReturn != MCI_MODE_PLAY) {
                if (psi->nRepeat > 0) {
                    psi->nRepeat = -1;
                    mciSendCommand(psi->wDeviceID, MCI_SEEK, MCI_SEEK_TO_START, 0);
                    mciSendCommand(psi->wDeviceID, MCI_PLAY, 0, 0);
                }
            } else
                psi->nRepeat = 1;
        }
        break;
    default:
        break;
    }
}
#endif /* CONIOEX */

/**
 * @brief   再生音量を設定する
 *
 * @param   hsound [入力] サウンド ハンドル
 * @param   percent [入力] 音量 (0 ～ 100)
 */
void setvolume(unsigned long long hsound, int percent)
#ifdef CONIOEX
;
#else /* !CONIOEX */
{
    __conioex_h_SoundInfo   *psi;

    if (!hsound)
        return;
    psi = (__conioex_h_SoundInfo *)hsound;
    if (!psi->wDeviceID)
        return;
    switch (psi->nDevType) {
    case MCI_DEVTYPE_DIGITAL_VIDEO: {
        MCI_DGV_SETAUDIO_PARMS  mdsp;

        ZeroMemory(&mdsp, sizeof(mdsp));
        mdsp.dwItem = MCI_DGV_SETAUDIO_VOLUME;
        mdsp.dwValue = percent * 10;
        mciSendCommand(psi->wDeviceID, MCI_SETAUDIO,
            MCI_DGV_SETAUDIO_ITEM | MCI_DGV_SETAUDIO_VALUE, (DWORD_PTR)&mdsp);
        break;
    }
    case MCI_DEVTYPE_SEQUENCER: {
        DWORD dwVolume;

        dwVolume = 0x0ffff * percent / 100;
        midiOutSetVolume(0, (DWORD)MAKELONG(dwVolume, dwVolume));
        break;
    }
    case MCI_DEVTYPE_WAVEFORM_AUDIO: {
        DWORD dwVolume;

        dwVolume = 0x0ffff * percent / 100;
        waveOutSetVolume(0, (DWORD)MAKELONG(dwVolume, dwVolume));
        break;
    }
    default:
        break;
    }
}
#endif /* CONIOEX */

#ifdef __cplusplus
}

#endif  /* Windows */

#elif defined(__APPLE__) || defined(__linux__)

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/time.h>
#include <signal.h>
#include <ctype.h>
#include <wchar.h>

/* Windows Types Compatibility */
typedef unsigned short WORD;
typedef unsigned int   DWORD;
typedef int            BOOL;
typedef struct {
    short X;
    short Y;
} COORD;

#define TRUE  1
#define FALSE 0
#define MAX_PATH 260

/* Cursor Constants */
#define NOCURSOR        0
#define SOLIDCURSOR     1 /* Not fully supported in ANSI, treats as Normal */
#define NORMALCURSOR    2

#define _NOCURSOR       NOCURSOR
#define _SOLIDCURSOR    SOLIDCURSOR
#define _NORMALCURSOR   NORMALCURSOR

/* Colors (Approximation using ANSI colors) */
#ifndef __COLORS
#define __COLORS
enum COLORS {
    BLACK,          /* 30 */
    BLUE,           /* 34 */
    GREEN,          /* 32 */
    CYAN,           /* 36 */
    RED,            /* 31 */
    MAGENTA,        /* 35 */
    BROWN,          /* 33 (Yellow dim) */
    LIGHTGRAY,      /* 37 */
    DARKGRAY,       /* 90 */
    LIGHTBLUE,      /* 94 */
    LIGHTGREEN,     /* 92 */
    LIGHTCYAN,      /* 96 */
    LIGHTRED,       /* 91 */
    LIGHTMAGENTA,   /* 95 */
    YELLOW,         /* 93 */
    WHITE           /* 97 */
};
#endif /* _COLORS */

/* Global State */
static struct termios __conioex_orig_termios;
static int __conioex_initialized = 0;
static DWORD __conioex_keymap[8] = {0};
static COORD __conioex_mouse_pos = {0, 0};

/* Internal: Restore terminal settings on exit */
static void __conioex_cleanup(void) {
    if (__conioex_initialized) {
        tcsetattr(STDIN_FILENO, TCSAFLUSH, &__conioex_orig_termios);
        printf("\033[?25h"); // Show cursor
        printf("\033[?1003l"); // Disable mouse tracking
    }
}

/* Internal: Initialize raw mode */
static void __conioex_init(void) {
    if (!__conioex_initialized) {
        struct termios raw;
        tcgetattr(STDIN_FILENO, &__conioex_orig_termios);
        raw = __conioex_orig_termios;
        
        // Disable echo, canonical mode, signals
        raw.c_lflag &= ~(ECHO | ICANON | ISIG | IEXTEN);
        raw.c_iflag &= ~(IXON | ICRNL | BRKINT | INPCK | ISTRIP);
        raw.c_oflag &= ~(OPOST);
        raw.c_cc[VMIN] = 0;
        raw.c_cc[VTIME] = 0;
        
        tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
        atexit(__conioex_cleanup);
        __conioex_initialized = 1;
        
        // Enable mouse tracking (Xterm 1003)
        // printf("\033[?1003h"); 
        fflush(stdout);
    }
}

/* --- Console Functions --- */

int wherex(void) {
    // Parsing ANSI response for cursor position is complex and slow.
    // Simplifying: assumes tracked by app or returns 0 if unknown.
    // For robust games, keep track of x/y manually.
    return 0; // Not easily implemented in raw stream without blocking
}

int wherey(void) {
    return 0; // Not easily implemented
}

void gotoxy(int x, int y) {
    printf("\033[%d;%dH", y, x);
    fflush(stdout);
}

void clreol(void) {
    printf("\033[K");
    fflush(stdout);
}

void clrscr(void) {
    printf("\033[2J\033[1;1H");
    fflush(stdout);
}

void highvideo(void) {
    printf("\033[1m");
    fflush(stdout);
}

void lowvideo(void) {
    printf("\033[2m");
    fflush(stdout);
}

void normvideo(void) {
    printf("\033[0m");
    fflush(stdout);
}

/* ANSI Color Map */
static int __ansi_fg_map[] = {30, 34, 32, 36, 31, 35, 33, 37, 90, 94, 92, 96, 91, 95, 93, 97};
static int __ansi_bg_map[] = {40, 44, 42, 46, 41, 45, 43, 47, 100, 104, 102, 106, 101, 105, 103, 107};

void textcolor(int newcolor) {
    if (newcolor >= 0 && newcolor <= 15) {
        printf("\033[%dm", __ansi_fg_map[newcolor]);
        fflush(stdout);
    }
}

void textbackground(int newcolor) {
    if (newcolor >= 0 && newcolor <= 15) {
        printf("\033[%dm", __ansi_bg_map[newcolor]);
        fflush(stdout);
    }
}

void textattr(int newattr) {
    textcolor(newattr & 0x0F);
    textbackground((newattr >> 4) & 0x0F);
}

void setcursortype(int cur_t) {
    if (cur_t == NOCURSOR)
        printf("\033[?25l");
    else
        printf("\033[?25h");
    fflush(stdout);
}
void _setcursortype(int cur_t) { setcursortype(cur_t); }

void insline(void) {
    printf("\033[L");
    fflush(stdout);
}

void delline(void) {
    printf("\033[M");
    fflush(stdout);
}

void setcaption(char *title) {
    printf("\033]0;%s\007", title);
    fflush(stdout);
}

int getcaption(char *title, int len) {
    return 0; // Not supported in ANSI
}

void msleep(unsigned long msecs) {
    usleep(msecs * 1000);
}

/* --- Input Defines (Virtual Keys Mapping) --- */
/* Mapping standard VK codes to arbitrary constants for logic compatibility */
#define VK_RETURN   0x0D
#define VK_ESCAPE   0x1B
#define VK_SPACE    0x20
#define VK_UP       0x26
#define VK_DOWN     0x28
#define VK_LEFT     0x25
#define VK_RIGHT    0x27
#define VK_F1       0x70
#define VK_F2       0x71
#define VK_F3       0x72
#define VK_F4       0x73
#define VK_F5       0x74
#define VK_F6       0x75
#define VK_F7       0x76
#define VK_F8       0x77
#define VK_F9       0x78
#define VK_F10      0x79
#define VK_F11      0x7A
#define VK_F12      0x7B
#define VK_SHIFT    0x10
#define VK_CONTROL  0x11
#define VK_MENU     0x12 /* Alt */
#define VK_BACK     0x08
#define VK_PAUSE    0x13
#define VK_INSERT   0x2D
#define VK_DELETE   0x2E
#define VK_TAB      0x09
#define VK_KANA     0x15
#define VK_CONVERT  0x1C
#define VK_LBUTTON  0x01
#define VK_RBUTTON  0x02
#define VK_MBUTTON  0x04
#define VK_NUMPAD0  0x60
#define VK_NUMPAD1  0x61
#define VK_NUMPAD2  0x62
#define VK_NUMPAD3  0x63
#define VK_NUMPAD4  0x64
#define VK_NUMPAD5  0x65
#define VK_NUMPAD6  0x66
#define VK_NUMPAD7  0x67
#define VK_NUMPAD8  0x68
#define VK_NUMPAD9  0x69
#define VK_DECIMAL  0x6E
#define VK_DIVIDE   0x6F
#define VK_ADD      0x6B
#define VK_MULTIPLY 0x6A
#define VK_SUBTRACT 0x6D

/* PK Defines match the original header */
#define PK_ENTER    VK_RETURN
#define PK_ESC      VK_ESCAPE
#define PK_SP       VK_SPACE
#define PK_UP       VK_UP
#define PK_DOWN     VK_DOWN
#define PK_RIGHT    VK_RIGHT
#define PK_LEFT     VK_LEFT
#define PK_SHIFT    VK_SHIFT
#define PK_CTRL     VK_CONTROL
#define PK_ALT      VK_MENU
#define PK_F1       VK_F1
#define PK_F2       VK_F2
#define PK_F3       VK_F3
#define PK_F4       VK_F4
#define PK_F5       VK_F5
#define PK_F6       VK_F6
#define PK_F7       VK_F7
#define PK_F8       VK_F8
#define PK_F9       VK_F9
#define PK_F10      VK_F10
#define PK_F11      VK_F11
#define PK_F12      VK_F12

#define PK_0 '0'
#define PK_1 '1'
#define PK_2 '2'
#define PK_3 '3'
#define PK_4 '4'
#define PK_5 '5'
#define PK_6 '6'
#define PK_7 '7'
#define PK_8 '8'
#define PK_9 '9'
#define PK_A 'A'
#define PK_B 'B'
#define PK_C 'C'
#define PK_D 'D'
#define PK_E 'E'
#define PK_F 'F'
#define PK_G 'G'
#define PK_H 'H'
#define PK_I 'I'
#define PK_J 'J'
#define PK_K 'K'
#define PK_L 'L'
#define PK_M 'M'
#define PK_N 'N'
#define PK_O 'O'
#define PK_P 'P'
#define PK_Q 'Q'
#define PK_R 'R'
#define PK_S 'S'
#define PK_T 'T'
#define PK_U 'U'
#define PK_V 'V'
#define PK_W 'W'
#define PK_X 'X'
#define PK_Y 'Y'
#define PK_Z 'Z'

/* Joystick constants (stubbed) */
#define PJ1_XPOS 0x0200
#define PJ_XPOS PJ1_XPOS
/* ... (omitting full list for brevity, handled by default case) */

/* Mouse constants */
#define PM_CURX 0x0101
#define PM_CURY 0x0102
#define PM_LEFT VK_LBUTTON

/* --- Input Implementation --- */

void reinport(void) {
    int i;
    for(i = 0; i < 8; i++) __conioex_keymap[i] = 0;
}

static void __conioex_set_key(int vk, int down) {
    if (down)
        __conioex_keymap[vk >> 5] |= (1 << (vk & 31));
    else
        __conioex_keymap[vk >> 5] &= ~(1 << (vk & 31));
}

int inport(int port) {
    /* Initialize on first call */
    __conioex_init();

    /* Joystick: Not supported in Terminal */
    if ((port & 0xfe00) == 0x0200) {
        return 0; 
    }

    /* Process Input Stream */
    /* Note: In a terminal, we only get key *presses*, not *releases*.
       We clear the map periodically or logic assumes repeat. 
       Here we try to read all available bytes in buffer. */
    
    unsigned char buf[16];
    int nread;
    
    /* Clear previous transient keys (hack for game loop) */
    reinport(); 

    while ((nread = read(STDIN_FILENO, buf, sizeof(buf))) > 0) {
        for (int i = 0; i < nread; i++) {
            unsigned char c = buf[i];
            
            if (c == 27 && i < nread - 1) { // Escape Sequence
                if (buf[i+1] == '[') {
                    i += 2;
                    switch(buf[i]) {
                        case 'A': __conioex_set_key(VK_UP, 1); break;
                        case 'B': __conioex_set_key(VK_DOWN, 1); break;
                        case 'C': __conioex_set_key(VK_RIGHT, 1); break;
                        case 'D': __conioex_set_key(VK_LEFT, 1); break;
                    }
                } else {
                    __conioex_set_key(VK_ESCAPE, 1);
                }
            } else if (c == 10 || c == 13) {
                __conioex_set_key(VK_RETURN, 1);
            } else if (c == 127 || c == 8) {
                __conioex_set_key(VK_BACK, 1);
            } else if (c == 32) {
                __conioex_set_key(VK_SPACE, 1);
            } else {
                int vk = toupper(c);
                __conioex_set_key(vk, 1);
            }
        }
    }

    /* Return Mouse (Stubbed) */
    if (port == PM_CURX) return 0;
    if (port == PM_CURY) return 0;

    /* Return Key State */
    return (__conioex_keymap[(port & 0x0FF) >> 5] & (1 << (port & 31))) != 0;
}

/* --- Sound Implementation (using afplay) --- */

typedef struct {
    pid_t pid;
    char path[MAX_PATH];
    int is_playing;
} __conioex_h_SoundInfo;

unsigned long long opensound(char *path) {
    __conioex_h_SoundInfo *psi = (__conioex_h_SoundInfo *)malloc(sizeof(__conioex_h_SoundInfo));
    if (!psi) return 0;
    strncpy(psi->path, path, MAX_PATH);
    psi->pid = 0;
    psi->is_playing = 0;
    return (unsigned long long)psi;
}

void closesound(unsigned long long hsound) {
    if (!hsound) return;
    __conioex_h_SoundInfo *psi = (__conioex_h_SoundInfo *)hsound;
    if (psi->pid > 0) {
        kill(psi->pid, SIGTERM);
    }
    free(psi);
}

void playsound(unsigned long long hsound, int repeat) {
    if (!hsound) return;
    __conioex_h_SoundInfo *psi = (__conioex_h_SoundInfo *)hsound;
    
    if (psi->pid > 0) {
        kill(psi->pid, SIGTERM);
        psi->pid = 0;
    }

    /* Use fork/exec to run afplay in background */
    pid_t pid = fork();
    if (pid == 0) {
        /* Child process */
        // Suppress stdout/stderr
        int devnull = open("/dev/null", O_WRONLY);
        dup2(devnull, 1);
        dup2(devnull, 2);
        
        execlp("afplay", "afplay", psi->path, NULL);
        exit(0);
    } else if (pid > 0) {
        /* Parent */
        psi->pid = pid;
        psi->is_playing = 1;
    }
    /* Note: afplay does not support seamless looping via command line easily. 
       'repeat' parameter is ignored in this simple implementation. */
}

void stopsound(unsigned long long hsound) {
    if (!hsound) return;
    __conioex_h_SoundInfo *psi = (__conioex_h_SoundInfo *)hsound;
    if (psi->pid > 0) {
        kill(psi->pid, SIGTERM);
        psi->pid = 0;
        psi->is_playing = 0;
    }
}

int checksound(unsigned long long hsound) {
    if (!hsound) return 0;
    __conioex_h_SoundInfo *psi = (__conioex_h_SoundInfo *)hsound;
    if (psi->pid > 0) {
        // Check if process is still running
        if (kill(psi->pid, 0) == 0) return 1;
        psi->pid = 0;
        psi->is_playing = 0;
    }
    return 0;
}

void updatesound(unsigned long long hsound) {
    /* Simple re-trigger if loop was requested - tricky with afplay. 
       For now, no-op. */
}

void setvolume(unsigned long long hsound, int percent) {
    /* afplay --volume X (where X is 0 to 255 or so, but log scale)
       Since we spawn a new process every play, we can't change volume 
       of running sound easily without killing and restarting. 
       Stubbed. */
}

#ifdef __cplusplus
}
#endif

#endif /* macOS/Linux */

#endif /* __CONIOEX_H */
