/* Allow unsafe CRT functions on Visual C++ 2005 and higher. */
#if _MSC_VER >= 1400
#define _CRT_SECURE_NO_DEPRECATE
#endif

#define STRICT

#include <stdlib.h>
#include <time.h>
#include <Windows.h>
#include <MMSystem.h>
#include <ShellAPI.h>

/* GetActiveWindow is made local to current thread in 32-bit Windows. The function of global scope is now GetForegroundWindow. */
#ifdef _WIN32
#define GetActiveWindow GetForegroundWindow
#endif

typedef struct spriteinfo {
    HBITMAP bitmaps[2];
    int x;
    int y;
    int width;
    int height;
} spriteinfo;

typedef struct resourceinfo {
    int resource;
    WORD flags;
    spriteinfo info;
} resourceinfo;

typedef struct windowinfo {
    HWND window;
    RECT rect;
    BYTE padding[66]; /* Unused. */
} windowinfo;

int word_9CF0 = 245; /* Palette search maximum index (unused). */
resourceinfo stru_9EE2[16] = { /* Resource list. */
    {101, 1, {{NULL, NULL}, 0, 0, 0, 0}},
    {102, 1, {{NULL, NULL}, 0, 0, 0, 0}},
    {103, 1, {{NULL, NULL}, 0, 0, 0, 0}},
    {104, 1, {{NULL, NULL}, 0, 0, 0, 0}},
    {105, 1, {{NULL, NULL}, 0, 0, 0, 0}},
    {106, 1, {{NULL, NULL}, 0, 0, 0, 0}},
    {107, 1, {{NULL, NULL}, 0, 0, 0, 0}},
    {108, 1, {{NULL, NULL}, 0, 0, 0, 0}},
    {109, 1, {{NULL, NULL}, 0, 0, 0, 0}},
    {110, 1, {{NULL, NULL}, 0, 0, 0, 0}},
    {111, 1, {{NULL, NULL}, 0, 0, 0, 0}}
};
resourceinfo stru_9FE2[16] = {0}; /* Resource list storing flipped images. */
WORD word_A15A[80] = { /* Normal action table (option "Gravity always on" disabled). */
    11, 11, 7, 7,
    11, 11, 7, 7,
    11, 11, 7, 7,
    11, 11, 7, 7,
    11, 11, 7, 7,
    11, 11, 7, 7,
    11, 11, 7, 7,
    11, 11, 7, 7,
    11, 11, 7, 7,
    11, 11, 7, 7,
    11, 7, 17, 20,
    11, 7, 17, 20,
    11, 7, 17, 20,
    11, 7, 17, 20,
    11, 7, 17, 20,
    11, 7, 17, 20,
    11, 7, 17, 20,
    13, 58, 15, 45,
    35, 53, 43, 47,
    45, 47, 49, 51
};
WORD word_A1FA[80] = { /* Normal action table (option "Gravity always on" enabled). */
    11, 11, 7, 7,
    11, 11, 7, 7,
    11, 11, 7, 7,
    11, 11, 7, 7,
    11, 11, 7, 7,
    11, 11, 7, 7,
    11, 11, 7, 7,
    11, 11, 7, 7,
    11, 11, 7, 7,
    11, 11, 7, 7,
    11, 7, 17, 20,
    11, 7, 17, 20,
    11, 7, 17, 20,
    11, 7, 17, 20,
    11, 7, 17, 20,
    11, 7, 17, 20,
    11, 7, 17, 20,
    13, 58, 15, 65,
    35, 53, 43, 75,
    45, 47, 49, 51
};
WORD word_A29A[8] = { /* Special action table. */
    116, 121, 126, 147,
    128, 135, 142, 147
};
int facing_direction = 1; /* Facing direction. 1 = left, -1 = right */
int facing_direction_sub = 1; /* Facing direction (sub). 1 = left, -1 = right */
WORD word_A2B4[6][8] = { /* Blink animations. */
    {7, 8, 7, 6, 7, 8, 7, 6},
    {32, 33, 32, 31, 32, 33, 32, 31},
    {74, 75, 74, 73, 74, 75, 74, 73},
    {79, 80, 79, 78, 79, 80, 79, 78},
    {82, 83, 82, 81, 82, 83, 82, 81},
    {35, 36, 35, 34, 35, 36, 35, 34}
};
WORD word_A314[2][4] = { /* Hang on window top edge animations. */
    {42, 43, 42, 44},
    {46, 47, 46, 47}
};
WORD word_A324[20] = { /* Collision animation with obsolete height offset. */
    62, 63, 63, 64, 64, 65, 65, 66, 66, 66,
    0, 10, 17, 21, 22, 21, 17, 10, 0, 0
};
WORD animation_snap[29] = { /* Taking picture (snap) animation. */
        6, 6, 6, 6, 37, 37, 37,38, 38, 38, 38, 38, 38, 38, 39, 39, 174, 39, 38, 38, 38, 38,38, 37, 37, 6, 6, 6, 0,
};
WORD animation_burp[8] = { /* Baa animation. */
        71, 72, 71, 72, 71, 72, 3, 0
};
WORD animation_thicc[2] = { /* Sneeze animation. */
        6, 106
};
WORD word_A38C[6] = { /* Amazed animation. */
    50, 51, 50, 51, 3, 0
};
WORD animation_beer[35] = { /* Eat animation. */
        58, 150, 60, 61, 60, 61, 60, 61, 58, 151, 60, 61, 60, 61, 60, 61, 2, 58, 152, 60, 61, 60, 61, 60, 61, 58, 153,
        60, 61, 60, 61, 60, 61, 3, 0
};
WORD animation_falling_van[2] = { /* Burn animation. (Falling jacex van)*/
        145, 0
};
WORD word_A422[13] = { /* Roll over animation (not used). */
        3, 93, 99, 100, 99, 100, 99, 100, 99, 100, 95, 3, 0
};
WORD word_A43C[8] = { /* Get up animation (left). */
        48, 48, 48, 49, 13, 12, 3, 0
};
WORD word_A44C[8] = { /* Get up animation (right). */
        48, 48, 48, 49, 13, 14, 3, 0
};
WORD animation_drift[17] = { /* Van drift animation. */
        134, 135, 136, 137, 138, 139, 140, 141, 142, 143,144, 154,155,156,157,0
};
WORD animation_crash[5] = { /* JX Van crash animation. */
        147, 148, 148, 148, 0
};
WORD word_A49E[89] = { /* get out of JX Van animation. */
        169, 169, 169, 169, 169, 169,
        170, 170, 170, 170, 170, 170, 170, 170, 170, 170,
        171, 171, 171, 171, 171, 171, 171, 171, 171, 171, 171, 171, 171, 171, 171, 171, 171, 171, 171, 171,
        172, 172, 172, 172, 172, 172, 172, 172, 172,
        67,67,67,
        45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45,45,
        79,79,79,
        34, 34, 34, 34, 35, 36, 35, 34, 35, 36, 35, 34, 34, 34,
        10, 10, 9, 9, 3, 0
};
WORD animation_blush[19] = { /* Blush animation. */
        6, 6, 128, 129, 130, 129, 128, 128, 129, 130, 130, 130,130,130, 129, 128, 0
};
WORD word_A524[9] = { /* Jerma walk */
        119, 120, 121, 122, 123, 124, 125, 126, 0
};
WORD word_A536[8] = { /* Spin animation. 0-3: face, 4-7: back */
        3, 9, 10, 11, 2, 14, 13, 12
};
WORD animation_nose_pick[13] = { /* Nose picking animation */
        6, 103, 104, 105, 104, 105, 104, 105, 104, 105, 103, 6, 0
};
WORD word_A798 = 0; /* Has cursor position changed in current timer period? */
int word_A79A = 0; /* Cursor position with respect to screen, X-coordinate */
int word_A79C = 0; /* Cursor position with respect to screen, Y-coordinate */
WORD word_A79E = 0; /* Dragging Screen Mate window? */
WORD word_A7A0 = 0; /* Destroy Screen Mate window by right double-click? */
WORD word_A7A2 = 0; /* Unused. */
RECT stru_A7A4 = {0, 0, 0, 0}; /* Screen Mate window rectangle. */
WORD word_A7AC = 0; /* Not to clear window on WM_PAINT? */
POINT stru_A7B0 = {0, 0}; /* Current cursor position. */
WORD word_A7B4 = 0; /* Not to clear window on WM_PAINT? (sub) */
HBITMAP word_A7B6[2] = {NULL, NULL}; /* Double buffer. */
HBITMAP word_A7BA = NULL; /* Sprite render target. */
HBITMAP word_A7BC = NULL; /* Sprite colour image for current frame. */
HBITMAP word_A7BE = NULL; /* Sprite mask image for current frame. */
int word_A7C0 = 0; /* Sprite X-coordinate on resource image for current frame. */
int word_A7C2 = 0; /* Sprite Y-coordinate on resource image for current frame. */
int word_A7C8 = 0; /* Sprite X-coordinate on resource image for previous frame. */
int word_A7CA = 0; /* Sprite Y-coordinate on resource image for previous frame (unused). */
WORD word_A7D0 = 0; /* Current framebuffer index. */
WORD word_A7D2 = 0; /* 0 to render sprite; 1 to update window. */
WORD word_A7D4 = 0; /* Unused. */
HBITMAP word_A7D8 = NULL; /* Sprite colour image for previous frame. */
int word_A7DA = 0; /* Screen X-coordinate for current frame. */
int word_A7DC = 0; /* Screen Y-coordinate for current frame. */
int word_A7DE = 0; /* Sprite width for current frame. */
int word_A7E0 = 0; /* Sprite height for current frame. */
int word_A7E2 = 0; /* Update area rectangle X-coordinate for current frame. */
int word_A7E4 = 0; /* Update area rectangle Y-coordinate for current frame. */
int word_A7E6 = 0; /* Update area rectangle width for current frame. */
int word_A7E8 = 0; /* Update area rectangle height for current frame. */
int word_A7EA = 0; /* Update area rectangle X-coordinate for previous frame. */
int word_A7EC = 0; /* Update area rectangle Y-coordinate for previous frame. */
int word_A7EE = 0; /* Update area rectangle width for previous frame. */
int word_A7F0 = 0; /* Update area rectangle height for previous frame. */
int word_A7F2 = 0; /* Screen X-coordinate for previous frame. */
int word_A7F4 = 0; /* Screen Y-coordinate for previous frame. */
int word_A7F6 = 0; /* Sprite width for previous frame. */
int word_A7F8 = 0; /* Sprite height for previous frame. */
WORD word_A7FA = 0; /* Current frame rectangle and previous frame rectangle have no intersecion? (unused) */
WORD word_A7FC = 0; /* Is gravity enabled? */
WORD word_A7FE = 0; /* Is collision with visible window enabled? */
int x_curr = 0; /* Current X-coordinate. */
int y_curr = 0; /* Current Y-coordinate. */
int sprite_index = 0; /* Sprite index. */
int y_speed = 0; /* Vertical speed. */
int word_A808 = 0; /* Horizontal speed. */
int word_A80C = 0; /* Y-coordinate memory. */
int x_sub = 0; /* Current X-coordinate (sub). */
int y_sub = 0; /* Current Y-coordinate (sub). */
int sprite_index_sub = 0; /* Sprite index (sub). */
HWND word_A81C = NULL; /* Active window or window to land on. */
RECT stru_A81E = {0L, 0L, 0L, 0L}; /* Rectangle of active window or window to land on. */
int animation_frame = 0; /* Animation frame counter. */
int word_A828 = 0; /* Random duration period counter. */
int word_A82A = 0; /* Random case number for action. */
WORD word_A82C = 0; /* Unused. */
HGLOBAL word_A82E = NULL; /* Global handle for holding WAVE resource in memory. */
int word_A830 = 0; /* Current time hour. */
int word_A832 = 0; /* Remaining times for chime. */
DWORD dword_A834 = 0; /* Tick count. */
int word_A838 = 0; /* Time check period counter. */
int frame_period_counter = 0; /* Frame period counter. */
int word_A83C = 0; /* Target X-coordinate for window edge attachment. */
int word_A83E = 0; /* Target Y-coordinate for window edge attachment. */
WORD word_A840 = 0; /* Bounce when falling? */
int word_A842 = 0; /* Case number for fall action. */
int word_A844 = 0; /* Collision vertical speed (unused). */
int word_A846 = 0; /* Collision spin frame counter (unused). */
WORD word_A848 = 0; /* Collision animation frame index. */
int word_A84A = 0; /* Known instance list update period counter. */
HBITMAP word_A850[2] = {NULL, NULL}; /* Double buffer (sub). */
HBITMAP word_A854 = NULL; /* Sprite render target (sub). */
HBITMAP word_A856 = NULL; /* Sprite colour image for current frame (sub). */
HBITMAP word_A858 = NULL; /* Sprite mask image for current frame (sub). */
HBITMAP word_A85A = NULL; /* Fade out processed colour image (sub). */
HBITMAP word_A85C = NULL; /* Fade out processed mask image (sub). */
int word_A85E = 0; /* Sprite X-coordinate on resource image for current frame (sub). */
int word_A860 = 0; /* Sprite Y-coordinate on resource image for current frame (sub). */
int word_A866 = 0; /* Sprite X-coordinate on resource image for previous frame (sub). */
int word_A868 = 0; /* Sprite Y-coordinate on resource image for previous frame (sub) (unused). */
WORD word_A86E = 0; /* Current framebuffer index (sub). */
WORD word_A870 = 0; /* 0 to render sprite; 1 to update window (sub). */
WORD word_A872 = 0; /* Unused. */
HBITMAP word_A876 = NULL; /* Sprite colour image for previous frame (sub). */
int word_A878 = 0; /* Screen X-coordinate for current frame (sub). */
int word_A87A = 0; /* Screen Y-coordinate for current frame (sub). */
int word_A87C = 0; /* Sprite width for current frame (sub). */
int word_A87E = 0; /* Sprite height for current frame (sub). */
int word_A880 = 0; /* Update area rectangle X-coordinate for current frame (sub). */
int word_A882 = 0; /* Update area rectangle Y-coordinate for current frame (sub). */
int word_A884 = 0; /* Update area rectangle width for current frame (sub). */
int word_A886 = 0; /* Update area rectangle height for current frame (sub). */
int word_A888 = 0; /* Update area rectangle X-coordinate for previous frame (sub). */
int word_A88A = 0; /* Update area rectangle Y-coordinate for previous frame (sub). */
int word_A88C = 0; /* Update area rectangle width for previous frame (sub). */
int word_A88E = 0; /* Update area rectangle height for previous frame (sub). */
int word_A890 = 0; /* Screen X-coordinate for previous frame (sub). */
int word_A892 = 0; /* Screen Y-coordinate for previous frame (sub). */
int word_A894 = 0; /* Sprite width for previous frame (sub). */
int word_A896 = 0; /* Sprite height for previous frame (sub). */
WORD word_A898 = 0; /* Current frame rectangle and previous frame rectangle have no intersecion? (sub) (unused) */
WORD poo_state = 0; /* State. */
spriteinfo sprites[512] = {{{NULL, NULL}, 0, 0, 0, 0}}; /* Sprite list. First 256 for left-facing sprites, last 256 for right-facing sprites. */
int word_C0A4 = 0; /* No mouse action consecutive period counter. */
UINT word_C0AC = 0U; /* Configuration: Chime */
WORD word_C0AE = 0; /* Screen Mate window on top of subwindow? (unused) */
HWND dude = NULL; /* Self instance window handle. */
HBITMAP word_C0B2 = NULL; /* UFO beam render target. */
HBRUSH word_C0B4 = NULL; /* UFO beam paint colour brush. */
UINT word_C0B6 = 0U; /* Configuration: Always moving */
HBITMAP word_C0B8 = NULL; /* UFO beam colour rectangle image. */
WORD word_C0BA = 0; /* Remaining no-update periods after clearing windows. */
windowinfo stru_C0BC[32] = {{NULL, {0, 0, 0, 0}, {0}}}; /* Currently visible window list. */
WORD word_CA3C = 0; /* Prevent special actions? */
WORD word_CA3E = 0; /* Always on top? (unused) */
int word_CA40 = 0; /* Known instance count. */
UINT word_CA42 = 0U; /* Configuration: Gravity always on */
HBRUSH word_CA44 = NULL; /* UFO beam mask colour brush. */
int word_CA46 = 0; /* Fade out frame counter. */
int word_CA48 = 0; /* Unused. */
HPALETTE word_CA4A = NULL; /* Palette being used by window. */
int word_CA4C = 0; /* Unused. */
int word_CA4E = 0; /* Unused. */
int word_CA50 = 0; /* Screen width. */
int word_CA52 = 0; /* Screen height. */
WORD word_CA54 = 0; /* Temporarily holds sleep timeout action. */
WORD word_CA56 = 0; /* Not to clear subwindow? */
HINSTANCE word_CA58 = NULL; /* Current instance. */
UINT word_CA5A = 0U; /* Configuration: Cry */
int word_CA5C = 0; /* UFO beam height (sub). */
WORD word_CA5E = 0; /* Unused. */
HWND instances[9] = {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL}; /* Known instance list. When no other instance exists, [8] is used to store subwindow handle. */
int word_CA72 = 0; /* UFO beam height. */
int word_CA74 = 0; /* Number of currently visible windows. */
WORD word_CA76 = 0; /* Sleeping after timeout? */
WORD word_CA78 = 0; /* Unused. */
#ifdef _WIN32
HWND ownerwindow = NULL;
#endif

int drift_accel = 0;
#define DEBUG 1005
#define FADE_OUT_INDEX 175
#define DRIFT_SPEED 35
#define MAX_INSTANCES 32

