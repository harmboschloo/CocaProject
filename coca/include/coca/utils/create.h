#ifndef COCA_CREATE_H_INCLUDED
#define COCA_CREATE_H_INCLUDED

namespace coca
{
    template<typename BaseType, typename Type>
    BaseType* create()
    {
        return new Type();
    }

} // namespace coca

#endif // COCA_CREATE_H_INCLUDED
