using System;
using System.Collections.Generic;
using System.Text;
using System.Threading.Tasks;

namespace Test
{
    class TestCase2 : TestCaseBase
    {
        public override async Task StartAsync()
        {
            Console.WriteLine("TestCase 2");
        }
    }
}
