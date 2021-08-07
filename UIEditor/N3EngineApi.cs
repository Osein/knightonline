using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace UIEditor
{
    static class N3EngineApi
    {
        private const string _dllName = "N3BaseDLL.dll";

        [DllImport(_dllName)]
        public static extern void InitEngine(IntPtr hInst, IntPtr hWnd);

        [DllImport(_dllName)]
        public static extern IntPtr GetWindowHandle();

        [DllImport(_dllName)]
        public static extern IntPtr CreateMainWindow(IntPtr hInst);

        public static void init(IntPtr hInst, IntPtr hWnd)
        {
            InitEngine(hInst, hWnd);
        }

        [DllImport(_dllName)]
        public static extern void RenderScene();
        
        public static void doRender()
        {
            RenderScene();
        }


    }
}
