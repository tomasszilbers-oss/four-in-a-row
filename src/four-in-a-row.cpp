#include <windows.h>
#include <windowsx.h>
#include <algorithm>
#include <string>

#include "GameController.h"
#include "Board.h"
#include "Player.h"
#include "GameState.h"

static const char* kWindowClassName = "FourInARowWindowClass";
static const char* kWindowTitle = "Four in a Row";

static GameController g_controller;

static int g_cellSize = 80;
static int g_offsetX = 0;
static int g_offsetY = 0;

static constexpr UINT_PTR kAnimTimerId = 1;
static constexpr UINT kAnimIntervalMs = 16; // ~60 FPS

struct DropAnimation
{
    bool active = false;
    int col = -1;
    int targetRow = -1;
    Player player;

    float y = 0.0f;
    float yTarget = 0.0f;
    float speed = 800.0f; // pixels/sec

    DWORD lastTick = 0;
};

static DropAnimation g_anim;

static void RecalculateLayout(HWND hwnd)
{
    RECT rc{};
    GetClientRect(hwnd, &rc);

    const int clientW = rc.right - rc.left;
    const int clientH = rc.bottom - rc.top;

    const int cols = Board::Cols;
    const int rows = Board::Rows;

    const int padding = 20;

    const int maxCellW = (clientW - 2 * padding) / cols;
    const int maxCellH = (clientH - 2 * padding) / rows;

    g_cellSize = std::max(20, std::min(maxCellW, maxCellH));

    const int boardW = g_cellSize * cols;
    const int boardH = g_cellSize * rows;

    g_offsetX = (clientW - boardW) / 2;
    g_offsetY = (clientH - boardH) / 2;
}

static int ColumnFromMouse(int mouseX, int mouseY)
{
    const int boardW = g_cellSize * Board::Cols;

    if (mouseX < g_offsetX || mouseX >= g_offsetX + boardW)
        return -1;

    const int col = (mouseX - g_offsetX) / g_cellSize;
    if (col < 0 || col >= Board::Cols)
        return -1;

    return col;
}

static RECT CellRect(int col, int row)
{
    // Board row 0 is the bottom row. For drawing, y grows downward.
    const int drawRow = (Board::Rows - 1) - row;

    RECT r{};
    r.left   = g_offsetX + col * g_cellSize;
    r.top    = g_offsetY + drawRow * g_cellSize;
    r.right  = r.left + g_cellSize;
    r.bottom = r.top + g_cellSize;
    return r;
}

static void DrawScene(HWND hwnd, HDC hdc) {
    RECT rc{};
    GetClientRect(hwnd, &rc);

    HBRUSH bg = (HBRUSH)(COLOR_WINDOW + 1);
    FillRect(hdc, &rc, bg);

    const Board& board = g_controller.GetBoard();

    HPEN gridPen = CreatePen(PS_SOLID, 1, RGB(120, 120, 120));
    HGDIOBJ oldPen = SelectObject(hdc, gridPen);

    HBRUSH redBrush = CreateSolidBrush(RGB(220, 40, 40));
    HBRUSH blackBrush = CreateSolidBrush(RGB(30, 30, 30));

    for (int row = 0; row < Board::Rows; ++row)
    {
        for (int col = 0; col < Board::Cols; ++col)
        {
            RECT cell = CellRect(col, row);

            Rectangle(hdc, cell.left, cell.top, cell.right, cell.bottom);

            RECT token = cell;
            int inset = 6;
            token.left += inset;
            token.top += inset;
            token.right -= inset;
            token.bottom -= inset;

            auto c = board.GetCell(col, row);
            if (c.has_value())
            {
                HBRUSH b = (*c == Player::Human) ? redBrush : blackBrush;
                HGDIOBJ oldBrush = SelectObject(hdc, b);
                Ellipse(hdc, token.left, token.top, token.right, token.bottom);
                SelectObject(hdc, oldBrush);
            }
        }
    }

    // --- draw animated token ---
    if (g_anim.active)
    {
        RECT cell = CellRect(g_anim.col, g_anim.targetRow);

        int inset = 6;
        int size = (cell.bottom - cell.top) - inset * 2;

        RECT r;
        r.left = cell.left + inset;
        r.right = r.left + size;
        r.top = static_cast<int>(g_anim.y);
        r.bottom = r.top + size;

        HBRUSH b = (g_anim.player == Player::Human) ? redBrush : blackBrush;
        HGDIOBJ oldBrush = SelectObject(hdc, b);
        Ellipse(hdc, r.left, r.top, r.right, r.bottom);
        SelectObject(hdc, oldBrush);
    }

    SelectObject(hdc, oldPen);
    DeleteObject(gridPen);
    DeleteObject(redBrush);
    DeleteObject(blackBrush);
}

static LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg)
    {
        case WM_CREATE:
            RecalculateLayout(hwnd);
            return 0;

        case WM_SIZE:
            RecalculateLayout(hwnd);
            InvalidateRect(hwnd, nullptr, TRUE);
            return 0;

        case WM_LBUTTONDOWN:
        {
            if (g_controller.GetState() == GameState::GameOver)
            {
                g_controller.NewGame(g_controller.IsVsComputer());
                InvalidateRect(hwnd, nullptr, TRUE);
                return 0;
            }

            if (g_anim.active)
                return 0;       //break;

            int x = GET_X_LPARAM(lParam);
            int y = GET_Y_LPARAM(lParam);

            int col = ColumnFromMouse(x, y);
            if (col < 0)
                return 0;       //break;

            auto dropRowOpt = g_controller.GetBoard().GetDropRow(col);
            if (!dropRowOpt.has_value())
            {
                MessageBeep(MB_ICONWARNING);
                return 0;       //break;
            }

            g_anim.active = true;
            g_anim.col = col;
            g_anim.targetRow = *dropRowOpt;
            g_anim.player = g_controller.GetCurrentPlayer();
            g_anim.lastTick = GetTickCount();

            int padding = 6;

            // Target cell
            RECT targetCell = CellRect(col, g_anim.targetRow);
            int size = (targetCell.bottom - targetCell.top) - padding * 2;

            g_anim.yTarget = static_cast<float>(targetCell.top + padding);

            // Top of board (row = highest visible row)
            RECT topCell = CellRect(col, Board::Rows - 1);

            // Start slightly above top of board
            g_anim.y = static_cast<float>(topCell.top - size - 20);

            SetTimer(hwnd, kAnimTimerId, kAnimIntervalMs, nullptr);
            return 0;       //break;
        }

        case WM_TIMER: {
            if (wParam != kAnimTimerId || !g_anim.active)
                return 0;       //break;

            DWORD now = GetTickCount();
            float dt = (now - g_anim.lastTick) / 1000.0f;
            g_anim.lastTick = now;

            g_anim.y += g_anim.speed * dt;

            if (g_anim.y >= g_anim.yTarget)
            {
                g_anim.y = g_anim.yTarget;
                g_anim.active = false;
                KillTimer(hwnd, kAnimTimerId);

                g_controller.ApplyMoveFromUI(g_anim.col);

                // Check if the game is over
                if (g_controller.GetState() == GameState::GameOver)
                {
                    auto outcome = g_controller.GetOutcome();

                    if (outcome == Outcome::Win)
                    {
                        auto winner = g_controller.GetWinner();
                        if (winner.has_value())
                        {
                            std::string message;

                            if (g_controller.IsVsComputer())
                            {
                                message = (*winner == Player::Human)
                                    ? "Human wins!"
                                    : "Computer wins!";
                            }
                            else
                            {
                                message = (*winner == Player::Human)
                                    ? "First player wins!"
                                    : "Second player wins!";
                            }

                            MessageBoxA(hwnd, message.c_str(), "Game Over",
                                        MB_OK | MB_ICONINFORMATION);
                        }
                    }
                    else if (outcome == Outcome::Tie)
                    {
                        MessageBoxA(hwnd, "It's a tie!",
                                    "Game Over",
                                    MB_OK | MB_ICONINFORMATION);
                    }
                }
            }

            InvalidateRect(hwnd, nullptr, TRUE);
            return 0;       //break;
        }

        case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);

            DrawScene(hwnd, hdc);

            EndPaint(hwnd, &ps);
            return 0;       //break;
        }

        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;

        default:
            return DefWindowProcA(hwnd, msg, wParam, lParam);
    }
    //return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow)
{
    g_controller.NewGame(false);

    WNDCLASSEXA wc{};
    wc.cbSize = sizeof(wc);
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = kWindowClassName;
    wc.hCursor = LoadCursorA(nullptr, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);

    if (!RegisterClassExA(&wc))
        return 1;

    HWND hwnd = CreateWindowExA(
        0,
        kWindowClassName,
        kWindowTitle,
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        900, 700,
        nullptr,
        nullptr,
        hInstance,
        nullptr
    );

    if (!hwnd)
        return 1;

#ifndef NDEBUG
    {
        char dbgTitle[128];
        wsprintfA(dbgTitle, "%s [DEBUG]", kWindowTitle);
        SetWindowTextA(hwnd, dbgTitle);
    }
#endif

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    MSG msg{};
    while (GetMessageA(&msg, nullptr, 0, 0) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessageA(&msg);
    }

    return 0;
}
