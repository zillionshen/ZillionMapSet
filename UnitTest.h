#pragma once

#include <iostream>
#include <string>
#include <unordered_map>

using namespace std;

typedef bool(*UnitTestFunction)();

enum TestCaseResult { TCROK, TCRFailed, TCRNotRun };

struct TestCase
{
    UnitTestFunction function;
    TestCaseResult result;
    TestCase() : function(0), result(TCRNotRun) {}
    TestCase(UnitTestFunction function) : function(function), result(TCRNotRun) {}
};

class UnitTest
{
private:
    unordered_map<string, TestCase> m_cases;
private:
    bool RunCase(const string & caseName, TestCase & tc)
    {
        if (tc.function == NULL) { cout << "Test Case not defined: " << caseName; return false; }

        tc.result = tc.function() ? TCROK : TCRFailed;
        cout << "Test case [" << caseName << "] result: " << (tc.result == TCROK ? "OK" : "Failed") << endl;

        return tc.result == TCROK;
    }

public:
    void AddCase(const string & caseName, UnitTestFunction function) { m_cases[caseName] = TestCase(function); }
    bool RunTillFailed() { bool result = true; for (auto c : m_cases) if (!RunCase(c.first, c.second)) { result = false; break; } return result; }
    bool RunAll() { bool result = true; for (auto a : m_cases) if (!RunCase(a.first, a.second)) result = false; return result; }
    bool RunOne(const string & caseName) { auto c = m_cases.find(caseName); if (c == m_cases.end()) return false; return RunCase(c->first, c->second); }
};
