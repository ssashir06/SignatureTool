using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Runtime.InteropServices;
using System.IO;

namespace Signature.CountingTool
{
    static class Program
    {
        /// <summary>
        /// アプリケーションのメイン エントリ ポイントです。
        /// </summary>
        [STAThread]
        static void Main()
        {
            string directory = 
                Environment.GetEnvironmentVariable("PATH") + ";" + 
                Path.Combine(Path.GetDirectoryName(Application.ExecutablePath), @"Dll");
            Environment.SetEnvironmentVariable("PATH", directory);

            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);
            Application.Run(new ImageListingForm());
        }
    }
}
