// Copyright (C) 2008-2009 Harm Boschloo

#ifndef COCA_UTILS_SINGLETON_TEST_H_INCLUDED
#define COCA_UTILS_SINGLETON_TEST_H_INCLUDED

#include <coca/utils.h>

/*
class TestSingletonMacro
{
    COCA_DECLARE_SINGLETON( TestSingletonMacro )

private:
    TestSingletonMacro();
    virtual ~TestSingletonMacro();
};

class TestSingletonTemplate : public coca::Singleton<TestSingletonTemplate>
{
private:
    friend class coca::Singleton<TestSingletonTemplate>;
    TestSingletonTemplate();
    virtual ~TestSingletonTemplate();
};
*/

void testSingleton();

#endif // COCA_UTILS_SINGLETON_TEST_H_INCLUDED

