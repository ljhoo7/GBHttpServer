using System;
using System.Collections.Generic;
using System.Net.Http;
using System.Text;
using System.Threading.Tasks;

namespace Test
{
    class TestCase1 : TestCaseBase
    {
        public override async Task StartAsync()
        {
            var ret = await g_httpClient.GetAsync("http://localhost:8000/test/test2").ConfigureAwait(false);

            var response = await ret.Content.ReadAsStringAsync().ConfigureAwait(false);

            Console.WriteLine($"Response : {response}");
        }
    }
}
