using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Reflection;

namespace Test
{
    class Program
    {
        static void Main(string[] args)
        {
            TestCaseBase focusedTestCase = new TestCase1();
            
            var typeList = Assembly.GetExecutingAssembly().GetTypes();
            var typeMap = new Dictionary<string, Type>();

            foreach (var iType in typeList)
            {
                if(true == iType.Name.StartsWith("TestCase"))
                {
                    typeMap.Add(iType.Name, iType);
                }
            }

            int currentCaseNumber = 1;
            while (true)
            {
                var currentTestCaseName = $"TestCase{currentCaseNumber}";
                var ret = typeMap.TryGetValue(currentTestCaseName, out var currentTestCaseType);

                if(false == ret)
                {
                    Console.WriteLine($"The '{currentTestCaseName}' not exists.");
                    break;
                }

                ++currentCaseNumber;

                var currentTestCase = Activator.CreateInstance(currentTestCaseType) as TestCaseBase;

                Debug.Assert(null != currentTestCase);

                currentTestCase.Start();
            }
        }
    }
}
