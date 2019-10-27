#ifndef BigInteger_h
#define BigInteger_h

#include <vector>
#include <string>
#include <iostream>

class BigInteger;
BigInteger abs (BigInteger element);

class BigInteger
{
public:
    
    friend std :: ostream& operator << (std :: ostream &, const BigInteger &);
    friend std :: istream& operator >> (std :: istream &, BigInteger &);
    
    BigInteger & operator = (const BigInteger &);
    explicit operator bool();
    
    friend bool operator == (const BigInteger &, const BigInteger &);
    friend bool operator != (const BigInteger &, const BigInteger &);
    friend bool operator < (const BigInteger &, const BigInteger &);
    friend bool operator <= (const BigInteger &, const BigInteger &);
    friend bool operator > (const BigInteger &, const BigInteger &);
    friend bool operator >= (const BigInteger &, const BigInteger &);
    
    // Unary minus
    friend BigInteger operator - (const BigInteger &);
    friend BigInteger & operator ++ (BigInteger &);
    friend BigInteger operator ++ (BigInteger &, int);
    friend BigInteger & operator -- (BigInteger &);
    friend BigInteger operator -- (BigInteger &, int);
    
    friend BigInteger operator + (const BigInteger &, const BigInteger &);
    friend BigInteger operator - (const BigInteger &, const BigInteger &);
    friend BigInteger operator * (const BigInteger &, const BigInteger &);
    friend BigInteger operator / (const BigInteger &, const BigInteger &);
    friend BigInteger operator % (const BigInteger &, const BigInteger &);
    
    friend BigInteger & operator += (BigInteger &, const BigInteger &);
    friend BigInteger & operator -= (BigInteger &, const BigInteger &);
    friend BigInteger & operator *= (BigInteger &, const BigInteger &);
    friend BigInteger & operator /= (BigInteger &, const BigInteger &);
    friend BigInteger & operator %= (BigInteger &, const BigInteger &);
    
private:
    
    std :: vector <unsigned char> data;
    char sign;     // 0 - if positive
                   // 1 - if negative
    
    static const unsigned char base = 10;
    
    
    void fromString (const std::string & str)
    {
        if (str.size() == 0) {
            return;
        }
        sign = (str[0] == '-' ? 1 : 0);
        
        data.clear();
        data.reserve(str.size());
        
        for (int i = static_cast<int>(str.size()) - 1, j = 0; i >= sign; i--, j++) {
            data.push_back(str[i] - '0');
        }
        
        if (data.size() == 1 && data[0] == 0) {
            sign = 0;
        }
    }
    
    static BigInteger sum (const BigInteger & first, const BigInteger & second, char result_sign)
    {
        BigInteger result;
        size_t size = std::max(first.data.size(), second.data.size());
        result.sign = result_sign;
        result.data.clear();
        result.data.reserve(size + 1);
        
        unsigned char carry = 0;
        
        
        for (size_t i = 0; i < size; i++) {
            unsigned char first_term = (i < first.data.size() ? first.data[i] : 0);
            unsigned char second_term = (i < second.data.size() ? second.data[i] : 0);
            
            result.data.push_back(first_term + second_term + carry);
            
            if (result.data[i] >= 10) {
                carry = 1;
                result.data[i] -= 10;
            } else {
                carry = 0;
            }
        }
        if (carry) {
            result.data.push_back(carry);
        }
        
        return result;
    }
    
    
    static BigInteger substraction (const BigInteger & first, const BigInteger & second, char result_sign)
    {
        BigInteger result;
        size_t size = std::max(first.data.size(), second.data.size());
        result.sign = result_sign;
        result.data.clear();
        result.data.reserve(size + 1);
        
        unsigned char carry = 0;
        
        for (size_t i = 0; i < size; i++) {
            unsigned char first_term = (i < first.data.size() ? first.data[i] : 0);
            unsigned char second_term = (i < second.data.size() ? second.data[i] : 0);
            
            result.data.push_back(first_term - second_term - carry);
            
            if (result.data[i] > 9) {
                result.data[i] += 10;
                carry = 1;
            } else {
                carry = 0;
            }
        }
        
        while (result.data.size() > 1 && result.data.back() == 0) {
            result.data.pop_back();
        }
        
        if (result.data.size() == 1 && result.data[0] == 0) {
            result.sign = 0;
        }
        
        
        return result;
    }
    
