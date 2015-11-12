#ifndef NOOP_H
#define NOOP_H

template<class T>
class CNoOp
{
public:
    T operator()(const T& /*Phase*/) const
    {
        return 0;//Phase;
    }
};

#endif // NOOP_H
