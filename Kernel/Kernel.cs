using System;
using System.Collections.Generic;
using System.Text;
using Sys = Cosmos.System;
using Shaper;
using Shaper.Application;

namespace Shaper.Kernel
{
    public class Kernel: Sys.Kernel
    {
        protected override void BeforeRun()
        {
            Shaper.Application.Global.Initialize();
            mConsole.Title = "ShaperCE";
        }
        
        protected override void Run()
        {
            mConsole.Write("/> ");
            mConsole.ReadLine();
            mConsole.Write("/> " + mConsole.CurrentInput); 
        }
    }
}