    static BigInteger mult (const BigInteger & first, const BigInteger & second, char result_sign)
    {
        BigInteger result;
        size_t size = first.data.size() * second.data.size();
        result.sign = result_sign;
        result.data.clear();
        result.data.reserve(size + 1);
        
        unsigned char carry = 0;
        
        for (size_t i = 0; i < size + 1; i++) {
            result.data.push_back(carry);
            carry = 0;
            for (size_t j = 0; j <= i; j++) {
                unsigned char first_term = (j < first.data.size() ? first.data[j] : 0);
                unsigned char second_term = (i - j < second.data.size() ? second.data[i - j] : 0);
                
                result.data[i] += first_term * second_term;
                
                if (result.data[i] > 9) {
                    carry += result.data[i] / 10;
                    result.data[i] %= 10;
                }
            }
        }
        
        while (result.data.size() > 1 && result.data.back() == 0) {
            result.data.pop_back();
        }
        
        if (result.data.size() == 1 && result.data[0] == 0) {
            result.sign = 0;
        }
        
        return result;
    }
    
    static BigInteger div (const BigInteger & first, const BigInteger & second, char result_sign)
    {
        if (second == 0) {
            std::invalid_argument("Devision by zero");
        }
        
        BigInteger result = 0;
        
        BigInteger num = abs(first), denom = abs(second), degree;
        
        while (num >= denom) {
            degree = 1;
            
            while (num >= denom) {
                denom *= BigInteger::base;
                degree *= BigInteger::base;
            }
            
            if (degree > 0) {
                denom.data.erase(denom.data.begin());
                degree.data.erase(degree.data.begin());
            }
            
            while (num >= denom) {
                num -= denom;
                result += degree;
            }
            
            denom = abs(second);
        }
        
        while (result.data.size() > 1 && result.data.back() == 0) {
            result.data.pop_back();
        }
        
        result.sign = result_sign;
        
        if (result.data.size() == 1 && result.data[0] == 0) {
            result.sign = 0;
        }
        
        return result;
    }
    
    static BigInteger mod (const BigInteger & first, const BigInteger & second)
    {
        if (second == 0) {
            std::invalid_argument("Devision by zero");
        }
        
        BigInteger result = 0;

        
        BigInteger num = abs(first), denom = abs(second), degree;
        
        while (num >= denom) {
            degree = 1;
            
            while (num >= denom) {
                denom *= BigInteger::base;
                degree *= BigInteger::base;
            }
            
            if (degree > 0) {
                denom.data.erase(denom.data.begin());
                degree.data.erase(degree.data.begin());
            }
            
            while (num >= denom) {
                num -= denom;
                result += degree;
            }
            
            denom = abs(second);
        }
        
        while (num.data.size() > 1 && num.data.back() == 0) {
            num.data.pop_back();
        }
        
        num.sign = first.sign;
        
        if (num.data.size() == 1 && num.data[0] == 0) {
            num.sign = 0;
        }
        
        return num;
    }
    
    
    
    // 0 - if equal
    // -1 - if first < second
    // 1 - if first > seocnd
    static char compare_by_module (const BigInteger & first, const BigInteger & second)
    {
        if (first.data.size() < second.data.size()) {
            return -1;
        } else if (first.data.size() > second.data.size()) {
            return 1;
        }
        
        size_t comp_size = first.data.size();
        
        for (int i = static_cast<int>(comp_size) - 1; i >= 0; i--) {
            if (first.data[i] < second.data[i]) {
                return -1;
            } else if (first.data[i] > second.data[i]) {
                return 1;
            }
        }
        
        return 0;
    }
    
public:
    
    BigInteger()
    {
        data.clear();
        data.push_back(0);
        sign = 0;
    }
    
    BigInteger(int element)
    {
        data.clear();
        if (element < 0) {
            element = -element;
            sign = 1;
        } else {
            sign = 0;
        }
        
        if (element == 0) {
            data.push_back(0);
            return;
        }
        
        while (element > 0) {
            data.push_back(element % 10);
            element /= 10;
        }
    }
    
    std :: string toString () const
    {
        std::string result;
        result.reserve(data.size() + 1);
        
        if (sign == 1) {
            result += '-';
        }
        
        for (int i = static_cast<int>(data.size()) - 1; i >= 0; i--) {
            result += data[i] + '0';
        }
        
        return result;
    }
};
 
BigInteger abs (BigInteger element)
{
    return (element < 0 ? -element : element);
}

std :: ostream& operator<< (std :: ostream &out, const BigInteger & number)
{
    out << number.toString();
    return out;
}

std :: istream& operator>> (std :: istream &in, BigInteger & number)
{
    std::string str;
    in >> str;
    number.fromString(str);
    return in;
}

