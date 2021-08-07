using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Interop;
using System.Diagnostics;
using System.Threading;

namespace UIEditor.EngineRenderSurface
{
    class EngineRenderSurfaceHost : HwndHost
    {
        [DllImport("user32.dll")]
        private static extern int SetWindowLong(IntPtr hWnd, int nIndex, int dwNewLong);

        [DllImport("user32.dll", SetLastError = true)]
        private static extern int GetWindowLong(IntPtr hWnd, int nIndex);

        [DllImport("user32")]
        private static extern IntPtr SetParent(IntPtr hWnd, IntPtr hWndParent);

        public const int GWL_STYLE = (-16);
        public const int WS_CHILD = 0x40000000;

        protected override HandleRef BuildWindowCore(HandleRef hwndParent)
        {
            IntPtr hInstance = Marshal.GetHINSTANCE(GetType().Module);
            var mainWnd = N3EngineApi.CreateMainWindow(hInstance);

            SetWindowLong(mainWnd, GWL_STYLE, WS_CHILD);
            SetParent(mainWnd, hwndParent.Handle);

            this.InvalidateVisual();

            HandleRef hwnd = new HandleRef(this, mainWnd);
            N3EngineApi.init(hInstance, mainWnd);
            return hwnd;
        }

        protected override void DestroyWindowCore(HandleRef hwnd)
        {
            
        }
    }
}
