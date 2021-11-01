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
                var iTask = Task.Run(async () => 
                {
                    switch(k % 4)
                    {
                        case 0:
                            {
                                var ret = await g_httpClient.GetAsync("http://localhost:8000/test/test2").ConfigureAwait(false);
                                var response = await ret.Content.ReadAsStringAsync().ConfigureAwait(false);
                            }
                            break;
                        case 1:
                            {
                                var ret = await g_httpClient.PostAsync("http://localhost:8000/test/test2", new StringContent("")).ConfigureAwait(false);
                                var response = await ret.Content.ReadAsStringAsync().ConfigureAwait(false);
                            }
                            break;
                        case 2:
                            break;
                        case 3:
                            break;
                    }
                });

                taskList.Add(iTask);
            }

            await Task.WhenAll(taskList).ConfigureAwait(false);
        }
    }
}