BigInteger & BigInteger::operator = (const BigInteger & second)
{
    this -> data = second.data;
    this -> sign = second.sign;
    return *this;
}

BigInteger::operator bool()
{
    return (*this) != 0;
}

bool operator == (const BigInteger & first, const BigInteger & second)
{
    if (first.sign != second.sign) {
        return false;
    }
    return BigInteger::compare_by_module(first, second) == 0;
}

bool operator != (const BigInteger & first, const BigInteger & second)
{
    if (first.sign != second.sign) {
        return true;
    }
    return BigInteger::compare_by_module(first, second) != 0;
}

bool operator < (const BigInteger & first, const BigInteger & second)
{
    if (first.sign < second.sign) {
        return false;
    } else if (first.sign > second.sign) {
        return true;
    }
    
    if (first.sign == 0) {
        return BigInteger::compare_by_module(first, second) == -1;
    } else {
        return BigInteger::compare_by_module(first, second) == 1;
    }
}

bool operator <= (const BigInteger & first, const BigInteger & second)
{
    if (first.sign < second.sign) {
        return false;
    } else if (first.sign > second.sign) {
        return true;
    }
    
    if (first.sign == 0) {
        return BigInteger::compare_by_module(first, second) <= 0;
    } else {
        return BigInteger::compare_by_module(first, second) >= 0;
    }
}

bool operator > (const BigInteger & first, const BigInteger & second)
{
    if (first.sign > second.sign) {
        return false;
    } else if (first.sign < second.sign) {
        return true;
    }
    
    if (first.sign == 0) {
        return BigInteger::compare_by_module(first, second) == 1;
    } else {
        return BigInteger::compare_by_module(first, second) == -1;
    }
}

bool operator >= (const BigInteger & first, const BigInteger & second)
{
    if (first.sign > second.sign) {
        return false;
    } else if (first.sign < second.sign) {
        return true;
    }
    
    if (first.sign == 0) {
        return BigInteger::compare_by_module(first, second) >= 0;
    } else {
        return BigInteger::compare_by_module(first, second) <= 0;
    }
}

// Unary minus
BigInteger operator - (const BigInteger & number)
{
    BigInteger result = number;
    result.sign = (result.sign == 1 || (result.data.size() == 1 && result.data[0] == 0) ? 0 : 1);
    return result;
}

BigInteger & operator ++ (BigInteger & element)
{
    return element = element + 1;
}

BigInteger operator ++ (BigInteger & element, int)
{
    BigInteger old_value = element;
    element = element + 1;
    return old_value;
}

BigInteger & operator -- (BigInteger & element)
{
    return element = element - 1;
}

BigInteger operator -- (BigInteger & element, int)
{
    BigInteger old_value = element;
    element = element - 1;
    return old_value;
}



BigInteger operator + (const BigInteger & first, const BigInteger & second)
{
    if (first.sign == second.sign) {
        return BigInteger::sum(first, second, first.sign);
    }
    
    if (BigInteger::compare_by_module(first, second) >= 0) {
        return BigInteger::substraction(first, second, first.sign);
    } else {
        return BigInteger::substraction(second, first, second.sign);
    }
}

BigInteger operator - (const BigInteger & first, const BigInteger & second)
{
    if (first.sign != second.sign) {
        return BigInteger::sum(first, second, first.sign);
    }
    
    if (BigInteger::compare_by_module(first, second) >= 0) {
        return BigInteger::substraction(first, second, first.sign);
    } else {
        return BigInteger::substraction(second, first, second.sign == 0 ? 1 : 0);
    }
}

BigInteger operator * (const BigInteger & first, const BigInteger & second)
{
    return BigInteger::mult(first, second, first.sign == second.sign ? 0 : 1);
}

BigInteger operator / (const BigInteger & first, const BigInteger & second)
{
    return BigInteger::div(first, second, first.sign == second.sign ? 0 : 1);
}

BigInteger operator % (const BigInteger & first, const BigInteger & second)
{
    return BigInteger::mod(first, second);
}

BigInteger & operator += (BigInteger & first, const BigInteger & second)
{
    return first = first + second;
}

BigInteger & operator -= (BigInteger & first, const BigInteger & second)
{
    return first = first - second;
}

BigInteger & operator *= (BigInteger & first, const BigInteger & second)
{
    return first = first * second;
}

BigInteger & operator /= (BigInteger & first, const BigInteger & second)
{
    return first = first / second;
}

BigInteger & operator %= (BigInteger & first, const BigInteger & second)
{
    return first = first % second;
}

#endif /* BigInteger_h */
