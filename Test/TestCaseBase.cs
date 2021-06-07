using System;
using System.Collections.Generic;
using System.Net.Http;
using System.Text;
using System.Threading.Tasks;

namespace Test
{
    abstract class TestCaseBase
    {
        public abstract Task StartAsync();

        protected static HttpClient g_httpClient = new HttpClient();
    }
}
