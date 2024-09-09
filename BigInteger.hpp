#ifndef _BigInteger_
#define _BigInteger_

#include <vector>
#include <bitset>
#include <string>
#define uint unsigned int
#define ull unsigned long long

class BigInt
{
public:
    std::vector<uint> num; // 索引较小的储存较低的位
    std::bitset<1> sign;   // 符号位，1为负数，0为正数
public:
    BigInt()
    {
        this->num.clear();
        this->sign.reset();
    }

    ~BigInt()
    {
        this->num.clear();
        this->sign.reset();
    }

    bool getSign() // true表示正数，false表示负数
    {
        return this->sign.none();
    }

    size_t size()
    {
        return this->num.size();
    }

    BigInt operator-()
    {
        this->sign = ~(this->sign);
    }

    BigInt operator=(BigInt a)
    {
        this->num = a.num;
        this->sign = a.sign;
    }

    BigInt operator=(std::string a)
    {
    }
};

BigInt operator+(BigInt a, BigInt b)
{
    if (!b.getSign())
    {
        return a - (-b);
    }
    else if (!a.getSign())
    {
        return b - (-a);
    }
    else
    {
        BigInt res;
        ull temp = 0;
        bool flag = false; // true代表a比较大，否则b大
        size_t minSize = 0;
        if (a.size() >= b.size())
        {
            flag = true;
            minSize = b.size();
        }
        else
        {
            flag = false;
            minSize = a.size();
        }

        register ull carry = 0;
        int i = 0;
        for (i = 0; i < minSize; i++)
        {
            ull temp = (ull)(a.num[i]) + (ull)(b.num[i]) + carry;
            res.num.push_back((uint)(temp));
            carry = temp >> 32;
        }

        if (flag)
        {
            for (; i < a.size(); i++)
            {
                res.num.push_back(a.num[i]);
            }
        }
        else
        {
            for (; i < b.size(); i++)
            {
                res.num.push_back(b.num[i]);
            }
        }
    }
}

BigInt operator-(BigInt a, BigInt b)
{
    if (!b.getSign())
    {
        return a + (-b);
    }
    else if (!a.getSign())
    {
        return -((-a) + b);
    }

    else
    {
        BigInt res;
        ull temp = 0;
        bool flag = false; // true代表a比较大，否则b大
        size_t minSize = 0;
        if (a.size() >= b.size())
        {
            flag = true;
            minSize = b.size();
        }
        else
        {
            flag = false;
            minSize = a.size();
        }

        register ull borrow = 0; // 使用 udi_t 类型来处理借位
        int i = 0;

        for (i = 0; i < minSize; i++)
        {
            borrow = (ull)a.num[i] - b.num[i] - borrow;         // 减去当前位和借位
            res.num.push_back(borrow);                         // 存储结果
            borrow = (borrow >> (sizeof(uint) * 8)) & 1; // 更新借位
        }

        if (flag)
        {
            for (; i < a.size(); i++)
            {
                res.num.push_back(a.num[i]);
            }
        }
        else
        {
            for (; i < b.size(); i++)
            {
                res.num.push_back(b.num[i]);
            }
        }
    }
}

BigInt operator*(BigInt a, BigInt b)
{
    BigInt res;
    res.num = std::vector<uint>(a.size() + b.size());
    ull temp = 0;
    bool flag = false; // true代表a比较大，否则b大
    size_t minSize = 0;
    if (a.size() >= b.size())
    {
        flag = true;
        minSize = b.size();
    }
    else
    {
        flag = false;
        minSize = a.size();
    }

    ull carry = 0;

    for (int i = 0; i < a.size(); i++)
    {
        ull carry = 0;
        for (int j = 0; j < b.size(); j++)
        {
            ull product = (ull)(res.num[i + j]) + (ull)(a.num[i]) * (ull)(b.num[j]) + carry;
            res.num[i + j] = (int)product;
            carry = product >> 32;
        }

        res.num[i + b.size()] += carry;
    }

    for (int i = res.size() - 1; i >= 0; i--)
    {
        if (res.num[i] == 0)
        {
            res.num.pop_back();
        }
        else
        {
            break;
        }
    }

    return res;
}

#endif