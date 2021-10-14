using System;
using System.Collections.Generic;
using System.Net.Http;
using System.Text;
using System.Threading.Tasks;

namespace Test
{
    class TestCase2 : TestCaseBase
    {
        public override async Task StartAsync()
        {
            Console.WriteLine("Stress test with multiple massive requests.");

            var taskList = new List<Task>();
            for(int k = 0; k < 100; ++k)
            {
                var iTask = Task.Run(() => 
                {

                });
                taskList.Add(iTask);
            }

            await Task.WhenAll(taskList).ConfigureAwait(false);
        }
    }
}