void PASCAL sub_10(void FAR *, void FAR *);
int PASCAL sub_114(void FAR *, void FAR *, int);
void PASCAL sub_230(void FAR *, void FAR *);
#define sub_414(p) ((((BITMAPINFOHEADER FAR *)p)->biClrUsed == 0) ? ((DWORD)1 << ((BITMAPINFOHEADER FAR *)p)->biBitCount) : (((BITMAPINFOHEADER FAR *)p)->biClrUsed))
WORD sub_155A(void FAR *);
WORD sub_15B4(void FAR *);
HPALETTE sub_1658(HDC, void FAR *);
HPALETTE sub_1791(HDC, BYTE, BYTE, BYTE);
void sub_17FD(HDC, HPALETTE, HPALETTE, int);
WORD sub_1945(void FAR *);
void FAR * sub_19EC(void FAR *);
void sub_1A16(void FAR *, void FAR *, UINT);
int PASCAL WinMain(HINSTANCE, HINSTANCE, LPSTR, int);
void sub_1DDC(void);
LRESULT CALLBACK sub_1DF3(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK sub_2699(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK sub_27FF(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK debug_window(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
void sub_2A21(void);
void destroy_subwindow(void);
void place_window_as_top(HWND arg_0);
void sub_2B01(HWND, HWND);
BOOL sub_2B30(HDC, spriteinfo *, int, int);
void sub_2EEC(spriteinfo *);
void sub_2F36(void);
void sub_2FB7(LPCSTR, LPCSTR, UINT, LPCSTR);
void sub_2FF8(void);
BOOL sub_306A(HWND);
void sub_3119();
void sub_31A8(int, int, int);
void sub_3237(HWND);
void sub_3284(HWND);
void sub_3717(HWND);
void sub_399D(HWND, int, int, int, int);
BOOL sub_39D6(HWND);
int sub_3A36(int, int, int, int);
void sub_3B4C(HWND);
BOOL sub_3C20(HWND);
void sub_3D12(HWND);
void sub_3D5F(HWND);
void sub_3DA7(HWND);
void sub_3DF0(void);
int sub_3E7C(HWND *, int, int, int, int);
int sub_408C(HWND *, int, int, int, int);
int sub_419E(HWND, int, int, int, int);
void sub_4210(int, UINT, WORD);
void sub_428E(void);
void sub_42AA(LPCSTR);
void play_sound(int arg_0, UINT arg_2, WORD arg_4);
BOOL sub_42F3(HDC);
void sub_44ED(void);
void sub_4559(void);
void sub_4614(BOOL);
void sub_46D2(void);
void sub_46F7(void);
void update_sprite(int, int, int);
void update_sprite_sub(int, int, int);
BOOL sub_48F3(HWND);
void sub_491D(HWND, LPRECT);
void sub_496F(int);
void sub_4B3B(void);
void sub_4C21(int, int, int);
int sub_4C91(int, int);
void sub_4CE1(void);
void sub_4CF8(void);
void sub_8FD7(int);
void set_state_debug(WPARAM arg_0);
void sub_91CD(int, int);
BOOL sub_9200(HWND);
void sub_930F();
void sub_9350(int, int, int);
void sub_93DF(HWND);
void fade_out(HWND arg_0);
BOOL sub_9A49(HWND);

/* Make mask bitmap image out of the first pixel (by simulating x86 assembly). */
void PASCAL sub_10(void FAR * arg_4, void FAR * arg_0)
{
#define ax (LOWORD(eax))
#define cx (LOWORD(ecx))
#define STACK_SIZE (2 * sizeof(WORD))
    BYTE var_2;
    WORD var_4;
    WORD var_6;

    HLOCAL stack = LocalAlloc(LMEM_FIXED, STACK_SIZE);

    register DWORD eax;
    register BYTE al;
    register DWORD ebx;
    register DWORD ecx;
    register WORD dx;
    register BYTE FAR * source;
    register BYTE FAR * destination;
    register BYTE * sp = (BYTE *)stack + STACK_SIZE;

    al = 0; /* xor eax, eax */
    ecx = 0; /* xor ecx, ecx */
    dx = 0; /* xor edx, edx */
    source = arg_0; /* lds si, [bp+arg_0] */
    destination = arg_4; /* les di, [bp+arg_4] */
    eax = (WORD)sub_414(source); /* call sub_414 */
    sp -= sizeof(WORD); /* push ax */
    *(WORD *)sp = ax;
    *(DWORD FAR *)destination = *(DWORD FAR *)source; /* movsd; biSize */
    source += 4;
    destination += 4;
    eax = (*(DWORD FAR *)source); /* lodsd */
    source += 4;
    var_6 = LOWORD(eax); /* mov [bp+var_6], ax; var_6 = biWidth */
    *(DWORD FAR *)destination = eax; /* stosd; biWidth */
    destination += 4;
    eax = (*(DWORD FAR *)source); /* lodsd */
    source += 4;
    ecx = eax; /* mov ecx, eax; ecx = biHeight */
    *(DWORD FAR *)destination = eax; /* stosd; biHeight */
    destination += 4;
    eax = (*(DWORD FAR *)source); /* lodsd */
    source += 4;
    eax = 0x10001; /* mov eax, 10001h */
    *(DWORD FAR *)destination = eax; /* stosd; biPlanes = 1, biBitCount = 1 */
    destination += 4;
    *(DWORD FAR *)destination = *(DWORD FAR *)source; /* movsd; biCompression */
    source += 4;
    destination += 4;
    eax = (*(DWORD FAR *)source); /* lodsd; biSizeImage */
    source += 4;
    var_4 = (var_6 + 31) / 32; /* mov ax, [bp+var_6]; add ax, 31; shr ax, 5; mov [bp+var_4], ax */
    eax = var_4 * 4 * cx; /* shl ax, 2; mul cx */
    *(DWORD FAR *)destination = eax; /* stosd; biSizeImage = ceil(biWidth, 32) * 4 * biHeight */
    destination += 4;
    *(DWORD FAR *)destination = *(DWORD FAR *)source; /* movsd; biXPelsPerMeter */
    source += 4;
    destination += 4;
    *(DWORD FAR *)destination = *(DWORD FAR *)source; /* movsd; biYPelsPerMeter */
    source += 4;
    destination += 4;
    eax = (*(DWORD FAR *)source); /* lodsd */
    source += 4;
    eax = 2; /* mov eax, 2 */
    *(DWORD FAR *)destination = eax; /* stosd; biClrUsed */
    destination += 4;
    eax = (*(DWORD FAR *)source); /* lodsd */
    source += 4;
    eax = 2; /* mov eax, 2 */
    *(DWORD FAR *)destination = eax; /* stosd; biClrImportant */
    destination += 4;
    eax = *(WORD *)sp; /* pop ax */
    sp += sizeof(WORD);
    source += ax * sizeof(RGBQUAD); /* shl ax, 2; add si, ax */
    eax = 0x00FFFFFF; /* mov eax, 0FFFFFFh */
    *(DWORD FAR *)destination = eax; /* stosd */
    destination += 4;
    eax = 0; /* xor eax, eax */
    *(DWORD FAR *)destination = eax; /* stosd */
    destination += 4;
    var_2 = *source; /* mov al, [esi]; mov [bp+var_2], al; Read the first colour */
    do {
        sp -= sizeof(WORD); /* push cx */
        *(WORD *)sp = cx;
        dx = var_6; /* mov dx, [bp+var_6]; dx = biWidth */
        ecx = var_4; /* mov cx, [bp+var_4]; ceil(biWidth, 32) */
        do {
            sp -= sizeof(WORD); /* push cx */
            *(WORD *)sp = cx;
            ebx = 0; /* xor ebx, ebx */
            ecx = 8; /* mov cx, 8 */
            do {
                if ((short)dx > 0) { /* cmp dx, 0; jle short loc_F0 */
                    eax = (*(DWORD FAR *)source); /* lods dword ptr [esi] */
                    source += 4;
                    dx -= 4; /* sub dx, 4 */
                    al = LOBYTE(ax) == var_2 ? 1 : 0; /* cmp al, [bp+var_2]; setz al; */
                    ebx = ebx << 1 | (al == 0 ? 1 : 0); /* cmp al, 1; rcl ebx, 1 */
                    eax >>= 8; /* shr eax, 8 */
                    al = LOBYTE(ax) == var_2 ? 1 : 0; /* cmp al, [bp+var_2]; setz al; */
                    ebx = ebx << 1 | (al == 0 ? 1 : 0); /* cmp al, 1; rcl ebx, 1 */
                    eax >>= 8; /* shr eax, 8 */
                    al = LOBYTE(ax) == var_2 ? 1 : 0; /* cmp al, [bp+var_2]; setz al; */
                    ebx = ebx << 1 | (al == 0 ? 1 : 0); /* cmp al, 1; rcl ebx, 1 */
                    eax >>= 8; /* shr eax, 8 */
                    al = LOBYTE(ax) == var_2 ? 1 : 0; /* cmp al, [bp+var_2]; setz al; */
                    ebx = ebx << 1 | (al == 0 ? 1 : 0); /* cmp al, 1; rcl ebx, 1 */
                    eax >>= 8; /* shr eax, 8 */
                } else { /* jmp short loc_F4 */
                    ebx <<= 4; /* shl ebx, 4 */
                }
            } while (--ecx != 0); /* loop loc_A4; 8 */
            eax = (DWORD)LOBYTE(LOWORD(ebx)) << 24 | (DWORD)HIBYTE(LOWORD(ebx)) << 16 | (DWORD)LOBYTE(HIWORD(ebx)) << 8 | (DWORD)HIBYTE(HIWORD(ebx)); /* mov eax, ebx; xchg al, ah; ror eax, 16; xchg al, ah */
            *(DWORD FAR *)destination = eax; /* stos dword ptr es:[edi] */
            destination += 4;
            ecx = *(WORD *)sp; /* pop cx */
            sp += sizeof(WORD);
        } while (--ecx != 0); /* loop loc_9D; ceil(biWidth, 32) */
        ecx = *(WORD *)sp; /* pop cx */
        sp += sizeof(WORD);
    } while (--ecx != 0); /* loop loc_96; biHeight */

    LocalFree(stack);
#undef STACK_SIZE
#undef cx
#undef ax
}

/* Make mask bitmap image out of the specific colour index in the palette (by simulating x86 assembly) (unused). */
int PASCAL sub_114(void FAR * arg_6, void FAR * arg_2, int arg_0)
{
#define ax (LOWORD(eax))
#define bl (LOBYTE(LOWORD(ebx)))
#define cx (LOWORD(ecx))
#define STACK_SIZE (sizeof(WORD) + sizeof(BYTE FAR *))
    BYTE var_2;
    int var_4;
    WORD var_6;
    WORD var_8;

    HLOCAL stack = LocalAlloc(LMEM_FIXED, STACK_SIZE);

    register DWORD eax;
    register BYTE al;
    register DWORD ebx;
    register DWORD ecx;
    register WORD dx;
    register BYTE FAR * source;
    register BYTE FAR * destination;
    register BYTE * sp = (BYTE *)stack + STACK_SIZE;

    var_4 = 0; /* mov [bp+var_4], 0 */
    al = 0; /* xor eax, eax */
    ecx = 0; /* xor ecx, ecx */
    dx = 0; /* xor edx, edx */
    source = arg_2; /* lds si, [bp+arg_2] */
    destination = arg_6; /* les di, [bp+arg_6] */
    eax = (WORD)sub_414(source); /* call sub_414 */
    sp -= sizeof(WORD); /* push ax */
    *(WORD *)sp = ax;
    *(DWORD FAR *)destination = *(DWORD FAR *)source; /* movsd; biSize */
    source += 4;
    destination += 4;
    eax = (*(DWORD FAR *)source); /* lodsd */
    source += 4;
    var_8 = LOWORD(eax); /* mov [bp+var_8], ax; var_8 = biWidth */
    *(DWORD FAR *)destination = eax; /* stosd; biWidth */
    destination += 4;
    eax = (*(DWORD FAR *)source); /* lodsd */
    source += 4;
    ecx = eax; /* mov ecx, eax; ecx = biHeight */
    *(DWORD FAR *)destination = eax; /* stosd; biHeight */
    destination += 4;
    eax = (*(DWORD FAR *)source); /* lodsd */
    source += 4;
    eax = 0x10001; /* mov eax, 10001h */
    *(DWORD FAR *)destination = eax; /* stosd; biPlanes = 1, biBitCount = 1 */
    destination += 4;
    *(DWORD FAR *)destination = *(DWORD FAR *)source; /* movsd; biCompression */
    source += 4;
    destination += 4;
    eax = (*(DWORD FAR *)source); /* lodsd; biSizeImage */
    source += 4;
    var_6 = (var_8 + 31) / 32; /* mov ax, [bp+var_8]; add ax, 31; shr ax, 5; mov [bp+var_6], ax */
    eax = var_6 * 4 * cx; /* shl ax, 2; mul cx */
    *(DWORD FAR *)destination = eax; /* stosd; biSizeImage = ceil(biWidth, 32) * 4 * biHeight */
    destination += 4;
    *(DWORD FAR *)destination = *(DWORD FAR *)source; /* movsd; biXPelsPerMeter */
    source += 4;
    destination += 4;
    *(DWORD FAR *)destination = *(DWORD FAR *)source; /* movsd; biYPelsPerMeter */
    source += 4;
    destination += 4;
    eax = (*(DWORD FAR *)source); /* lodsd */
    source += 4;
    eax = 2; /* mov eax, 2 */
    *(DWORD FAR *)destination = eax; /* stosd; biClrUsed */
    destination += 4;
    eax = (*(DWORD FAR *)source); /* lodsd */
    source += 4;
    eax = 2; /* mov eax, 2 */
    *(DWORD FAR *)destination = eax; /* stosd; biClrImportant */
    destination += 4;
    sp -= sizeof(BYTE FAR *); /* push esi */
    *(BYTE FAR * *)sp = source;
    source += arg_0 * sizeof(RGBQUAD); /* mov ax, [bp+arg_0]; shl ax, 2; add si, ax */
    *(DWORD FAR *)destination = *(DWORD FAR *)source; /* movsd; Mask colour */
    source += 4;
    destination += 4;
    eax = 0; /* xor eax, eax */
    *(DWORD FAR *)destination = eax; /* stosd */
    destination += 4;
    source = *(BYTE FAR * *)sp; /* pop esi */
    sp += sizeof(BYTE FAR *);
    eax = *(WORD *)sp; /* pop ax */
    sp += sizeof(WORD);
    source += ax * sizeof(RGBQUAD); /* shl ax, 2; add si, ax */
    var_2 = LOBYTE(arg_0); /* mov ax, [bp+arg_0]; mov [bp+var_2], al; The specified colour */
    do {
        sp -= sizeof(WORD); /* push cx */
        *(WORD *)sp = cx;
        dx = var_8; /* mov dx, [bp+var_8]; dx = biWidth */
        ecx = var_6; /* mov cx, [bp+var_6]; ceil(biWidth, 32) */
        do {
            sp -= sizeof(WORD); /* push cx */
            *(WORD *)sp = cx;
            ebx = 0; /* xor ebx, ebx */
            ecx = 8; /* mov cx, 8 */
            do {
                if ((short)dx > 0) { /* cmp dx, 0; jle short loc_209 */
                    eax = (*(DWORD FAR *)source); /* lods dword ptr [esi] */
                    source += 4;
                    dx -= 4; /* sub dx, 4 */
                    al = LOBYTE(ax) == var_2 ? 1 : 0; /* cmp al, [bp+var_2]; setz al; */
                    ebx = ebx << 1 | (al == 0 ? 1 : 0); /* cmp al, 1; rcl ebx, 1 */
                    eax >>= 8; /* shr eax, 8 */
                    al = LOBYTE(ax) == var_2 ? 1 : 0; /* cmp al, [bp+var_2]; setz al; */
                    ebx = ebx << 1 | (al == 0 ? 1 : 0); /* cmp al, 1; rcl ebx, 1 */
                    eax >>= 8; /* shr eax, 8 */
                    al = LOBYTE(ax) == var_2 ? 1 : 0; /* cmp al, [bp+var_2]; setz al; */
                    ebx = ebx << 1 | (al == 0 ? 1 : 0); /* cmp al, 1; rcl ebx, 1 */
                    eax >>= 8; /* shr eax, 8 */
                    al = LOBYTE(ax) == var_2 ? 1 : 0; /* cmp al, [bp+var_2]; setz al; */
                    ebx = ebx << 1 | (al == 0 ? 1 : 0); /* cmp al, 1; rcl ebx, 1 */
                    eax >>= 8; /* shr eax, 8 */
                    al = bl; /* mov al, bl */
                    al &= 0x0F; /* and al, 0Fh */
                    var_4 += al < 0x0F ? 1 : 0; /* cmp al, 0Fh; adc [bp+var_4], 0 */
                } else { /* jmp short loc_20D */
                    ebx <<= 4; /* shl ebx, 4 */
                }
            } while (--ecx != 0); /* loop loc_1B3; 8 */
            eax = (DWORD)LOBYTE(LOWORD(ebx)) << 24 | (DWORD)HIBYTE(LOWORD(ebx)) << 16 | (DWORD)LOBYTE(HIWORD(ebx)) << 8 | (DWORD)HIBYTE(HIWORD(ebx)); /* mov eax, ebx; xchg al, ah; ror eax, 16; xchg al, ah */
            *(DWORD FAR *)destination = eax; /* stos dword ptr es:[edi] */
            destination += 4;
            ecx = *(WORD *)sp; /* pop cx */
            sp += sizeof(WORD);
        } while (--ecx != 0); /* loop loc_1AC; ceil(biWidth, 32) */
        ecx = *(WORD *)sp; /* pop cx */
        sp += sizeof(WORD);
    } while (--ecx != 0); /* loop loc_1A5; biHeight */

    LocalFree(stack);

    return var_4; /* mov ax, [bp+var_4]; Number of non-transparent blocks */
#undef STACK_SIZE
#undef cx
#undef bl
#undef ax
}

/* Decompress bitmap image. */
void PASCAL sub_230(void FAR * arg_4, void FAR * arg_0)
{
    BYTE FAR * source = arg_0;
    BYTE FAR * destination = arg_4;
    BYTE FAR * originalsource = NULL;
    BYTE FAR * originaldestination = NULL;
    WORD var_2 = (WORD)((BITMAPINFOHEADER FAR *)source)->biWidth;
    DWORD ebx = var_2 + 3 & (WORD)-4;
    LONG edx = ((BITMAPINFOHEADER FAR *)source)->biHeight;
    DWORD compression = ((BITMAPINFOHEADER FAR *)source)->biCompression;
    WORD bitcount = ((BITMAPINFOHEADER FAR *)source)->biBitCount;
    DWORD counter = ((BITMAPINFOHEADER FAR *)source)->biSize;
    BYTE bytebuffer[2] = {0, 0};
    for (; counter != 0; counter -= 1) {
        *destination++ = *source++;
    }
    ((BITMAPINFOHEADER FAR *)arg_4)->biCompression = BI_RGB;
    counter = sub_414(arg_0) * 4;
    for (; counter != 0; counter -= 1) {
        *destination++ = *source++;
    }
    if (compression == BI_RGB) {
        if (bitcount == 4) {
            ((BITMAPINFOHEADER FAR *)arg_4)->biBitCount = 8;
            if (((BITMAPINFOHEADER FAR *)arg_4)->biClrUsed == 0) {
                ((BITMAPINFOHEADER FAR *)arg_4)->biClrUsed = 16;
            }
            compression = edx;
            var_2 = (var_2 + 1) / 2 + 3 & -4;
            do {
                originalsource = source;
                originaldestination = destination;
                counter = ebx;
                do {
                    if ((counter & 1) == 0) {
                        bytebuffer[0] = *source++;
                        bytebuffer[1] = bytebuffer[0];
                        bytebuffer[0] >>= 4;
                    } else {
                        bytebuffer[0] = bytebuffer[1];
                        bytebuffer[0] &= 0x0F;
                    }
                    *destination++ = bytebuffer[0];
                } while (--counter != 0);
                destination = originaldestination + ebx;
                source = originalsource + var_2;
            } while (--compression != 0);
        } else {
            counter = ebx * edx;
            for (; counter != 0; counter -= 1) {
                *destination++ = *source++;
            }
        }
    } else if (compression == BI_RLE4) {
        ((BITMAPINFOHEADER FAR *)arg_4)->biBitCount = 8;
        if (((BITMAPINFOHEADER FAR *)arg_4)->biClrUsed == 0) {
            ((BITMAPINFOHEADER FAR *)arg_4)->biClrUsed = 16;
        }
        for (;;) {
            originaldestination = destination;
            for (;;) {
                bytebuffer[0] = *source++;
                bytebuffer[1] = *source++;
                if (bytebuffer[0] == 0) {
                    if (bytebuffer[1] == 0) {
                        destination = originaldestination + ebx;
                        break;
                    } else if (bytebuffer[1] == 1) {
                        return;
                    } else if (bytebuffer[1] == 2) {
                        bytebuffer[0] = *source++;
                        bytebuffer[1] = *source++;
                        if (bytebuffer[1] != 0) {
                            do {
                                destination += ebx;
                                originaldestination += ebx;
                            } while (bytebuffer[1] != 0);
                        }
                        destination += bytebuffer[0];
                    } else {
                        counter = bytebuffer[1];
                        do {
                            bytebuffer[0] = *source++;
                            bytebuffer[1] = *source++;
                            *destination++ = bytebuffer[0] >> 4;
                            if (--counter == 0) {
                                break;
                            }
                            *destination++ = bytebuffer[0] & 0x0F;
                            if (--counter == 0) {
                                break;
                            }
                            *destination++ = bytebuffer[1] >> 4;
                            if (--counter == 0) {
                                break;
                            }
                            *destination++ = bytebuffer[1] & 0x0F;
                        } while (--counter != 0);
                    }
                } else {
                    counter = bytebuffer[0] / 2;
                    for (; counter != 0; counter -= 1) {
                        *destination++ = bytebuffer[1] >> 4;
                        *destination++ = bytebuffer[1] & 0x0F;
                    }
                    if ((bytebuffer[0] & 1) != 0) {
                        *destination++ = bytebuffer[1] >> 4;
                    }
                }
            }
        }
    } else if (compression == BI_RLE8) {
        for (;;) {
            originaldestination = destination;
            for (;;) {
                bytebuffer[0] = *source++;
                bytebuffer[1] = *source++;
                if (bytebuffer[0] == 0) {
                    if (bytebuffer[1] == 0) {
                        destination = originaldestination + ebx;
                        break;
                    } else if (bytebuffer[1] == 1) {
                        return;
                    } else if (bytebuffer[1] == 2) {
                        bytebuffer[0] = *source++;
                        bytebuffer[1] = *source++;
                        if (bytebuffer[1] != 0) {
                            do {
                                destination += ebx;
                                originaldestination += ebx;
                            } while (bytebuffer[1] != 0);
                        }
                        destination += bytebuffer[0];
                    } else {
                        counter = bytebuffer[1];
                        do {
                            *destination++ = *source++;
                        } while (--counter != 0);
                        source += bytebuffer[1] & 1;
                    }
                } else {
                    do {
                        *destination++ = bytebuffer[1];
                    } while (--bytebuffer[0] != 0);
                }
            }
        }
    }
}

/* Get palette size. */
WORD sub_155A(void FAR * arg_0)
{
    DWORD FAR * var_4;
    WORD var_6;
    var_4 = arg_0;
    var_6 = sub_15B4(var_4);
    if (*var_4 == 12) {
        return var_6 * sizeof(RGBTRIPLE);
    } else {
        return var_6 * sizeof(RGBQUAD);
    }
}

/* Get number of colours in palette. */
WORD sub_15B4(void FAR * arg_0)
{
    WORD var_2;
    BITMAPINFOHEADER FAR * var_6;
    BITMAPCOREHEADER FAR * var_A;
    var_6 = arg_0;
    var_A = arg_0;
    if (var_6->biSize != 12) {
        if (var_6->biClrUsed != 0) {
            return (WORD)var_6->biClrUsed;
        }
        var_2 = var_6->biBitCount;
    } else {
        var_2 = var_A->bcBitCount;
    }
    switch (var_2) {
    case 1:
        return 2;
    case 4:
        return 16;
    case 8:
        return 256;
    default:
        return 0;
    }
}

/* Create palette based on given bitmap image. */
HPALETTE sub_1658(HDC arg_0, void FAR * arg_2)
{
    const BYTE FAR * var_4;
    HPALETTE var_6;
    LOGPALETTE * var_8;
    int var_A;
    int var_C;
    if ((GetDeviceCaps(arg_0, RASTERCAPS) & RC_PALETTE) == 0) {
        var_A = 256;
    } else {
        var_A = arg_2 != NULL ? ((BITMAPINFOHEADER FAR *)arg_2)->biClrUsed != 0 ? (int)(((BITMAPINFOHEADER FAR *)arg_2)->biClrUsed) : (1 << ((BITMAPINFOHEADER FAR *)arg_2)->biBitCount) : GetDeviceCaps(arg_0, SIZEPALETTE);
    }
    var_8 = (LOGPALETTE *)LocalAlloc(LPTR, (var_A + 2) * sizeof(PALETTEENTRY));
    var_8->palVersion = 0x0300;
    var_8->palNumEntries = (WORD)var_A;
    if (arg_2 != NULL) {
        var_4 = (BYTE FAR *)arg_2 + ((BITMAPINFOHEADER FAR *)arg_2)->biSize;
        for (var_C = 0; var_C < var_A; var_C += 1) {
            var_8->palPalEntry[var_C].peRed = var_4[2];
            var_8->palPalEntry[var_C].peGreen = var_4[1];
            var_8->palPalEntry[var_C].peBlue = var_4[0];
            var_8->palPalEntry[var_C].peFlags = 0;
            var_4 += sizeof(PALETTEENTRY);
        }
    } else {
        GetSystemPaletteEntries(arg_0, 0U, var_A, var_8->palPalEntry);
    }
    var_6 = CreatePalette(var_8);
    LocalFree(var_8);
    return var_6;
}

/* Create mask palette based on given RGB values. */
HPALETTE sub_1791(HDC arg_0, BYTE arg_2, BYTE arg_3, BYTE arg_4)
{
    const BYTE FAR * var_4;
    HPALETTE var_6;
    LOGPALETTE * var_8;
    var_8 = (LOGPALETTE *)LocalAlloc(LPTR, 12U);
    var_8->palVersion = 0x0300;
    var_8->palNumEntries = 1;
    var_8->palPalEntry[0].peRed = arg_2;
    var_8->palPalEntry[0].peGreen = arg_3;
    var_8->palPalEntry[0].peBlue = arg_4;
    var_8->palPalEntry[0].peFlags = 0;
    var_4/* += 4 */;
    var_6 = CreatePalette(var_8);
    LocalFree(var_8);
    return var_6;
}

/* Set palette entries based on another palette with nearest colours (unused). */
void sub_17FD(HDC arg_0, HPALETTE arg_2, HPALETTE arg_4, int arg_6)
{
    PALETTEENTRY * var_2;
    PALETTEENTRY * var_4;
    int var_6;
    int var_8;
    COLORREF var_C;
    COLORREF var_10;
    int var_12;
    if ((GetDeviceCaps(arg_0, RASTERCAPS) & RC_PALETTE) == 0) {
        return;
    }
    var_6 = GetDeviceCaps(arg_0, SIZEPALETTE);
    if (var_6 == 0) {
        var_6 = 256;
    }
    var_2 = (PALETTEENTRY *)LocalAlloc(LPTR, arg_6 * sizeof(PALETTEENTRY));
    var_4 = (PALETTEENTRY *)LocalAlloc(LPTR, var_6 * sizeof(PALETTEENTRY));
    GetPaletteEntries(arg_4, 0U, arg_6, var_2);
    GetPaletteEntries(arg_2, 0U, var_6, var_4);
    for (var_8 = 0; var_8 < arg_6; var_8 += 1) {
        var_C = *(COLORREF *)&var_2[var_8];
        var_12 = GetNearestPaletteIndex(arg_2, var_C);
        var_10 = *(COLORREF *)&var_4[var_12];
        if (var_10 != var_C) {
            if (var_12 < 10 || var_12 > word_9CF0) {
                if (word_9CF0 < 10) {
                    break;
                } else {
                    var_12 = word_9CF0--;
                }
            }
            *(COLORREF *)&var_4[var_12] = var_C;
        }
    }
    SetPaletteEntries(arg_2, 0U, var_6, var_4);
    LocalFree(var_4);
    LocalFree(var_2);
}

/* Get colour index of first pixel. */
WORD sub_1945(void FAR * arg_0)
{
    BYTE FAR * var_4;
    BYTE var_6;
    var_4 = sub_19EC(arg_0);
    var_6 = *var_4++;
    switch (((BITMAPINFOHEADER FAR *)arg_0)->biCompression) {
    case BI_RGB:
        if (((BITMAPINFOHEADER FAR *)arg_0)->biBitCount == 4) {
            var_6 >>= 4;
        }
        break;
    case BI_RLE8:
        if (var_6 == 0) {
            var_4 += 1;
        }
        var_6 = *var_4;
        break;
    case BI_RLE4:
        if (var_6 == 0) {
            var_4 += 1;
        }
        var_6 = *var_4 >> 4;
        break;
    default:
        break;
    }
    return var_6;
}

/* Get pointer to pixel bits in bitmap image. */
void FAR * sub_19EC(void FAR * arg_0)
{
    return (void FAR *)((BYTE FAR *)arg_0 + *(WORD FAR *)arg_0 + ((BITMAPINFOHEADER FAR *)arg_0)->biClrUsed * sizeof(RGBQUAD));
}

/* Flip bitmap image. arg_8 contains 1: Flip horizontally, arg_8 contains 2: Flip vertically */
void sub_1A16(void FAR * arg_0, void FAR * arg_4, UINT arg_8)
{
    BYTE FAR * var_4;
    BYTE FAR * var_8;
    BYTE FAR * var_C;
    int var_E;
    int var_10;
    int var_12;
    int var_14;
    LONG var_18;
    var_4 = arg_0;
    var_8 = arg_4;
    var_C = sub_19EC(arg_4);
    while (var_C > var_8) {
        *var_4++ = *var_8++;
    }
    var_E = ((BITMAPINFOHEADER FAR *)arg_4)->biBitCount == 4 ? (int)(-(((BITMAPINFOHEADER FAR *)arg_4)->biWidth % 8) & 7) : (int)(-(((BITMAPINFOHEADER FAR *)arg_4)->biWidth % 4) & 3);
    var_10 = ((BITMAPINFOHEADER FAR *)arg_4)->biBitCount == 4 ? (int)(((BITMAPINFOHEADER FAR *)arg_4)->biWidth + var_E) / 2 : (int)(((BITMAPINFOHEADER FAR *)arg_4)->biWidth + var_E);
    if ((arg_8 & 2) != 0) {
        var_C += (((BITMAPINFOHEADER FAR *)arg_4)->biHeight - 1) * var_10;
    }
    if ((arg_8 & 1) != 0) {
        var_18 = ((BITMAPINFOHEADER FAR *)arg_4)->biBitCount == 4 ? ((BITMAPINFOHEADER FAR *)arg_4)->biWidth / 2 : ((BITMAPINFOHEADER FAR *)arg_4)->biWidth;
        var_C += var_18 - 1;
    }
    for (var_12 = 0; ((BITMAPINFOHEADER FAR *)arg_4)->biHeight > var_12; var_12 += 1) {
        var_8 = var_C;
        if (((BITMAPINFOHEADER FAR *)arg_4)->biBitCount == 4) {
            if ((arg_8 & 1) != 0) {
                for (var_14 = 0; var_14 < var_10; var_14 += 1) {
                    *var_4 = *var_8 >> 4 & 0x0F | *var_8 << 4;
                    var_8 -= 1;
                    var_4 += 1;
                }
            } else {
                for (var_14 = 0; var_14 < var_10; var_14 += 1) {
                    *var_4++ = *var_8++;
                }
            }
        } else {
            if ((arg_8 & 1) != 0) {
                for (var_14 = 0; var_14 < var_10; var_14 += 1) {
                    *var_4++ = *var_8--;
                }
            } else {
                for (var_14 = 0; var_14 < var_10; var_14 += 1) {
                    *var_4++ = *var_8++;
                }
            }
        }
        var_18 = (arg_8 & 2) != 0 ? -var_10 : var_10;
        var_C += var_18;
    }
}

/* WinMain function. */
int PASCAL WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
    HWND var_2;
    MSG var_14;
    WNDCLASS var_2E;
    if (hPrevInstance == NULL) {
        var_2E.style = CS_DBLCLKS;
        var_2E.lpfnWndProc = sub_1DF3;
        var_2E.cbClsExtra = 0;
        var_2E.cbWndExtra = 8;
        var_2E.hInstance = hInstance;
        var_2E.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(100));
        var_2E.hCursor = LoadCursor(hInstance, MAKEINTRESOURCE(106));
        var_2E.hbrBackground = NULL;
        var_2E.lpszMenuName = NULL;
        var_2E.lpszClassName = "ScreenMatePoo";
        if (RegisterClass(&var_2E) == 0) {
            return 0;
        }
    }
    if (hPrevInstance == NULL) {
        var_2E.style = CS_DBLCLKS;
        var_2E.lpfnWndProc = sub_2699;
        var_2E.cbClsExtra = 0;
        var_2E.cbWndExtra = 0;
        var_2E.hInstance = hInstance;
        var_2E.hIcon = NULL;
        var_2E.hCursor = LoadCursor(NULL, IDC_ARROW);
        var_2E.hbrBackground = NULL;
        var_2E.lpszMenuName = NULL;
        var_2E.lpszClassName = "ScreenMatePooSub";
        if (RegisterClass(&var_2E) == 0) {
            return 0;
        }
    }
    word_CA58 = hInstance;
    if (FindWindow("ScreenMatePoo", "Screen Mate") != NULL) {
        word_CA3C = 1;
    }
#ifdef _WIN32
    /* In 32-bit Windows, popup window is now in the taskbar by default. Additional code is needed to hide the popup window from taskbar while keeping it in the Alt+Tab list. */
    var_2E.style = 0;
    var_2E.lpfnWndProc = DefWindowProc;
    var_2E.cbClsExtra = 0;
    var_2E.cbWndExtra = 0;
    var_2E.hInstance = hInstance;
    var_2E.hIcon = NULL;
    var_2E.hCursor = NULL;
    var_2E.hbrBackground = NULL;
    var_2E.lpszMenuName = NULL;
    var_2E.lpszClassName = "ScreenMatePooOwner";
    if (RegisterClass(&var_2E) == 0) {
        return 0;
    }
    /* Create a hidden owner top-level window to hide visible windows from taskbar. */
    ownerwindow = CreateWindowEx(0L, "ScreenMatePooOwner", NULL, 0L, 0, 0, 0, 0, NULL, NULL, hInstance, NULL);
    if (ownerwindow == NULL) {
        return 0;
    }
    /* Set the visible window to be owned by the hidden top-level window. */
    var_2 = CreateWindowEx(0L, "ScreenMatePoo", "Screen Mate", WS_POPUP, 0, 0, 0, 0, ownerwindow, NULL, hInstance, NULL);
#else
    var_2 = CreateWindowEx(0L, "ScreenMatePoo", "Screen Mate", WS_POPUP, 0, 0, 0, 0, NULL, NULL, hInstance, NULL);
#endif
    if (var_2 == NULL) {
        return 0;
    }
    ShowWindow(var_2, nShowCmd);
    UpdateWindow(var_2);
    while (GetMessage(&var_14, NULL, 0U, 0U)) {
        TranslateMessage(&var_14);
        DispatchMessage(&var_14);
    }
#ifdef _WIN32
    DestroyWindow(ownerwindow);
#endif
    return (int)var_14.wParam;
}

/* Set cursor position changed flag (unused). */
void sub_1DDC(void)
{
    word_A798 = 1;
}

/* Window procedure. */
LRESULT CALLBACK sub_1DF3(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    HDC var_2;
    void FAR * var_6;
    HGLOBAL var_8;
    HRSRC var_A;
    void FAR * var_E;
    int var_10;
    int var_12;
    RECT var_1A;
    POINT var_1E;
    char var_122[260];
    POINT var_126;
    UINT var_128;
    FARPROC proc;
    LPWINDOWPOS windowpos;
#ifdef _WIN32
    HANDLE user32;
#endif
    switch (uMsg) {
    case WM_CREATE:
        if (!sub_3C20(hWnd)) {
            MessageBox(hWnd, "Screen Mate‚ÍÅ‘å‚WŒÂ‚Ü‚Å‚Å‚·", "Screen Mate", MB_ICONHAND | MB_OK);
            return -1;
        }
#ifdef _WIN32
        /* Additional code that allows drag-and-drop across different privileges in Windows Vista and higher. */
        user32 = GetModuleHandle("user32.dll");
        if (user32 != NULL) {
            BOOL (WINAPI * changewindowmessagefilterex)(HWND, UINT, DWORD, LPVOID) = (BOOL (WINAPI *)(HWND, UINT, DWORD, LPVOID))GetProcAddress(user32, "ChangeWindowMessageFilterEx");
            if (changewindowmessagefilterex != NULL) {
                changewindowmessagefilterex(hWnd, 0x0049, 1, NULL); /* WM_COPYGLOBALDATA */
                changewindowmessagefilterex(hWnd, 0x004A, 1, NULL); /* WM_COPYDATA */
                changewindowmessagefilterex(hWnd, WM_DROPFILES, 1, NULL);
            }
        }
#endif
            DragAcceptFiles(hWnd, TRUE);
            dude = hWnd;
            sub_2F36();
            if (word_CA3E != 0) {
                SetWindowPos(hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
            }
            var_2 = GetDC(hWnd);
            var_A = FindResource(word_CA58, MAKEINTRESOURCE(101), RT_BITMAP);
            var_8 = LoadResource(word_CA58, var_A);
            var_6 = LockResource(var_8);
            var_E = (void FAR *) ((BYTE FAR *) var_6 + *(WORD FAR *) var_6 + sub_155A(var_6));
            word_CA4A = sub_1658(var_2, var_6);
            FreeResource(var_8);
            SelectPalette(var_2, word_CA4A, FALSE);
            RealizePalette(var_2);
            if (!sub_42F3(var_2)) {
                MessageBox(hWnd, "ƒƒ‚ƒŠ‚ª•s‘«‚µ‚Ä‚¢‚Ü‚·", "Screen Mate", MB_ICONHAND | MB_OK);
                ReleaseDC(hWnd, var_2);
                return -1;
            }
            ReleaseDC(hWnd, var_2);
            if (!sub_306A(hWnd)) {
                MessageBox(hWnd, "ƒƒ‚ƒŠ‚ª•s‘«‚µ‚Ä‚¢‚Ü‚·", "Screen Mate", MB_ICONHAND | MB_OK);
                return -1;
            }
            SetTimer(hWnd, 1U, 108U, NULL);
            break;
        case WM_DROPFILES:
            if (instances[MAX_INSTANCES-1] == NULL) {
                if (DragQueryFile((HDROP) wParam, 0U, var_122, 260U) != 0U) {
                    sub_42AA(var_122);
                    sub_8FD7(4);
                }
            }
            DragFinish((HDROP) wParam);
            break;
        case WM_USER + 1:
            switch (lParam) {
                case 0x202:
                case 0x205:
                    if (word_C0AE != 0) {
                        if (instances[MAX_INSTANCES-1] != NULL) {
                            place_window_as_top(instances[MAX_INSTANCES-1]);
                        }
                        place_window_as_top(hWnd);
                    } else {
                        place_window_as_top(hWnd);
                        if (instances[MAX_INSTANCES-1] != NULL) {
                            place_window_as_top(instances[MAX_INSTANCES-1]);
                        }
                    }
                    place_window_as_top(hWnd);
                    *(HMENU *) var_122 = CreatePopupMenu();
                    AppendMenu(*(HMENU *) var_122, 0U, 101U, "Screen Mate‚ÌÝ’è...");
                    AppendMenu(*(HMENU *) var_122, 0U, IDCANCEL, "Screen Mate‚ÌI—¹");
                    GetCursorPos(&var_126);
                    TrackPopupMenu(*(HMENU *) var_122, 0U, var_126.x, var_126.y, 0, hWnd, NULL);
                    DestroyMenu(*(HMENU *) var_122);
                    return 0;
                default:
                    break;
            }
            return 0;
        case WM_WINDOWPOSCHANGING:
            windowpos = (LPWINDOWPOS) lParam;
            if (word_A7A2 != 0) {
                windowpos = (LPWINDOWPOS) lParam;
            }
            windowpos->flags |= SWP_NOCOPYBITS;
            word_A7AC = 1;
            return 0;
        case WM_WINDOWPOSCHANGED:
            return 0;
        case WM_TIMER:
            if (word_C0BA != 0) {
                word_C0BA -= 1;
                return 0;
            }
            if (word_C0B6 == 0) {
                GetCursorPos(&var_1E);
                if (stru_A7B0.x != var_1E.x || stru_A7B0.y != var_1E.y) {
                    stru_A7B0.x = var_1E.x;
                    stru_A7B0.y = var_1E.y;
                    word_A798 = 1;
                }
                if (word_CA76 != 0) {
                    if (word_A798 != 0) {
                        word_CA76 = 0;
                        word_C0A4 = 0;
                        sub_8FD7(0);
                    }
                } else {
                    if (word_A798 != 0) {
                        word_A798 = 0;
                        word_C0A4 = 0;
                    } else {
                        if (word_C0A4++ > 300) {
                            sub_8FD7(3);
                        }
                    }
                }
            }
            word_A7AC = 0;
            dude = hWnd;
            sub_4CF8();
            sub_3284(hWnd);
            sub_3717(hWnd);
            word_A7A2 = 1;
            return 0;
        case WM_USER:
            if (wParam == 1) {
                sub_3D5F((HWND) lParam);
            }
            if (wParam == 2) {
                sub_3DA7((HWND) lParam);
            }
            return 0;
        case WM_PAINT:
            if (word_A7AC != 0) {
                word_A7AC = 0;
                ValidateRect(hWnd, NULL);
                return 0;
            }
            sub_3237(hWnd);
            ValidateRect(hWnd, NULL);
            return 0;
            GetWindowRect(hWnd, &var_1A);
            if (stru_A7A4.top == var_1A.top && stru_A7A4.bottom == var_1A.bottom && stru_A7A4.left == var_1A.left &&
                stru_A7A4.right == var_1A.right) {
                sub_3237(hWnd);
                ValidateRect(hWnd, NULL);
                return 0;
            }
            GetWindowRect(hWnd, &stru_A7A4);
            ValidateRect(hWnd, NULL);
            return 0;
            break;
        case WM_QUIT:
            return 0;
        case WM_COMMAND:
            switch (wParam) {
                case 101U:
                    SendMessage(hWnd, WM_USER + 2, 0, 0);
                    break;
                case IDCANCEL:
                    DestroyWindow(hWnd);
                    return 0;
                    break;
                default:
                    break;
            }
            break;
        case WM_LBUTTONDOWN:
        case WM_RBUTTONDOWN:
            if (instances[MAX_INSTANCES-1] != NULL) {
                break;
            }
            if (word_A79E != 0) {
                break;
            }
            SetCapture(hWnd);
            GetWindowRect(hWnd, &var_1A);
            word_A79A = (short) LOWORD(lParam) + var_1A.left;
            word_A79C = (short) HIWORD(lParam) + var_1A.top;
            word_A79E = 1;
            sub_8FD7(1);
            sub_3284(hWnd);
            sub_3717(hWnd);
            break;
        case WM_MOUSEMOVE:
            if (word_A79E == 0) {
                break;
            }
            GetWindowRect(hWnd, &var_1A);
            var_10 = (short) LOWORD(lParam) + var_1A.left;
            var_12 = (short) HIWORD(lParam) + var_1A.top;
            if (word_A79A == var_10 && word_A79C == var_12) {
                break;
            }
            sub_91CD(var_10 - word_A79A, var_12 - word_A79C);
            word_A79A = var_10;
            word_A79C = var_12;
            sub_3284(hWnd);
            sub_3717(hWnd);
            break;
        case WM_RBUTTONUP:
            if (word_A7A0 != 0) {
                DestroyWindow(hWnd);
                break;
            }
        case WM_LBUTTONUP:
            if (word_A79E != 0) {
                GetWindowRect(hWnd, &var_1A);
                var_10 = (short) LOWORD(lParam) + var_1A.left;
                var_12 = (short) HIWORD(lParam) + var_1A.top;
                sub_91CD(var_10 - word_A79A, var_12 - word_A79C);
                sub_8FD7(0);
                if (uMsg == WM_RBUTTONUP) {
                    sub_8FD7(2);
                }
                sub_3284(hWnd);
                sub_3717(hWnd);
                ReleaseCapture();
                word_A79E = 0;
            }
            break;
        case WM_RBUTTONDBLCLK:
            word_A7A0 = 1;
            break;
        case WM_LBUTTONDBLCLK:
        case WM_USER + 2:
            *(WORD *) var_122 = word_CA3E;
            var_128 = word_CA42;
            dude = hWnd;
            if ((HIBYTE(GetKeyState(VK_SHIFT)) & 0x80) != 0 && (HIBYTE(GetKeyState(VK_CONTROL)) & 0x80) != 0) {
                proc = MakeProcInstance((FARPROC) debug_window, word_CA58);
                DialogBox(word_CA58, MAKEINTRESOURCE(108), hWnd, (DLGPROC) proc);
            } else {
                proc = MakeProcInstance((FARPROC) sub_27FF, word_CA58);
                DialogBox(word_CA58, MAKEINTRESOURCE(107), hWnd, (DLGPROC) proc);
            }
            FreeProcInstance(proc);
            sub_3237(hWnd);
            if (*(WORD *) var_122 != word_CA3E) {
                if (word_CA3E != 0) {
                    SetWindowPos(hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
                } else {
                    SetWindowPos(hWnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
                }
            }
            if (var_128 != word_CA42 && word_CA42 != 0) {
                sub_8FD7(2);
            }
            break;
        case WM_DESTROY:
            sub_3D12(hWnd);
            if (instances[MAX_INSTANCES-1] != NULL) {
                destroy_subwindow();
            }
            KillTimer(hWnd, 1U);
            sub_428E();
            sub_3119((WORD) 0);
            sub_44ED();
            DeleteObject(word_CA4A);
            DragAcceptFiles(hWnd, FALSE);
            PostQuitMessage(0);
            break;
        default:
            break;
    }
    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

/* Window procedure (sub). */
LRESULT CALLBACK sub_2699(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    LPWINDOWPOS var_4;
    switch (uMsg) {
        case WM_CREATE:
            if (!sub_9200(hWnd)) {
                DestroyWindow(hWnd);
                return 1;
            }
            SetTimer(hWnd, 1U, 108U, NULL);
            break;
        case WM_WINDOWPOSCHANGING:
            var_4 = (LPWINDOWPOS) lParam;
            var_4->flags |= SWP_NOCOPYBITS;
            word_A7B4 = 1;
            return 0;
        case WM_WINDOWPOSCHANGED:
            return 0;
        case WM_TIMER:
            word_A7B4 = 0;
            fade_out(hWnd);
            if (!sub_9A49(hWnd)) {
                word_CA3C = 1;
                sub_4CE1();
            }
            return 0;
        case WM_PAINT:
            if (word_A7B4 != 0) {
                word_A7B4 = 0;
                ValidateRect(hWnd, NULL);
                return 0;
            }
            sub_93DF(hWnd);
            ValidateRect(hWnd, NULL);
            return 0;
        case WM_ERASEBKGND:
            return 0;
        case WM_DESTROY:
            sub_930F(0);
            KillTimer(hWnd, 1U);
            break;
        default:
            break;
    }
    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

/* Configuration window callback. */
BOOL CALLBACK sub_27FF(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg) {
        case WM_INITDIALOG:
            SendDlgItemMessage(hDlg, 1001, BM_SETCHECK, (WPARAM) word_C0AC, 0);
            SendDlgItemMessage(hDlg, 1002, BM_SETCHECK, (WPARAM) word_CA5A, 0);
            SendDlgItemMessage(hDlg, 1003, BM_SETCHECK, (WPARAM) word_C0B6, 0);
            SendDlgItemMessage(hDlg, 1004, BM_SETCHECK, (WPARAM) word_CA42, 0);
            return TRUE;
        case WM_COMMAND:
            if (wParam == IDRETRY) { //unused
                WinHelp(hDlg, "Scmpoo16.hlp", HELP_INDEX, 0L);
                return TRUE;
            }
            if (wParam == IDOK || wParam == DEBUG) {
                word_C0AC = IsDlgButtonChecked(hDlg, 1001);
                word_CA5A = IsDlgButtonChecked(hDlg, 1002);
                word_C0B6 = IsDlgButtonChecked(hDlg, 1003);
                word_CA42 = IsDlgButtonChecked(hDlg, 1004);
                sub_2FF8();
            }
            if (wParam == DEBUG) { //secret menu
                EndDialog(hDlg, (int) wParam);
                FARPROC proc = MakeProcInstance((FARPROC) debug_window, word_CA58);
                DialogBox(word_CA58, MAKEINTRESOURCE(108), hDlg, (DLGPROC) proc);
                FreeProcInstance(proc);
                return FALSE;
            }
            if (wParam == IDABORT) {
                DestroyWindow(dude);
            }
            if (wParam == IDOK || wParam == IDCANCEL || wParam == IDABORT) {
                EndDialog(hDlg, (int) wParam);
            }
            break;
        default:
            break;
    }
    return FALSE;
}

/* Debug window callback. */
BOOL CALLBACK debug_window(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_INITDIALOG:
            CheckRadioButton(hDlg, 1002, 1019, 1002);
            return TRUE;
        case WM_COMMAND:
            if (wParam == IDOK || wParam == IDCANCEL) {
                EndDialog(hDlg, (int) wParam);
            }
            if (((wParam >= 1002 && wParam <= 1031) || (wParam == 1036 /* roll animation*/ )) && IsDlgButtonChecked(hDlg, (int) wParam) != 0U) {
                set_state_debug(wParam - 1002); //buttons to states
            }
            switch (wParam) {
                case 1032:
                    sub_91CD(0, -20);
                    break;
                case 1033:
                    sub_91CD(0, 20);
                    break;
                case 1034:
                    sub_91CD(-20, 0);
                    break;
                case 1035:
                    sub_91CD(20, 0);
                    break;
                default:
                    break;
            }
            break;
        default:
            break;
    }
    return FALSE;
}

/* Create subwindow. */
void sub_2A21(void)
{
    if (instances[MAX_INSTANCES-1] != NULL) {
        return;
    }
#ifdef _WIN32
    /* Set the visible window to be owned by the hidden top-level window. */
    instances[MAX_INSTANCES-1] = CreateWindowEx(0L, "ScreenMatePooSub", "ScreenMate Sub", WS_POPUP, 0, 0, 0, 0, ownerwindow, NULL, word_CA58, NULL);
#else
    instances[MAX_INSTANCES-1] = CreateWindowEx(0L, "ScreenMatePooSub", "ScreenMate Sub", WS_POPUP, 0, 0, 0, 0, NULL, NULL, word_CA58, NULL);
#endif
    if (instances[MAX_INSTANCES-1] != NULL) {
        ShowWindow(instances[MAX_INSTANCES-1], SW_SHOWNA);
        UpdateWindow(instances[MAX_INSTANCES-1]);
    } else {
        word_CA3C = 1;
        sub_4CE1();
    }
}

/* Destroy subwindow. */
void destroy_subwindow(void)
{
    if (instances[MAX_INSTANCES-1] != NULL) {
        DestroyWindow(instances[MAX_INSTANCES-1]);
        instances[MAX_INSTANCES-1] = NULL;
    }
}

/* Place window to topmost position. */
void place_window_as_top(HWND arg_0)
{
    if (word_CA3E == 0) {
        SetWindowPos(arg_0, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_NOACTIVATE);
        SetWindowPos(arg_0, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_NOACTIVATE);
    }
}

/* Place a window on top of another. */
void sub_2B01(HWND arg_0, HWND arg_2)
{
    if (word_CA3E == 0) {
        SetWindowPos(arg_0, arg_2, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_NOACTIVATE);
    }
}

/* Load resource image, generate (flipped) colour and mask images, store handles into sprite info structure. */
BOOL sub_2B30(HDC arg_0, spriteinfo * arg_2, int arg_4, int arg_6)
{
    void FAR * var_4;
    void FAR * var_8;
    void FAR * var_C;
    void FAR * var_10;
    void FAR * var_14;
    HGLOBAL var_16;
    HGLOBAL var_18;
    HGLOBAL var_1A;
    var_16 = NULL;
    var_18 = NULL;
    var_1A = NULL;
    var_16 = LoadResource(word_CA58, FindResource(word_CA58, MAKEINTRESOURCE(arg_4), RT_BITMAP));
    var_C = LockResource(var_16);
    if (var_16 == NULL) {
        return FALSE;
    }
    arg_2->x = 0;
    arg_2->y = 0;
    arg_2->width = *(int FAR *)((BYTE FAR *)var_C + 4);
    arg_2->height = *(int FAR *)((BYTE FAR *)var_C + 8);
    if (arg_6 < 0) {
        var_18 = GlobalAlloc(GMEM_MOVEABLE, 65535);
        if (var_18 == NULL) {
            goto loc_2EB2;
        }
        var_10 = GlobalLock(var_18);
        if (arg_6 < -1) {
            var_1A = GlobalAlloc(GMEM_MOVEABLE, 65535);
            if (var_1A == NULL) {
                goto loc_2EB2;
            }
            var_14 = GlobalLock(var_1A);
            sub_230(var_14, var_C);
            if (arg_6 == -2) {
                sub_1A16(var_10, var_14, 2U);
            } else {
                sub_1A16(var_10, var_14, 1U);
            }
            GlobalFree(var_1A);
            var_1A = NULL;
        } else {
            sub_230(var_10, var_C);
        }
        var_4 = (void FAR *)((BYTE FAR *)var_10 + *(WORD FAR *)var_10 + sub_155A(var_10));
        var_8 = (void FAR *)((BYTE FAR *)var_10 + *(WORD FAR *)var_10 + sub_1945(var_10) * sizeof(RGBQUAD));
        *(DWORD FAR *)var_8 = 0;
        arg_2->bitmaps[0] = CreateDIBitmap(arg_0, var_10, CBM_INIT, var_4, var_10, DIB_RGB_COLORS);
        if (arg_2->bitmaps[0] == NULL) {
            goto loc_2EB2;
        }
        sub_10(var_10, var_10);
        var_4 = (void FAR *)((BYTE FAR *)var_10 + *(WORD FAR *)var_10 + sub_155A(var_10));
        arg_2->bitmaps[1] = CreateDIBitmap(arg_0, var_10, CBM_INIT, var_4, var_10, DIB_RGB_COLORS);
        if (arg_2->bitmaps[1] == NULL) {
            goto loc_2EB2;
        }
        FreeResource(var_16);
        var_16 = NULL;
        GlobalFree(var_18);
        var_18 = NULL;
        return TRUE;
    } else {
        var_4 = (void FAR *)((BYTE FAR *)var_C + *(WORD FAR *)var_C + sub_155A(var_C));
        arg_2->bitmaps[0] = CreateDIBitmap(arg_0, var_C, CBM_INIT, var_4, var_C, DIB_RGB_COLORS);
        if (arg_2->bitmaps[0] == NULL) {
            goto loc_2EB2;
        }
        FreeResource(var_16);
        var_16 = NULL;
        if (arg_6 == 0) {
            arg_2->bitmaps[1] = NULL;
            return TRUE;
        }
        var_16 = LoadResource(word_CA58, FindResource(word_CA58, MAKEINTRESOURCE(arg_6), RT_BITMAP));
        var_C = LockResource(var_16);
        if (var_16 == NULL) {
            goto loc_2EB2;
        }
        arg_2->x = 0;
        arg_2->y = 0;
        arg_2->width = *(int FAR *)((BYTE FAR *)var_C + 4);
        arg_2->height = *(int FAR *)((BYTE FAR *)var_C + 8);
        var_4 = (void FAR *)((BYTE FAR *)var_C + *(WORD FAR *)var_C + sub_155A(var_C));
        arg_2->bitmaps[1] = CreateDIBitmap(arg_0, var_C, CBM_INIT, var_4, var_C, DIB_RGB_COLORS);
        if (arg_2->bitmaps[1] == NULL) {
            goto loc_2EB2;
        }
        FreeResource(var_16);
        return TRUE;
    }
loc_2EB2:
    if (var_16 != NULL) {
        FreeResource(var_16);
    }
    if (var_18 != NULL) {
        GlobalFree(var_18);
    }
    if (var_1A != NULL) {
        GlobalFree(var_1A);
    }
    return FALSE;
}

/* Release sprite images. */
void sub_2EEC(spriteinfo * arg_0)
{
    if (arg_0->bitmaps[0] != NULL) {
        DeleteObject(arg_0->bitmaps[0]);
    }
    if (arg_0->bitmaps[1] != NULL) {
        DeleteObject(arg_0->bitmaps[1]);
    }
    arg_0->bitmaps[0] = NULL;
    arg_0->bitmaps[1] = NULL;
}

/* Read configuration from file. */
void sub_2F36(void)
{
    word_CA78 = 1;
    word_CA3E = 0;
    word_C0AC = 0U;
    word_CA5A = 0U;
    word_CA5A = GetPrivateProfileInt("Stray", "Sound", 0, "scmate.ini");
    word_C0AC = GetPrivateProfileInt("Stray", "Alarm", 0, "scmate.ini");
    word_C0B6 = GetPrivateProfileInt("Stray", "NoSleep", 0, "scmate.ini");
    word_CA42 = GetPrivateProfileInt("Stray", "GForce", 1, "scmate.ini");
}

/* Save individual configuration to file. */
void sub_2FB7(LPCSTR arg_0, LPCSTR arg_4, UINT arg_8, LPCSTR arg_A)
{
    char var_28[40];
    wsprintf(var_28, "%u", arg_8);
    WritePrivateProfileString(arg_0, arg_4, var_28, arg_A);
}

/* Save configurations to file. */
void sub_2FF8(void)
{
    sub_2FB7("Stray", "Sound", word_CA5A, "scmate.ini");
    sub_2FB7("Stray", "Alarm", word_C0AC, "scmate.ini");
    sub_2FB7("Stray", "NoSleep", word_C0B6, "scmate.ini");
    sub_2FB7("Stray", "GForce", word_CA42, "scmate.ini");
}

/* Initialize bitmaps. */
BOOL sub_306A(HWND arg_0)
{
    HDC var_2;
    var_2 = GetDC(arg_0);
    word_A7B6[0] = CreateCompatibleBitmap(var_2, 100, 100);
    if (word_A7B6[0] == NULL) {
        goto loc_3104;
    }
    word_A7B6[1] = CreateCompatibleBitmap(var_2, 100, 100);
    if (word_A7B6[1] == NULL) {
        goto loc_3104;
    }
    word_A7BA = CreateCompatibleBitmap(var_2, 100, 100);
    if (word_A7BA == NULL) {
        goto loc_3104;
    }
    word_CA4C = 0;
    word_CA4E = 0;
    word_CA50 = GetSystemMetrics(SM_CXSCREEN);
    word_CA52 = GetSystemMetrics(SM_CYSCREEN);
    ReleaseDC(arg_0, var_2);
    return TRUE;
loc_3104:
    ReleaseDC(arg_0, var_2);
    return FALSE;
}

/* Release bitmaps. */
void sub_3119()
{
    DeleteObject(word_A7B6[0]);
    DeleteObject(word_A7B6[1]);
    DeleteObject(word_A7BA);
    if (word_C0B4 != NULL) {
        DeleteObject(word_C0B4);
        word_C0B4 = NULL;
    }
    if (word_CA44 != NULL) {
        DeleteObject(word_CA44);
        word_CA44 = NULL;
    }
    if (word_C0B2 != NULL) {
        DeleteObject(word_C0B2);
        word_C0B2 = NULL;
    }
    if (word_C0B8 != NULL) {
        DeleteObject(word_C0B8);
        word_C0B8 = NULL;
    }
}

/* Update window position and sprite to be actually used. */
void sub_31A8(int arg_0, int arg_2, int arg_4)
{
    word_A7DA = arg_0;
    word_A7DC = arg_2;
    word_A7BC = sprites[arg_4].bitmaps[0];
    word_A7BE = sprites[arg_4].bitmaps[1];
    word_A7C0 = sprites[arg_4].x;
    word_A7C2 = sprites[arg_4].y;
    word_A7DE = sprites[arg_4].width;
    word_A7E0 = sprites[arg_4].height;
}

/* Clear window. */
void sub_3237(HWND arg_0)
{
    word_A7F2 = 0;
    word_A7F4 = 0;
    word_A7F6 = 0;
    word_A7F8 = 0;
    MoveWindow(arg_0, 0, 0, 0, 0, TRUE);
    word_A7D4 = 1;
    word_C0BA = 1;
    word_A7D8 = NULL;
}

/* Render sprite with double buffering. */
void sub_3284(HWND arg_0)
{
    HDC var_2;
    HDC var_4;
    HDC var_6;
    int var_C;
    int var_E;
    int var_10;
    int var_12;
    int var_14;
    int var_16;
    int var_18;
    int var_1A;
    int var_1C;
    int var_1E;
    if (word_A7D2 != 0) {
        return;
    }
    if (word_A7F2 == word_A7DA && word_A7F4 == word_A7DC && word_A7D8 == word_A7BC && word_A7C8 == word_A7C0 && word_CA72 == 0) {
        return;
    }
    word_A7D0 ^= 1;
    var_2 = GetDC(NULL);
    SelectPalette(var_2, word_CA4A, FALSE);
    var_4 = CreateCompatibleDC(var_2);
    var_6 = CreateCompatibleDC(var_2);
    SelectPalette(var_6, word_CA4A, FALSE);
    SelectPalette(var_4, word_CA4A, FALSE);
    var_16 = max(word_A7DA, word_A7F2);
    var_14 = max(word_A7DC, word_A7F4);
    var_12 = min(word_A7DE + word_A7DA, word_A7F6 + word_A7F2) - var_16;
    var_10 = min(word_A7DC + word_A7E0, word_A7F4 + word_A7F8) - var_14;
    if (var_12 <= 0 || var_10 <= 0) {
        word_A7FA = 1;
        if (word_A7D4 != 0) {
            word_A7D4 = 0;
        }
        word_A7E2 = word_A7DA;
        word_A7E4 = word_A7DC;
        word_A7E6 = word_A7DE;
        word_A7E8 = word_A7E0;
        SelectObject(var_4, word_A7B6[word_A7D0]);
        BitBlt(var_4, 0, 0, word_A7E6, word_A7E8, var_2, word_A7E2, word_A7E4, SRCCOPY);
    } else {
        word_A7FA = 0;
        word_A7E2 = min(word_A7DA, word_A7F2);
        word_A7E4 = min(word_A7DC, word_A7F4);
        word_A7E6 = max(word_A7DE + word_A7DA, word_A7F6 + word_A7F2) - word_A7E2;
        word_A7E8 = max(word_A7DC + word_A7E0, word_A7F4 + word_A7F8) - word_A7E4;
        SelectObject(var_4, word_A7B6[word_A7D0]);
        BitBlt(var_4, 0, 0, word_A7E6, word_A7E8, var_2, word_A7E2, word_A7E4, SRCCOPY);
        var_1E = max(word_A7E2, word_A7EA);
        var_1C = max(word_A7E4, word_A7EC);
        var_1A = min(word_A7E6 + word_A7E2, word_A7EE + word_A7EA) - var_1E;
        var_18 = min(word_A7E4 + word_A7E8, word_A7EC + word_A7F0) - var_1C;
        var_16 = max(0, var_1E - word_A7E2);
        var_14 = max(0, var_1C - word_A7E4);
        var_E = max(0, var_1E - word_A7EA);
        var_C = max(0, var_1C - word_A7EC);
        if (var_1A > 0 && var_18 > 0) {
            SelectObject(var_6, word_A7B6[LOBYTE(word_A7D0) - 0xFF & 1]);
            BitBlt(var_4, var_16, var_14, var_1A, var_18, var_6, var_E, var_C, SRCCOPY);
        }
    }
    if (word_A7BC != NULL) {
        SelectObject(var_6, word_A7BA);
        BitBlt(var_6, 0, 0, word_A7E6, word_A7E8, var_4, 0, 0, SRCCOPY);
        var_16 = max(0, word_A7DA - word_A7E2);
        var_14 = max(0, word_A7DC - word_A7E4);
        if (word_A7BE != NULL) {
            SelectObject(var_4, word_A7BE);
            BitBlt(var_6, var_16, var_14, word_A7DE, word_A7E0, var_4, word_A7C0, word_A7C2, SRCAND);
            SelectObject(var_4, word_A7BC);
            BitBlt(var_6, var_16, var_14, word_A7DE, word_A7E0, var_4, word_A7C0, word_A7C2, SRCPAINT);
        } else {
            SelectObject(var_4, word_A7BC);
            BitBlt(var_6, var_16, var_14, word_A7DE, word_A7E0, var_4, word_A7C0, word_A7C2, SRCCOPY);
        }
        word_A7D2 = 1;
        word_CA5E = 1;
        MoveWindow(arg_0, word_A7E2, word_A7E4, word_A7E6, word_A7E8 + word_CA72, TRUE);
        word_CA5E = 0;
    }
    DeleteDC(var_4);
    DeleteDC(var_6);
    word_A7EA = word_A7E2;
    word_A7EC = word_A7E4;
    word_A7EE = word_A7E6;
    word_A7F0 = word_A7E8;
    word_A7F2 = word_A7DA;
    word_A7F4 = word_A7DC;
    word_A7F6 = word_A7DE;
    word_A7F8 = word_A7E0;
    word_A7D8 = word_A7BC;
    word_A7C8 = word_A7C0;
    word_A7CA = word_A7C2;
    ReleaseDC(NULL, var_2);
}

/* Render UFO beam (if any) and present render targets onto window. */
void sub_3717(HWND arg_0)
{
    HDC var_2;
    HDC var_4;
    RECT var_C;
    HDC var_E;
#ifdef WIN32
    HDC screen;
#endif
    if (word_A7D2 == 0) {
        return;
    }
    word_A7D2 = 0;
    var_2 = GetDC(arg_0);
    SelectPalette(var_2, word_CA4A, FALSE);
    RealizePalette(var_2);
    var_4 = CreateCompatibleDC(var_2);
    SelectPalette(var_4, word_CA4A, FALSE);
    SelectObject(var_4, word_A7BA);
    BitBlt(var_2, 0, 0, word_A7E6, word_A7E8, var_4, 0, 0, SRCCOPY);
    if (word_CA72 != 0) {
        if (word_C0B8 == NULL) {
            word_C0B8 = CreateCompatibleBitmap(var_2, 40, word_CA52 * 4 / 5);
            if (word_C0B8 == NULL) {
                goto loc_398B;
            }
        }
        if (word_C0B2 == NULL) {
            word_C0B2 = CreateCompatibleBitmap(var_2, 40, word_CA52 * 4 / 5);
            if (word_C0B2 == NULL) {
                goto loc_398B;
            }
        }
        if (word_CA44 == NULL) {
            word_CA44 = CreateSolidBrush(RGB(255, 255, 0));
        }
        if (word_C0B4 == NULL) {
            word_C0B4 = CreateSolidBrush(RGB(128, 128, 0));
        }
        var_E = CreateCompatibleDC(var_2);
        SelectObject(var_E, word_C0B2);
#ifdef _WIN32
        /* Screen contents with height of only 40 pixels can be captured from window device context on Windows 10. Capture directly from screen instead. */
        screen = GetDC(NULL);
        BitBlt(var_E, 0, 0, 40, word_CA72, screen, word_A7E2, word_A7E4 + 40, SRCCOPY);
        ReleaseDC(NULL, screen);
#else
        BitBlt(var_E, 0, 0, 40, word_CA72, var_2, 0, 40, SRCCOPY);
#endif
        var_C.left = 0;
        var_C.top = 0;
        var_C.right = 40;
        var_C.bottom = word_CA72;
        SelectObject(var_4, word_C0B8);
        FillRect(var_4, &var_C, word_CA44);
        BitBlt(var_E, 0, 0, 40, word_CA72, var_4, 0, 0, SRCAND);
        FillRect(var_4, &var_C, word_C0B4);
        BitBlt(var_E, 0, 0, 40, word_CA72, var_4, 0, 0, SRCPAINT);
        BitBlt(var_2, 0, 40, 40, word_CA72, var_E, 0, 0, SRCCOPY);
        DeleteDC(var_E);
        DeleteDC(var_4);
    } else {
        if (word_C0B4 != NULL) {
            DeleteObject(word_C0B4);
            word_C0B4 = NULL;
        }
        if (word_CA44 != NULL) {
            DeleteObject(word_CA44);
            word_CA44 = NULL;
        }
        if (word_C0B2 != NULL) {
            DeleteObject(word_C0B2);
            word_C0B2 = NULL;
        }
        if (word_C0B8 != NULL) {
            DeleteObject(word_C0B8);
            word_C0B8 = NULL;
        }
        DeleteDC(var_4);
    }
    ReleaseDC(arg_0, var_2);
    return;
loc_398B:
    ReleaseDC(arg_0, var_2);
}

/* Unused. */
void sub_399D(HWND arg_0, int arg_2, int arg_4, int arg_6, int arg_8)
{
    MoveWindow(arg_0, arg_2, arg_4, arg_6, arg_8, FALSE);
    MoveWindow(arg_0, 0, 0, 0, 0, TRUE);
}

/* Find if a window has a match in known instance list. */
BOOL sub_39D6(HWND arg_0)
{
    int var_2;
    if (word_CA40 == 0) {
        return FALSE;
    }
    for (var_2 = 0; var_2 < 8; var_2 += 1) {
        if (instances[var_2] == arg_0 && instances[var_2] != NULL) {
            return TRUE;
        }
    }
    return FALSE;
}

/* Find X-coordinate of possible collision with other instances. Return zero when no collision detected. */
int sub_3A36(int arg_0, int arg_2, int arg_4, int arg_6)
{
    int var_2;
    int var_4;
    int var_6;
    int var_8;
    int var_A;
    for (var_2 = 0; var_2 < 8; var_2 += 1) {
        if (instances[var_2] != NULL) {
            var_4 = (short) GetWindowWord(instances[var_2], 0);
            var_8 = (short) GetWindowWord(instances[var_2], 2);
            var_6 = var_4 + 40;
            var_A = var_8 + 40;
            if (var_6 == 0) {
                continue;
            }
            if ((var_8 <= arg_4 && var_A > arg_4 || var_8 < arg_6 && var_A > arg_6) && var_6 > arg_0 && var_6 <= arg_2 && arg_2 > arg_0) {
                return var_6;
            }
            if ((var_8 <= arg_4 && var_A > arg_4 || var_8 < arg_6 && var_A > arg_6) && var_4 >= arg_2 && var_4 < arg_0 && arg_2 < arg_0) {
                return var_4;
            }
        }
    }
    return 0;
}

/* Populate known instance list by searching for visible windows with name match. */
void sub_3B4C(HWND arg_0)
{
    HWND var_2;
    UINT var_4;
    int var_6;
    int var_8;
    char var_48[64];
    for (var_6 = 0; var_6 < 8; var_6 += 1) {
        instances[var_6] = NULL;
    }
    var_2 = GetDesktopWindow();
    var_4 = GW_CHILD;
    var_6 = 0;
    var_8 = 0;
    while ((var_2 = GetWindow(var_2, var_4)) != NULL && var_6 < 64) {
        var_4 = GW_HWNDNEXT;
        if (var_2 == arg_0) {
            continue;
        }
        if ((GetWindowLong(var_2, GWL_STYLE) & WS_VISIBLE) != 0) {
            GetWindowText(var_2, var_48, 16);
            if (lstrcmp(var_48, "Screen Mate") == 0) {
                instances[var_8] = var_2;
                var_8 += 1;
                if (var_8 > 8) {
                    return;
                }
            }
            var_6 += 1;
        }
    }
    word_CA40 = var_8;
}

/* Populate known instance list by searching for visible windows with name match, then notify other instances of self creation. */
BOOL sub_3C20(HWND arg_0)
{
    HWND var_2;
    UINT var_4;
    int var_6;
    int var_8;
    char var_48[64];
    var_2 = GetDesktopWindow();
    var_4 = GW_CHILD;
    var_6 = 0;
    var_8 = 0;
    while ((var_2 = GetWindow(var_2, var_4)) != NULL && var_6 < 64) {
        var_4 = GW_HWNDNEXT;
        if (var_2 == arg_0) {
            continue;
        }
        if ((GetWindowLong(var_2, GWL_STYLE) & WS_VISIBLE) != 0) {
            GetWindowText(var_2, var_48, 16);
            if (lstrcmp(var_48, "Screen Mate") == 0) {
                instances[var_8] = var_2;
                var_8 += 1;
                if (var_8 >= MAX_INSTANCES) {
                    return FALSE;
                }
            }
            var_6 += 1;
        }
    }
    word_CA40 = var_8;
    word_CA48 = var_8;
    for (var_6 = 0; var_6 < word_CA40; var_6 += 1) {
        SendMessage(instances[var_6], WM_USER, (WPARAM) 1, (LPARAM) arg_0);
    }
    return TRUE;
}

/* Notify other instances of self destruction. */
void sub_3D12(HWND arg_0)
{
    int var_2;
    for (var_2 = 0; var_2 < 8; var_2 += 1) {
        if (instances[var_2] != NULL) {
            SendMessage(instances[var_2], WM_USER, (WPARAM) 2, (LPARAM) arg_0);
        }
    }
}

/* Add window into known instance list. */
void sub_3D5F(HWND arg_0)
{
    int var_2;
    for (var_2 = 0; var_2 < 8; var_2 += 1) {
        if (instances[var_2] == NULL) {
            word_CA40 += 1;
            instances[var_2] = arg_0;
            break;
        }
    }
}

/* Remove window from known instance list. */
void sub_3DA7(HWND arg_0)
{
    int var_2;
    for (var_2 = 0; var_2 < 8; var_2 += 1) {
        if (instances[var_2] == arg_0) {
            word_CA40 -= 1;
            instances[var_2] = NULL;
            break;
        }
    }
}

/* Populate known visible window list. */
void sub_3DF0(void)
{
    HWND var_2;
    UINT var_4;
    int var_6;
    var_2 = GetDesktopWindow();
    var_4 = GW_CHILD;
    var_6 = 0;
    while ((var_2 = GetWindow(var_2, var_4)) != NULL && var_6 < 32) {
        var_4 = GW_HWNDNEXT;
        if (var_2 == dude) {
            continue;
        }
        if ((GetWindowLong(var_2, GWL_STYLE) & WS_VISIBLE) != 0) {
            GetWindowRect(var_2, &stru_C0BC[var_6].rect);
            stru_C0BC[var_6].window = var_2;
            var_6 += 1;
        }
    }
    word_CA74 = var_6;
}

/* Find X-coordinate of possible collision with which visible window. */
int sub_3E7C(HWND * arg_0, int arg_2, int arg_4, int arg_6, int arg_8)
{
    int var_2;
    int var_4;
    RECT var_C;
    for (var_2 = 0; var_2 < word_CA74; var_2 += 1) {
        if (arg_8 > arg_6) {
            if (stru_C0BC[var_2].rect.right >= arg_6 && stru_C0BC[var_2].rect.right < arg_8 && stru_C0BC[var_2].rect.top < arg_2 && stru_C0BC[var_2].rect.bottom > arg_4) {
                for (var_4 = 0; var_4 < var_2; var_4 += 1) {
                    if (stru_C0BC[var_4].rect.top <= arg_2 && stru_C0BC[var_4].rect.bottom >= arg_4 && stru_C0BC[var_4].rect.left <= arg_6 && stru_C0BC[var_4].rect.right >= arg_8) {
                        break;
                    }
                }
                if (var_4 == var_2) {
                    if (IsWindow(stru_C0BC[var_2].window)) {
                        GetWindowRect(stru_C0BC[var_2].window, &var_C);
                        if (stru_C0BC[var_2].rect.right == var_C.right) {
                            *arg_0 = stru_C0BC[var_2].window;
                            return stru_C0BC[var_2].rect.right;
                        }
                    }
                }
            }
        } else {
            if (stru_C0BC[var_2].rect.left <= arg_6 && stru_C0BC[var_2].rect.left > arg_8 && stru_C0BC[var_2].rect.top < arg_2 && stru_C0BC[var_2].rect.bottom > arg_4) {
                for (var_4 = 0; var_4 < var_2; var_4 += 1) {
                    if (stru_C0BC[var_4].rect.top <= arg_2 && stru_C0BC[var_4].rect.bottom >= arg_4 && stru_C0BC[var_4].rect.left <= arg_6 && stru_C0BC[var_4].rect.right >= arg_8) {
                        break;
                    }
                }
                if (var_4 == var_2) {
                    if (IsWindow(stru_C0BC[var_2].window)) {
                        GetWindowRect(stru_C0BC[var_2].window, &var_C);
                        if (stru_C0BC[var_2].rect.left == var_C.left) {
                            *arg_0 = stru_C0BC[var_2].window;
                            return stru_C0BC[var_2].rect.left;
                        }
                    }
                }
            }
        }
    }
    return 0;
}

/* Find Y-coordinate of possible landing top edge of which visible window. */
int sub_408C(HWND * arg_0, int arg_2, int arg_4, int arg_6, int arg_8)
{
    int var_2;
    int var_4;
    for (var_2 = 0; var_2 < word_CA74; var_2 += 1) {
        if (stru_C0BC[var_2].rect.top <= arg_2 && stru_C0BC[var_2].rect.top > arg_4 && stru_C0BC[var_2].rect.left < arg_8 && stru_C0BC[var_2].rect.right > arg_6 && stru_C0BC[var_2].rect.top > 10) {
            for (var_4 = 0; var_4 < var_2; var_4 += 1) {
                if (stru_C0BC[var_4].rect.left <= arg_6 && stru_C0BC[var_4].rect.right >= arg_8 && stru_C0BC[var_4].rect.top <= arg_4 && stru_C0BC[var_4].rect.bottom >= arg_2) {
                    break;
                }
            }
            if (var_4 == var_2) {
                *arg_0 = stru_C0BC[var_2].window;
                return stru_C0BC[var_2].rect.top;
            }
        }
    }
    if (arg_2 >= word_CA52 && arg_4 <= word_CA52) {
        *arg_0 = NULL;
        return word_CA52;
    }
    return 0;
}

/* Get window top Y-coordinate if it is possible to land on the window. */
int sub_419E(HWND arg_0, int arg_2, int arg_4, int arg_6, int arg_8)
{
    RECT var_8;
    if (IsWindow(arg_0)) {
        GetWindowRect(arg_0, &var_8);
        if (var_8.top <= arg_2 && var_8.top > arg_4 && var_8.left < arg_8 && var_8.right > arg_6) {
            return var_8.top;
        }
    }
    if (arg_2 > word_CA52) {
        return -1;
    }
    return 0;
}

/* Play sound by resource ID and additional flags. */
void sub_4210(int arg_0, UINT arg_2, WORD arg_4)
{
    LPCSTR lpszSoundName;
    if (word_A82E != NULL) {
        sndPlaySound(NULL, SND_SYNC);
        GlobalUnlock(word_A82E);
        FreeResource(word_A82E);
        word_A82E = NULL;
    }
    word_A82E = LoadResource(word_CA58, FindResource(word_CA58, MAKEINTRESOURCE(arg_0), "WAVE"));
    lpszSoundName = LockResource(word_A82E);
    sndPlaySound(lpszSoundName, arg_2 | (SND_ASYNC | SND_MEMORY));
}

/* Stop playing sound. */
void sub_428E(void)
{
    sndPlaySound(NULL, SND_SYNC);
}

/* Play sound by name. */
void sub_42AA(LPCSTR lpszSoundName)
{
    sndPlaySound(lpszSoundName, SND_ASYNC);
}

/* Play sound by resource ID and additional flags (when option "Cry" enabled). */
void play_sound(int arg_0, UINT arg_2, WORD arg_4)
{
    if (word_CA5A != 0U) {
        sub_4210(arg_0, arg_2, arg_4);
    }
}

/* Generate sprites from loaded resource images. */
BOOL sub_42F3(HDC arg_0)
{
    int var_2;
    int var_4;
    for (var_2 = 0; var_2 < 16; var_2 += 1) {
        if (stru_9EE2[var_2].resource == 0) {
            break;
        }
        if (!sub_2B30(arg_0, &stru_9EE2[var_2].info, stru_9EE2[var_2].resource, -1)) {
            return FALSE;
        }
        if (!sub_2B30(arg_0, &stru_9FE2[var_2].info, stru_9EE2[var_2].resource, -3)) {
            return FALSE;
        }
        for (var_4 = 0; var_4 < 16; var_4 += 1) {
            sprites[var_2 * 16 + var_4].bitmaps[0] = stru_9EE2[var_2].info.bitmaps[0];
            sprites[var_2 * 16 + var_4].bitmaps[1] = stru_9EE2[var_2].info.bitmaps[1];
            sprites[var_2 * 16 + var_4].width = 40;
            sprites[var_2 * 16 + var_4].height = 40;
            sprites[var_2 * 16 + var_4].x = var_4 * 40;
            sprites[var_2 * 16 + var_4].y = 0;
            sprites[var_2 * 16 + var_4 + 256].bitmaps[0] = stru_9FE2[var_2].info.bitmaps[0];
            sprites[var_2 * 16 + var_4 + 256].bitmaps[1] = stru_9FE2[var_2].info.bitmaps[1];
            sprites[var_2 * 16 + var_4 + 256].width = 40;
            sprites[var_2 * 16 + var_4 + 256].height = 40;
            sprites[var_2 * 16 + var_4 + 256].x = (15 - var_4) * 40;
            sprites[var_2 * 16 + var_4 + 256].y = 0;
        }
    }
    return TRUE;
}

/* Release resource images. */
void sub_44ED(void)
{
    int var_2;
    for (var_2 = 0; var_2 < 16; var_2 += 1) {
        if (stru_9EE2[var_2].resource == 0) {
            break;
        }
        sub_2EEC(&stru_9EE2[var_2].info);
        if (stru_9EE2[var_2].flags == 1) {
            sub_2EEC(&stru_9FE2[var_2].info);
        }
    }
}

/* Turn around when approaching screen border or otherwise with 1/20 probability.  */
void sub_4559(void) {
    if (facing_direction > 0 && x_curr < 0) {
        poo_state = 24;
    }
    if (facing_direction < 0 && word_CA50 - sprites[sprite_index].width < x_curr) {
        poo_state = 24;
    }
    if (facing_direction > 0 && word_CA50 - sprites[sprite_index].width > x_curr && rand() % 20 == 0) {
        poo_state = 24;
    }
    if (facing_direction < 0 && x_curr > 0 && rand() % 20 == 0) {
        poo_state = 24;
    }
}

/* Flag-controlled collision and turn around. */
void sub_4614(BOOL arg_0) {
    if (word_A7FC == 0) {
        if (facing_direction > 0 && x_curr < 0) {
            poo_state = 30;
        }
        if (facing_direction < 0 && word_CA50 - sprites[sprite_index].width < x_curr) {
            poo_state = 30;
        }
    }
    if (arg_0) {
        if (facing_direction > 0 && word_CA50 - 80 > x_curr && rand() % 20 == 0) {
            poo_state = 24;
        }
        if (facing_direction < 0 && x_curr > 40 && rand() % 20 == 0) {
            poo_state = 24;
        }
    }
}

/* Switch to standing sprite after certain frames. */
void sub_46D2(void) {
    if (animation_frame-- <= 0) {
        poo_state = 42;
    }
}

/* Process chime. */
void sub_46F7(void)
{
    struct tm * var_2;
    time_t var_6;
    DWORD var_A;
    int var_C;
    if (word_A832 != 0) {
        var_A = GetTickCount();
        if (dword_A834 + 1000 < var_A) {
            dword_A834 = var_A;
            word_A832 -= 1;
            if (word_A832 != 0) {
                sub_4210(108, 0U, 0);
            } else if (word_CA76 != 0) {
                poo_state = 113;
            } else {
                poo_state = 1;
            }
        }
    } else {
        if (word_A838++ < 10) {
            return;
        }
        word_A838 = 0;
        time(&var_6);
        var_2 = localtime(&var_6);
        var_C = var_2->tm_hour % 12;
        if (var_C == 0) {
            var_C = 12;
        }
        if (var_2->tm_min == 0 && var_C != word_A830) {
            destroy_subwindow();
            dword_A834 = 0;
            word_A830 = var_C;
            word_A832 = word_A830 + 1;
            poo_state = 81;
        }
    }
}

/* Update window position and sprite to be painted. */
void update_sprite(int arg_0, int arg_2, int arg_4) {
    SetWindowWord(dude, 0, (short) x_curr);
    SetWindowWord(dude, 2, (short) y_curr);
    if (arg_4 >= 9 && arg_4 <= 14) {
        sub_31A8(arg_0, arg_2, arg_4);
    } else if (facing_direction > 0) {
        sub_31A8(arg_0, arg_2, arg_4);
    } else {
        sub_31A8(arg_0, arg_2, arg_4 + 256);
    }
}

/* Update window position and sprite to be painted (sub). */
void update_sprite_sub(int arg_0, int arg_2, int arg_4) {
    if (arg_4 >= 9 && arg_4 <= 14) {
        sub_9350(arg_0, arg_2, arg_4);
    } else if (facing_direction_sub > 0) {
        sub_9350(arg_0, arg_2, arg_4);
    } else {
        sub_9350(arg_0, arg_2, arg_4 + 256);
    }
}

/* Return TRUE if the window handle is NULL or if the handle contains an existing window. */
BOOL sub_48F3(HWND arg_0) {
    if (arg_0 == NULL) {
        return TRUE;
    } else {
        return IsWindow(arg_0);
    }
}

/* Get window rect. If the window handle is NULL, get a screen rect located right under the current screen. */
void sub_491D(HWND arg_0, LPRECT arg_2)
{
    if (arg_0 == NULL) {
        arg_2->left = 0;
        arg_2->right = word_CA50;
        arg_2->top = word_CA52;
        arg_2->bottom = word_CA52 * 2;
    } else {
        GetWindowRect(arg_0, arg_2);
    }
}

/* Process when out of screen view or at different positions on top of visible window. */
void sub_496F(int arg_0) {
    RECT var_8;
    if (word_A7FC == 0) {
        return;
    }
    if (word_A81C != NULL) {
        if (!sub_48F3(word_A81C)) {
            if (arg_0 == 2) {
                poo_state = 94;
            } else {
                poo_state = 102;
            }
            return;
        }
        sub_491D(word_A81C, &var_8);
        if (var_8.top > stru_A81E.top || x_curr + 40 < var_8.left || var_8.right < x_curr) {
            if (arg_0 == 2) {
                poo_state = 94;
            } else {
                poo_state = 102;
            }
            return;
        }
        if (var_8.top < stru_A81E.top) {
            y_curr = var_8.top - sprites[sprite_index].height;
            stru_A81E.top = var_8.top;
            stru_A81E.bottom = var_8.bottom;
            stru_A81E.left = var_8.left;
            stru_A81E.right = var_8.right;
            update_sprite(x_curr, y_curr, sprite_index);
            return;
        }
        if (arg_0 == 1) {
            if (x_curr + 8 < var_8.left && facing_direction > 0) {
                poo_state = 105;
                x_curr = var_8.left - 10;
                return;
            }
            if (x_curr + 32 >= var_8.right && facing_direction < 0) {
                poo_state = 105;
                x_curr = var_8.right - 30;
                return;
            }
            if (rand() % 20 - 1 == 0 && word_CA52 - y_curr > 100) {
                poo_state = 104;
                return;
            }
        }
        if (arg_0 == 2) {
            if (x_curr + 32 < var_8.left || x_curr + 8 > var_8.right) {
                poo_state = 94;
                return;
            }
        }
    }
    if (sprites[sprite_index].width + x_curr < 0 || x_curr > word_CA50) {
        poo_state = 0;
        return;
    }
}

/* Process when climbing up side of a window. */
void sub_4B3B(void) {
    RECT var_8;
    if (word_A7FC == 0) {
        return;
    }
    if (word_A81C != NULL) {
        if (!sub_48F3(word_A81C)) {
            poo_state = 102;
            return;
        }
        sub_491D(word_A81C, &var_8);
        if (var_8.right < stru_A81E.right && facing_direction > 0 || var_8.left > stru_A81E.left && facing_direction < 0) {
            poo_state = 102;
            return;
        }
        if (var_8.right > stru_A81E.right && facing_direction > 0 || var_8.left < stru_A81E.left && facing_direction < 0) {
            if (facing_direction > 0) {
                x_curr = var_8.right + 10;
            } else {
                x_curr = var_8.left - 50;
            }
            update_sprite(x_curr, y_curr, sprite_index);
            poo_state = 102;
            return;
        }
    }
}

/* Detect collision with other instances, action controlled by a flag. */
void sub_4C21(int arg_0, int arg_2, int arg_4) {
    if (arg_2 < arg_0) {
        arg_0 += 40;
        arg_2 = arg_0 - 80;
    } else {
        arg_2 = arg_0 + 80;
    }
    if (sub_3A36(arg_0, arg_2, y_curr, y_curr + 40) != 0) {
        if (arg_4 == 1) {
            poo_state = 24;
        }
        if (arg_4 == 2) {
            poo_state = 30;
        }
    }
}

/* Detect collision with other instances and find X-coordinate. */
int sub_4C91(int arg_0, int arg_2)
{
    if (arg_2 < arg_0) {
        arg_0 += 40;
        arg_2 = arg_0 - 80;
    } else {
        arg_2 = arg_0 + 80;
    }
    return sub_3A36(arg_0, arg_2, y_curr, y_curr + 40);
}

/* Reinitialize state. */
void sub_4CE1(void)
{
    poo_state = 0;
}

/* Process state change on each timer expiration. */
void sub_4CF8(void)
{
    int var_2;
    int var_4;
    HWND var_6;
    HWND var_8;
    RECT var_10;
    POINT var_14;
    if (word_A84A++ > 100) {
        sub_3B4C(dude);
        word_A84A = 0;
    }
    if (word_C0AC != 0) {
        sub_46F7();
    }
loc_4D33:
    switch (poo_state) {
        case 0:
            word_A7FC = 0;
            srand((unsigned int) GetTickCount());
            x_curr = -80;
            y_curr = -80;
            poo_state = 1;
        case 1:
            word_A844 = 0;
            if (word_CA42 != 0U) {
                poo_state = 2;
                goto loc_4D33;
            }
            word_CA72 = 0;
            destroy_subwindow();
            if (word_CA54 != 0) {
                poo_state = word_CA54;
                word_CA54 = 0;
                break;
            }
            if (rand() % 20 == 5 && word_A7FC == 0) {
                poo_state = 85;
                break;
            }
            if (rand() % 40 == 5 && word_A7FC == 0 && word_CA3C == 0) {
                poo_state = 4;
                break;
            }
            poo_state = word_A15A[rand() % 80];
            if (x_curr > word_CA50 || x_curr < -40 || y_curr < -40 || y_curr > word_CA52) {
                if ((rand() & 1) == 0) {
                    facing_direction = 1;
                    x_curr = word_CA50 + word_CA4C;
                    y_curr = rand() % (word_CA52 - 64) + word_CA4E;
                } else {
                    facing_direction = -1;
                    x_curr = -40;
                    y_curr = rand() % (word_CA52 - 64) + word_CA4E;
                }
                poo_state = 11;
            }
            break;
        case 2:
            word_A7FC = 1;
            word_CA72 = 0;
            destroy_subwindow();
            if (word_CA54 != 0) {
                poo_state = word_CA54;
                word_CA54 = 0;
                break;
            }
            poo_state = word_A1FA[rand() % 80];
            if (x_curr > word_CA50 || x_curr < -40 || y_curr < -40 || y_curr > word_CA52) {
                if (rand() % 10 == 0 && word_CA3C == 0) {
                    poo_state = 6;
                    break;
                }
                word_A81C = GetActiveWindow();
                if (word_A81C == dude || word_A81C == instances[MAX_INSTANCES-1] || word_A81C == NULL || sub_39D6(word_A81C)) {
                    poo_state = 3;
                    goto loc_4D33;
                }
                sub_491D(word_A81C, &stru_A81E);
                if (stru_A81E.top < 10) {
                    poo_state = 3;
                    goto loc_4D33;
                }
                x_curr = (rand() % stru_A81E.right - stru_A81E.left) / 3 + (stru_A81E.right - stru_A81E.left) / 2 +
                         stru_A81E.left - 20;
                y_curr = -40;
                word_A840 = 0;
                y_speed = 0;
                word_A808 = 0;
                word_A842 = rand() % 2;
                poo_state = 92;
                if (rand() % 3 == 0) {
                    poo_state = 3;
                    goto loc_4D33;
                }
            }
            break;
        case 3:
            word_A7FC = 1;
            x_curr = rand() % (word_CA50 - 40);
            y_curr = -(rand() % 20 - (-40));
            word_A840 = 0;
            y_speed = 0;
            word_A808 = 0;
            word_A842 = rand() % 2;
            if (rand() % 3 == 0) {
                place_window_as_top(dude);
            }
            poo_state = 97;
            break;
        case 153:
            break;
        case 154:
            break;
        case 4:
            if (word_CA50 / 2 - 20 > x_curr) {
                facing_direction = 1;
            } else {
                facing_direction = -1;
            }
            sprite_index = 4;
            update_sprite(x_curr, y_curr, sprite_index);
            poo_state = 5;
            break;
        case 5:
            if (frame_period_counter++ < 1) {
                break;
            }
            frame_period_counter = 0;
            x_curr -= facing_direction * 16;
            sprite_index = sprite_index == 4 ? 5 : 4;
            update_sprite(x_curr, y_curr, sprite_index);
            if (x_curr < -40 || x_curr > word_CA50) {
                poo_state = 6;
            }
            break;
        case 6:
            poo_state = word_A29A[rand() % 8];
            break;
        case 7:
            word_A7FE = 0;
            if ((rand() & 1) == 0) {
                word_A7FE = 1;
            }
            if (word_A7FE != 0) {
                sub_3DF0();
            }
            sprite_index = 4;
            update_sprite(x_curr, y_curr, sprite_index);
            animation_frame = rand() % 10 + 10;
            poo_state = 8;
            break;
        case 8:
            if (frame_period_counter++ < 1) {
                break;
            }
            frame_period_counter = 0;
            if (word_A7FE != 0) {
                if (facing_direction > 0) {
                    var_2 = sub_3E7C(&var_6, y_curr, y_curr + 40, -(facing_direction * 16 - x_curr), x_curr);
                } else {
                    var_2 = sub_3E7C(&var_6, y_curr, y_curr + 40, -(facing_direction * 16 - x_curr) + 40, x_curr + 40);
                }
                if (var_2 != 0) {
                    if (facing_direction > 0) {
                        x_curr = var_2;
                    } else {
                        x_curr = var_2 - 40;
                    }
                    update_sprite(x_curr, y_curr, sprite_index);
                    poo_state = 30;
                    break;
                }
            }
            if (word_A82C == 0) {
                x_curr -= facing_direction * 16;
            }
            sprite_index = sprite_index == 4 ? 5 : 4;
            update_sprite(x_curr, y_curr, sprite_index);
            if (rand() % 50 == 0 && word_A7FC != 0) {
                poo_state = 9;
            }
            sub_4614(TRUE);
            sub_46D2();
            sub_496F(2);
            sub_4C21(-(facing_direction * 16 - x_curr), facing_direction * 16 + x_curr, 2);
            break;
        case 9:
            y_speed = -11;
            word_A808 = -(facing_direction * 8);
            word_A80C = y_curr;
            poo_state = 10;
        case 10:
            x_curr += word_A808;
            y_curr += y_speed;
            y_speed += 2;
            if (y_speed >= -1 && y_speed <= 1) {
                sprite_index = 23;
            } else if (y_speed < -1) {
                sprite_index = 30;
            } else {
                sprite_index = 24;
            }
            if (word_A80C <= y_curr) {
                y_curr = word_A80C;
                poo_state = 7;
            }
            update_sprite(x_curr, y_curr, sprite_index);
            sub_4614(FALSE);
            sub_4C21(word_A808 + x_curr, x_curr - word_A808, 2);
            if (poo_state == 30 && word_A80C != y_curr) {
                word_A844 = y_curr - word_A80C;
            }
            break;
        case 11:
            word_A7FE = 0;
            if ((word_A7FC & !(rand() & 1)) != 0) {
                word_A7FE = 1;
            }
            if (word_A7FE != 0) {
                sub_3DF0();
            }
            animation_frame = rand() % 10 + 10;
            sprite_index = 2;
            update_sprite(x_curr, y_curr, sprite_index);
            poo_state = 12;
            break;
        case 12:
            if (frame_period_counter++ < 1) {
                break;
            }
            frame_period_counter = 0;
            if (word_A7FE != 0) {
                if (facing_direction > 0) {
                    var_2 = sub_3E7C(&var_6, y_curr, y_curr + 40, -(facing_direction * 6 - x_curr), x_curr);
                } else {
                    var_2 = sub_3E7C(&var_6, y_curr, y_curr + 40, -(facing_direction * 6 - x_curr) + 40, x_curr + 40);
                }
                if (var_2 != 0) {
                    if (facing_direction > 0) {
                        x_curr = var_2;
                    } else {
                        x_curr = var_2 - 40;
                    }
                    word_A81C = var_6;
                    sub_491D(word_A81C, &stru_A81E);
                    word_A83E = stru_A81E.top - 12;
                    word_A7FC = 1;
                    word_A83C = x_curr;
                    sprite_index = 30;
                    sub_2B01(dude, word_A81C);
                    poo_state = 89;
                    break;
                }
            }
            if (word_A82C == 0) {
                x_curr -= facing_direction * 6;
            }
            sprite_index = sprite_index == 2 ? 3 : 2;
            update_sprite(x_curr, y_curr, sprite_index);
            sub_4559();
            sub_46D2();
            sub_496F(1);
            sub_4C21(-(facing_direction * 6 - x_curr), facing_direction * 6 + x_curr, 1);
            break;
        case 13:
            word_A82A = rand() % 2;
            animation_frame = (rand() % 32) + 32;
            if (word_A82A != 0) {
                sprite_index = 88;
            } else {
                sprite_index = 86;
            }
            update_sprite(x_curr, y_curr, sprite_index);
            poo_state = 14;
            break;
        case 14:
            if (frame_period_counter++ < 2) {
                break;
            }
            frame_period_counter = 0;
            if (word_A82C == 0) {
                x_curr -= facing_direction * 6;
            }
            if (word_A82A != 0) {
                sprite_index = sprite_index == 88 ? 89 : 88;
            } else {
                sprite_index = sprite_index == 86 ? 87 : 86;
            }
            update_sprite(x_curr, y_curr, sprite_index);
            sub_4559();
            sub_46D2();
            sub_496F(1);
            break;
        case 15:
            word_A82A = rand() % 2;
            animation_frame = rand() % 3 + 3;
            if (word_A82A != 0) {
                sprite_index = 54;
            } else {
                sprite_index = 52;
            }
            update_sprite(x_curr, y_curr, sprite_index);
            poo_state = 16;
            break;
        case 16:
            if (frame_period_counter++ < 3) {
                break;
            }
            frame_period_counter = 0;
            if (word_A82A != 0) {
                sprite_index = sprite_index == 54 ? 55 : 54;
            } else {
                sprite_index = sprite_index == 52 ? 53 : 52;
            }
            update_sprite(x_curr, y_curr, sprite_index);
            sub_4559();
            sub_46D2();
            sub_496F(0);
            break;
        case 17:
            sprite_index = 6;
            update_sprite(x_curr, y_curr, sprite_index);
            poo_state = 18;
            break;
        case 18:
            if (frame_period_counter++ < 1) {
                break;
            }
            frame_period_counter = 0;
            sprite_index += 1;
            update_sprite(x_curr, y_curr, sprite_index);
            if (sprite_index == 8) {
                sprite_index = 0;
                poo_state = 19;
                animation_frame = rand() % 8 + 8;
            }
            sub_496F(0);
            break;
        case 19:
            if (frame_period_counter++ < 4) {
                break;
            }
            frame_period_counter = 0;
            sprite_index = sprite_index == 0 ? 1 : 0;
            update_sprite(x_curr, y_curr, sprite_index);
            sub_46D2();
            sub_496F(0);
            break;
        case 20:
            word_A82A = rand() % 3;
            if (word_A82A == 0) {
                sprite_index = 6;
            } else if (word_A82A == 1) {
                sprite_index = 31;
            } else {
                sprite_index = 73;
            }
            update_sprite(x_curr, y_curr, sprite_index);
            poo_state = 21;
            frame_period_counter = rand() % 15 + rand() % 15;
            sub_496F(0);
            break;
        case 21:
            sub_496F(0);
            if (frame_period_counter-- > 0) {
                break;
            }
            poo_state = 22;
            animation_frame = 0;
            break;
        case 22:
            sprite_index = word_A2B4[word_A82A][animation_frame];
            update_sprite(x_curr, y_curr, sprite_index);
            animation_frame += 1;
            if (animation_frame > 7) {
                poo_state = 23;
                frame_period_counter = rand() % 15 + rand() % 15;
            }
            sub_496F(0);
            break;
        case 23:
            sub_496F(0);
            if (frame_period_counter-- > 0) {
                break;
            }
            poo_state = 1;
            break;
        case 24:
            if (frame_period_counter++ < 1) {
                break;
            }
            frame_period_counter = 0;
            sprite_index = 3;
            update_sprite(x_curr, y_curr, sprite_index);
            if ((rand() & 1) != 0) {
                word_A82A = 0;
            } else {
                word_A82A = 1;
            }
            poo_state = 25;
            animation_frame = 0;
            break;
        case 25:
            if (frame_period_counter++ < 1) {
                break;
            }
            frame_period_counter = 0;
            if (word_A82A != 0) {
                if (facing_direction > 0) {
                    sprite_index = animation_frame + 9;
                } else {
                    sprite_index = 11 - animation_frame;
                }
            } else {
                if (facing_direction > 0) {
                    sprite_index = animation_frame + 12;
                } else {
                    sprite_index = 14 - animation_frame;
                }
            }
            update_sprite(x_curr, y_curr, sprite_index);
            animation_frame += 1;
            if (animation_frame > 2) {
                facing_direction = -facing_direction;
                poo_state = 26;
            }
            sub_496F(0);
            break;
        case 26:
            if (frame_period_counter++ < 1) {
                break;
            }
            frame_period_counter = 0;
            sprite_index = 3;
            update_sprite(x_curr, y_curr, sprite_index);
            poo_state = 1;
            sub_496F(0);
            break;
        case 27:
            y_speed = -10;
            word_A808 = facing_direction * 8;
            word_A80C = y_curr;
            word_A848 = 0;
            poo_state = 28;
        case 28:
            x_curr += word_A808;
            y_curr += y_speed;
            y_speed += 2;
            sprite_index = word_A324[word_A848];
            word_A848 += 1;
            update_sprite(x_curr, y_curr, sprite_index);
            if (sprite_index == 64) {
                word_A842 = 3;
                poo_state = 99;
                break;
            }
            break;
        case 29:
            frame_period_counter = 0;
            animation_frame = 0;
            word_A82A = 0;
            if ((rand() & 7) == 0) {
                word_A82A = 1;
            }
            if (rand() % 5 == 0) {
                word_A82A = 2;
            }
            poo_state = 32;
            if (word_A82A != 0) {
                poo_state = 34;
            }
            goto loc_4D33;
        case 30:
            if (word_A7FC != 0) {
                poo_state = 27;
                goto loc_4D33;
            } else {
                poo_state = 24;
                goto loc_4D33;
            }
            frame_period_counter = 0;
            animation_frame = 0;
            word_A82A = 0;
            if ((rand() & 7) == 0) {
                word_A82A = 1;
            }
            if (rand() % 5 == 0) {
                word_A82A = 2;
            }
            poo_state = 31;
        case 31:
            sub_4C21(facing_direction * 10 + x_curr, x_curr, 2);
            if (poo_state == 30) {
                if (animation_frame != 0) {
                    word_A844 -= word_A324[animation_frame + 9];
                }
                break;
            }
            sprite_index = word_A324[animation_frame];
            update_sprite(x_curr, y_curr - word_A324[animation_frame + 10], sprite_index);
            animation_frame += 1;
            if (word_A82A != 0 && sprite_index == 66) {
                if (word_A844 != 0) {
                    y_curr -= word_A844;
                    x_curr += facing_direction * 10;
                    update_sprite(x_curr, y_curr, sprite_index);
                }
                word_A846 = 3;
                poo_state = 34;
                break;
            }
            if (animation_frame > 8) {
                poo_state = 32;
                break;
            }
            x_curr += facing_direction * 10;
            break;
        case 32:
            sub_496F(0);
            if (frame_period_counter++ < 8) {
                break;
            }
            frame_period_counter = 0;
            facing_direction = -facing_direction;
            sprite_index = 48;
            update_sprite(x_curr, y_curr, sprite_index);
            poo_state = 33;
            break;
        case 33:
            sub_496F(0);
            if (frame_period_counter++ < 15) {
                break;
            }
            frame_period_counter = 0;
            poo_state = 1;
            break;
        case 34:
            x_curr += facing_direction * 8;
            if (sprite_index >= 70 || sprite_index <= 62) {
                sprite_index = 63;
            } else {
                sprite_index += 1; // roll
            }
            update_sprite(x_curr, y_curr, sprite_index);
            if (word_A82A == 2 && sprite_index == 70) {
                poo_state = 69;
                break;
            }
            if (x_curr > word_CA50 || x_curr < -40) {
                poo_state = 1;
            }
            sub_4C21(facing_direction * 8 + x_curr, -(facing_direction * 8 - x_curr), 2);
            if (poo_state == 30) {
                if (word_A846-- > 0) {
                    facing_direction = -facing_direction;
                    poo_state = 34;
                } else {
                    poo_state = 34;
                }
            }
            sub_496F(1);
            break;
        case 35: // pick nose
            animation_frame = 0;
            poo_state = 37;
        case 36:
            break;
        case 37:
            if (frame_period_counter++ < 2) {
                break;
            }
            frame_period_counter = 0;
            sprite_index = animation_nose_pick[animation_frame++];
            if (sprite_index == 0) {
                poo_state = 1;
                break;
            }
            update_sprite(x_curr, y_curr, sprite_index);
            sub_496F(0);
            break;
        case 38:
        case 39:
            poo_state = 1;
            break;
        case 40:
            if (frame_period_counter++ < 1) {
                break;
            }
            frame_period_counter = 0;
            update_sprite(x_curr, y_curr, sprite_index);
            if (--sprite_index < 103) {
                animation_frame = 0;
                poo_state = 41;
                break;
            }
            sub_496F(0);
            break;
        case 41:
            if (frame_period_counter++ < 1) {
                break;
            }
            frame_period_counter = 0;
            if (facing_direction > 0) {
                sprite_index = 13 - animation_frame;
            } else {
                sprite_index = animation_frame + 13;
            }
            update_sprite(x_curr, y_curr, sprite_index);
            animation_frame += 1;
            if (animation_frame > 1) {
                poo_state = 42;
            }
            sub_496F(0);
            break;
        case 42:
            sub_496F(0);
            if (frame_period_counter++ < 1) {
                break;
            }
            frame_period_counter = 0;
            sprite_index = 3;
            update_sprite(x_curr, y_curr, sprite_index);
            poo_state = 1;
            animation_frame = 0;
            break;
        case 43:
            //play_sound(109, 0U, 0);
            animation_frame = 0;
            frame_period_counter = 0;
            poo_state = 44;
        case 44:
            sprite_index = animation_snap[animation_frame++];
            if (sprite_index == 0) {
                poo_state = 1;
                break;
            }
            if(animation_frame == 12)
                play_sound(112, 0U, 0);
            update_sprite(x_curr, y_curr, sprite_index);
            sub_496F(0);
            break;
        case 45:
            play_sound(108, 0U, 0);
            animation_frame = 0;
            poo_state = 46;
        case 46:
            if (frame_period_counter++ < 0) {
                break;
            }
            frame_period_counter = 0;
            sprite_index = animation_burp[animation_frame];
            animation_frame += 1;
            if (sprite_index == 0) {
                poo_state = 1;
                break;
            }
            update_sprite(x_curr, y_curr, sprite_index);
            sub_496F(0);
            break;
        case 47:
            animation_frame = 0;
            poo_state = 48;
        case 48:
            if (frame_period_counter++ < 5) {
                break;
            }
            frame_period_counter = 0;
            if(animation_frame == 2) {
                play_sound(110, 0U, 0);
            }
            sprite_index = animation_thicc[animation_frame++ == 4 ? 1 : 0];

            if (animation_frame == 9) {
                poo_state = 1;
                break;
            }
            update_sprite(x_curr, y_curr, sprite_index);
            sub_496F(0);
            break;
        case 49:
            animation_frame = 0;
            poo_state = 50;
        case 50:
            if (frame_period_counter++ < 1) {
                break;
            }
            frame_period_counter = 0;
            sprite_index = word_A38C[animation_frame];
            animation_frame += 1;
            if (sprite_index == 0) {
                poo_state = 1;
                break;
            }
            update_sprite(x_curr, y_curr, sprite_index);
            sub_496F(0);
            break;
        case 51:
            animation_frame = 0;
            poo_state = 52;
        case 52:
            if (frame_period_counter++ < 0) {
                break;
            }
            frame_period_counter = 0;
            sprite_index = sprite_index == 56 ? 57 : 56;
            if (animation_frame++ > 30) {
                sprite_index = 3;
                poo_state = 1;
            }
            update_sprite(x_curr, y_curr, sprite_index);
            sub_496F(0);
            break;
        case 53:
            animation_frame = 0;
            poo_state = 54;
            sub_2A21();
            facing_direction_sub = facing_direction;
            y_sub = y_curr;
            sprite_index_sub = 149;
            if (facing_direction > 0) {
                x_sub = x_curr - 40;
            } else {
                x_sub = x_curr + 40;
            }
            update_sprite_sub(x_sub, y_sub, sprite_index_sub);
            word_C0AE = 1;
            sub_2B01(instances[MAX_INSTANCES-1], dude);
            break;
        case 54:
            if (frame_period_counter++ < 2) {
                break;
            }
            frame_period_counter = 0;
            sprite_index = animation_beer[animation_frame];
            animation_frame += 1;
            if (sprite_index == 2) {
                x_curr -= facing_direction * 8;
                update_sprite(x_curr, y_curr, sprite_index);
                break;
            }
            if (sprite_index >= 149 && sprite_index <= 153) {
                sprite_index_sub = sprite_index;
                if (sprite_index == 153) {
                    sprite_index_sub = 173;
                }
                update_sprite_sub(x_sub, y_sub, sprite_index_sub);
                sprite_index = animation_beer[animation_frame];
                animation_frame += 1;
            }
            if (sprite_index == 0) {
                destroy_subwindow();
                poo_state = 1;
                break;
            }
            update_sprite(x_curr, y_curr, sprite_index);
            break;
        case 55:
            break;
            if (frame_period_counter++ < 1) {
                break;
            }
            frame_period_counter = 0;
            x_curr -= facing_direction * 6;
            sprite_index = 2;
            update_sprite(x_curr, y_curr, sprite_index);
            sub_4559();
            sub_46D2();
            sub_496F(1);
            sub_4C21(-(facing_direction * 6 - x_curr), facing_direction * 6 + x_curr, 1);
            poo_state = 54;
            break;
        case 56:
            animation_frame = 0;
            poo_state = 57;
        case 57:
            if (frame_period_counter++ < 2) {
                break;
            }
            frame_period_counter = 0;
            sprite_index = animation_beer[animation_frame];
            animation_frame += 1;
            if (sprite_index >= 149 && sprite_index <= 153) {
                sprite_index = animation_beer[animation_frame];
                animation_frame += 1;
            }
            if (animation_frame >= 16) {
                poo_state = 42;
                break;
            }
            update_sprite(x_curr, y_curr, sprite_index);
            sub_496F(0);
            break;
        case 58:
            if (frame_period_counter++ < 1) {
                break;
            }
            frame_period_counter = 0;
            sprite_index = 3;
            update_sprite(x_curr, y_curr, sprite_index);
            poo_state = 59;
            animation_frame = 0;
            break;
        case 59:
            if (frame_period_counter++ < 1) {
                break;
            }
            frame_period_counter = 0;
            if (facing_direction > 0) {
                sprite_index = animation_frame + 9;
            } else {
                sprite_index = 11 - animation_frame;
            }
            animation_frame += 1;
            if (animation_frame > 2) {
                sprite_index = 34;
                frame_period_counter = -10;
                poo_state = 60;
                animation_frame = 0;
            }
            update_sprite(x_curr, y_curr, sprite_index);
            sub_496F(0);
            break;
        case 60:
            if (frame_period_counter++ < 0) {
                break;
            }
            frame_period_counter = 0;
            sprite_index = word_A2B4[5][animation_frame];
            update_sprite(x_curr, y_curr, sprite_index);
            animation_frame += 1;
            if (animation_frame > 7) {
                animation_frame = 0;
                poo_state = 61;
                frame_period_counter = -5;
            }
            sub_496F(0);
            break;
        case 61:
            if (frame_period_counter++ < 1) {
                break;
            }
            frame_period_counter = 0;
            if (facing_direction > 0) {
                sprite_index = 10 - animation_frame;
            } else {
                sprite_index = animation_frame + 10;
            }
            update_sprite(x_curr, y_curr, sprite_index);
            animation_frame += 1;
            if (animation_frame > 1) {
                poo_state = 42;
            }
            sub_496F(0);
            break;
        case 64:
            break;
        case 65:
            sprite_index = 3;
            update_sprite(x_curr, y_curr, sprite_index);
            animation_frame = 0;
            poo_state = 66;
            break;
        case 66:
            if (frame_period_counter++ < 1) {
                break;
            }
            frame_period_counter = 0;
            if (animation_frame == 0) {
                if (facing_direction > 0) {
                    sprite_index = 9;
                } else {
                    sprite_index = 11;
                }
            } else {
                sprite_index = 10;
            }
            update_sprite(x_curr, y_curr, sprite_index);
            if (animation_frame++ > 0) {
                poo_state = 67;
                word_A828 = (rand() % 4 + 4) * 8 + 64;
                animation_frame = 0;
                break;
            }
            sub_496F(0);
            break;
        case 67:
            if (--animation_frame < 0) {
                animation_frame = 79;
            }
            x_curr -= facing_direction * 8;
            sprite_index = word_A524[animation_frame % 8];
            update_sprite(x_curr, y_curr, sprite_index);
            if (facing_direction > 0 && x_curr < 0) {
                poo_state = 30;
            }
            if (facing_direction < 0 && word_CA50 - sprites[sprite_index].width < x_curr) {
                poo_state = 30;
            }
            if (--word_A828 <= 0) {
                poo_state = 68;
                animation_frame = 0;
            }
            sub_496F(2);
            sub_4C21(-(facing_direction * 8 - x_curr), facing_direction * 8 + x_curr, 2);
            break;
        case 68:
            if (frame_period_counter++ < 1) {
                break;
            }
            frame_period_counter = 0;
            if (animation_frame == 1) {
                if (facing_direction > 0) {
                    sprite_index = 9;
                } else {
                    sprite_index = 11;
                }
            } else if (animation_frame == 0) {
                sprite_index = 10;
            } else {
                sprite_index = 3;
            }
            update_sprite(x_curr, y_curr, sprite_index);
            if (animation_frame++ > 1) {
                poo_state = 1;
                break;
            }
            sub_496F(0);
            break;
        case 62:
            poo_state = 63;
            animation_frame = 0;
            break;
        case 63:
            if (frame_period_counter++ < 2) {
                break;
            }
            frame_period_counter = 0;
            sprite_index = animation_blush[animation_frame];
            if (sprite_index == 0) {
                poo_state = 1;
                break;
            }
            update_sprite(x_curr, y_curr, sprite_index);
            animation_frame += 1;
            sub_496F(0);
            break;
        case 75:
            animation_frame = rand() % 8 + 8;
            word_A828 = animation_frame;
            sprite_index = facing_direction > 0 ? 12 : 14;
            update_sprite(x_curr, y_curr, sprite_index);
            poo_state = 76;
            break;
        case 76:
            if (frame_period_counter++ < 1) {
                break;
            }
            frame_period_counter = 0;
            sprite_index = 13;
            update_sprite(x_curr, y_curr, sprite_index);
            poo_state = 77;
            break;
        case 77:
            if (frame_period_counter++ < 2) {
                break;
            }
            frame_period_counter = 0;
            sprite_index = sprite_index == 131 ? 132 : 131;
            y_curr -= 8;
            update_sprite(x_curr, y_curr, sprite_index);
            if (animation_frame-- <= 0) {
                animation_frame = word_A828;
                poo_state = 78;
            }
            break;
        case 78:
            sprite_index = 133;
            y_curr += 8;
            update_sprite(x_curr, y_curr, sprite_index);
            if (animation_frame-- <= 0) {
                poo_state = 79;
            }
            break;
        case 79:
            if (frame_period_counter++ < 10) {
                break;
            }
            frame_period_counter = 0;
            poo_state = 80;
            animation_frame = 3;
            break;
        case 80:
            if (frame_period_counter++ < 1) {
                break;
            }
            frame_period_counter = 0;
            if (facing_direction > 0) {
                sprite_index = word_A43C[animation_frame];
                animation_frame += 1;
            } else {
                sprite_index = word_A44C[animation_frame];
                animation_frame += 1;
            }
            if (sprite_index == 0) {
                poo_state = 1;
                break;
            }
            update_sprite(x_curr, y_curr, sprite_index);
            break;
        case 69:
            frame_period_counter = 0;
            animation_frame = 0;
            poo_state = 70;
        case 70:
            if (facing_direction > 0) {
                sprite_index = word_A536[animation_frame % 8];
            } else {
                sprite_index = word_A536[(animation_frame + 4) % 8];
            }
            if (sprite_index == 2) {
                sprite_index = 3;
                if (facing_direction > 0) {
                    facing_direction = -facing_direction;
                    update_sprite(x_curr, y_curr, sprite_index);
                    facing_direction = -facing_direction;
                } else {
                    update_sprite(x_curr, y_curr, sprite_index);
                }
            } else if (sprite_index == 3) {
                if (facing_direction < 0) {
                    facing_direction = -facing_direction;
                    update_sprite(x_curr, y_curr, sprite_index);
                    facing_direction = -facing_direction;
                } else {
                    update_sprite(x_curr, y_curr, sprite_index);
                }
            } else {
                update_sprite(x_curr, y_curr, sprite_index);
            }
            if (animation_frame++ >= 16) {
                sprite_index = 67;
                update_sprite(x_curr, y_curr, sprite_index);
                poo_state = 71;
            }
            sub_496F(0);
            break;
        case 71:
            sub_496F(0);
            if (frame_period_counter++ < 14) {
                break;
            }
            frame_period_counter = 0;
            sprite_index = 96;
            update_sprite(x_curr, y_curr, sprite_index);
            poo_state = 72;
            break;
        case 72:
            sub_496F(0);
            if (frame_period_counter++ < 30) {
                break;
            }
            frame_period_counter = 0;
            sprite_index = 3;
            update_sprite(x_curr, y_curr, sprite_index);
            poo_state = 1;
            break;
        case 73:
            animation_frame = 0;
            poo_state = 74;
        case 74:
            if (frame_period_counter++ < 2) {
                break;
            }
            frame_period_counter = 0;
            sprite_index = word_A422[animation_frame];
            animation_frame += 1;
            if (sprite_index == 0) {
                poo_state = 1;
                break;
            }
            update_sprite(x_curr, y_curr, sprite_index);
            sub_496F(0);
            break;
        case 81:
            sprite_index = 4;
            update_sprite(x_curr, y_curr, sprite_index);
            poo_state = 82;
            break;
        case 82:
            frame_period_counter = 0;
            sprite_index = sprite_index == 4 ? 5 : 4;
            update_sprite(x_curr, y_curr, sprite_index);
            break;
        case 83:
            break;
        case 84:
            poo_state = 0;
            break;
        case 85:
            word_A81C = GetActiveWindow();
            if (word_A81C == dude || word_A81C == instances[MAX_INSTANCES-1] || word_A81C == NULL || sub_39D6(word_A81C)) {
                poo_state = 1;
                break;
            }
            sub_491D(word_A81C, &stru_A81E);
            if (stru_A81E.top < 10) {
                poo_state = 1;
                break;
            }
            if (facing_direction > 0 && stru_A81E.right < x_curr && stru_A81E.top < y_curr && y_curr + 40 < stru_A81E.bottom ||
                facing_direction < 0 && x_curr + 40 < stru_A81E.left && stru_A81E.top < y_curr &&
                y_curr + 40 < stru_A81E.bottom) {
                poo_state = 87;
                break;
            }
            word_A83C = (rand() % stru_A81E.right - stru_A81E.left) / 3 + (stru_A81E.right - stru_A81E.left) / 2 +
                        stru_A81E.left - 20;
            word_A83E = stru_A81E.top - 40;
            if (word_CA50 / 2 - 20 > x_curr) {
                facing_direction = 1;
            } else {
                facing_direction = -1;
            }
            sprite_index = 4;
            update_sprite(x_curr, y_curr, sprite_index);
            poo_state = 86;
            break;
        case 86:
            if (frame_period_counter++ < 1) {
                break;
            }
            frame_period_counter = 0;
            x_curr -= facing_direction * 16;
            sprite_index = sprite_index == 4 ? 5 : 4;
            update_sprite(x_curr, y_curr, sprite_index);
            if (x_curr < -40 || x_curr > word_CA50) {
                if (!sub_48F3(word_A81C)) {
                    poo_state = 1;
                    break;
                }
                if (rand() % 3 == 0) {
                    poo_state = 3;
                    goto loc_4D33;
                }
                word_A840 = 0;
                poo_state = 92;
                word_A7FC = 1;
                x_curr = word_A83C;
                y_curr = -40;
                y_speed = 0;
                word_A808 = 0;
                word_A842 = rand() % 2;
                if (word_A842 != 0) {
                    word_A808 = -(facing_direction * 3);
                }
                sub_2B01(dude, word_A81C);
            }
            break;
        case 87:
            sub_2B01(dude, word_A81C);
            if (facing_direction > 0) {
                word_A83C = stru_A81E.right;
                word_A83E = stru_A81E.top;
            } else {
                word_A83C = stru_A81E.left - 40;
                word_A83E = stru_A81E.top;
            }
            poo_state = 88;
        case 88:
            if (frame_period_counter++ < 1) {
                break;
            }
            frame_period_counter = 0;
            x_curr -= facing_direction * 16;
            sprite_index = sprite_index == 4 ? 5 : 4;
            if (word_A83C >= x_curr && facing_direction > 0 || word_A83C <= x_curr && facing_direction < 0) {
                if (!sub_48F3(word_A81C)) {
                    poo_state = 1;
                    break;
                }
                sub_491D(word_A81C, &var_10);
                if (var_10.left == stru_A81E.left && var_10.right == stru_A81E.right && var_10.top < y_curr &&
                    y_curr + 40 < var_10.bottom) {
                    word_A83E = var_10.top - 12;
                    word_A7FC = 1;
                    x_curr = word_A83C;
                    sprite_index = 30;
                    poo_state = 89;
                    break;
                } else {
                    poo_state = 1;
                    break;
                }
            }
            update_sprite(x_curr, y_curr, sprite_index);
            break;
        case 89:
            if (frame_period_counter++ < 1) {
                break;
            }
            frame_period_counter = 0;
            update_sprite(x_curr, y_curr, sprite_index);
            y_curr -= 6;
            sprite_index = sprite_index == 15 ? 16 : 15;
            if (word_A83E >= y_curr) {
                poo_state = 90;
                break;
            }
            sub_4B3B();
            break;
        case 90:
            if (frame_period_counter++ < 2) {
                break;
            }
            frame_period_counter = 0;
            x_curr -= facing_direction * 8;
            y_curr = word_A83E - 20;
            sprite_index = 76;
            update_sprite(x_curr, y_curr, sprite_index);
            poo_state = 91;
            break;
        case 91:
            if (frame_period_counter++ < 2) {
                break;
            }
            frame_period_counter = 0;
            x_curr += facing_direction * -24;
            y_curr -= 8;
            sprite_index = 3;
            update_sprite(x_curr, y_curr, sprite_index);
            poo_state = 11;
            break;
        case 92:
            y_speed += 4;
            word_A80C = y_curr;
            x_curr += word_A808;
            y_curr += y_speed;
            if ((var_4 = sub_419E(word_A81C, sprites[sprite_index].height + y_curr,
                                  sprites[sprite_index].height + word_A80C, x_curr,
                                  sprites[sprite_index].width + x_curr)) != 0) {
                if (var_4 == -1) {
                    update_sprite(x_curr, y_curr, sprite_index);
                    poo_state = 0;
                    break;
                }
                y_curr = var_4 - sprites[sprite_index].height;
                if (y_speed < 64 && word_A840 == 0 || y_speed < 8) {
                    SetWindowPos(dude, word_A81C, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_NOACTIVATE);
                    word_A840 = 0;
                    animation_frame = 0;
                    poo_state = 93;
                    if (y_speed < 36) {
                        sprite_index = 49;
                        frame_period_counter = -4;
                    } else {
                        if ((rand() & 3) == 0) {
                            sprite_index = 48;
                        } else {
                            sprite_index = 42;
                        }
                        frame_period_counter = -12;
                    }
                    update_sprite(x_curr, y_curr, sprite_index);
                    break;
                } else {
                    y_speed = y_speed * 2 / -3;
                    word_A840 = 1;
                }
            }
            if (word_A842 != 0) {
                sprite_index = sprite_index == 4 ? 5 : 4;
            } else {
                sprite_index = 42;
            }
            update_sprite(x_curr, y_curr, sprite_index);
            break;
        case 93:
            if (frame_period_counter++ < 1) {
                break;
            }
            frame_period_counter = 0;
            if (word_A842 != 0) {
                poo_state = 11;
                sprite_index = 2;
                break;
            }
            if (animation_frame == 0) {
                sprite_index = 13;
            } else if (animation_frame == 1) {
                if (facing_direction > 0) {
                    sprite_index = 12;
                } else {
                    sprite_index = 14;
                }
            } else if (animation_frame == 2) {
                sprite_index = 3;
            }
            update_sprite(x_curr, y_curr, sprite_index);
            if (animation_frame++ >= 2) {
                poo_state = 11;
            }
            break;
        case 94:
            word_A7FC = 1;
            y_speed = 0;
            word_A808 = -(facing_direction * 8);
            word_A842 = 1;
            poo_state = 99;
            goto loc_4D33;
        case 95:
            word_A7FC = 1;
            y_speed = 0;
            word_A808 = -(facing_direction * 3);
            word_A842 = 1;
            poo_state = 99;
            goto loc_4D33;
        case 96:
            word_A7FC = 1;
            y_speed = 0;
            word_A808 = 0;
            word_A842 = 0;
            poo_state = 99;
            goto loc_4D33;
        case 97:
            word_A7FC = 1;
            y_speed = 0;
            word_A808 = 0;
            word_A842 = 1;
            poo_state = 99;
            goto loc_4D33;
        case 98:
            word_A7FC = 1;
            y_speed = 0;
            word_A808 = 0;
            word_A842 = 2;
            poo_state = 99;
            goto loc_4D33;
        case 99:
            sub_3DF0();
            y_speed += 4;
            word_A80C = y_curr;
            x_curr += word_A808;
            y_curr += y_speed;
            if (word_A80C > word_CA52) {
                update_sprite(x_curr, y_curr, sprite_index);
                poo_state = 0;
                break;
            }
            if ((var_4 = sub_408C(&word_A81C, sprites[sprite_index].height + y_curr,
                                  sprites[sprite_index].height + word_A80C, x_curr,
                                  sprites[sprite_index].width + x_curr)) != 0) {
                if (!sub_48F3(word_A81C)) {
                    update_sprite(x_curr, y_curr, sprite_index);
                    poo_state = 0;
                    break;
                }
                sub_491D(word_A81C, &stru_A81E);
                y_curr = var_4 - sprites[sprite_index].height;
                if (word_A842 == 3) {
                    sprite_index = 66;
                    update_sprite(x_curr, y_curr, sprite_index);
                    poo_state = 29;
                    break;
                }
                if (y_speed < 64 && word_A840 == 0 || y_speed < 8) {
                    if (word_A81C != NULL) {
                        SetWindowPos(dude, word_A81C, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_NOACTIVATE);
                    }
                    word_A840 = 0;
                    animation_frame = 0;
                    poo_state = 100;
                    if (y_speed < 36) {
                        sprite_index = 49;
                        frame_period_counter = -4;
                    } else {
                        sprite_index = (rand() & 3) == 0 ? 67 : 49; //dude textures for landing
                        frame_period_counter = -10;
                    }
                    if (word_A842 == 2) {
                        if (y_speed < 36) {
                            sprite_index = 41;
                        } else {
                            sprite_index = 45;
                        }
                    }
                    update_sprite(x_curr, y_curr, sprite_index);
                    break;
                } else {
                    if ((rand() & 7) == 0 && word_A840 == 0) {
                        word_A840 = 0;
                        animation_frame = 0;
                        poo_state = 100;
                        sprite_index = 48;
                        frame_period_counter = -12;
                        if (word_A842 == 2) {
                            sprite_index = 45;
                        }
                        update_sprite(x_curr, y_curr, sprite_index);
                        break;
                    }
                    y_speed = y_speed * 2 / -3;
                    word_A840 = 1;
                }
            }
            if (word_A842 == 2) {
                sprite_index = sprite_index == 40 ? 41 : 40;
            } else if (word_A842 == 1) {
                sprite_index = sprite_index == 4 ? 5 : 4;
            } else if (word_A842 == 0) {
                sprite_index = 42;
            } else {
                sprite_index = word_A324[word_A848];
                word_A848 += 1;
                if (sprite_index == 66) {
                    word_A848 -= 1;
                }
            }
            if (word_A842 == 3 && sub_4C91(x_curr, x_curr - word_A808) != 0) {
                facing_direction = -facing_direction;
                poo_state = 30;
                break;
            }
            update_sprite(x_curr, y_curr, sprite_index);
            break;
        case 100:
            if (frame_period_counter++ < 1) {
                break;
            }
            frame_period_counter = 0;
            if (word_A842 == 1) {
                poo_state = 11;
                sprite_index = 2;
                break;
            }
            if (word_A842 == 2) {
                animation_frame = 0;
                poo_state = 101;
                break;
            }
            if (animation_frame == 0) {
                sprite_index = 13;
            } else if (animation_frame == 1) {
                if (facing_direction > 0) {
                    sprite_index = 12;
                } else {
                    sprite_index = 14;
                }
            } else if (animation_frame == 2) {
                sprite_index = 3;
            }
            update_sprite(x_curr, y_curr, sprite_index);
            if (animation_frame++ >= 2) {
                poo_state = 11;
            }
            break;
        case 101:
            if (frame_period_counter++ < 1) {
                break;
            }
            frame_period_counter = 0;
            if (animation_frame == 0) {
                sprite_index = 31;
                frame_period_counter = -8;
            } else if (animation_frame == 2) {
                sprite_index = 3;
            }
            update_sprite(x_curr, y_curr, sprite_index);
            if (animation_frame++ >= 6) {
                poo_state = 11;
            }
            break;
        case 102:
            sub_428E();
            animation_frame = 6;
            sprite_index = 3;
            word_A82A = 0;
            if (rand() % 3 == 0) {
                word_A82A = 1;
            }
            poo_state = 103;
        case 103:
            if (word_A82A != 0) {
                sprite_index = sprite_index == 50 ? 51 : 50;
            } else {
                sprite_index = sprite_index == 4 ? 5 : 4;
            }
            update_sprite(x_curr, y_curr, sprite_index);
            if (animation_frame-- <= 0) {
                poo_state = 97;
            }
            break;
        case 104:
            word_A842 = 0;
            poo_state = 106;
            goto loc_4D33;
        case 105:
            word_A842 = 1;
            poo_state = 106;
            goto loc_4D33;
        case 106:
            if (word_A842 == 0) {
                var_14.x = x_curr;
                var_14.y = y_curr + 39;
                *(HWND *) &var_10 = WindowFromPoint(var_14);
                var_14.x = x_curr + 39;
                var_8 = WindowFromPoint(var_14);
                if (*(HWND *) &var_10 == dude && var_8 == dude) {
                    place_window_as_top(dude);
                } else if (*(HWND *) &var_10 == dude) {
                    sub_2B01(dude, var_8);
                } else {
                    sub_2B01(dude, *(HWND *) &var_10);
                }
                sprite_index = 81;
            } else {
                sprite_index = 78;
            }
            update_sprite(x_curr, y_curr, sprite_index);
            poo_state = 107;
            animation_frame = 0;
            break;
        case 107:
            sprite_index = word_A2B4[4 - word_A842][animation_frame];
            update_sprite(x_curr, y_curr, sprite_index);
            animation_frame += 1;
            if (animation_frame > 7) {
                if (word_A842 != 0) {
                    if ((rand() & 1) == 0) {
                        poo_state = 111;
                    } else {
                        poo_state = 109;
                    }
                } else {
                    if ((rand() & 1) == 0) {
                        poo_state = 111;
                    } else {
                        poo_state = 108;
                    }
                }
            }
            break;
        case 108:
            if (frame_period_counter++ < 10) {
                break;
            }
            frame_period_counter = 0;
            sprite_index = 3;
            update_sprite(x_curr, y_curr, sprite_index);
            poo_state = 1;
            break;
        case 109:
            word_A808 = -(facing_direction * 14);
            sprite_index = 23;
            x_curr += word_A808;
            update_sprite(x_curr, y_curr, sprite_index);
            poo_state = 95;
            animation_frame = 0;
            break;
        case 110:
            x_curr += word_A808;
            word_A808 += facing_direction;
            update_sprite(x_curr, y_curr, sprite_index);
            if (animation_frame++ > 3) {
                poo_state = 95;
            }
            break;
        case 111:
            if (word_A842 != 0) {
                x_curr += facing_direction * -26;
                y_curr += 35;
                facing_direction = -facing_direction;
            } else {
                word_A82A = rand() % 2;
                if (word_A82A != 0) {
                    y_curr += 36;
                } else {
                    y_curr += 20;
                }
            }
            animation_frame = 0;
            poo_state = 112;
        case 112:
            if (animation_frame == 0) {
                if (frame_period_counter++ < 10) {
                    break;
                }
                frame_period_counter = 0;
            } else {
                if (frame_period_counter++ < 1) {
                    break;
                }
                frame_period_counter = 0;
            }
            if (word_A842 != 0) {
                sprite_index = sprite_index == 40 ? 41 : 40;
            } else {
                sprite_index = word_A314[word_A82A][animation_frame % 4];
            }
            update_sprite(x_curr, y_curr, sprite_index);
            animation_frame += 1;
            if (animation_frame > 12) {
                if (word_A842 != 0) {
                    poo_state = 98;
                } else {
                    poo_state = 96;
                }
            }
            break;
        case 113:
            word_CA76 = 1;
            sprite_index = 6;
            update_sprite(x_curr, y_curr, sprite_index);
            poo_state = 114;
            break;
        case 114:
            if (frame_period_counter++ < 1) {
                break;
            }
            frame_period_counter = 0;
            sprite_index += 1;
            update_sprite(x_curr, y_curr, sprite_index);
            if (sprite_index == 8) {
                sprite_index = 0;
                poo_state = 115;
            }
            sub_496F(0);
            break;
        case 115:
            sub_496F(0);
            if (frame_period_counter++ < 4) {
                break;
            }
            frame_period_counter = 0;
            sprite_index = sprite_index == 0 ? 1 : 0;
            update_sprite(x_curr, y_curr, sprite_index);
            break;
        case 116:
            x_curr = word_CA50;
            y_curr = word_CA52 * 7 / 8;
            sprite_index = 4;
            facing_direction = 1;
            update_sprite(x_curr, y_curr, sprite_index);
            poo_state = 117;
            break;
        case 117:
            if (frame_period_counter++ < 1) {
                break;
            }
            frame_period_counter = 0;
            x_curr -= facing_direction * 16;
            sprite_index = sprite_index == 4 ? 5 : 4;
            update_sprite(x_curr, y_curr, sprite_index);
            if (word_CA50 / 2 - 20 >= x_curr) {
                poo_state = 118;
            }
            break;
        case 118:
            play_sound(109, 0U, 0);
            sub_2A21();
            facing_direction_sub = -1;
            x_sub = -40;
            y_sub = word_CA52 / 8;
            sprite_index_sub = 154;
            animation_frame = 0;
            poo_state = 119;
            word_C0AE = 0;
            place_window_as_top(dude);
            place_window_as_top(instances[MAX_INSTANCES-1]);
            break;
        case 119:
            if (animation_frame != 0) {
                sprite_index = word_A2B4[2][animation_frame];
                update_sprite(x_curr, y_curr, sprite_index);
                animation_frame += 1;
                if (animation_frame > 7) {
                    animation_frame = 0;
                }
            } else {
                sprite_index = 73;
                update_sprite(x_curr, y_curr, sprite_index);
                if (rand() % 20 == 0) {
                    animation_frame = 1;
                }
            }
            if (frame_period_counter++ < 1) {
                break;
            }
            frame_period_counter = 0;
            x_sub -= facing_direction_sub * 16;
            sprite_index_sub = sprite_index_sub == 154 ? 155 : 154;
            update_sprite_sub(x_sub, y_sub, sprite_index_sub);
            if (x_sub > x_curr) {
                facing_direction = -1;
                update_sprite(x_curr, y_curr, sprite_index);
            }
            if (x_sub > word_CA50) {
                destroy_subwindow();
                poo_state = 120;
            }
            break;
        case 120:
            if (frame_period_counter++ < 1) {
                break;
            }
            frame_period_counter = 0;
            x_curr -= facing_direction * 16;
            sprite_index = sprite_index == 4 ? 5 : 4;
            update_sprite(x_curr, y_curr, sprite_index);
            if (x_curr > word_CA50) {
                poo_state = 1;
            }
            break;
        case 121:
            x_curr = word_CA50;
            y_curr = word_CA52 * 7 / 8;
            sprite_index = 4;
            facing_direction = 1;
            sub_2A21();
            facing_direction_sub = -1;
            x_sub = -40;
            y_sub = word_CA52 * 7 / 8;
            sprite_index_sub = 154;
            update_sprite(x_curr, y_curr, sprite_index);
            update_sprite_sub(x_sub, y_sub, sprite_index_sub);
            poo_state = 122;
            word_C0AE = 0;
            place_window_as_top(dude);
            place_window_as_top(instances[MAX_INSTANCES-1]);
            break;
        case 122:
            if (frame_period_counter++ < 1) {
                break;
            }
            frame_period_counter = 0;
            x_curr -= facing_direction * 16;
            sprite_index = sprite_index == 4 ? 5 : 4;
            x_sub -= facing_direction_sub * 16;
            sprite_index_sub = sprite_index_sub == 154 ? 155 : 154;
            if (x_curr - x_sub <= 46) {
                x_curr = word_CA50 / 2 + 3;
                x_sub = word_CA50 / 2 - 43;
                sprite_index = 3;
                sprite_index_sub = 157;
                update_sprite(x_curr, y_curr, sprite_index);
                update_sprite_sub(x_sub, y_sub, sprite_index_sub);
                animation_frame = 0;
                poo_state = 124;
            } else {
                update_sprite(x_curr, y_curr, sprite_index);
                update_sprite_sub(x_sub, y_sub, sprite_index_sub);
            }
            break;
        case 123:
            if (frame_period_counter++ < 3) {
                break;
            }
            frame_period_counter = 0;
            sprite_index = animation_frame + 127;
            animation_frame += 1;
            update_sprite(x_curr, y_curr, sprite_index);
            if (animation_frame >= 4) {
                poo_state = 124;
                animation_frame = 0;
            }
            break;
        case 124: // dude getting into van
            if (frame_period_counter++ < 2) {
                break;
            }
            frame_period_counter = 0;
            sprite_index = sprite_index == 2 ? 3 : 2;
            x_curr -= 5;
            update_sprite(x_curr, y_curr, sprite_index);
            place_window_as_top(instances[MAX_INSTANCES-1]);
            place_window_as_top(dude);
            if(animation_frame++ < 6) {
                break;
            }

            destroy_subwindow();
            animation_frame = 0;
            frame_period_counter = 0;
            drift_accel = 0;
            x_curr = x_sub; //relocate dude to van
            facing_direction = -1;
            poo_state = 1245;
            break;
        case 1245: //van acceleration

            sprite_index = 154 + (animation_frame%4);
            x_curr += drift_accel;
            drift_accel += 3;
            update_sprite(x_curr, y_curr, sprite_index);

            if(drift_accel <= DRIFT_SPEED)
                break;

            drift_accel = DRIFT_SPEED;
            animation_frame = 0;
            frame_period_counter = 0;
            facing_direction = 1;
            play_sound(109, 0U, 0);
            poo_state = 125;
        case 125: // van drifting
            if(drift_accel > - 1.5 * DRIFT_SPEED){ // exit van speed
                drift_accel -= 5;
            }

            sprite_index = animation_drift[animation_frame];
            if(sprite_index == 0) {
                animation_frame -= 4; //animation loop (4 last frames)
                sprite_index = animation_drift[animation_frame];
            }
            else {
                animation_frame++;
            }
            x_curr += drift_accel;
            update_sprite(x_curr, y_curr, sprite_index);
            if (x_curr < -40) {
                destroy_subwindow();
                poo_state = 1;
            }
            break;
        case 126:
            play_sound(109, 0U, 0);
            x_curr = word_CA50;
            y_curr = word_CA52 * 7 / 8;
            sprite_index = 4;
            facing_direction = 1;
            sub_2A21();
            facing_direction_sub = 1;
            x_sub = word_CA50 + 46;
            y_sub = word_CA52 * 7 / 8;
            sprite_index_sub = 154;
            update_sprite(x_curr, y_curr, sprite_index);
            update_sprite_sub(x_sub, y_sub, sprite_index_sub);
            poo_state = 127;
            word_C0AE = 0;
            place_window_as_top(dude);
            place_window_as_top(instances[MAX_INSTANCES-1]);
            break;
        case 127:
            if (frame_period_counter++ < 1) {
                break;
            }
            frame_period_counter = 0;
            x_curr -= facing_direction * 16;
            sprite_index = sprite_index == 4 ? 5 : 4;
            x_sub -= facing_direction_sub * 16;
            sprite_index_sub = sprite_index_sub == 154 ? 155 : 154;
            if (x_sub < -40) {
                destroy_subwindow();
                poo_state = 1;
            } else {
                update_sprite(x_curr, y_curr, sprite_index);
                update_sprite_sub(x_sub, y_sub, sprite_index_sub);
            }
            break;
        case 128:
            x_curr = word_CA50;
            y_curr = word_CA52 * 7 / 8;
            sprite_index = 4;
            facing_direction = 1;
            update_sprite(x_curr, y_curr, sprite_index);
            poo_state = 129;
            break;
        case 129:
            if (frame_period_counter++ < 1) {
                break;
            }
            frame_period_counter = 0;
            x_curr -= facing_direction * 16;
            sprite_index = sprite_index == 4 ? 5 : 4;
            update_sprite(x_curr, y_curr, sprite_index);
            if (word_CA50 / 2 - 20 >= x_curr) {
                poo_state = 130;
            }
            break;
        case 130:
            sub_2A21();
            facing_direction_sub = -1;
            x_sub = -40;
            y_sub = word_CA52 / 8;
            sprite_index_sub = 158;
            animation_frame = 0;
            poo_state = 131;
            word_C0AE = 0;
            place_window_as_top(dude);
            place_window_as_top(instances[MAX_INSTANCES-1]);
            break;
        case 131:
            if (animation_frame != 0) {
                sprite_index = word_A2B4[2][animation_frame];
                update_sprite(x_curr, y_curr, sprite_index);
                animation_frame += 1;
                if (animation_frame > 7) {
                    animation_frame = 0;
                }
            } else {
                sprite_index = 73;
                update_sprite(x_curr, y_curr, sprite_index);
                if (rand() % 20 == 0) {
                    animation_frame = 1;
                }
            }
            x_sub -= facing_direction_sub * 16;
            if (sprite_index_sub == 160) {
                sprite_index_sub = 158;
            } else {
                sprite_index_sub += 1;
            }
            if (x_sub > x_curr) {
                x_sub = x_curr;
                sprite_index_sub = 162;
                poo_state = 132;
            }
            update_sprite_sub(x_sub, y_sub, sprite_index_sub);
            break;
        case 132:
            sprite_index = 73;
            update_sprite(x_curr, y_curr, sprite_index);
            word_CA5C += 40;
            if (y_curr - y_sub - 40 <= word_CA5C) {
                word_CA5C = y_curr - y_sub - 40;
                word_CA5C -= 20;
                poo_state = 133;
            }
            update_sprite_sub(x_sub, y_sub, sprite_index_sub);
            if (frame_period_counter++ < 1) {
                break;
            }
            frame_period_counter = 0;
            if (sprite_index_sub == 164) {
                sprite_index_sub = 162;
            } else {
                sprite_index_sub += 1;
            }
            break;
        case 133:
            word_CA5C -= 20;
            if (word_CA5C <= 0) {
                word_CA5C = 0;
                y_curr = y_sub + 40;
                poo_state = 134;
                sprite_index = sprite_index == 4 ? 5 : 4;
                update_sprite(x_curr, y_curr, sprite_index);
                sprite_index_sub = 158;
                update_sprite_sub(x_sub, y_sub, sprite_index_sub);
                break;
            }
            update_sprite_sub(x_sub, y_sub, sprite_index_sub);
            sprite_index = sprite_index == 4 ? 5 : 4;
            y_curr -= 20;
            update_sprite(x_curr, y_curr, sprite_index);
            if (frame_period_counter++ < 1) {
                break;
            }
            frame_period_counter = 0;
            if (sprite_index_sub == 164) {
                sprite_index_sub = 162;
            } else {
                sprite_index_sub += 1;
            }
            break;
        case 134:
            x_curr = -80;
            update_sprite(x_curr, y_curr, sprite_index);
            x_sub -= facing_direction_sub * 16;
            if (sprite_index_sub == 160) {
                sprite_index_sub = 158;
            } else {
                sprite_index_sub += 1;
            }
            if (x_sub > word_CA50) {
                destroy_subwindow();
                sub_428E();
                poo_state = 1;
                break;
            }
            update_sprite_sub(x_sub, y_sub, sprite_index_sub);
            break;
        case 135:
            y_sub = word_CA52 * 7 / 8;
            facing_direction = -1;
            x_curr = -40;
            y_curr = word_CA52 / 8;
            sprite_index = 158;
            animation_frame = 0;
            poo_state = 136;
            break;
        case 136:
            x_curr -= facing_direction * 16;
            if (sprite_index == 160) {
                sprite_index = 158;
            } else {
                sprite_index += 1;
            }
            if (word_CA50 / 2 - 20 < x_curr) {
                x_curr = word_CA50 / 2 - 20;
                sprite_index = 162;
                poo_state = 137;
            }
            update_sprite(x_curr, y_curr, sprite_index);
            break;
        case 137:
            word_CA72 += 40;
            if (y_sub - y_curr - 40 <= word_CA72) {
                word_CA72 = y_sub - y_curr - 40;
                poo_state = 138;
            }
            update_sprite(x_curr, y_curr, sprite_index);
            if (frame_period_counter++ < 1) {
                break;
            }
            frame_period_counter = 0;
            if (sprite_index == 164) {
                sprite_index = 162;
            } else {
                sprite_index += 1;
            }
            break;
        case 138:
            if (frame_period_counter++ < 4) {
                break;
            }
            frame_period_counter = 0;
            sub_2A21();
            x_sub = x_curr;
            sprite_index_sub = 167;
            update_sprite_sub(x_sub, y_sub, sprite_index_sub);
            update_sprite(x_curr, y_curr, sprite_index);
            poo_state = 139;
            word_C0AE = 0;
            place_window_as_top(dude);
            place_window_as_top(instances[MAX_INSTANCES-1]);
            break;
        case 139:
            if (word_CA72 != 0) {
                word_CA72 -= 40;
                if (word_CA72 <= 0) {
                    sprite_index = 158;
                    word_CA72 = 0;
                }
                if (sprite_index == 164) {
                    sprite_index = 162;
                } else {
                    sprite_index += 1;
                }
            } else {
                x_curr -= facing_direction * 16;
                if (sprite_index == 160) {
                    sprite_index = 158;
                } else {
                    sprite_index += 1;
                }
            }
            if (x_curr > word_CA50) {
                poo_state = 140;
            }
            update_sprite(x_curr, y_curr, sprite_index);
            if (frame_period_counter++ < 1) {
                break;
            }
            frame_period_counter = 0;
            sprite_index_sub = sprite_index_sub == 167 ? 168 : 167;
            update_sprite_sub(x_sub, y_sub, sprite_index_sub);
            break;
        case 140:
            sprite_index_sub = 166;
            update_sprite_sub(x_sub, y_sub, sprite_index_sub);
            if (frame_period_counter++ < 1) {
                break;
            }
            frame_period_counter = 0;
            word_CA46 += 1;
            if (word_CA46 > 8) {
                destroy_subwindow();
                sub_428E();
                poo_state = 1;
            }
            break;
        case 141:
            break;
        case 142:
            x_curr = -80;
            y_curr = word_CA52 / 8;
            update_sprite(x_curr, y_curr, sprite_index);
            sub_2A21();
            facing_direction_sub = -1;
            x_sub = -40;
            y_sub = word_CA52 * 7 / 8;
            sprite_index_sub = 158;
            animation_frame = 0;
            poo_state = 143;
            word_C0AE = 0;
            place_window_as_top(dude);
            place_window_as_top(instances[MAX_INSTANCES-1]);
            break;
        case 143:
            x_sub -= facing_direction_sub * 16;
            if (sprite_index_sub == 160) {
                sprite_index_sub = 158;
            } else {
                sprite_index_sub += 1;
            }
            if (word_CA52 / 8 < x_sub) {
                x_sub = word_CA52 / 8;
                x_curr = word_CA50;
                y_curr = y_sub;
                sprite_index = 4;
                facing_direction = 1;
                poo_state = 144;
            }
            update_sprite_sub(x_sub, y_sub, sprite_index_sub);
            break;
        case 144:
            if (frame_period_counter++ < 1) {
                break;
            }
            frame_period_counter = 0;
            if (sprite_index_sub == 160) {
                sprite_index_sub = 158;
            } else {
                sprite_index_sub += 1;
            }
            update_sprite_sub(x_sub, y_sub, sprite_index_sub);
            x_curr -= facing_direction * 16;
            sprite_index = sprite_index == 4 ? 5 : 4;
            update_sprite(x_curr, y_curr, sprite_index);
            if (x_sub + 40 >= x_curr) {
                x_curr = -80;
                update_sprite(x_curr, y_curr, sprite_index);
                poo_state = 145;
            }
            break;
        case 145:
            y_sub -= 40;
            if (sprite_index_sub == 160) {
                sprite_index_sub = 158;
            } else {
                sprite_index_sub += 1;
            }
            if (y_sub < -40) {
                destroy_subwindow();
                sub_428E();
                poo_state = 1;
                break;
            }
            update_sprite_sub(x_sub, y_sub, sprite_index_sub);
            break;
        case 146:
            break;
        case 147:
            play_sound(109, 0U, 0);
            sub_2A21();
            word_CA56 = 1;
            facing_direction_sub = 1;
            sprite_index_sub = 146;
            animation_frame = 0;
            x_curr = word_CA50;
            y_curr = -40;
            facing_direction = 1;
            word_A808 = word_CA50 / -96;
            y_speed = word_CA52 / 96;
            x_sub = word_A808 * 92 + word_CA50;
            y_sub = y_speed * 92 - 20;
            poo_state = 148;
            word_C0AE = 1;
            place_window_as_top(instances[MAX_INSTANCES-1]);
            place_window_as_top(dude);
        case 148:
            if (frame_period_counter++ < 0) {
                break;
            }
            frame_period_counter = 0;
            update_sprite_sub(x_sub, y_sub, sprite_index_sub);
            x_curr += word_A808;
            y_curr += y_speed;
            sprite_index = animation_falling_van[0];

            update_sprite(x_curr, y_curr, sprite_index);
            if (x_sub + 10 > x_curr || y_sub + 20 < y_curr) {
                animation_frame = 0;
                poo_state = 149;
                sprite_index = 173;
                update_sprite(x_curr, y_curr, sprite_index);
                break;
            }
            break;
        case 149:
            if (frame_period_counter++ < 1) {
                break;
            }
            frame_period_counter = 0;
            x_curr = -80;
            update_sprite(x_curr, y_curr, sprite_index);
            if (animation_frame == 0) {
                play_sound(111, 0U, 0);
            }
            sprite_index_sub = animation_crash[animation_frame++];
            if (sprite_index_sub == 0) {
                x_curr = x_sub;
                y_curr = y_sub;
                animation_frame = 0;
                poo_state = 150;
                break;
            }
            update_sprite_sub(x_sub, y_sub, sprite_index_sub);
            break;
        case 150:
            sprite_index = 169;
            sprite_index = word_A49E[animation_frame];
            animation_frame += 1;
            if (sprite_index == 0) {
                sprite_index = 3;
                poo_state = 151;
                break;
            }
            if (sprite_index >= 81 && sprite_index <= 83) {
                update_sprite(x_curr, y_curr - 20, sprite_index);
            } else {
                update_sprite(x_curr, y_curr, sprite_index);
            }
            break;
        case 151:
            if (frame_period_counter++ < 1) {
                break;
            }
            frame_period_counter = 0;
            x_curr -= facing_direction * 6;
            sprite_index = sprite_index == 2 ? 3 : 2;
            update_sprite(x_curr, y_curr, sprite_index);
            if (x_curr < -40) {
                destroy_subwindow();
                poo_state = 1;
                break;
            }
            break;
        case 152:
            break;
        default:
            break;
    }
}

/* Environment-affected action change, controlled by a flag. */
void sub_8FD7(int arg_0) {
    switch (arg_0) {
        case 0:
            poo_state = 1;
            if (word_A7FC != 0) {
                poo_state = 97;
            }
            break;
        case 1:
            poo_state = 81;
            break;
        case 2:
            poo_state = 97;
            break;
        case 3:
            word_CA54 = 113;
            break;
        case 4:
            poo_state = 56;
            break;
        default:
            break;
    }
}

/* Process debug window action change. */
void set_state_debug(WPARAM arg_0) {

    word_CA72 = 0;
    sub_428E();
    destroy_subwindow();
    switch (arg_0) {
        case 0:
            poo_state = 1;
            break;
        case 1:
            poo_state = 7;
            break;
        case 2:
            poo_state = 11;
            break;
        case 3:
            poo_state = 13;
            break;
        case 4:
            poo_state = 15;
            break;
        case 5:
            poo_state = 17;
            break;
        case 6:
            poo_state = 20;
            break;
        case 7:
            poo_state = 24;
            break;
        case 8:
            poo_state = 30;
            break;
        case 9:
            poo_state = 35;
            break;
        case 10:
            poo_state = 43;
            break;
        case 11:
            poo_state = 45;
            break;
        case 12:
            poo_state = 49;
            break;
        case 13:
            poo_state = 51;
            break;
        case 14:
            poo_state = 53;
            break;
        case 15:
            poo_state = 58;
            break;
        case 16:
            poo_state = 47;
            break;
        case 17:
            poo_state = 147;
            break;
        case 18:
            poo_state = 116;
            break;
        case 19:
            poo_state = 121;
            break;
        case 20:
            poo_state = 126;
            break;
        case 21:
            poo_state = 128;
            break;
        case 22:
            poo_state = 135;
            break;
        case 23:
            poo_state = 142;
            break;
        case 24:
            poo_state = 65;
            break;
        case 25:
            poo_state = 62;
            break;
        case 26:
            poo_state = 75;
            break;
        case 27:
            poo_state = 96;
            break;
        case 28:
            poo_state = 9;
            break;
        case 29:
            poo_state = 69;
            break;
        case 34: // roll animation
            poo_state = 34;
            break;
        default:
            break;
    }
}

/* Move window by offset. */
void sub_91CD(int arg_0, int arg_2) {
    x_curr += arg_0;
    y_curr += arg_2;
    update_sprite(x_curr, y_curr, sprite_index);
}

/* Initialize bitmaps (sub). */
BOOL sub_9200(HWND arg_0)
{
    HDC var_2;
    var_2 = GetDC(arg_0);
    word_A850[0] = CreateCompatibleBitmap(var_2, 100, 100);
    if (word_A850[0] == NULL) {
        goto loc_92FA;
    }
    word_A850[1] = CreateCompatibleBitmap(var_2, 100, 100);
    if (word_A850[1] == NULL) {
        goto loc_92FA;
    }
    word_A854 = CreateCompatibleBitmap(var_2, 100, 100);
    if (word_A854 == NULL) {
        goto loc_92FA;
    }
    word_A85A = CreateCompatibleBitmap(var_2, 40, 40);
    if (word_A85A == NULL) {
        goto loc_92FA;
    }
    word_A85C = CreateCompatibleBitmap(var_2, 40, 40);
    if (word_A85C == NULL) {
        goto loc_92FA;
    }
    word_CA4C = 0;
    word_CA4E = 0;
    word_CA50 = GetSystemMetrics(SM_CXSCREEN);
    word_CA52 = GetSystemMetrics(SM_CYSCREEN);
    ReleaseDC(arg_0, var_2);
    word_CA46 = 0;
    word_CA5C = 0;
    word_CA56 = 0;
    word_A890 = 0;
    word_A892 = 0;
    word_A894 = 0;
    word_A896 = 0;
    return TRUE;
loc_92FA:
    ReleaseDC(arg_0, var_2);
    return FALSE;
}

/* Release bitmaps (sub). */
void sub_930F()
{
    DeleteObject(word_A85C);
    DeleteObject(word_A85A);
    DeleteObject(word_A850[0]);
    DeleteObject(word_A850[1]);
    DeleteObject(word_A854);
}

/* Update window position and sprite to be actually used (sub). */
void sub_9350(int arg_0, int arg_2, int arg_4)
{
    word_A878 = arg_0;
    word_A87A = arg_2;
    word_A856 = sprites[arg_4].bitmaps[0];
    word_A858 = sprites[arg_4].bitmaps[1];
    word_A85E = sprites[arg_4].x;
    word_A860 = sprites[arg_4].y;
    word_A87C = sprites[arg_4].width;
    word_A87E = sprites[arg_4].height;
}

/* Clear window (sub). */
void sub_93DF(HWND arg_0)
{
    if (word_CA56 != 0) {
        return;
    }
    word_A890 = 0;
    word_A892 = 0;
    word_A894 = 0;
    word_A896 = 0;
    MoveWindow(arg_0, 0, 0, 0, 0, TRUE);
    word_A872 = 1;
    word_C0BA = 1;
    word_A876 = NULL;
}

/* Render sprite with double buffering (with fade out effect) (sub). */
void fade_out(HWND arg_0) {
    HDC var_2;
    HDC var_4;
    HDC var_6;
    int var_C;
    int var_E;
    int var_10;
    int var_12;
    int var_14;
    int var_16;
    int var_18;
    int var_1A;
    int var_1C;
    int var_1E;
    if (word_A870 != 0) {
        return;
    }
    if (word_A890 == word_A878 && word_A892 == word_A87A && word_A876 == word_A856 && word_A866 == word_A85E && word_CA46 == 0 && word_CA5C == 0) {
        return;
    }
    word_A86E ^= 1;
    var_2 = GetDC(NULL);
    SelectPalette(var_2, word_CA4A, FALSE);
    var_4 = CreateCompatibleDC(var_2);
    var_6 = CreateCompatibleDC(var_2);
    SelectPalette(var_6, word_CA4A, FALSE);
    SelectPalette(var_4, word_CA4A, FALSE);
    var_16 = max(word_A878, word_A890);
    var_14 = max(word_A87A, word_A892);
    var_12 = min(word_A87C + word_A878, word_A894 + word_A890) - var_16;
    var_10 = min(word_A87A + word_A87E, word_A892 + word_A896) - var_14;
    if (var_12 <= 0 || var_10 <= 0) {
        word_A898 = 1;
        if (word_A872 != 0) {
            word_A872 = 0;
        }
        word_A880 = word_A878;
        word_A882 = word_A87A;
        word_A884 = word_A87C;
        word_A886 = word_A87E;
        SelectObject(var_4, word_A850[word_A86E]);
        BitBlt(var_4, 0, 0, word_A884, word_A886, var_2, word_A880, word_A882, SRCCOPY);
    } else {
        word_A898 = 0;
        word_A880 = min(word_A878, word_A890);
        word_A882 = min(word_A87A, word_A892);
        word_A884 = max(word_A87C + word_A878, word_A894 + word_A890) - word_A880;
        word_A886 = max(word_A87A + word_A87E, word_A892 + word_A896) - word_A882;
        SelectObject(var_4, word_A850[word_A86E]);
        BitBlt(var_4, 0, 0, word_A884, word_A886, var_2, word_A880, word_A882, SRCCOPY);
        var_1E = max(word_A880, word_A888);
        var_1C = max(word_A882, word_A88A);
        var_1A = min(word_A884 + word_A880, word_A88C + word_A888) - var_1E;
        var_18 = min(word_A882 + word_A886, word_A88A + word_A88E) - var_1C;
        var_16 = max(0, var_1E - word_A880);
        var_14 = max(0, var_1C - word_A882);
        var_E = max(0, var_1E - word_A888);
        var_C = max(0, var_1C - word_A88A);
        if (var_1A > 0 && var_18 > 0) {
            SelectObject(var_6, word_A850[LOBYTE(word_A86E) - 0xFF & 1]);
            BitBlt(var_4, var_16, var_14, var_1A, var_18, var_6, var_E, var_C, SRCCOPY);
        }
    }
    if (word_A856 != NULL) {
        SelectObject(var_6, word_A854);
        BitBlt(var_6, 0, 0, word_A884, word_A886, var_4, 0, 0, SRCCOPY);
        var_16 = max(0, word_A878 - word_A880);
        var_14 = max(0, word_A87A - word_A882);
        if (word_A858 != NULL) {
            if (word_CA46 != 0) {
                if (word_CA46 == 1) {
                    SelectObject(var_4, word_A858);
                    SelectObject(var_6, word_A85C);
                    BitBlt(var_6, 0, 0, 40, 40, var_4, word_A85E, word_A860, SRCCOPY);
                    SelectObject(var_4, word_A856);
                    SelectObject(var_6, word_A85A);
                    BitBlt(var_6, 0, 0, 40, 40, var_4, word_A85E, word_A860, SRCCOPY);
                }
                SelectObject(var_4, word_A85C);
                SelectObject(var_6, sprites[FADE_OUT_INDEX].bitmaps[0]);
                BitBlt(var_4, word_CA46 - 1, word_CA46 - 1, 41 - word_CA46, 40, var_6, sprites[FADE_OUT_INDEX].x, 0, SRCPAINT);
                SelectObject(var_4, word_A85A);
                SelectObject(var_6, sprites[FADE_OUT_INDEX].bitmaps[1]);
                BitBlt(var_4, word_CA46 - 1, word_CA46 - 1, 41 - word_CA46, 40, var_6, sprites[FADE_OUT_INDEX].x, 0, SRCAND);
                SelectObject(var_6, word_A854);
                SelectObject(var_4, word_A85C);
                BitBlt(var_6, var_16, var_14, word_A87C, word_A87E, var_4, 0, 0, SRCAND);
                SelectObject(var_4, word_A85A);
                BitBlt(var_6, var_16, var_14, word_A87C, word_A87E, var_4, 0, 0, SRCPAINT);
            } else {
                SelectObject(var_4, word_A858);
                BitBlt(var_6, var_16, var_14, word_A87C, word_A87E, var_4, word_A85E, word_A860, SRCAND);
                SelectObject(var_4, word_A856);
                BitBlt(var_6, var_16, var_14, word_A87C, word_A87E, var_4, word_A85E, word_A860, SRCPAINT);
            }
        } else {
            SelectObject(var_4, word_A856);
            BitBlt(var_6, var_16, var_14, word_A87C, word_A87E, var_4, word_A85E, word_A860, SRCCOPY);
        }
        word_A870 = 1;
        word_CA5E = 1;
        MoveWindow(arg_0, word_A880, word_A882, word_A884, word_A886 + word_CA5C, TRUE);
        word_CA5E = 0;
    }
    DeleteDC(var_4);
    DeleteDC(var_6);
    word_A888 = word_A880;
    word_A88A = word_A882;
    word_A88C = word_A884;
    word_A88E = word_A886;
    word_A890 = word_A878;
    word_A892 = word_A87A;
    word_A894 = word_A87C;
    word_A896 = word_A87E;
    word_A876 = word_A856;
    word_A866 = word_A85E;
    word_A868 = word_A860;
    ReleaseDC(NULL, var_2);
}

/* Render UFO beam (if any) and present render targets onto window (sub). */
BOOL sub_9A49(HWND arg_0)
{
    HDC var_2;
    HDC var_4;
    RECT var_C;
    HDC var_E;
#ifdef _WIN32
    HDC screen;
#endif
    if (word_A870 == 0) {
        return TRUE;
    }
    word_A870 = 0;
    var_2 = GetDC(arg_0);
    SelectPalette(var_2, word_CA4A, FALSE);
    RealizePalette(var_2);
    var_4 = CreateCompatibleDC(var_2);
    SelectPalette(var_4, word_CA4A, FALSE);
    SelectObject(var_4, word_A854);
    BitBlt(var_2, 0, 0, word_A884, word_A886, var_4, 0, 0, SRCCOPY);
    if (word_CA5C != 0) {
        if (word_C0B8 == NULL) {
            word_C0B8 = CreateCompatibleBitmap(var_2, 40, word_CA52 * 4 / 5);
            if (word_C0B8 == NULL) {
                goto loc_9CC3;
            }
        }
        if (word_C0B2 == NULL) {
            word_C0B2 = CreateCompatibleBitmap(var_2, 40, word_CA52 * 4 / 5);
            if (word_C0B2 == NULL) {
                goto loc_9CC3;
            }
        }
        if (word_CA44 == NULL) {
            word_CA44 = CreateSolidBrush(RGB(255, 255, 0));
        }
        if (word_C0B4 == NULL) {
            word_C0B4 = CreateSolidBrush(RGB(128, 128, 0));
        }
        var_E = CreateCompatibleDC(var_2);
        SelectObject(var_E, word_C0B2);
#ifdef _WIN32
        /* Screen contents with height of only 40 pixels can be captured from window device context on Windows 10. Capture directly from screen instead. */
        screen = GetDC(NULL);
        BitBlt(var_E, 0, 0, 40, word_CA5C, screen, word_A880, word_A882 + 40, SRCCOPY);
        ReleaseDC(NULL, screen);
#else
        BitBlt(var_E, 0, 0, 40, word_CA5C, var_2, 0, 40, SRCCOPY);
#endif
        var_C.left = 0;
        var_C.top = 0;
        var_C.right = 40;
        var_C.bottom = word_CA5C;
        SelectObject(var_4, word_C0B8);
        FillRect(var_4, &var_C, word_CA44);
        BitBlt(var_E, 0, 0, 40, word_CA5C, var_4, 0, 0, SRCAND);
        FillRect(var_4, &var_C, word_C0B4);
        BitBlt(var_E, 0, 0, 40, word_CA5C, var_4, 0, 0, SRCPAINT);
        BitBlt(var_2, 0, 40, 40, word_CA5C, var_E, 0, 0, SRCCOPY);
        DeleteDC(var_E);
        DeleteDC(var_4);
    } else {
        if (word_C0B4 != NULL) {
            DeleteObject(word_C0B4);
            word_C0B4 = NULL;
        }
        if (word_CA44 != NULL) {
            DeleteObject(word_CA44);
            word_CA44 = NULL;
        }
        if (word_C0B2 != NULL) {
            DeleteObject(word_C0B2);
            word_C0B2 = NULL;
        }
        if (word_C0B8 != NULL) {
            DeleteObject(word_C0B8);
            word_C0B8 = NULL;
        }
        DeleteDC(var_4);
    }
    ReleaseDC(arg_0, var_2);
    return TRUE;
loc_9CC3:
    ReleaseDC(arg_0, var_2);
    DestroyWindow(arg_0);
    return FALSE;
}
