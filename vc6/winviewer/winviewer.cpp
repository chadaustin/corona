#include <string>
#include <windows.h>
#include <commctrl.h>
#include <commdlg.h>
#include <shellapi.h>
#include <stdio.h>
#include <stdlib.h>
#include "corona.h"
#include "resource.h"


typedef unsigned char byte;

struct RGBA {
  byte red, green, blue, alpha;
};

struct BGRA {
  byte blue, green, red, alpha;
};



static char TITLE[] = "Corona Image Viewer";
static char CLASSNAME[] = "CoronaWinView";


static HDC gDC;
static HBITMAP gBitmap;
static int gImageWidth;
static int gImageHeight;


static std::string BrowseForFileDialog(HWND parent, const char* title) {
  char file[MAX_PATH] = ""  ;
  OPENFILENAME ofn;
  memset(&ofn, 0, sizeof(ofn));
  ofn.lStructSize = sizeof(ofn);
  ofn.hwndOwner   = parent;
  ofn.hInstance   = GetModuleHandle(NULL);
  ofn.lpstrFile   = file;
  ofn.nMaxFile    = MAX_PATH;
  ofn.lpstrTitle  = title;
  ofn.Flags       = OFN_FILEMUSTEXIST;
  if (!GetOpenFileName(&ofn)) {
    DWORD error = CommDlgExtendedError();
    return "";
  }
  return file;
}

static bool UpdateImage(const char* filename) {
  if (!filename) {
    if (gDC) {
      DeleteDC(gDC);
      gDC = NULL;
    }
    if (!gBitmap) {
      DeleteObject(gBitmap);
      gBitmap = NULL;
    }
    return true;
  }

  corona::Image* image = corona::OpenImage(filename, corona::PF_R8G8B8A8);
  if (!image) {
    return false;
  }
  gImageWidth = image->getWidth();
  gImageHeight = image->getHeight();

  // destroy the old DC and bitmap
  UpdateImage(0);

  gDC = CreateCompatibleDC(NULL);
  if (!gDC) {
    return false;
  }
   
  BITMAPINFO bmi;
  memset(&bmi, 0, sizeof(bmi));
  BITMAPINFOHEADER& bmih = bmi.bmiHeader;
  bmih.biSize        = sizeof(bmih);
  bmih.biWidth       = gImageWidth;
  bmih.biHeight      = -gImageHeight;
  bmih.biPlanes      = 1;
  bmih.biBitCount    = 32;
  bmih.biCompression = BI_RGB;

  BGRA* dest;
  gBitmap = CreateDIBSection(
    GetDC(NULL), &bmi, DIB_RGB_COLORS,
    (void**)&dest, NULL, 0);

  if (!gBitmap) {
    DeleteDC(gDC);
    gDC = NULL;
    return false;
  }
  if (!dest) {
    DeleteDC(gDC);
    gDC = NULL;
    DeleteObject(gBitmap);
    gBitmap = NULL;
    return false;
  }

  RGBA* src = (RGBA*)image->getPixels();
  for (int i = 0; i < gImageWidth * gImageHeight; ++i) {
    dest->red   = src->red;
    dest->green = src->green;
    dest->blue  = src->blue;
    dest->alpha = src->alpha;
    ++src;
    ++dest;
  }

  delete image;
  return true;
}


static LRESULT CALLBACK WindowProc(
  HWND window,
  UINT msg,
  WPARAM wparam,
  LPARAM lparam)
{
  switch (msg) {
    case WM_CREATE: {
      DragAcceptFiles(window, TRUE);
      return 0;
    }

    case WM_DESTROY: {
      PostQuitMessage(0);
      return 0;
    }

    case WM_PAINT: {
      PAINTSTRUCT ps;
      BeginPaint(window, &ps);

      if (gDC && gBitmap) {
        HBITMAP old = (HBITMAP)SelectObject(gDC, gBitmap);

        BitBlt(ps.hdc, 0, 0, gImageWidth, gImageHeight,
               gDC, 0, 0, SRCCOPY);

        SelectObject(gDC, old);

        RECT clientRect;
        GetClientRect(window, &clientRect);

        RECT right = clientRect;
        right.left = gImageWidth;

        RECT bottom = clientRect;
        bottom.top  = gImageHeight;

        RECT corner = clientRect;
        corner.left = gImageWidth;
        corner.top  = gImageHeight;

        FillRect(ps.hdc, &right,  (HBRUSH)(COLOR_WINDOW + 1));
        FillRect(ps.hdc, &bottom, (HBRUSH)(COLOR_WINDOW + 1));
        FillRect(ps.hdc, &corner, (HBRUSH)(COLOR_WINDOW + 1));

      } else {
        RECT clientRect;
        GetClientRect(window, &clientRect);
        FillRect(ps.hdc, &clientRect, (HBRUSH)(COLOR_WINDOW + 1));
      }

      EndPaint(window, &ps);
      return 0;
    }

    case WM_CLOSE: {
      DestroyWindow(window);
      return 0;
    }

    case WM_COMMAND: {
      switch (LOWORD(wparam)) {
        case ID_FILE_OPEN: {
          std::string image = BrowseForFileDialog(window, "Open Image");
          if (!image.empty()) {
            InvalidateRect(window, NULL, TRUE);
            if (!UpdateImage(image.c_str())) {
              char str[1024];
              sprintf(str, "Failed to open image '%s'", image.c_str());
              MessageBox(window, str, "Open Image", MB_OK | MB_ICONERROR);
            }
          }
          return 0;
        }

        case ID_FILE_EXIT: {
          DestroyWindow(window);
          return 0;
        }

        default:
          return 0;
      }
    }

    case WM_DROPFILES: {
      HDROP drop = (HDROP)wparam;
      int fileCount = DragQueryFile(drop, 0xFFFFFFFF, 0, 0);
      if (fileCount == 0) {
        return 0;
      }

      // we will always need to repaint
      InvalidateRect(window, NULL, TRUE);

      char buffer[MAX_PATH];
      DragQueryFile(drop, 0, buffer, MAX_PATH);
      if (!UpdateImage(buffer)) {
        char str[1024];
        sprintf(str, "Failed to open image '%s'", buffer);
        MessageBox(window, str, "Open Image", MB_OK | MB_ICONERROR);
      }

      return 0;
    }


    default: {
      return DefWindowProc(window, msg, wparam, lparam);
    }
  }
}


int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

  if (__argc >= 2) {
    if (!UpdateImage(__argv[1])) {
      char str[1024];
      sprintf(str, "Could not open image '%s'", __argv[1]);
      MessageBox(NULL, str, TITLE, MB_OK | MB_ICONERROR);
      return 1;
    }
  }

  InitCommonControls();

  WNDCLASS wc;
  memset(&wc, 0, sizeof(wc));
  wc.lpfnWndProc   = WindowProc;
  wc.hInstance     = GetModuleHandle(NULL);
  wc.hIcon         = LoadIcon(NULL, MAKEINTRESOURCE(IDI_APPLICATION));
  wc.hCursor       = LoadCursor(NULL, MAKEINTRESOURCE(IDC_ARROW));
  wc.lpszMenuName  = MAKEINTRESOURCE(IDR_MENU);
  wc.lpszClassName = CLASSNAME;
  RegisterClass(&wc);

  HWND window = CreateWindow(
    CLASSNAME, TITLE,
    WS_OVERLAPPEDWINDOW,
    CW_USEDEFAULT, 0,
    CW_USEDEFAULT, 0,
    NULL, NULL, GetModuleHandle(NULL), NULL);
  if (!window) {
    MessageBox(NULL, "Window creation failed", TITLE, MB_OK | MB_ICONERROR);
    return 1;
  }

  ShowWindow(window, SW_SHOW);
  UpdateWindow(window);

  MSG msg;
  while (GetMessage(&msg, NULL, 0, 0) > 0) {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }

  UpdateImage(0);
  return msg.wParam;
}
